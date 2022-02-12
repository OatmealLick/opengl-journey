#version 410

in vec2 uv;
in vec3 normal;

layout(location=0) out vec4 vFragColor;
void main() {
    vFragColor.a = 1.f;
    vFragColor.rgb = vec3(0.3f, 0.3f, 1.0f);
}
