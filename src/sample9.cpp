#include <iostream>
// #include <glut.h>
#include <GL/glut.h> // GLUTライブラリを使用
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texture;
const char *imagePath = "./imgs/hq720.jpg";

// GLuint loadTexture(const char *imagePath) {
//     int width, height, channels;
//     unsigned char *data = stbi_load(imagePath, &width, &height, &channels, 0);
//     if (data) {
//         GLuint textureID;
//         glGenTextures(1, &textureID);
//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//         stbi_image_free(data);
//         return textureID;
//     } else {
//         std::cerr << "Failed to load image: " << imagePath << std::endl;
//         return 0;
//     }
// }

GLuint loadTexture(const char *imagePath) {
    // OpenGLのテクスチャ座標系が画像の左下隅を原点としているのに対し、stb_imageは左上隅を原点としているため
    stbi_set_flip_vertically_on_load(true); // 追加
    int width, height, channels;
    unsigned char *data = stbi_load(imagePath, &width, &height, &channels, 0);
    if (data) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        if (channels == 1)
            format = GL_LUMINANCE;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unsupported number of channels: " << channels << std::endl;
            stbi_image_free(data);
            return 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        return textureID;
    } else {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return 0;
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
        // OpenGLのテクスチャ座標系は、左下隅が原点（0,0）で、右上隅が（1,1）、テクスチャ座標は、通常、UV座標とも呼ばれます
        // 左下の頂点
        glTexCoord2f(0.0, 0.0); // テクスチャ座標（左下）
        glVertex2f(-1.0, -1.0); // 頂点座標

        // 右下の頂点
        glTexCoord2f(1.0, 0.0); // テクスチャ座標（右下）
        glVertex2f(1.0, -1.0); // 頂点座標

        // 右上の頂点
        glTexCoord2f(1.0, 1.0); // テクスチャ座標（右上）
        glVertex2f(1.0, 1.0); // 頂点座標

        // 左上の頂点
        glTexCoord2f(0.0, 1.0); // テクスチャ座標（左上）
        glVertex2f(-1.0, 1.0); // 頂点座標

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Image Display");
    texture = loadTexture(imagePath);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}