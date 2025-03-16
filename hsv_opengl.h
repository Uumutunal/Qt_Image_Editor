#ifndef HSV_OPENGL_H
#define HSV_OPENGL_H

#include <QThread>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOffscreenSurface>
#include <QImage>

class hsv_opengl:public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit hsv_opengl(QObject *parent = nullptr);
    ~hsv_opengl();
    void processImage(QImage img, float hue, float saturation, float value);

    QImage getResult();

protected:
    void initializeGL();
    void cleanupGL();
    void run();

private:
    QOpenGLContext *context;
    QOffscreenSurface *surface;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLTexture *texture;
    GLuint VBO, VAO, EBO;
    GLuint fbo, fboTexture;
    float hueAdjust, satAdjust, valAdjust;
    QImage img;
    QImage result;
};

#endif // HSV_OPENGL_H
