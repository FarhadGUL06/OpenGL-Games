#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}

bool Tema3::checkPointCollision(glm::vec3 point, glm::vec3 other, float radius) {
    float distance = sqrt(
        (other.x - point.x) * (other.x - point.x) +
        (other.y - point.y) * (other.y - point.y) +
        (other.z - point.z) * (other.z - point.z)
    );
    return distance < radius;
}

pair<int,int> Tema3::checkPointInObject(glm::vec3 point, float radius) {
    for (int i = 0; i < posObjects.size(); i++) {
        // Verific coliziunea DOAR daca sunt elementele in quad
        if (point[2] + proximityRender > posObjects[i][2] &&
            point[0] + proximityRender > posObjects[i][0] &&
            posObjects[i][0] > point[0] - proximityRender) {
            
            // Daca intersecteaza cel putin un inamic
            if (checkPointCollision(point, posObjects[i], radius)) {
                if (typeObjects[i] != 1) {
                    pair<int, int> ret(0, i);
                    return ret;
                }
                if (typeObjects[i] == 1) {
                    pair<int, int> ret(1, i);
                    return ret;
                }
            }
        }
    }
    pair<int, int> ret(2, -1);
    return ret;
}

void Tema3::PointInObject(glm::vec3 point) {
    // Verificam daca avem coliziune in momentul acesta cu vreun obstacol
    pair<int, int> pos = checkPointInObject(point, sizeOfCar * 3.5f);
    // Obstacol
    if (pos.first == 0) {
        if (pos.second != -1) {
            // Stergem obstacolul
            if (typeObjects[pos.second] == 2) {
                // Daca e felinar, stergem din vectorul de stalpi
                for (int k = 0; k < posStalpi.size(); k++) {
                    if (posObjects[pos.second] == posStalpi[k]) {
                        posStalpi.erase(posStalpi.begin() + k);
                        break;
                    }
                }
            }
            if (typeObjects[pos.second] == 0) {
                // Daca e copac sau cadou, stergem lumina
                for (int k = 0; k < posLightPoint.size(); k++) {
                    if (posObjects[pos.second] == posLightPoint[k]) {
                        posLightPoint.erase(posLightPoint.begin() + k);
                        break;
                    }
                }
            }
            posObjects.erase(posObjects.begin() + pos.second);
            typeObjects.erase(typeObjects.begin() + pos.second);
            movingAcceleration = 0;
            cout << "Ati busit un obstacol! Apasati R pentru a reporni!\n";
        }
    }

    // Cadou
    if (pos.first == 1) {
        if (pos.second != -1) {
            // Am lovit un cadou

            if (typeObjects[pos.second] == 1) {
                // Daca e orice brad sau cadou
                for (int k = 0; k < posLightPoint.size(); k++) {
                    if (posObjects[pos.second] == posLightPoint[k]) {
                        posLightPoint.erase(posLightPoint.begin() + k);

                        break;
                    }
                }
            }
            posObjects.erase(posObjects.begin() + pos.second);
            typeObjects.erase(typeObjects.begin() + pos.second);
            noGifts++;
            cout << "Ati prins un cadou! Total cadou runda: " << noGifts << "\n";
        }
    }

    

}

