//
// Created by lick on 14.11.2020.
//

#include "camera.h"
#include "glm/glm.hpp"

void Camera::translation(const glm::vec3 &translationVector) {
    const glm::mat4 I(1.0f);
    M_ = glm::translate(I, translationVector);
}

void Camera::look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
    z_ = glm::normalize(eye - center);
    x_ = glm::normalize(glm::cross(up, z_));
    y_ = glm::normalize(glm::cross(z_, x_));
    position_ = eye;
    center_ = center;
}

void Camera::perspective(float fov, float aspect, float near, float far) {
    fov_ = fov;
    aspect_ = aspect;
    near_ = near;
    far_ = far;
}

glm::mat4 Camera::view() const {
    glm::mat4 V(1.0f);
    for (int i = 0; i < 3; ++i) {
        V[i][0] = x_[i];
        V[i][1] = y_[i];
        V[i][2] = z_[i];
    }

    auto t = -glm::vec3{
            glm::dot(x_, position_),
            glm::dot(y_, position_),
            glm::dot(z_, position_),
    };
    V[3] = glm::vec4(t, 1.0f);

    return V;

}

void Camera::zoom(float y_offset) {
    auto x = fov_ / glm::pi<float>();
    auto y = inverse_logistic(x);
    y += y_offset;
    x = logistic(y);
    fov_ = x * glm::pi<float>();
}

void Camera::rotate_around_center(float angle, const glm::vec3 &axis) {
    rotate_around_point(angle, axis, center_);
}

glm::mat4 Camera::projection() const {
    auto far = 10.f;
    return glm::ortho(-far, far, -far, far, -far, 1000.f);
//    return glm::ortho(0.f, 600.f, 0.f, 800.f, 0.1f, 1000.f);
//    return glm::perspective(fov_, aspect_, near_, far_);
}

void Camera::rotate_around_point(float angle, const glm::vec3 &axis, const glm::vec3 &c) {
    auto R = rotation(angle, axis);
    x_ = R * x_;
    y_ = R * y_;
    z_ = R * z_;

    auto t = position_ - c;
    t = R * t;
    position_ = c + t;

}
