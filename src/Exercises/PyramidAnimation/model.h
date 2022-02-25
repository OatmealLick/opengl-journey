//
// Created by lick on 2/12/2022.
//

#ifndef INC_3DGRAPHICSEXAMPLES_MODEL_H
#define INC_3DGRAPHICSEXAMPLES_MODEL_H


#include <vector>
#include "glad/glad.h"
#include "glm/detail/type_mat4x4.hpp"

class Model {
public:
    Model(const std::string &path);
    ~Model() = default;

    void draw(glm::mat4 &PVM, GLuint u_pvm_buffer_) const;
    GLuint diffuseTexture;

private:
    GLuint vao_;
    std::vector<GLfloat> verticesData;
    std::vector<GLushort> indices;

};


#endif //INC_3DGRAPHICSEXAMPLES_MODEL_H
