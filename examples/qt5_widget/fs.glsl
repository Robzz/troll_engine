#version 330

uniform vec3 triangleColor;

void main() {
    gl_FragColor = vec4(triangleColor, 1.f);
}
