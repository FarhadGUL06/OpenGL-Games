#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/transform3Dt.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}
float Tema2::triangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    glm::vec3 AB = b - a;
    glm::vec3 AC = c - a;
    float crossProduct = AB.x * AC.z - AB.z * AC.x;
    float area = abs(crossProduct) / 2;
    return area;
}

bool Tema2::checkPointInTriangle(glm::vec3 point, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    float bigTriangleArea = triangleArea(a, b, c);
    float sumOfAreas = 0;
    sumOfAreas += triangleArea(a, b, point);
    sumOfAreas += triangleArea(a, c, point);
    sumOfAreas += triangleArea(b, c, point);
    // Daca suma ariilor triunghirilor mici e aproape egala
    if (sumOfAreas > bigTriangleArea - 0.0005f && sumOfAreas < bigTriangleArea + 0.0005f) {
        // Suntem in cel putin un triunghi
        return true;
    }
    else {
        return false;
    }
}

bool Tema2::checkPointInside(glm::vec3 point) {
    for (int i = 0; i < vertexA.size() - 1; i++) {
        // R anterior
        // A anterior
        // A curent

        // R anterior
        // R curent
        // A curent
        if (
            checkPointInTriangle(point, vertexR[i].position, vertexA[i].position, vertexA[i + 1].position) ||
            checkPointInTriangle(point, vertexR[i].position, vertexR[i + 1].position, vertexA[i + 1].position)
            ) {
            // Se afla in unul din triunghiurile pistei
            return true;
        }
    }
    // Verificam daca se afla in triunghiurile care leaga inceputul de sfarsit
    if (
        checkPointInTriangle(point, vertexR[vertexR.size() - 1].position, vertexA[vertexA.size() - 1].position, vertexA[0].position) ||
        checkPointInTriangle(point, vertexR[vertexR.size() - 1].position, vertexR[0].position, vertexA[0].position)
        ) {
        // Se afla in unul din triunghiurile pistei
        return true;
    }
    return false;
}
bool Tema2::checkPointCollision(glm::vec3 point, glm::vec3 other, float radius) {
    float distance = sqrt(
        (other.x - point.x) * (other.x - point.x) +
        (other.y - point.y) * (other.y - point.y) +
        (other.z - point.z) * (other.z - point.z)
    );
    return distance < radius;
}


bool Tema2::checkPointInEnemies(glm::vec3 point) {
    for (int i = 0; i < posEnemiesCollision.size(); i++) {
        // Daca intersecteaza cel putin un inamic
        if (checkPointCollision(point, posEnemiesCollision[i], sizeOfCar * 2)) {
            return true;
        }
    }
    return false;
}

void Tema2::generatePista(glm::vec3 center, glm::vec3 color, string name) {
    std::vector<unsigned int> indices;
    std::vector<unsigned int> indicesPista;
    vector<VertexFormat> vertex;

    // De la 0 la 54
    int distRosu = 5;
    int distAlbastru = 5;
    int lengthIndices;
    // vectorul perpendicular pe X0Z
    glm::vec3 UP = glm::vec3(0, 1, 0);
    glm::vec3 D, P, R, A;
    int i;
    float j;
    for (i = 1; i < vertexPista.size() + 1; i++) {
        int punct = i;
        if (i == vertexPista.size()) {
            punct = 0;
        }
        // vectorul directie
        if (punct == 0) {
            D = vertexPista[punct].position - vertexPista[i - 1].position;
        }
        else {
            D = vertexPista[i].position - vertexPista[i - 1].position;
        }
        for (j = 1; j < numberTriangles; j++) {
            // vectorul perpendicular pe D
            P = glm::normalize(glm::cross(D, UP));
            // Punct exterior
            R = vertexPista[i - 1].position + (j * D) / (float) numberTriangles + glm::vec3(P[0] * distRosu, P[1] * distRosu, P[2] * distRosu);
            // Punct interior
            A = vertexPista[i - 1].position + (j * D) / (float) numberTriangles - glm::vec3(P[0] * distAlbastru, P[1] * distAlbastru, P[2] * distAlbastru);
            // Adaugam noile puncte
            vertexR.push_back(VertexFormat(R, color));
            vertexA.push_back(VertexFormat(A, color));
        }
        // Pentru directia de mers si rotatie a inamicilor
        forwardEnemies.push_back(glm::normalize(D));

        // Pentru generat copacii
        int pozR = rand() % 3 + 2;
        int pozA = rand() % 3 + 2;
        int noTrees = rand() % 3 + 1;
        int freq = 2;
        for (int k = 0; k < noTrees; k++) {
            // Generam in functie de frecventa copacii
            if (i % (freq / 2) == 0) {
                if (i % freq == 0) {
                    // Generam copac in R (outter)
                    glm::vec3 copac = vertexPista[i - 1].position + glm::vec3(P[0] * (distRosu + pozR), P[1] * (distRosu + pozR), P[2] * (distRosu + pozR));
                    posTree.push_back(copac);
                }
                else {
                    // Generam copac in A (inner)
                    glm::vec3 copac = vertexPista[i - 1].position - glm::vec3(P[0] * (distAlbastru + pozA), P[1] * (distAlbastru + pozA), P[2] * (distAlbastru + pozA));
                    posTree.push_back(copac);
                }
            }
            pozR += 4;
            pozA += 4;
        }

        // Pentru debug
        indicesPista.push_back(i - 1);
        indicesPista.push_back(i);
        indicesPista.push_back(0);
    } 

    // Legam la pasul anterior
    for (i = 0; i < vertexA.size(); i++) {
        vertex.push_back(vertexR[i]);
        vertex.push_back(vertexA[i]);

        if (i > 1) {
            indices.push_back(i * 2 - 4); // R anterior
            indices.push_back(i * 2 - 3); // A anterior
            indices.push_back(i * 2 - 1); // A curent

            indices.push_back(i * 2 - 4); // R anterior
            indices.push_back(i * 2 - 2); // R curent
            indices.push_back(i * 2 - 1); // A curent
        }
    }
    lengthIndices = vertexA.size() * 2 - 2;
    indices.push_back(lengthIndices - 2); // R anterior
    indices.push_back(lengthIndices - 1); // A anterior
    indices.push_back(1); // A curent

    indices.push_back(lengthIndices - 2); // R anterior
    indices.push_back(0); // R curent
    indices.push_back(1); // A curent

    // Pentru inamici, introducem un nou punct
    vertexPista.push_back(vertexPista[0]);

    Mesh* pista = new Mesh(name);
    pista->SetDrawMode(GL_TRIANGLES);
    pista->InitFromData(vertex, indices);
    meshes[name] = pista;
}


