#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec2 a_uv;
layout(location=2) in  vec3 a_normal;
layout(std140) uniform Transformations {
    mat4 pvm;
};
out vec2 uv;
out vec3 normal;

void main() {
    gl_Position = pvm * a_vertex_position;
    uv = a_uv;
    normal = a_normal;
}
