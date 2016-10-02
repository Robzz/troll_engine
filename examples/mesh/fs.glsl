#version 330

// Calculations are done in world space

in vec2 texCoord;
in vec3 normal;
in vec3 positionWorld;

uniform sampler2D tex;
uniform vec3 cameraWorldPos;

layout(std140) uniform Light {
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 position;
} light;

layout(std140) uniform Material {
    vec3 Ks;
    float alpha;
    vec3 Ka;
} material;

vec4 phong_diffuse(vec3 L, vec3 N) {
    return vec4(light.diffuseColor * dot(L, N), 1);
}

vec4 phong_specular(vec3 R, vec3 V) {
    return pow(dot(R, V), material.alpha) * vec4(material.Ks, 1);
}

void main() {
    vec3 N = normalize(normal);
    vec3 L = normalize(light.position - positionWorld);
    vec3 R = reflect(L,N);
    vec3 V = normalize(cameraWorldPos - positionWorld);
    gl_FragColor = vec4(material.Ka, 1) +
                   texture2D(tex,texCoord) * phong_diffuse(L, N) +
                   vec4(material.Ks, 1) * phong_specular(R, V);
}