void Tema2::generateLinie(glm::vec3 center, glm::vec3 color, string name, float offset) {
    std::vector<unsigned int> indices;
    vector<VertexFormat> vertex;
    vector<VertexFormat> vertexLinieA, vertexLinieR, vertexLinie;
    // De la 0 la 54
    float distRosu = 0.2f;
    float distAlbastru = 0.2f;
    if (offset == -5) {
        distAlbastru = 4.8f;
    }
    if (offset == 5) {
        distRosu = 4.8f;
    }
    int lengthIndices;
    // vectorul perpendicular pe X0Z
    glm::vec3 UP = glm::vec3(0, 1, 0);
    glm::vec3 D, P, R, A;
    int i;
    float j;
    for (i = 1; i < vertexPista.size() + 1; i++) {
        int punct = i;
        if (i == vertexPista.size()) {
            punct = 0;
        }
        // vectorul directie
        if (punct == 0) {
            D = vertexPista[punct].position - vertexPista[i - 1].position;
        }
        else {
            D = vertexPista[i].position - vertexPista[i - 1].position;
        }
        for (j = 1; j < numberTriangles; j++) {
            // vectorul perpendicular pe D
            P = glm::normalize(glm::cross(D, UP));
            // Punct exterior
            R = vertexPista[i - 1].position + (j * D) / (float)numberTriangles + glm::vec3(P[0] * distRosu, P[1] * distRosu, P[2] * distRosu);
            // Punct interior
            A = vertexPista[i - 1].position + (j * D) / (float)numberTriangles - glm::vec3(P[0] * distAlbastru, P[1] * distAlbastru, P[2] * distAlbastru);
            // Adaugam noile puncte
            vertexLinie.push_back(VertexFormat(R, color));
            vertexLinie.push_back(VertexFormat(A, color));
        }
        if (offset == -5 || offset == 5) {
            R = vertexPista[i - 1].position + glm::vec3(P[0] * distRosu, P[1] * distRosu, P[2] * distRosu);
            A = vertexPista[i - 1].position - glm::vec3(P[0] * distAlbastru, P[1] * distAlbastru, P[2] * distAlbastru);
            vertexLinieR.push_back(VertexFormat(R, color));
            vertexLinieA.push_back(VertexFormat(A, color));
        }
    }

    if (offset == 0) {
        for (i = 2; i < vertexLinie.size(); i++) {
            if ((i / (2 * numberTriangles)) % frecvLinie == 0) {
                i = i + 2 * numberTriangles;
                continue;
            }
            indices.push_back(i * 2 - 4); // R anterior
            indices.push_back(i * 2 - 3); // A anterior
            indices.push_back(i * 2 - 1); // A curent

            indices.push_back(i * 2 - 4); // R anterior
            indices.push_back(i * 2 - 2); // R curent
            indices.push_back(i * 2 - 1); // A curent
        }

        Mesh* linie = new Mesh(name);
        linie->InitFromData(vertexLinie, indices);
        linie->SetDrawMode(GL_TRIANGLES);
        meshes[name] = linie;
        return;
    }
    // Stabilim offsetul liniei
    vector<VertexFormat> newVertexLinie, newVertexRes;
    if (offset == -5) {
        // Outter
        for (i = 0; i < vertexLinieA.size(); i++) {
            newVertexLinie.push_back(vertexLinieA[i]);
        }
    }
    if (offset == 5) {
        // Inner
        for (i = 0; i < vertexLinieR.size(); i++) {
            newVertexLinie.push_back(vertexLinieR[i]);
        }
    }
    distRosu = 0.05f;
    distAlbastru = 0.05f;
    for (i = 1; i < newVertexLinie.size() + 1; i++) {
        int punct = i;
        if (i == newVertexLinie.size()) {
            punct = 0;
        }
        // vectorul directie
        if (punct == 0) {
            D = newVertexLinie[punct].position - newVertexLinie[i - 1].position;
        }
        else {
            D = newVertexLinie[i].position - newVertexLinie[i - 1].position;
        }
        for (j = 1; j < numberTriangles; j++) {
            // vectorul perpendicular pe D
            P = glm::normalize(glm::cross(D, UP));
            // Punct exterior
            R = newVertexLinie[i - 1].position + (j * D) / (float)numberTriangles + glm::vec3(P[0] * distRosu, P[1] * distRosu, P[2] * distRosu);
            // Punct interior
            A = newVertexLinie[i - 1].position + (j * D) / (float)numberTriangles - glm::vec3(P[0] * distAlbastru, P[1] * distAlbastru, P[2] * distAlbastru);
            // Adaugam noile puncte
            newVertexRes.push_back(VertexFormat(R, color));
            newVertexRes.push_back(VertexFormat(A, color));
        }
    }
    // Formam noul rezultat
    for (i = 2; i < newVertexRes.size(); i++) {
        if (i % frecvLinieSecundara == 0) {
            i = i + frecvLinieSecundara + 1;
            continue;
        }
        indices.push_back(i * 2 - 4); // R anterior
        indices.push_back(i * 2 - 3); // A anterior
        indices.push_back(i * 2 - 1); // A curent

        indices.push_back(i * 2 - 4); // R anterior
        indices.push_back(i * 2 - 2); // R curent
        indices.push_back(i * 2 - 1); // A curent
    }

    Mesh* linie = new Mesh(name);
    linie->InitFromData(newVertexRes, indices);
    linie->SetDrawMode(GL_TRIANGLES);
    meshes[name] = linie;
    return;
}