void Tema3::GenerateObjects(bool isFirst) {
    // Salvam pozitiile
    savedPOZ = posPlan[2];
    savedPOY = posPlan[1];
    savedPOX = posPlan[0];

    // Generam randomOX, randomOZ
    // Generam positionOY in functie de cele 2 valori randomOX, randomOZ

    for (int i = 0; i < noObjects; i++) {
        glm::vec3 estimatedPos;
        float randomOX, randomOZ;
        if (isFirst) {
            //randomOX = (float)(rand() % 200) - 100;
            randomOX = (float)(rand() % (2 * (int)proximityGenerate)) - (int)proximityGenerate;
            randomOZ = (float)(rand() % ((int)proximityGenerate + (int)proximityGenerate)) - (int)proximityGenerate + proximityObject * 8;
            //randomOZ = (float)(rand() % 100) - 25;
        }
        else {
            //randomOX = (float)(rand() % 100) - 50;
            //randomOZ = (float)(rand() % 50) + 25;
            randomOX = (float)(rand() % (2 * (int) proximityGenerate)) - (int)proximityGenerate;
            randomOZ = (float)(rand() % (int) proximityGenerate) - (int)proximityGenerate / 2;
        }
        estimatedPos[0] = (savedPOX + randomOX);// * sin(RADIANS(angleMovingCar)));
        estimatedPos[1] = savedPOY - randomOZ * sin(RADIANS(anglePlane));
        estimatedPos[2] = savedPOZ + randomOZ * cos(RADIANS(anglePlane)); // * cos(RADIANS(angleMovingCar / 2)));

        pair<int, int> checkingObj = checkPointInObject(estimatedPos, proximityObject);
        if (checkingObj.second == -1) {
            // Nu avem obiect in zona deci il adaugam
            posObjects.push_back(estimatedPos);
            // Tip random
            int randomType = rand() % 4;
            if (randomType == 2) {
                posStalpi.push_back(estimatedPos);
            }
            if (randomType == 0 || randomType == 1) {
                  posLightPoint.push_back(estimatedPos);

            }
            typeObjects.push_back(randomType);
        }
    }
}


void Tema3::Init()
{
    // Informatii de joc
    //srand(time(NULL));
    srand(5);
    // Pozitie plan initiala
    posPlan = glm::vec3(0, -sizeOfPlan / 4, sizeOfPlan / 2);
    posCar = posPlan;

    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    // Incarcam textura pentru zapada
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }
    // Incarcam textura pentru jucator - ski
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "patine.jpg").c_str(), GL_REPEAT);
        mapTextures["patine"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "corp.jpg").c_str(), GL_REPEAT);
        mapTextures["corp"] = texture;
    }


    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "cone.jpg").c_str(), GL_REPEAT);
        mapTextures["cone"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.jpg").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "rock1.jpg").c_str(), GL_REPEAT);
        mapTextures["rock1"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "rock2.jpg").c_str(), GL_REPEAT);
        mapTextures["rock2"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stalp1.jpg").c_str(), GL_REPEAT);
        mapTextures["stalp1"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stalp2.jpg").c_str(), GL_REPEAT);
        mapTextures["stalp2"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpg").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;
    }
    // Load pentru plan 

    Mesh* mesh = new Mesh("plan");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("cub");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;


    // Pentru copaci
    mesh = new Mesh("cone");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Pentru pietre
    mesh = new Mesh("sphere");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh->GetMeshID()] = mesh;


    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Camera
    cameraPrincipala = new implemented::CameraTema3();
    cameraPrincipala->Set(glm::vec3(0, 1.5f, 2.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    // Proiectia
    projectionMatrixGame = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // Actualizam camera 
    //posCar[1] += (sizeOfPlan / 2) * sin(RADIANS(anglePlane));
    //posCar[2] -= (sizeOfPlan / 2) * cos(RADIANS(anglePlane));
    //updateCamera();
    if (savedPOZ == 0) {
        // Generam elementele prima data
        GenerateObjects(true);
    }

    // Light & material properties
    lightPosition = glm::vec3(0, 1, 1);
    lightDirection = glm::vec3(0, -1, 0);
    materialShininess = 30;
    materialKd = 0.5;
    materialKs = 0.5;

    glm::mat4 unitate = glm::mat4(1);
    lightDirection = glm::vec3(0, -1, 0); // sa pointeze in jos
    unitate = glm::rotate(unitate, RADIANS(30), glm::vec3(1, 0, 0));
    lightDirection = glm::vec3(unitate * glm::vec4(lightDirection, 0));

    for (int k = 0; k < 5; k++) {
        glm::vec3 randColor;
        randColor[0] = (float)(rand() % 256) / 256.0f;
        randColor[1] = (float)(rand() % 256) / 256.0f;
        randColor[2] = (float)(rand() % 256) / 256.0f;
        colorLightPoint.push_back(randColor);
    }

    cout << "Pentru statistici, apasati S.\n";
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    DrawCoordinateSystem(cameraPrincipala->GetViewMatrix(), projectionMatrixGame);

    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::updateCamera() {
    // Actualizam pozitia masinii in functie de pozitia camerei care este manevrata
    cameraPrincipala->position = posCar + glm::vec3(0, 0.5f, 3.5f);
}

void Tema3::RenderTree(int i) {
    glm::vec3 posObj;
    posObj = glm::vec3(0, 1.35f, 0);
    // Copac con sus
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f));
    RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["cone"], 2, 0, -1);

    // Copac con mijloc
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj - glm::vec3(0, 0.35f, 0.2f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.45f));
    RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["cone"], 2, 0, -1);

    // Copac baza
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj - glm::vec3(0, 0.65f, 0.34f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 1.1f, 0.1f));
    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["wood"], 2, 0, -1);
}


