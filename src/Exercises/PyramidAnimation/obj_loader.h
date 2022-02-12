//
// Created by lick on 2/12/2022.
//

#ifndef INC_3DGRAPHICSEXAMPLES_OBJ_LOADER_H
#define INC_3DGRAPHICSEXAMPLES_OBJ_LOADER_H


#include <string>
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glad/glad.h"

class ObjLoader {
public:
    static bool LoadObj(const std::string &path, std::vector<GLfloat> &verticesData, std::vector<GLushort> &indices);
};


#endif //INC_3DGRAPHICSEXAMPLES_OBJ_LOADER_H
