#version 330

in vec4 v_position;

uniform mat4 m_modelView;

void main() {
    gl_Position = m_modelView * v_position;
}
