cmake_minimum_required(VERSION 3.15)
project(PyramidAnimation)

set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${PROJECT_NAME}
        main.cpp
        app.h
        app.cpp
        $<TARGET_OBJECTS:glad>
        camera.h camera_controller.h camera_controller.cpp camera.cpp rotation.h pyramid.h pyramid.cpp obj_loader.cpp obj_loader.h model.cpp model.h)

target_link_libraries(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

