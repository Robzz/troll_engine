#version 330

in vec3 v_position;
uniform mat4 modelView;

void main() {
    gl_Position = modelView * vec4(v_position, 1.f);
}
