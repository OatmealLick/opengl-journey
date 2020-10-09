//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"

#include "glad/glad.h"
#include "camera.h"
#include "camera_controller.h"
#include "pyramid.h"
#include <glm/glm.hpp>
#include <chrono>

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void scroll_callback(double xoffset, double yoffset) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void key_callback(int key, int scancode, int action, int mods) override;

    void set_camera(Camera *camera) { camera_ = camera; }

    Camera *camera() { return camera_; }

    void set_controller(CameraController *controller) { controller_ = controller; }

    CameraController *controller() { return controller_; }

    ~SimpleShapeApplication();

private:
    std::chrono::steady_clock::time_point start_;
    double lastFrame;
    float earth_rotation_period;
    float earth_orbital_rotation_period;
    float moon_rotation_period;
    float moon_orbital_rotation_period;
    float satellite_rotation_period;
    float satellite_orbital_rotation_period;
    Pyramid *pyramid;
    Camera *camera_;
    CameraController *controller_;

    GLuint u_pvm_buffer;

    static glm::mat4 self_rotation_matrix(float elapsed_time, float rotation_period) ;

    static glm::mat4 horizontal_orbit_translation_matrix(float elapsed_time, float orbit_width, float orbit_depth,
                                                         float orbital_rotation_period) ;

    static glm::mat4 vertical_orbit_translation_matrix(float elapsed_time, float orbit_width, float orbit_depth,
                                                float orbital_rotation_period);
};