void Tema2::generateTree(glm::vec3 center, string name) {
    glm::vec3 colorBase = glm::vec3(0.38f, 0.15f, 0);
    glm::vec3 colorLeaf = glm::vec3(0, 0.40f, 0.01f);

    float length = 0.5f;
    int heightBase = 6;
    // Vertexi pentru baza
    vector<VertexFormat> vertex{
        VertexFormat(center + glm::vec3(-length / 2, -length * heightBase, -length / 2), colorBase), // A - 0
        VertexFormat(center + glm::vec3(length / 2, -length * heightBase, -length / 2), colorBase), // B - 1
        VertexFormat(center + glm::vec3(length / 2, -length * heightBase, length / 2), colorBase), // C - 2
        VertexFormat(center + glm::vec3(-length / 2, -length * heightBase, length / 2), colorBase), // D - 3
        VertexFormat(center + glm::vec3(-length / 2, length * heightBase, -length / 2), colorBase), // E - 4
        VertexFormat(center + glm::vec3(length / 2, length * heightBase, -length / 2), colorBase), // F - 5
        VertexFormat(center + glm::vec3(length / 2, length * heightBase, length / 2), colorBase), // G - 6
        VertexFormat(center + glm::vec3(-length /2, length * heightBase, length / 2), colorBase), // H - 7
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 4,
        1, 5, 4,

        1, 2, 5,
        2, 6, 5,

        2, 3, 6,
        6, 7, 3,

        3, 0, 7,
        0, 7, 4,

        4, 5, 7,
        5, 6, 7,

        0, 1, 3,
        1, 3, 2
    };
    
    float heigthOfBase = 1.3f * length * heightBase;
    length *= 3.2f;
    // Pentru frunze
    vertex.push_back(VertexFormat(center + glm::vec3(-length, heigthOfBase  - length, -length), colorLeaf)); // A - 8
    vertex.push_back(VertexFormat(center + glm::vec3(length, heigthOfBase - length, -length), colorLeaf)); // B - 9
    vertex.push_back(VertexFormat(center + glm::vec3(length, heigthOfBase - length, length), colorLeaf)); // C - 10
    vertex.push_back(VertexFormat(center + glm::vec3(-length, heigthOfBase - length, length), colorLeaf)); // D - 11
    vertex.push_back(VertexFormat(center + glm::vec3(-length, heigthOfBase + length, -length), colorLeaf)); // E - 12
    vertex.push_back(VertexFormat(center + glm::vec3(length, heigthOfBase + length, -length), colorLeaf)); // F - 13
    vertex.push_back(VertexFormat(center + glm::vec3(length, heigthOfBase + length, length), colorLeaf)); // G - 14
    vertex.push_back(VertexFormat(center + glm::vec3(-length, heigthOfBase + length, length), colorLeaf)); // H - 15

    std::vector<unsigned int> indices2 =
    {
        8, 9, 12,
        9, 13, 12,

        9, 10, 13,
        10, 14, 13,

        10, 11, 14,
        14, 15, 11,

        11, 8, 15,
        8, 15, 12,

        12, 13, 15,
        13, 14, 15,

        8, 9, 11,
        9, 11, 10
    };

    indices.insert(indices.end(), indices2.begin(), indices2.end());

    Mesh* car = new Mesh(name);
    car->InitFromData(vertex, indices);
    meshes[name] = car;
}

