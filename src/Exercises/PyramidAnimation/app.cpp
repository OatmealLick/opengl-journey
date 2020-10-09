//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application/utils.h"
#include "pyramid.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    auto *c = new Camera();
    set_camera(c);
    set_controller(new CameraController(c));

    pyramid = new Pyramid();

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    auto transformations_binding_point = 0;
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    } else {
        glUniformBlockBinding(program, u_transformations_index, transformations_binding_point);
    }

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    camera_->perspective(glm::radians(80.0f), (float) w / (float) h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 2.0f, 5.0f),
                     glm::vec3(0.0f, -2.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));

    glGenBuffers(1, &u_pvm_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_pvm_buffer);

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 4 * 4 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glUseProgram(program);

    start_ = std::chrono::steady_clock::now();
    earth_rotation_period = 4.0f;
    earth_orbital_rotation_period = 20.0f;
    moon_rotation_period = 10.0f;
    moon_orbital_rotation_period = moon_rotation_period;
    satellite_rotation_period = 2.0f;
    satellite_orbital_rotation_period = satellite_rotation_period;
}

void SimpleShapeApplication::frame() {
    double currentFrame = glfwGetTime();
    double deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    auto now = std::chrono::steady_clock::now();
//    float elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_).count();
    float elapsed_time = 0.0f;

    //earth
    auto R = self_rotation_matrix(elapsed_time, earth_rotation_period);
    float orbit_width = 10.0f;
    float orbit_depth = 8.0f;
    auto O = horizontal_orbit_translation_matrix(elapsed_time, orbit_width, orbit_depth, earth_orbital_rotation_period);
//    auto earth_PVM = camera_->projection() * camera_->view() * O * R;
    auto earth_PVM = camera_->projection() * camera_->view();
    pyramid->draw(earth_PVM, u_pvm_buffer);

    //moon
    //inverting rotation to mimic moon showing only one side to earth
    //todo invert rotation with parameter way, not that hacky way with minus period
    auto S_moon = glm::scale(glm::mat4(1.0f), glm::vec3{0.5f, 0.5f, 0.5f});
    auto R_moon = self_rotation_matrix(elapsed_time, -moon_rotation_period);
    auto O_moon = horizontal_orbit_translation_matrix(elapsed_time, 3.0f, 3.0f, moon_orbital_rotation_period);
    auto moon_PVM = camera_->projection() * camera_->view() * O * O_moon * R_moon * S_moon;
    pyramid->draw(moon_PVM, u_pvm_buffer);

    //satellite
    auto S_satellite = glm::scale(glm::mat4(1.0f), glm::vec3{0.25f, 0.25f, 0.25f});
    auto R_satellite = self_rotation_matrix(elapsed_time, satellite_rotation_period);
    auto O_satellite = vertical_orbit_translation_matrix(elapsed_time, 1.5f, 1.5f, satellite_orbital_rotation_period);
    auto satellite_PVM = camera_->projection() * camera_->view() * O * O_satellite * R_satellite * S_satellite;
    pyramid->draw(satellite_PVM, u_pvm_buffer);

    controller_->Update(deltaTime);
}

glm::mat4 SimpleShapeApplication::horizontal_orbit_translation_matrix(float elapsed_time,
                                                                      float orbit_width,
                                                                      float orbit_depth,
                                                                      float orbital_rotation_period) {
    auto orbital_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / orbital_rotation_period;
    auto translation_vector = glm::vec3{(orbit_width * cos(orbital_rotation_angle)),
                                        0.0f,
                                        (orbit_depth * sin(orbital_rotation_angle))
    };
    return glm::translate(glm::mat4(1.0f), translation_vector);
}

glm::mat4 SimpleShapeApplication::vertical_orbit_translation_matrix(float elapsed_time,
                                                                    float orbit_width,
                                                                    float orbit_depth,
                                                                    float orbital_rotation_period) {
    auto orbital_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / orbital_rotation_period;
    auto translation_vector = glm::vec3{(orbit_width * cos(orbital_rotation_angle)),
                                        (orbit_depth * sin(orbital_rotation_angle)),
                                        0.0f
    };
    return glm::translate(glm::mat4(1.0f), translation_vector);
}

glm::mat4 SimpleShapeApplication::self_rotation_matrix(float elapsed_time,
                                                       float rotation_period) {
    auto rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / rotation_period;
    return glm::rotate(glm::mat4(1.0f), rotation_angle, glm::vec3{0.0f, 1.0f, 0.0f});
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    const float aspect_ = (float) w / (float) h;
    camera_->set_aspect(aspect_);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera_->zoom(-(float) yoffset / 30.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controller_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controller_->LMB_pressed((float) x, (float) y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controller_->LMB_released((float) x, (float) y);
    }

}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);

    if (controller_) {
        controller_->mouse_moved((float) x, (float) y);
    }
}

SimpleShapeApplication::~SimpleShapeApplication() {
    if (camera_) {
        delete camera_;
    }
}

void SimpleShapeApplication::key_callback(int key, int scancode, int action, int mods) {
    Application::key_callback(key, scancode, action, mods);

    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            controller_->Start(-1, 0);
        } else if (action == GLFW_RELEASE) {
            controller_->Stop(-1, 0);
        }
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            controller_->Start(1, 0);
        } else if (action == GLFW_RELEASE) {
            controller_->Stop(1, 0);
        }
    }
    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            controller_->Start(0, 1);
        } else if (action == GLFW_RELEASE) {
            controller_->Stop(0, 1);
        }
    }
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            controller_->Start(0, -1);
        } else if (action == GLFW_RELEASE) {
            controller_->Stop(0, -1);
        }
    }
}
