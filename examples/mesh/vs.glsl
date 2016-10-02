#version 330

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoord;

out vec3 positionWorld;
out vec3 normal;
out vec2 texCoord;

uniform mat4 camera;
uniform mat4 modelView;
uniform mat4 projection;
uniform mat3 normalTransform;

void main() {
    vec4 worldPos = modelView * vec4(v_position, 1.f);
    gl_Position = projection * camera * worldPos;
    positionWorld = worldPos.xyz;
    normal = normalTransform * v_normal;
    texCoord = v_texCoord;
}
