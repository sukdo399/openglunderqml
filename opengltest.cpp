#include "opengltest.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtCore/QRunnable>

#include <math.h>

OpenglTest::OpenglTest()
    : m_t(0)
    , m_renderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &OpenglTest::handleWindowChanged);
}

void OpenglTest::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void OpenglTest::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &OpenglTest::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &OpenglTest::cleanup, Qt::DirectConnection);
        // Ensure we start with cleared to black. The OpenglTest's blend mode relies on this.
//        win->setColor(Qt::black);
    }
}

void OpenglTest::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

class CleanupJob : public QRunnable
{
public:
    CleanupJob(OpenglTestRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    OpenglTestRenderer *m_renderer;
};

void OpenglTest::releaseResources()
{
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

OpenglTestRenderer::~OpenglTestRenderer()
{
    delete m_program;
}

void OpenglTest::sync()
{
    if (!m_renderer) {
        m_renderer = new OpenglTestRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &OpenglTestRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &OpenglTestRenderer::paint, Qt::DirectConnection);
        m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
        m_renderer->setWindow(window());
    }
    m_renderer->setT(m_t);
}

void OpenglTestRenderer::init()
{
    if (!m_program) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL || rif->graphicsApi() == QSGRendererInterface::OpenGLRhi);

        initializeOpenGLFunctions();

        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    "uniform mat4   u_mvpMatrix;"
                                                    "attribute vec4 a_position;"
                                                    "attribute vec4 a_color;"
                                                    "varying vec4   v_color;"
                                                    "void main()"
                                                    "{"
                                                    "  gl_Position = u_mvpMatrix * a_position;"
                                                    "  v_color = a_color;"
                                                    "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "mediump float;"
                                                    "varying vec4 v_color;"
                                                    "void main()"
                                                    "{"
                                                    "  gl_FragColor = v_color;"
                                                    "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->link();


        /* Get the attribute locations */
        position_loc = m_program->attributeLocation("a_position");
        color_loc = m_program->attributeLocation("a_color");

        /* Get the uniform locations */
        mvp_matrix_loc = m_program->uniformLocation("u_mvpMatrix");


        glGenBuffers(1, &vertexID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &indiceID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        /* Init GL Status */
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepthf(1.0f);

        /* Make Matrix */
        float aspect_ratio = (float)m_viewportSize.width() / (float)m_viewportSize.height();
        LoadIdentityMatrix(&projection);
        LoadIdentityMatrix(&modelview);
        LoadIdentityMatrix(&mvp);

        PerspectiveMatrix(&projection, PROJECTION_FOVY, aspect_ratio, PROJECTION_NEAR, PROJECTION_FAR);

        /* Viewport */
        glViewport(0,0, m_viewportSize.width(), m_viewportSize.height());
    }
}

