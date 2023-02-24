#version 330

// Input
// TODO(student): Get vertex attributes from each location

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float timeElapse;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 normal_color;

void main()
{
    // TODO(student): Send output to fragment shader
    uniform vec3 lightPosition;
    uniform vec3 eyePos;
    uniform float materialKd;
    uniform float materialKs;
    uniform int materialShininess;

uniform vec3 object_color;
    frag_color = v_color;
    // EX 5
    normal_color = 0.3f + v_normal * sin(2*timeElapse);

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position + 0.5f * (sin(2*timeElapse) + cos(2*timeElapse)) , 1.0);

}


