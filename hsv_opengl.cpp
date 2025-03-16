#include "hsv_opengl.h"

hsv_opengl::hsv_opengl(QObject  *parent)  : context(nullptr), surface(nullptr), texture(nullptr),
    hueAdjust(0.0f), satAdjust(1.0f), valAdjust(1.0f) {}

void hsv_opengl::initializeGL()
{
    // Initialize OpenGL context and surface
    context = new QOpenGLContext();
    context->setFormat(QSurfaceFormat::defaultFormat());
    surface = new QOffscreenSurface();
    surface->setFormat(context->format());
    surface->create();
    context->create();
    context->makeCurrent(surface);

    // Initialize OpenGL functions
    initializeOpenGLFunctions();

    // Create and configure the FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a texture to attach to the FBO
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        qWarning("Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Load Shader
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/Administrator/Desktop/Python/qt/texture2/hsvShader.vert")) {
        qWarning("Failed to load vertex shader");
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/Administrator/Desktop/Python/qt/texture2/hsvShader.frag")) {
        qWarning("Failed to load fragment shader");
    }

    if (!shaderProgram.link()) {
        qWarning("Failed to link shader program");
    }

    // Setup Quad
    float vertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    GLuint indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    context->doneCurrent();
}

void hsv_opengl::cleanupGL()
{
    if (context) {
        context->makeCurrent(surface);

        // Delete the FBO and its texture
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &fboTexture);

        context->doneCurrent();
    }
    delete texture;
    delete surface;
    delete context;
}

void hsv_opengl::run()
{
    if (!context) initializeGL();

    context->makeCurrent(surface);

    // Bind the FBO for offscreen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, img.width(), img.height());

    glClear(GL_COLOR_BUFFER_BIT);

    // Load Image as Texture
    if (texture) delete texture;
    texture = new QOpenGLTexture(img.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Bind the shader program and texture
    shaderProgram.bind();
    texture->bind(0); // Bind to texture unit 0
    shaderProgram.setUniformValue("image", 0); // Use texture unit 0

    // Pass HSV adjustments
    shaderProgram.setUniformValue("hueAdjust", hueAdjust);
    shaderProgram.setUniformValue("satAdjust", satAdjust);
    shaderProgram.setUniformValue("valAdjust", valAdjust);

    // Draw the quad
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shaderProgram.release();
    texture->release();

    // Read pixels from the FBO
    QImage result(img.width(), img.height(), QImage::Format_RGBA8888);
    glReadPixels(0, 0, result.width(), result.height(), GL_RGBA, GL_UNSIGNED_BYTE, result.bits());

    this->result = result.mirrored();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    context->doneCurrent();
}

void hsv_opengl::processImage(QImage img, float hue, float saturation, float value)
{
    hueAdjust = hue;
    satAdjust = saturation;
    valAdjust = value;
    this->img = img;

    run();
}

QImage hsv_opengl::getResult()
{
    return result;
}

hsv_opengl::~hsv_opengl()
{
    cleanupGL();
}

