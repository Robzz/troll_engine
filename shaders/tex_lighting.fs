#version 330

in vec3 vs_normal;
in vec3 vs_position;
in vec2 vs_texCoord;

uniform float ambient_intensity = 0.2;
uniform vec3 diffuseColor = vec3(0.2, 0.2, 0.6);
uniform vec3 v_lightPosition = vec3(0, 0, 0);
uniform mat4 m_world;
uniform sampler2D u_tex;

void main() {
    vec3 texSample = texture2D(u_tex, vs_texCoord).xyz;
    vec3 ambient = ambient_intensity * vec3(1); 
    vec3 worldPos = vec3(m_world * vec4(vs_position, 1));
    vec3 L = normalize(v_lightPosition - worldPos), N = normalize(vs_normal);
    vec3 diffuse = clamp(dot(L, N), 0, 1) * texSample;
    gl_FragColor = clamp(vec4(ambient + diffuse, 1),
                         vec4(0),
                         vec4(1));
}