void Tema2::generateGround(glm::vec3 center, glm::vec3 color, string name) {
    int lenGround = 200;
    int sizeOfGround = 2;

    vector<VertexFormat> vertex{
        VertexFormat(center + glm::vec3(-lenGround, 0, -lenGround), color), // stanga sus
        VertexFormat(center + glm::vec3(-lenGround, 0, lenGround), color), // stanga jos
        VertexFormat(center + glm::vec3(lenGround, 0, -lenGround), color), // dreapta sus
        VertexFormat(center + glm::vec3(lenGround, 0, lenGround), color), // dreapta jos
    };

    vertex.push_back(VertexFormat(center, color));

    std::vector<unsigned int> indices =
    { 
        0, 1, 2,
        2, 3, 1
    };


    //Mesh* ground = new Mesh(name);
    //ground->InitFromData(vertex, indices);
    //meshes[name] = ground;
    Mesh* mesh = new Mesh(name);
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    meshes[mesh->GetMeshID()] = mesh;
}

void Tema2::generateCar(glm::vec3 center, glm::vec3 color, string name) {
    float length = sizeOfCar;
    vector<VertexFormat> vertex{
        VertexFormat(center + glm::vec3(-length, -length / 2, -length / 2), color), // A - 0
        VertexFormat(center + glm::vec3(length, -length / 2, -length / 2), color), // B - 1
        VertexFormat(center + glm::vec3(length, -length / 2, length / 2), color), // C - 2
        VertexFormat(center + glm::vec3(-length, -length / 2, length / 2), color), // D - 3
        VertexFormat(center + glm::vec3(-length, length / 2, -length / 2), color), // E - 4
        VertexFormat(center + glm::vec3(length, length / 2, -length / 2), color), // F - 5
        VertexFormat(center + glm::vec3(length, length / 2, length / 2), color), // G - 6
        VertexFormat(center + glm::vec3(-length, length / 2, length / 2), color), // H - 7
    };
    // Vertex pentru tot terenul

    std::vector<unsigned int> indices =
    {
        0, 1, 4,
        1, 5, 4,

        1, 2, 5,
        2, 6, 5,

        2, 3, 6,
        6, 7, 3,

        3, 0, 7,
        0, 7, 4,

        4, 5, 7,
        5, 6, 7,

        0, 1, 3,
        1, 3, 2
    };

    Mesh* car = new Mesh(name);
    car->InitFromData(vertex, indices);
    meshes[name] = car;
}

void Tema2::renderMultipleTrees(int typeOfRender) {
    // Aici vom randa mai multi copaci din vectorul posTree
    // generat odata cu pista
    glm::mat4 modelMatrix;
    for (int i = 0; i < posTree.size(); i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3Dt::Translate(posTree[i][0], posTree[i][1], posTree[i][2]);
        RenderSimpleMesh(meshes["tree"], shaders["shaderGame"], modelMatrix, typeOfRender, 0, colorStandard);
    }
}

bool Tema2::checkPointInAbilities(glm::vec3 point) {
    for (int i = 0; i < posAbilitiesCollision.size(); i++) {
        // Daca intersecteaza abilitatea
        if (checkPointCollision(point, posAbilitiesCollision[i], sizeOfCar * 2) && showAbility[i]) {
            showAbility[i] = false;
            abilityTime = 5.0f;
            if (i % 2 == 0) {
                isAbilitySpeed = true;
            }
            else {
                isAbilitySlowEnemy = true;
            }
            return true;
        }
    }
    return false;
}

