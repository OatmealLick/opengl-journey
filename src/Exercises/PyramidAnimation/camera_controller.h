#pragma once

#include "camera.h"

class CameraController {
public:
    CameraController(Camera *camera) : camera_(camera),
                                       scale_(0.03f),
                                       LMB_pressed_(false),
                                       panAmount(3.0f),
                                       horizontal(0),
                                       vertical(0) {}

    void rotate_camera(float dx, float dy);

    void mouse_moved(float x, float y);

    void LMB_pressed(float x, float y);

    void LMB_released(float x, float y);

    void Update(double deltaTime);

    void Start(int8_t vertical, int8_t horizontal);
    void Stop(int8_t vertical, int8_t horizontal);
private:
    inline bool IsPanning() const { return vertical != 0 || horizontal != 0; }
    glm::vec3 PanningOffset() const;

    float panAmount;
    int8_t vertical;
    int8_t horizontal;

    Camera *camera_;
    float scale_;
    bool LMB_pressed_;
    float x_;
    float y_;
};