//
// Created by lick on 2/12/2022.
//

#include <iostream>
#include "obj_loader.h"
#include <fstream>
#include <sstream>
#include <string>

bool ObjLoader::LoadObj(const std::string &path,
                        std::vector<GLfloat> &verticesData,
                        std::vector<GLushort> &indices) {
    std::ifstream infile(path);
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string header;
        if (!(iss >> header)) {
            std::cerr << "ERROR" << std::endl;
            break;
        }
        std::cout << "processing " << header << std::endl;
        if (header == "#") {
            continue;
        } else if (header == "v") {
            float one, two, three;
            if (!(iss >> one >> two >> three)) {
                std::cerr << "ERROR" << std::endl;
            }
            std::cout << "Adding v " << one << ", " << two << ", " << three << std::endl;
            vertices.emplace_back(one, two, three);
        } else if (header == "vt") {
            float one, two;
            if (!(iss >> one >> two)) {
                std::cerr << "ERROR" << std::endl;
            }
            std::cout << "Adding vt " << one << ", " << two << std::endl;
            uvs.emplace_back(one, 1 - two);
        } else if (header == "vn") {
            float one, two, three;
            if (!(iss >> one >> two >> three)) {
                std::cerr << "ERROR" << std::endl;
            }
            std::cout << "Adding vt " << one << ", " << two << ", " << three << std::endl;
            normals.emplace_back(one, two, three);
        } else if (header == "f") {
            char sep;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            for (int i = 0; i < 3; ++i) {
                if (!(iss >> vertexIndex[i] >> sep >> uvIndex[i] >> sep >> normalIndex[i])) {
                    std::cerr << "ERROR" << std::endl;
                }
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
        }
    }

    return true;
}
