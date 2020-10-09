//
// Created by lick on 18.11.2020.
//

#pragma once

#include "glad/glad.h"
#include <vector>


class Pyramid  {
public:
    Pyramid();
    ~Pyramid() = default;
    void draw(glm::mat4 &PVM, GLuint u_pvm_buffer_) const;
private:
    GLuint vao_;
    GLuint buffer_[2]; //todo for now there is no need for it to be field
    std::vector<GLfloat> vertices = {
            -0.5f, -0.5f, 0.5f, 0.8f, 0.1f, 0.8f,
            0.5f, -0.5f, 0.5f, 0.8f, 0.1f, 0.8f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.1f, 0.8f, // here ends first triangle

            0.5f, -0.5f, 0.5f, 0.1f, 0.8f, 0.8f,
            0.5f, -0.5f, -0.5f, 0.1f, 0.8f, 0.8f,
            0.0f, 0.5f, 0.0f, 0.1f, 0.8f, 0.8f, // here ends second triangle

            0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.8f,
            -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.8f,
            0.0f, 0.5f, 0.0f, 0.2f, 0.2f, 0.8f, // here ends third triangle

            -0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.3f,
            -0.5f, -0.5f, 0.5f, 0.8f, 0.8f, 0.3f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.3f, // here ends fourth triangle

            -0.5f, -0.5f, 0.5f, 0.3f, 0.1f, 0.3f,
            0.5f, -0.5f, 0.5f, 0.3f, 0.1f, 0.3f,
            -0.5f, -0.5f, -0.5f, 0.3f, 0.1f, 0.3f, // here ends base 1 - ABD
            0.5f, -0.5f, -0.5f, 0.3f, 0.1f, 0.3f, // here ends base 2 - DBC
    };

    std::vector<GLushort> indices = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            14, 13, 12,
            15, 13, 14
    };
};