#version 330

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// Uniform properties
uniform sampler2D texture;

// TODO(student): Declare various other uniforms
uniform float movingSpeedOX, movingSpeedOZ;
uniform int isPlane;

// Mai multe surse de lumina
uniform int noStalpi;
uniform vec3 posStalpi[500];

uniform int noLightPoint;
uniform vec3 posLightPoint[500];
uniform vec3 colorLightPoint[5];

// Output
layout(location = 0) out vec4 out_color;


float point_light_directional(vec4 object_color)
{	
    vec4 out_color_aux;

    // LUMINA DIRECTIONALA
	vec3 L = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 N = normalize(world_normal);

    float ambient_light = 0;// 0.25 * material_kd;

    float diffuse_light = material_kd * max(dot(N, L), 0);

    float specular_light = 0;

    float light_directional = ambient_light + diffuse_light + specular_light;
    return light_directional;
}

float spot_light_stalpi_each(vec3 posSpot)
{	
	vec3 L = normalize(posSpot - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 N = normalize(world_normal);

    float diffuse_light = material_kd * max(dot(N, L), 0);

    float ambient_light = 0;

    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);    
    }

    // LUMINA STALPILOR
    float light_objects = 0;

    float cutoff = radians(30.0f);
    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(cutoff);
    if (spot_light > spot_light_limit)
    {
        float atenuare_liniara = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float factor_de_atenuare = pow(atenuare_liniara, 2);
        light_objects = ambient_light + factor_de_atenuare * (diffuse_light + specular_light);
    } else {
        // Sunt in afara spotty
        light_objects = 0;
    }
	return light_objects;
}


float spot_light_stalpi(vec3 posObj)
{	
    // LUMINA STALPILOR
    float light_spot1 = spot_light_stalpi_each(posObj + vec3(-0.6f, 1.7f, 0.5f));
    float light_spot2 = spot_light_stalpi_each(posObj - vec3(-0.6f, -1.7f, -0.5f));
	return light_spot1 + light_spot2;
}

float light_point_compute(vec3 posObj, int i, int indiceColor, vec3 colorEachLightPoint)
{	
    // LUMINA CADOURILOR SI A BRAZILOR

    float randX = colorEachLightPoint[0];
    float randY = colorEachLightPoint[1];
    float randZ = colorEachLightPoint[2];


    float colorObj = 1;//vec3 (randX, randY, randZ);

    if (indiceColor == 0) {
        colorObj = randX;
    }
    if (indiceColor == 1) {
        colorObj = randY;
    }
    if (indiceColor == 2) {
        colorObj = randZ;
    }


    vec3 L = normalize(posObj + vec3(0, 0.8f, 0) - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 N = normalize(world_normal);
    vec3 R = reflect (-L, world_normal);

    float diffuse_light = 2 * material_kd * colorObj * max(dot(N, L), 0);

    float ambient_light = 0;// 0.25 * material_kd;

    float specular_light =  0;

    if (diffuse_light > 0)
    {
        specular_light = 1.2f * material_ks * colorObj * pow(max(dot(N, H), 0), material_shininess);    
    }
    float light_point;

    // Atenuarea luminii
    float d	= distance(posObj + vec3(0, 0.8f, 0), world_position);
	float factorAtenuare = 1 / (1 + d * d);
    light_point =  factorAtenuare * (diffuse_light + specular_light);
	return light_point;
}

void main()
{

    vec4 color;

    // Pentru schimbarea pozitiei texturii in functie de timp pe OX

    if (isPlane == 1) {
        color = texture2D(texture, texcoord + vec2(movingSpeedOX, movingSpeedOZ));
    } else {
        color = texture2D(texture, texcoord);
    }
    
    int i;
    float light_directional = point_light_directional(color);
    float ambient_light = 0.25 * material_kd;
    float light_stalpi = 0;
    for (i = 0; i < noStalpi; i++) {
        //vec4 out_color_aux = spot_light_stalpi(color, posStalpi[i]);
        //color = color + out_color_aux;
        light_stalpi += spot_light_stalpi(posStalpi[i]);
    }

    float light_pointR = 0;
    float light_pointG = 0;
    float light_pointB = 0;
    for (i = 0; i < noLightPoint; i++) {
        int randPos = int(posLightPoint[i][2]);
        randPos = randPos % 5;
        light_pointR += light_point_compute(posLightPoint[i], i, 0, colorLightPoint[randPos]);
        light_pointG += light_point_compute(posLightPoint[i], i, 1, colorLightPoint[randPos]);
        light_pointB += light_point_compute(posLightPoint[i], i, 2, colorLightPoint[randPos]);
    }
    color[0] = color[0] * (ambient_light + light_directional + light_stalpi + light_pointR);
    color[1] = color[1] * (ambient_light + light_directional + light_stalpi + light_pointG);
    color[2] = color[2] * (ambient_light + light_directional + light_stalpi + light_pointB);

    color = color;
    out_color = color;
    
    if (out_color.a <= 0.5f) {
       discard;
    }
}
