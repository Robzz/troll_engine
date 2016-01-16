#version 330

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoord;

out vec3 vs_normal;
out vec3 vs_position;
out vec2 vs_texCoord;

uniform mat4 m_camera;
uniform mat4 m_world;
uniform mat4 m_proj;
uniform mat3 m_normalTransform;

void main() {
    gl_Position = m_proj * m_camera * m_world * vec4(v_position, 1);
    vs_normal = m_normalTransform * v_normal;
    vs_position = v_position;
    vs_texCoord = v_texCoord;
}