void Tema3::RenderRock(int i) {
    glm::vec3 posObj;
    posObj = glm::vec3(0, 0.2f, 0);
    // Prima piatra
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f));
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["rock1"], -1, 0, -1);
   
    // A doua piatra
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj - glm::vec3(0.2f, -0.1f, 0.3f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.45f));
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["rock2"], -1, 0, -1);
}

void Tema3::RenderStalp(int i) {
    glm::vec3 posObj;
    posObj = glm::vec3(0, 0.7f, 0);
    // Baza stalpului
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 1.5f, 0.1f));
    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["stalp1"], 1, 0, -1);

    // Felinarul
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj - glm::vec3(0.02f, -0.6f, -0.35f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9f, 0.2f, 0.2f));

    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["stalp2"], 1, 1, i);
}


void Tema3::RenderGift(int i) {
    glm::vec3 posObj;
    posObj = glm::vec3(0, 0.2f, 0);
    // Prima piatra
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posObjects[i] + posObj);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["gift"], 3, 0, -1);
}

void Tema3::RenderObjects() {
    // Pentru randat - ma intereseaza doar OX si OZ
    // sa fie aproximativ langa posPlan
   
    for (int i = 0; i < posObjects.size(); i++) {
        // Randam doar ce intra in quad
        if (posCar[2] + proximityRender > posObjects[i][2] && 
            posCar[0] + proximityRender > posObjects[i][0] && 
            posObjects[i][0] > posCar[0] - proximityRender) {
            if (typeObjects[i] == 0) {
                RenderTree(i);
            }
            if (typeObjects[i] == 1) {
                RenderGift(i);
            }
            if (typeObjects[i] == 2) {
                RenderStalp(i);
            }
            if (typeObjects[i] == 3) {
                RenderRock(i);
            }
        }
        // Daca a ramas in spate, stergem din lista de obiecte
        if (posCar[2] - proximityRender > posObjects[i][2]) {
            if (typeObjects[i] == 2) {
                // Daca e stalp
                for (int k = 0; k < posStalpi.size(); k++) {
                    if (posObjects[i] == posStalpi[k]) {
                        posStalpi.erase(posStalpi.begin() + k);
                    }
                }
            }

            if (typeObjects[i] == 0 || typeObjects[i] == 1) {
                // Daca e orice brad sau cadou
                for (int k = 0; k < posLightPoint.size(); k++) {
                    if (posObjects[i] == posLightPoint[k]) {
                        posLightPoint.erase(posLightPoint.begin() + k);
                        break;
                    }
                }
            }
            posObjects.erase(posObjects.begin() + i);
            typeObjects.erase(typeObjects.begin() + i);
        }
    }

}

bool Tema3::CheckObjectInQuad(int position) {

    for (int i = 0; i < posObjects.size(); i++) {
        // Randam doar ce intra in quad
        if (posObjects[position][2] + proximityRender > posObjects[i][2] &&
            posObjects[position][0] + proximityRender > posObjects[i][0] &&
            posObjects[i][0] > posObjects[position][0] - proximityRender) {
            return true;
        }
        // Daca a ramas in spate, stergem din lista de obiecte
        if (posObjects[position][2] - proximityRender > posObjects[i][2]) {
            return false;
        }
    }
    return false;

}