void Tema2::renderAbilities(int typeOfRender) {
    /*
        Vor fi 2 tipuri de abilitati :
            - viteza mai mare la masina
            - viteza mai mica pentru ceilalti inamici
    */ 
    glm::vec3 colorAbility(0.7f, 0.7f, 0.1f);
    int poz[5], freq = 10;
    poz[0] = vertexPista.size() / 4;
    poz[1] = poz[0] + freq;
    poz[2] = poz[0] + 2 * freq;
    poz[3] = poz[0] + 3 * freq;

    for (int i = 0; i < 4; i++) {
        if (showAbility[i]) {
            glm::mat4 modelMatrix;
            modelMatrix = glm::mat4(1);
            glm::vec3 posAbility(vertexPista[poz[i]].position[0] + 1.5f, vertexPista[poz[i]].position[1] + 0.6f, vertexPista[poz[i]].position[2]);
            modelMatrix *= transform3Dt::Translate(posAbility[0], posAbility[1], posAbility[2]);
            RenderSimpleMesh(meshes["sphere"], shaders["shaderGame"], modelMatrix, typeOfRender, 1, colorAbility);
        }
    }
}
void Tema2::gameInitAbility() {
    int poz[5], freq = 10;
    poz[0] = vertexPista.size() / 4;
    poz[1] = poz[0] + freq;
    poz[2] = poz[0] + 2 * freq;
    poz[3] = poz[0] + 3 * freq;

    for (int i = 0; i < 4; i++) {
        glm::vec3 posAbility(vertexPista[poz[i]].position[0] + 1.5f, vertexPista[poz[i]].position[1] + 0.6f, vertexPista[poz[i]].position[2]);
        posAbilitiesCollision.push_back(posAbility);
    }
}


void Tema2::gameStart() {
    for (int i = 0; i < 4; i++) {
        showAbility[i] = true;
    }
}

void Tema2::renderMultipleEnemies(int typeOfRender, float deltaTimeSeconds, glm::mat4 modelMatrixPista) {
    // Aici vom randa mai multe masini inamice
    glm::mat4 modelMatrix;
    glm::vec3 colorEnemy;
    glm::vec3 D, P, R, A;

    // vectorul perpendicular pe X0Z
    glm::vec3 UP = glm::vec3(0, 1, 0);
    // Distante
    for (int i = 0; i < numberEnemy; i++) {
        // Pentru distante
        float distRosu = 2.0f * (float) i / 10;
        float distAlbastru = 2.0f * (float) i / 10;
        glm::vec3 P1 = vertexPista[posVertex[i] - 1].position;
        glm::vec3 P2 = vertexPista[posVertex[i]].position;

        // Verificam daca a trecut de segmentul curent
        if (glm::length(posEnemies[i] - P1) >= glm::length(P2 - P1)) {
            posVertex[i]++;
        }

        // Daca am ajuns la final, reiau de la inceput
        if (posVertex[i] >= vertexPista.size()) {
            posVertex[i] = 1;
            posEnemies[i] = P2;
        }
        D = P2 - P1;
        // vectorul perpendicular pe D
        P = glm::normalize(glm::cross(D, UP));
        // Masina exteriora
        R = glm::vec3(P[0] * distRosu, P[1] * distRosu, P[2] * distRosu);
        // Masina interiora
        A = glm::vec3(P[0] * distAlbastru, P[1] * distAlbastru, P[2] * distAlbastru);

        colorEnemy = glm::normalize(P2) + glm::vec3(0.3f, 0.3f, 0.3f);
        modelMatrix = glm::mat4();
        glm::vec3 forwardEnemy = glm::normalize(P2 - P1);
        posEnemies[i] = posEnemies[i] + forwardEnemy * deltaTimeSeconds * speedCarEnemy;
        if (i % 2 == 1) {
            modelMatrix = transform3Dt::Translate(posEnemies[i][0] + R[0], posEnemies[i][1] + sizeOfCar / 2 + R[1] - 0.1f, posEnemies[i][2] + R[2]);
            posEnemiesCollision[i][0] = posEnemies[i][0] + R[0];
            posEnemiesCollision[i][1] = posEnemies[i][1] + sizeOfCar / 2 + R[1];
            posEnemiesCollision[i][2] = posEnemies[i][2] + R[2];
        }
        else {
            modelMatrix = transform3Dt::Translate(posEnemies[i][0] - A[0], posEnemies[i][1] + sizeOfCar / 2 - A[1] - 0.1f, posEnemies[i][2] - A[2]);
            posEnemiesCollision[i][0] = posEnemies[i][0] - A[0];
            posEnemiesCollision[i][1] = posEnemies[i][1] + sizeOfCar / 2 - A[1];
            posEnemiesCollision[i][2] = posEnemies[i][2] - A[2];
        }

        modelMatrix *= transform3Dt::RotateOY(atan2(forwardEnemy.x, forwardEnemy.z) + PI / 2);
        RenderSimpleMesh(meshes["car"], shaders["shaderGame"], modelMatrix, typeOfRender, 1, colorEnemy);
    }
}