void OpenglTestRenderer::TranslateMatrix(glMatrix *result, float tx, float ty, float tz)
{
    result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void OpenglTestRenderer::MultiplyMatrix(glMatrix *result, glMatrix *srcA, glMatrix *srcB)
{
    glMatrix tmp;
    int i;

    for (i=0; i<4; i++)    {
        tmp.m[i][0] =    (srcA->m[i][0] * srcB->m[0][0]) +
                        (srcA->m[i][1] * srcB->m[1][0]) +
                        (srcA->m[i][2] * srcB->m[2][0]) +
                        (srcA->m[i][3] * srcB->m[3][0]) ;

        tmp.m[i][1] =    (srcA->m[i][0] * srcB->m[0][1]) +
                        (srcA->m[i][1] * srcB->m[1][1]) +
                        (srcA->m[i][2] * srcB->m[2][1]) +
                        (srcA->m[i][3] * srcB->m[3][1]) ;

        tmp.m[i][2] =    (srcA->m[i][0] * srcB->m[0][2]) +
                        (srcA->m[i][1] * srcB->m[1][2]) +
                        (srcA->m[i][2] * srcB->m[2][2]) +
                        (srcA->m[i][3] * srcB->m[3][2]) ;

        tmp.m[i][3] =    (srcA->m[i][0] * srcB->m[0][3]) +
                        (srcA->m[i][1] * srcB->m[1][3]) +
                        (srcA->m[i][2] * srcB->m[2][3]) +
                        (srcA->m[i][3] * srcB->m[3][3]) ;
    }

    memcpy(result, &tmp, sizeof(glMatrix));
}

void OpenglTestRenderer::RotationMatrix(glMatrix *result, float angle, float x, float y, float z)
{
    float sinAngle, cosAngle;
    float mag = sqrtf(x * x + y * y + z * z);

    sinAngle = sinf(angle * (float)M_PI / 180.0f);
    cosAngle = cosf(angle * (float)M_PI / 180.0f);

    if (mag > 0.0f)    {
        float xx, yy, zz, xy, yz, zx, xs, ys, zs;
        float oneMinusCos;
        glMatrix rotMat;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
        rotMat.m[1][0] = (oneMinusCos * xy) - zs;
        rotMat.m[2][0] = (oneMinusCos * zx) + ys;
        rotMat.m[3][0] = 0.0F;

        rotMat.m[0][1] = (oneMinusCos * xy) + zs;
        rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
        rotMat.m[2][1] = (oneMinusCos * yz) - xs;
        rotMat.m[3][1] = 0.0F;

        rotMat.m[0][2] = (oneMinusCos * zx) - ys;
        rotMat.m[1][2] = (oneMinusCos * yz) + xs;
        rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
        rotMat.m[3][2] = 0.0F;

        rotMat.m[0][3] = 0.0F;
        rotMat.m[1][3] = 0.0F;
        rotMat.m[2][3] = 0.0F;
        rotMat.m[3][3] = 1.0F;

        MultiplyMatrix(result, &rotMat, result);
    }
}

void OpenglTestRenderer::LoadIdentityMatrix(glMatrix *result)
{
    memset(result, 0x0, sizeof(glMatrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}

void OpenglTestRenderer::PerspectiveMatrix(glMatrix *result, float fovy, float aspect, float zNear, float zFar)
{
    glMatrix m;
    float sine, cotangent, deltaZ;
    float radians = fovy / 2.0f * (float)M_PI / 180.0f;

    deltaZ = zFar - zNear;
    sine = sinf(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
        return;
    }
    cotangent = cosf(radians) / sine;

    m.m[0][0] = cotangent / aspect; m.m[0][1] =                          0; m.m[0][2] =                          0; m.m[0][3] =  0;
    m.m[1][0] =                  0; m.m[1][1] =                  cotangent; m.m[1][2] =                          0; m.m[1][3] =  0;
    m.m[2][0] =                  0; m.m[2][1] =                          0; m.m[2][2] =   -(zFar + zNear) / deltaZ; m.m[2][3] = -1;
    m.m[3][0] =                  0; m.m[3][1] =                          0; m.m[3][2] = -2 * zNear * zFar / deltaZ; m.m[3][3] =  0;

    MultiplyMatrix(result, &m, result);
}

void OpenglTestRenderer::paint()
{
    // Play nice with the RHI. Not strictly needed when the scenegraph uses
    // OpenGL directly.
    m_window->beginExternalCommands();

    m_program->bind();
    m_program->enableAttributeArray(0);

    LoadIdentityMatrix(&modelview);
    TranslateMatrix(&modelview, 0.0f, 0.0f, -4.0f);
    RotationMatrix(&modelview, angle, 1.0f, 1.0f, 0.0);

//    angle = m_t;
    angle += 0.3f;
    if (angle > 360.0f)
        angle -= 360.0f;

    /* Compute the final MVP by multiplying the model-view and perspective matrices together */
    MultiplyMatrix(&mvp, &modelview, &projection);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

//    glUseProgram(m_program->programId());

    /* Enable cube array */
    glBindBuffer(GL_ARRAY_BUFFER, vertexID);

    glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(0));
    glVertexAttribPointer(color_loc,    3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(position_loc);
    glEnableVertexAttribArray(color_loc);

    /* Load the MVP matrix */
    glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, (GLfloat*)&mvp.m[0][0]);

    /* Finally draw the elements */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceID);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLushort), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));


    m_program->disableAttributeArray(0);
    m_program->release();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();

    m_window->endExternalCommands();
}
