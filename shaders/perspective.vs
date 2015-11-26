#version 330

in vec3 v_position;
in vec3 v_normal;

out vec3 vs_normal;

uniform mat4 m_modelView;

void main() {
    gl_Position = m_modelView * vec4(v_position, 1);
    vs_normal = v_normal;
}
