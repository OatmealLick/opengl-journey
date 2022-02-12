//
// Created by lick on 2/12/2022.
//

#include <iostream>
#include <cstring>
#include "obj_loader.h"

bool ObjLoader::LoadObj(const std::string &path,
                        std::vector<GLfloat> &verticesData,
                        std::vector<GLushort> &indices) {
    FILE *file = fopen(path.c_str(), "r");
    if (file == nullptr) {
        std::cerr << "File " << path << " not found or could not be opened." << std::endl;
        return false;
    }

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    char line[128];
    while (true) {
        int res = fscanf(file, "%s", line);
        if (res == EOF) {
            break;
        }

        if (strcmp(line, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        } else if (strcmp(line, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uvs.push_back(uv);
        } else if (strcmp(line, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        } else if (strcmp(line, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("hmpf");
                return false;
            }

            for (int i = 0; i < 3; ++i) {
                indices.push_back(int(verticesData.size() / 8));

                auto vertex = vertices[vertexIndex[i] - 1];
                verticesData.push_back(vertex.x);
                verticesData.push_back(vertex.y);
                verticesData.push_back(vertex.z);

                auto uv = uvs[uvIndex[i] - 1];
                verticesData.push_back(uv.x);
                verticesData.push_back(uv.y);

                auto normal = normals[normalIndex[i] - 1];
                verticesData.push_back(normal.x);
                verticesData.push_back(normal.y);
                verticesData.push_back(normal.z);
            }

//            vertexIndices.push_back(vertexIndex[0]);
//            vertexIndices.push_back(vertexIndex[1]);
//            vertexIndices.push_back(vertexIndex[2]);
//            uvIndices.push_back(uvIndex[0]);
//            uvIndices.push_back(uvIndex[1]);
//            uvIndices.push_back(uvIndex[2]);
//            normalIndices.push_back(normalIndex[0]);
//            normalIndices.push_back(normalIndex[1]);
//            normalIndices.push_back(normalIndex[2]);
        }
    }

    return true;
}
