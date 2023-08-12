#include <iostream>
#include <glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// このコードは、GLUTを使ってウィンドウを作成し、stb_image.hを使って画像ファイルを読み込み、
// OpenGLのテクスチャを作成しています。テクスチャを使って四角形を描画する例を示しています。
// `loadTexture`関数に画像ファイルのパスを渡すことで、テクスチャIDを取得できます。
// このテクスチャIDを使って、OpenGLで描画することができます。

GLuint loadTexture(const char* filename) {
    
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    // std::cout << channels << std::endl;
    if (!image) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    //　テクスチャを生成
    glGenTextures(1, &texture);
    //　テクスチャをバインド
    glBindTexture(GL_TEXTURE_2D, texture);
    // 　テクスチャを拡大・縮小する方法の指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    } else {
        std::cerr << "Unsupported image format: " << filename << std::endl;
        stbi_image_free(image);
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    return texture;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, loadTexture("./imgs/hq720.jpg"));
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f,  0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL with GLUT");

    glutDisplayFunc(display);
    // glutReshapeFunc(resize);

    glutIdleFunc(display);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}