void Tema3::RenderScene() {

    float factorZ = 0.00f;
    float factorX = 0.067f;

    // Planul
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posPlan - glm::vec3(0, 0.1f, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -sizeOfPlan / 2));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(50.0f, 0.1f, 50.0f));

    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["snow"], 0, 0, -1);

    // Corpul
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posCar + glm::vec3(0, sizeOfCar / 1.2f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.12f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(angleMovingCar), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["corp"], -1, 0, -1);

    // Ski 1
    posSki1 = posCar + glm::vec3(sizeOfCar / 2, sizeOfCar / 3.0f, -sizeOfCar / 3.0f);
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posSki1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-factorX * cos(RADIANS(angleMovingCar)), 0, factorZ * sin(RADIANS(angleMovingCar))));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(angleMovingCar), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(factorX * cos(RADIANS(angleMovingCar)), 0, -factorZ * sin(RADIANS(angleMovingCar))));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.01f, 0.3f));
    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["patine"], -1, 0, -1);

    // Ski 2
    posSki2 = posCar + glm::vec3(-sizeOfCar / 2, sizeOfCar / 3.0f, -sizeOfCar / 3.0f);
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, posSki2);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(anglePlane), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(factorX * cos(RADIANS(angleMovingCar)), 0, factorZ * sin(RADIANS(angleMovingCar))));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(angleMovingCar), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-factorX * cos(RADIANS(angleMovingCar)), 0, -factorZ * sin(RADIANS(angleMovingCar))));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.01f, 0.3f));
    RenderSimpleMesh(meshes["cub"], shaders["LabShader"], modelMatrix, mapTextures["patine"], -1, 0, -1);

    RenderObjects();

}

void Tema3::Update(float deltaTimeSeconds)
{
    // Timpul supravietuit
    if (movingAcceleration) {
        timeSurvived += deltaTimeSeconds;
    }

    // Verificam dimensiunile ferestrei
    glm::ivec2 resolution = window->GetResolution();
    widthGame = resolution.x;

    // Actualizam camera 
    updateCamera();

    // Coordonatele masinii
    posPlan[0] += movingAcceleration * sin(RADIANS(angleMovingCar)) * deltaTimeSeconds;
    posPlan[1] += ( - movingAcceleration * sin(RADIANS(anglePlane)) + 0 * abs(sin(RADIANS(angleMovingCar))))* deltaTimeSeconds;
    posPlan[2] += movingAcceleration * (cos(RADIANS(anglePlane))) * deltaTimeSeconds; // * cos(RADIANS(angleMovingCar / 2))

    posCar = posPlan;
    posCar[1] += (sizeOfPlan / 2) * sin(RADIANS(anglePlane));
    posCar[2] -= (sizeOfPlan / 2) * cos(RADIANS(anglePlane));
    // Generam obiecte random
    if (savedPOZ + proximityGenerate < posPlan[2] && savedPOZ != 0) {
        GenerateObjects(false);
    }
    
    // Randam obiectele
    RenderScene();

    // Actualizam distanta de miscare
    movingSpeed += movingAcceleration * deltaTimeSeconds;

    // Verificam daca s-a produs coliziune
    if (movingAcceleration) {
        PointInObject(posCar);
    }
}


void Tema3::FrameEnd()
{
}


