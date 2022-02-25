//
// Created by lick on 2/12/2022.
//

#include <string>
#include <iostream>
#include "model.h"
#include "obj_loader.h"
#include "glm/glm.hpp"
#include "3rdParty/src/stb/stb_image.h"

Model::Model(const std::string &path) {
    ObjLoader::LoadObj(path, verticesData, indices);
    GLuint idx_buffer_handle;
    glGenBuffers(1, &idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(GLfloat), verticesData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          reinterpret_cast<GLvoid *>(5 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    int width, height, n_channels;
    auto filename = std::string(PROJECT_DIR) + "/resources/primitive-house-texture.png";
    uint8_t *data = stbi_load(filename.c_str(), &width, &height, &n_channels, 0);
    if (data) {
        std::cout << "Loaded texture of dimensions (" << width << ", " << height << ")" << std::endl;
    } else {
        std::cout << "No such file or directory: " << filename << std::endl;
    }
    glGenTextures(0, &diffuseTexture);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::draw(glm::mat4 &PVM, GLuint u_pvm_buffer_) const{
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * 4 * sizeof(float), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
