
#include "camera_controller.h"

void CameraController::rotate_camera(float dx, float dy) {
    camera_->rotate_around_center(-scale_ * dy, camera_->x());
    // I feel like rotation around y axis is more natural for horizontal movement
    camera_->rotate_around_center(-scale_ * dx, glm::vec3{0.0f, 1.0f, 0.0f});
}

void CameraController::mouse_moved(float x, float y) {
    if (LMB_pressed_) {
        auto dx = x - x_;
        auto dy = y - y_;
        x_ = x;
        y_ = y;

        rotate_camera(dx, dy);
    }
}

void CameraController::LMB_pressed(float x, float y) {
    LMB_pressed_ = true;
    x_ = x;
    y_ = y;
}

void CameraController::LMB_released(float x, float y) {
    LMB_pressed_ = false;
    auto dx = x - x_;
    auto dy = y - y_;

    rotate_camera(dx, dy);
}
void CameraController::Start(int8_t vertical, int8_t horizontal) {
    if (this->vertical == 0) {
        this->vertical = vertical;
    }
    if (this->horizontal == 0) {
        this->horizontal = horizontal;
    }
}

void CameraController::Stop(int8_t vertical, int8_t horizontal) {
    if (vertical != 0) {
        this->vertical = 0;
    }
    if (horizontal != 0) {
        this->horizontal = 0;
    }
}

glm::vec3 CameraController::PanningOffset() const {
    return glm::normalize(glm::vec3{-horizontal, 0.0f, vertical}) * panAmount;
}

void CameraController::Update(double deltaTime) {
    if (IsPanning()) {
        camera_->Move(PanningOffset() * (float)deltaTime);
    }
}