void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture, int typeObject, int isSpotty, int positionObject)
{
    /* 
        Cod lumini:
        -1 orice
        0 plan
        1 stalp
        2 brad
        -1 piatra
        3 cadou

    */
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);


    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, posCar[0], posCar[1] + 50.0f, posCar[2]);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
   // printf("%f %f %f\n", lightDirection[0], lightDirection[1], lightDirection[2]);
    glUniform3f(light_direction, lightDirection[0], lightDirection[1], lightDirection[2]);

    /*for (int i = 0; i < posObjects.size(); i++) {
        if (CheckObjectInQuad(i)) {
            posObjectsInQuad.push_back(posObjects[i]);
            typeObjectsInQuad.push_back(typeObjects[i]);
        }
    }*/

    //location = glGetUniformLocation(shader->program, "typeObjects");
    //glUniform1iv(location, typeObjects.size(), typeObjects.data());


    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = cameraPrincipala->position;
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    //glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(cameraPrincipala->GetViewMatrix()));


    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    //glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrixGame));



    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
    }

    // Trimitem valori pentru miscarea texturii
    GLuint location = glGetUniformLocation(shader->program, "movingSpeedOX");
    glUniform1f(location, posPlan[0] / (sizeOfPlan * ratioPlan));

    location = glGetUniformLocation(shader->program, "movingSpeedOZ");
    glUniform1f(location, posPlan[2] / (sizeOfPlan * ratioPlan - 7.0f));

    // Trimitem daca e plan
    location = glGetUniformLocation(shader->program, "isPlane");
    if (texture == mapTextures["snow"]) {
        glUniform1i(location, 1);
    }
    else {
        glUniform1i(location, 0);
    }
    // Trimitem nr de obiecte, pozitiile lor si tipul
    location = glGetUniformLocation(shader->program, "noStalpi");
    int sizeArr = (int)posStalpi.size();
    glUniform1i(location, sizeArr);

    location = glGetUniformLocation(shader->program, "posStalpi");
    glUniform3fv(location, posStalpi.size(), (GLfloat*)posStalpi.data());

    location = glGetUniformLocation(shader->program, "noLightPoint");
    int sizeArrLP = (int)posLightPoint.size();
    glUniform1i(location, sizeArrLP);


    location = glGetUniformLocation(shader->program, "posLightPoint");
    glUniform3fv(location, posLightPoint.size(), (GLfloat*)posLightPoint.data());


    location = glGetUniformLocation(shader->program, "colorLightPoint");
    glUniform3fv(location, colorLightPoint.size(), (GLfloat*)colorLightPoint.data());

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;


    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            cameraPrincipala->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            cameraPrincipala->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            cameraPrincipala->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            cameraPrincipala->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            cameraPrincipala->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            cameraPrincipala->TranslateUpward(deltaTime * cameraSpeed);
        }
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_R)
    {
        if (movingAcceleration == 0) {
            movingAcceleration = 2.5f;
            cout << "Jocul a reinceput! Runda: " << noRound << "\n\n";
            allGifts.push_back(noGifts);
            allTimes.push_back(timeSurvived);
            noRound++;
            noGifts = 0;
            timeSurvived = 0;
        }
    }
    if (key == GLFW_KEY_S)
    {
            cout << "Runda: " << noRound << ":\n";
            cout << "Numar cadouri adunate: " << noGifts << "\n";
            cout << "Timp pe teren: " << (int) timeSurvived << " secunde" << "\n";
            if (movingAcceleration == 0) {
                cout << "Apasati pe R pentru a reporni!\n\n";
            }
    }

    if (key == GLFW_KEY_A)
    {
        if (noRound > 1) {
            cout << "Statisticile pentru toate rundele:\n";
            for (int i = 0; i < allTimes.size(); i++) {
                cout << "In runda " << i + 1 << " ati colectat " << allGifts[i] << " cadouri in " << allTimes[i] << " secunde!\n";
            }
            cout << endl;
        }
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    // 45 ... OX
    // y .... mouseX
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            cameraPrincipala->RotateFirstPerson_OY(-sensivityOY * deltaX);
            cameraPrincipala->RotateFirstPerson_OX(-sensivityOX * deltaY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            cameraPrincipala->RotateThirdPerson_OX(-sensivityOX * deltaY);
            cameraPrincipala->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
    angleMovingCar = (45.0f * (mouseX - (widthGame / 2))) / (widthGame / 2);
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
