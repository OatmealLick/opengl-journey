//
// Created by lick on 12.11.2020.
//

#pragma once

#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "rotation.h"

class Camera {
public:

    void translation(const glm::vec3 &translationVector);

    void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);

    void perspective(float fov, float aspect, float near, float far);

    void set_aspect(float aspect) {
        aspect_ = aspect;
    }

    glm::mat4 view() const;

    glm::mat4 model() const { return M_; }

    glm::mat4 projection() const;

    glm::vec3 x() const { return x_; }

    glm::vec3 y() const { return y_; }

    glm::vec3 z() const { return z_; }

    glm::vec3 position() const { return position_; }

    glm::vec3 center() const { return center_; }

    void zoom(float y_offset);

    void rotate_around_center(float angle, const glm::vec3 &axis);

    void Move(const glm::vec3 &offset) { position_ += offset; }

private:
    float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 M_;

    glm::vec3 position_;
    glm::vec3 center_;
    glm::vec3 x_;
    glm::vec3 y_;
    glm::vec3 z_;

    static float logistic(float y) {
        return 1.0f / (1.0f + std::exp(-y));
    }

    static float inverse_logistic(float x) {
        return std::log(x / (1.0f - x));
    }

    void rotate_around_point(float angle, const glm::vec3 &axis, const glm::vec3 &c);
};