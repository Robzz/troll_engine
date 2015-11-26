#version 330

in float vertex_diffuse_intensity;

void main() {
    vec3 diffuseColor = vec3(1);
    gl_FragColor = vec4(diffuseColor*vertex_diffuse_intensity, 1);
}
