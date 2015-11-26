#version 330

in vec3 v_position;
in vec3 v_normal;

out vec3 vs_normal;
out float vertex_diffuse_intensity;

uniform mat4 m_world;
uniform mat4 m_proj;
uniform mat3 m_normalTransform;
uniform vec3 v_lightPosition;

void main() {
    gl_Position = m_proj * m_world * vec4(v_position, 1);
    vs_normal = m_normalTransform * v_normal;
    vec3 worldPos = vec3(m_world * vec4(v_position, 1));
    vec3 L = normalize(v_lightPosition - worldPos);
    vertex_diffuse_intensity = clamp(dot(L, vs_normal), 0, 1);
}
