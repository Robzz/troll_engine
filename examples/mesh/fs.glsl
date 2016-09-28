#version 330

uniform vec3 teapotColor;

void main() {
    gl_FragColor = vec4(teapotColor, 1.f);
}
