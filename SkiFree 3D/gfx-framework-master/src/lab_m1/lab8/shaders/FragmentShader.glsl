#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform ivec2 is_spotty;
uniform vec2 cut_off;

// Mai multe surse de lumina
uniform vec3 point_light_pos[2];
uniform vec3 point_light_dir[2];


// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec4 point_light_contribution(vec3 light_pos, vec3 light_dir, int i)
{
	vec4 out_color_aux;
	vec3 L = normalize(light_pos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 N = normalize(world_normal);

    // TODO(student): Define ambient light component
    float ambient_light = 0.25 * material_kd;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * max(dot(N, L), 0);

    // TODO(student): Compute specular light component
    float specular_light =  0;


    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
    // Am specular light doar daca am difuze light
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);    
    }


    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float light = 0;
    if (is_spotty[i] == 1) {
        float cutoff = radians(cut_off[i]);
        float spot_light = dot(-L, light_dir);
        float spot_light_limit = cos(cutoff);
        if (spot_light > spot_light_limit)
        {
	        // fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	        // se calculeaza atenuarea luminii
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
            light = ambient_light + light_att_factor * (diffuse_light + specular_light);
        } else {
            // Sunt in afara spotty
            light = ambient_light;
        }
    } else {
        // TODO(student): Compute light
	    light = ambient_light + diffuse_light + specular_light;
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
    out_color_aux = vec4(object_color * light, 1);
	return out_color_aux;
}

void main()
{
    // TODO(student): Define ambient, diffuse and specular light components
    // TODO(student): Compute world space vectors
    int i;
    out_color = glm::vec4(0, 0, 0, 0);
    for (i = 0; i < 2; i++) {
        vec4 out_color_aux = point_light_contribution(point_light_pos[i], point_light_dir[i], i);
        out_color = out_color + out_color_aux;
    }
}
