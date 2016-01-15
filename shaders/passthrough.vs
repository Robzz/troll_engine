#version 330

in vec3 v_position;
in vec2 v_texCoord;

out vec3 vs_position;
out vec2 vs_texCoord;

uniform mat4 m_camera;

void main() {
    gl_Position = m_camera * vec4(v_position, 1);
    vs_texCoord = v_texCoord;
}
