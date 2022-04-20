#ifndef OPENGLTEST_H
#define OPENGLTEST_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

typedef struct
{
    GLfloat m[4][4];
}glMatrix;

#define PROJECTION_FAR        30.0f
#define PROJECTION_FOVY        30.0f
#define PROJECTION_NEAR        0.1f
#define PI                    3.1415926534f
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class OpenglTestRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenglTestRenderer() : m_t(0), m_program(0) { }
    ~OpenglTestRenderer();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

public slots:
    void init();
    void paint();

private:
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;

    void TranslateMatrix(glMatrix *result, float tx, float ty, float tz);
    void MultiplyMatrix(glMatrix *result, glMatrix *srcA, glMatrix *srcB);
    void RotationMatrix(glMatrix *result, float angle, float x, float y, float z);
    void LoadIdentityMatrix(glMatrix *result);
    void PerspectiveMatrix(glMatrix *result, float fovy, float aspect, float zNear, float zFar);

    float angle = 0.0f;
    glMatrix projection;
    glMatrix modelview;
    glMatrix mvp;

    GLuint vertexID;
    GLuint indiceID;
    GLuint program_object = 0;
    GLuint position_loc;
    GLuint color_loc;
    GLuint mvp_matrix_loc;

    GLushort indices[6][6] =
    {
        0, 2, 3, 0, 3, 1,    // front
        2, 4, 5, 2, 5, 3,    // left
        4, 6, 7, 4, 7, 5,    // back
        6, 0, 1, 6, 1, 7,    // right
        0, 6, 4, 0, 4, 2,    // top
        1, 3, 5, 1, 5, 7     // bottom
    };

    GLfloat vertices[8][6] =
    {
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,        // 0
         0.5f, -0.5f, -0.5f, 1.0f,    0,    0,        // 1
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,    0,        // 2
        -0.5f, -0.5f, -0.5f, 1.0f,    0, 1.0f,        // 3
        -0.5f,  0.5f,  0.5f,    0, 1.0f, 1.0f,        // 4
        -0.5f, -0.5f,  0.5f,    0, 1.0f,    0,        // 5
         0.5f,  0.5f,  0.5f,    0,    0, 1.0f,        // 6
         0.5f, -0.5f,  0.5f,  0.5f, 1.0f, 0.5         // 7
    };
};

class OpenglTest : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    QML_ELEMENT

public:
    OpenglTest();

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    qreal m_t;
    OpenglTestRenderer *m_renderer;
};

#endif // OPENGLTEST_H
