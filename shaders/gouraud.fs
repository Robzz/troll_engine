#version 330

in float vertex_diffuse_intensity;

uniform float ambient_intensity = 0.2;

void main() {
    vec3 diffuseColor = vec3(1);
    vec3 ambient = ambient_intensity * diffuseColor;
    vec3 diffuse = vertex_diffuse_intensity * diffuseColor;
    gl_FragColor = clamp(vec4(ambient + diffuse, 1),
                         vec4(0),
                         vec4(1));
}
