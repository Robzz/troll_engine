#version 330

in vec2 texCoord;

uniform sampler2D tex;

layout(std140) uniform Options {
    bool softenEdges;
    bool outline;
    bool glow;
    float softenRange;
} options;

void main() {
    vec4 color = texture2D(tex, texCoord);
    if(options.softenEdges) {
        float f = options.softenRange / 2;
        if(color.r < 0.5f - f)
            discard;
        else if(color.r > 0.5f + f)
            gl_FragColor = vec4(1, 1, 1, 1);
        else {
            gl_FragColor = vec4(vec3(1), smoothstep(0.5f - f, 0.5f + f, color.r));
        }
    }
    else if(color.r < 0.5f)
        discard;
    else
        gl_FragColor = vec4(1);
}
