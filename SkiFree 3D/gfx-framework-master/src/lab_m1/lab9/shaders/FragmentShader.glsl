#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// TODO(student): Declare various other uniforms
uniform int toMix;
uniform float timeElapse;
uniform int isGlobe;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color1;
    vec4 color2;
    vec4 color;

    // Pentru schimbarea pozitiei texturii in functie de timp pe OX
    if (isGlobe == 1) {
        color1 = texture2D(texture_1, texcoord + vec2(timeElapse/30.5f, 0));
    } else {
        color1 = texture2D(texture_1, texcoord);
    }

    color2 = texture2D(texture_2, texcoord);
    
    if (toMix == 1) {
        color = mix(color1, color2, 0.5f);
    } else {
        color = color1;
    }
    
    out_color = color;
    if (out_color.a <= 0.5f) {
        discard;
    }

}
