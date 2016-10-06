#version 330

in vec3 v_position;
in vec2 v_texCoord;

out vec2 texCoord;

uniform mat4 camera;
uniform mat4 modelView;
uniform mat4 projection;

void main() {
    vec4 worldPos = modelView * vec4(v_position, 1.f);
    gl_Position = projection * worldPos;
    texCoord = v_texCoord;
}