void Tema2::generateShapes() {
    // Pozitie
    glm::vec3 posGround = glm::vec3(0, 0, 0);
    glm::vec3 posPista = glm::vec3(0, 0, 0);
    glm::vec3 center = glm::vec3(0, 0, 0);

    // Culori
    glm::vec3 colorGround = glm::vec3(0, 0.7f, 0.4f);
    glm::vec3 colorCar = glm::vec3(0.1f, 0.2f, 0.6f);
    glm::vec3 colorLinie = glm::vec3(1, 1, 1);


    // Nume mesh
    string nameGround = "ground";
    string nameCar = "car";
    string namePista = "pista";
    string nameTree = "tree";
    string nameLinieMijl = "linie_mijl";
    string nameLinieOutter = "linie_out";
    string nameLinieInner = "linie_in";


 
    posCar[0] = 35;
    posCar[1] = - sizeOfCar / 2;
    posCar[2] = 15;

    rotateCar = 0;
    speedCar = 5.0f;
    speedCarEnemy = 4.0f;

    generateGround(posGround, colorGround, nameGround);
    generateCar(center, colorCar, nameCar);
    generatePista(posPista, colorPista, namePista);
    // linia de mijloc
    generateLinie(posPista, colorLinie, nameLinieMijl, 0);
    // linia din outter
    generateLinie(posPista, colorLinie, nameLinieOutter, -5);
    // linia din inner
    generateLinie(posPista, colorLinie, nameLinieInner, 5);
    generateTree(center, nameTree);

    Mesh* mesh = new Mesh("sphere");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh->GetMeshID()] = mesh;
}

