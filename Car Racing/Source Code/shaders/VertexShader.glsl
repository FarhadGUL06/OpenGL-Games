#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform int use_color;
uniform int type_of_render;
uniform vec3 pos_car;

// Output
out vec3 color;


void main()
{
    if (use_color == 1) {
        // Daca e inamic, punem culoarea pusa de noi
        color = object_color;
    } else {
        color = v_color;
    }

    if (type_of_render == 0) {
        // Shader pentru jocul principal
        float scaleFactor = 0.01f;
        vec4 new_pos = Model * vec4(v_position, 1.0);
        vec3 working_pos = new_pos.xyz;
        new_pos.y = new_pos.y - dot(pos_car - working_pos, pos_car - working_pos) * scaleFactor;
        gl_Position = Projection * View * new_pos;
    } else {
        gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    }
}
