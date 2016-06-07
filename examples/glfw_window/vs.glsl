#version 330

in vec3 v_position;

void main() {
    gl_Position = vec4(v_position, 1.f);
}