void Tema2::Init()
{
    for (int i = 0; i < numberEnemy; i++) {
        //glm::vec3 nothing(0, 0, 0);
        posVertex.push_back((4 * i) + 1);
        posEnemies.push_back(vertexPista[(4 * i) + 1].position);
        posEnemiesCollision.push_back(vertexPista[(4 * i) + 1].position);
    }

    // Pentru generari random
    srand(time(NULL));
    polygonMode = GL_FILL;
    renderCameraTarget = true;

    generateShapes();

    // Camera pentru jocul principal
    cameraPrincipala = new implemented::CameraTema2();
    cameraPrincipala->Set(glm::vec3(posCar[0], posCar[1] + 2, posCar[2] + distanceToTarget), glm::vec3(posCar[0], posCar[1] + 2, posCar[2]), glm::vec3(0, 1, 0));

    // Camera pentru minimap
    cameraMinimap = new implemented::CameraTema2();
    cameraMinimap->Set(glm::vec3(0, 3, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
    cameraMinimap->RotateFirstPerson_OY(3.14f);

    projectionMatrixGame = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    projectionMatrixMinimap = glm::ortho(-miniMapSize, miniMapSize, -miniMapSize, miniMapSize, 0.1f, 200.f);

    // Cream shader-ul pentru tema
    Shader* shader = new Shader("shaderGame");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    gameInitAbility();
}

void Tema2::FrameStart()
{
    glClearColor(colorR, colorG, colorB, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderScene(int typeOfRender, float deltaTimeSeconds) {
    // Randam obiectul principal
    glm::mat4 modelMatrix = glm::mat4(1);
    updateCamera();
    modelMatrix *= transform3Dt::Translate(posCar[0], posCar[1], posCar[2]);
    glm::vec3 posCameraPrincipala = cameraPrincipala->GetTargetPosition();
    modelMatrix *= transform3Dt::RotateOY(rotateCar + PI / 2);
    RenderSimpleMesh(meshes["car"], shaders["shaderGame"], modelMatrix, typeOfRender, 0, colorStandard);

    // Randam linia din mijloc
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3Dt::Translate(0, -0.2f, 0);
    RenderSimpleMesh(meshes["linie_mijl"], shaders["shaderGame"], modelMatrix, typeOfRender, 0, colorStandard);

    // Randam linia din outter
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3Dt::Translate(0, 0, 0);
    RenderSimpleMesh(meshes["linie_out"], shaders["shaderGame"], modelMatrix, typeOfRender, 0, colorStandard);

    // Randam linia din inner
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3Dt::Translate(0, 0, 0);
    RenderSimpleMesh(meshes["linie_in"], shaders["shaderGame"], modelMatrix, typeOfRender, 0, colorStandard);

    // Randam pista
    glm::mat4 modelMatrixPista = glm::mat4(1);
    modelMatrixPista *= transform3Dt::Translate(0, 0, 0);
    RenderSimpleMesh(meshes["pista"], shaders["shaderGame"], modelMatrixPista, typeOfRender, 0, colorStandard);

    // Randam podeaua
    glm::vec3 colorGround = glm::vec3(0, 0.7f, 0.4f);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3Dt::Translate(0, -0.3f , 0);
    modelMatrix *= transform3Dt::Scale(3, 0, 3);
    RenderSimpleMesh(meshes["ground"], shaders["shaderGame"], modelMatrix, typeOfRender, 1, colorGround);

    // Randam toti copacii
    renderMultipleTrees(typeOfRender);

    // Randam masinile
    renderMultipleEnemies(typeOfRender, deltaTimeSeconds, modelMatrixPista);

    // Randam abilitatile
    renderAbilities(typeOfRender);
}

void Tema2::updateMinimapPos() {
    // Sets the resolution of the small viewport
    resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x - 50 - resolution.x / 4.f, 50, resolution.x / 4.f, resolution.y / 4.f);
}

void Tema2::updateStats(float deltaTimeSeconds) {
    if (abilityTime >= 0) {
        abilityTime = abilityTime - deltaTimeSeconds;
    }
    if (abilityTime < 0) {
        isAbilitySpeed = false;
        isAbilitySlowEnemy = false;
    }
    if (isAbilitySpeed) {
        speedCar = 9.5f;
        rotateSpeed = 1.0f;
    }
    else {
        speedCar = 5.0f;
        rotateSpeed = 0.5f;
    }
    if (isAbilitySlowEnemy) {
        speedCarEnemy = 1.9f;
    }
    else {
        speedCarEnemy = 4.0f;
    }
    // Pentru a retine cand terminam lapurile
    if (checkPointCollision(posCar, vertexPista[0].position, 12) && isInLap == true) {
        gameStart();
        isInLap = false;
        counterLaps++;
    }
    // Trece de jumatatea pistei
    if (checkPointCollision(posCar, vertexPista[vertexPista.size() / 2].position, 12) && isInLap == false) {
        isInLap = true;
    }
}

void Tema2::updateBackgroundColor(float delta) {
    // Schimbam culoarea in functie de timp
    colorB += colorSign * delta / 7.0f;
    if (colorB > 1) {
        colorSign *= -1;
    }
    if (colorB < 0.30f) {
        colorSign *= -1;
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    // Background
    updateBackgroundColor(deltaTimeSeconds);
    updateStats(deltaTimeSeconds);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    RenderScene(0, deltaTimeSeconds);
    //DrawCoordinateSystem(cameraPrincipala->GetViewMatrix(), projectionMatrixGame);

    // Minimap
    updateMinimapPos();
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderScene(1, deltaTimeSeconds);
    //DrawCoordinateSystem(cameraMinimap->GetViewMatrix(), projectionMatrixMinimap);
    updateCamera();
}

void Tema2::FrameEnd()
{
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int typeOfRender, int useColor, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
    glUniform1i(glGetUniformLocation(shader->program, "use_color"), useColor);
    glUniform3fv(glGetUniformLocation(shader->program, "pos_car"), 1, glm::value_ptr(posCar));
    glUniform1i(glGetUniformLocation(shader->program, "type_of_render"), typeOfRender);

    // MODEL
    int location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // VIEW
    location = glGetUniformLocation(shader->program, "View");
    if (typeOfRender == 0) {
        // In jocul principal
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraPrincipala->GetViewMatrix()));
    }
    else {
        // In minimap
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(cameraMinimap->GetViewMatrix()));
    }

    // PROJECTION
    location = glGetUniformLocation(shader->program, "Projection");
    if (typeOfRender == 0) {
        // In jocul principal
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrixGame));
    }
    else {
        // In minimap
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrixMinimap));
    }
    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::updateCamera() {
    // Actualizam pozitia masinii in functie de pozitia camerei care este manevrata
    posCar = cameraPrincipala->position - glm::vec3(distanceToTarget * sin(rotateCar), 1.2f, distanceToTarget * cos(rotateCar));
    cameraMinimap->position = posCar + glm::vec3(0, 7, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;
    if (window->KeyHold(GLFW_KEY_W)) {
        glm::vec3 nextPos = posCar;
        glm::vec3 dir = glm::normalize(glm::vec3(cameraPrincipala->forward.x, 0, cameraPrincipala->forward.z));
        nextPos += dir * (deltaTime * speedCar );
        // Verificam daca este posibila deplasarea in fata 
        if (checkPointInside(nextPos)) {
            // Suntem pe pista
            if (!checkPointInEnemies(posCar)) {
                // Nu ciocnim inamici
                if (checkPointInAbilities(posCar)) {
                    // Am atins o abilitate
                }
                cameraPrincipala->MoveForward(deltaTime * speedCar);
                updateCamera();
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // Verificam daca este permisa rotatia (pozitia ramane pe loc)
        if (checkPointInside(posCar) && !checkPointInEnemies(posCar)) {
            // Daca se merge cu spatele, se inverseaza sensul rotatiei
            if (window->KeyHold(GLFW_KEY_S)) {
                rotateCar -= rotateSpeed * deltaTime;
                cameraPrincipala->RotateThirdPerson_OY(-rotateSpeed * deltaTime);
            }
            else {
                rotateCar += rotateSpeed * deltaTime;
                cameraPrincipala->RotateThirdPerson_OY(rotateSpeed * deltaTime);
            }
            updateCamera();
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // Verificam daca este posibila deplasarea in spate
        glm::vec3 nextPos = posCar;
        glm::vec3 dir = glm::normalize(glm::vec3(cameraPrincipala->forward.x, 0, cameraPrincipala->forward.z));
        nextPos += dir * (-deltaTime * speedCar);
        if (checkPointInside(nextPos)) {
            // Suntem pe pista
            if (!checkPointInEnemies(posCar)) {
                // Nu ciocnim inamici
                cameraPrincipala->MoveForward(-deltaTime * speedCar);
                updateCamera();
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // Verificam daca este permisa rotatia (pozitia ramane pe loc)
        if (checkPointInside(posCar) && !checkPointInEnemies(posCar)) {
            // Daca se merge cu spatele, se inverseaza sensul rotatiei
            if (window->KeyHold(GLFW_KEY_S)) {
                rotateCar += rotateSpeed * deltaTime;
                cameraPrincipala->RotateThirdPerson_OY(rotateSpeed * deltaTime);
            }
            else {
                rotateCar -= rotateSpeed * deltaTime;
                cameraPrincipala->RotateThirdPerson_OY(-rotateSpeed * deltaTime);
            }
            updateCamera();
        }
    }

    // PENTRU DEBUG
    
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            cameraPrincipala->TranslateForward(deltaTime * cameraSpeed);
            cameraMinimap->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            cameraPrincipala->TranslateRight(-deltaTime * cameraSpeed);
            cameraMinimap->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            cameraPrincipala->TranslateForward(-deltaTime * cameraSpeed);
            cameraMinimap->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            cameraPrincipala->TranslateRight(deltaTime * cameraSpeed);
            cameraMinimap->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            cameraPrincipala->TranslateUpward(-deltaTime * cameraSpeed);
            cameraMinimap->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            cameraPrincipala->TranslateUpward(deltaTime * cameraSpeed);
            cameraMinimap->TranslateUpward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_G)) {
            fov -= deltaTime * cameraSpeed;
            projectionMatrixGame = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.f);
        }

        if (window->KeyHold(GLFW_KEY_H)) {
            fov += deltaTime * cameraSpeed;
            projectionMatrixGame = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.f);
        }

        if (orto) {
            projectionMatrixGame = glm::ortho(-5.f, 5.f, -5.f, 5.f, 0.1f, 200.f);

        }
        if (persp) {
            projectionMatrixGame = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);

        }

        if (window->KeyHold(GLFW_KEY_C)) {
            bottom -= deltaTime;
            top += deltaTime;
            projectionMatrixGame = glm::ortho(left1, right1, bottom, top, 0.01f, 200.0f);
        }

        if (window->KeyHold(GLFW_KEY_B)) {
            bottom += deltaTime;
            top -= deltaTime;
            projectionMatrixGame = glm::ortho(left1, right1, bottom, top, 0.01f, 200.0f);
        }

        if (window->KeyHold(GLFW_KEY_M)) {
            left1 -= deltaTime;
            right1 += deltaTime;
            projectionMatrixGame = glm::ortho(left1, right1, bottom, top, 0.01f, 200.0f);
        }

        if (window->KeyHold(GLFW_KEY_N)) {
            left1 += deltaTime;
            right1 -= deltaTime;
            projectionMatrixGame = glm::ortho(left1, right1, bottom, top, 0.01f, 200.0f);
        }
    }
    
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = renderCameraTarget;
    }

    if (key == GLFW_KEY_O)
    {
        orto = true;
        persp = false;
    }

    if (key == GLFW_KEY_P)
    {
        persp = true;
        orto = false;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            cameraPrincipala->RotateFirstPerson_OY(-sensivityOY * deltaX);
            cameraPrincipala->RotateFirstPerson_OX(-sensivityOX * deltaY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            cameraPrincipala->RotateThirdPerson_OX(-sensivityOX * deltaY);
            cameraPrincipala->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

