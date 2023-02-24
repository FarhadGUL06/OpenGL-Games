#include "lab_m1/Tema1/Tema1.h"

#include <iostream>
#include <string>
#include <vector>

#include "lab_m1/Tema1/transform.h"

using namespace std;
using namespace m1;

Tema1::Tema1() {}

Tema1::~Tema1() {}

// Functii de generare

void Tema1::generateCiocDuck(glm::vec3 center, glm::vec3 color, string name) {
    vector<VertexFormat> verticeCiocDuck{
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(0, 16, 0), color),
        VertexFormat(center + glm::vec3(25, 5, 0), color),

    };
    // Vertex pentru toata rata
    vertexDuck.push_back(VertexFormat(center, color));
    vertexDuck.push_back(VertexFormat(center + glm::vec3(0, 16, 0), color));
    vertexDuck.push_back(VertexFormat(center + glm::vec3(25, 5, 0), color));

    std::vector<unsigned int> indiceCiocDuck = {0, 1, 2};

    // Indicii pentru toata rata
    indicesDuck.push_back(indiceStartDuck);
    indicesDuck.push_back(indiceStartDuck + 1);
    indicesDuck.push_back(indiceStartDuck + 2);
    indiceStartDuck += 3;

    Mesh *triangle = new Mesh(name);
    triangle->InitFromData(verticeCiocDuck, indiceCiocDuck);
    meshes[name] = triangle;
}

void Tema1::generateUpWingDuck(glm::vec3 center, glm::vec3 color, string name) {
    vector<VertexFormat> verticeUpWingDuck{
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(70, 12, 0), color),
        VertexFormat(center + glm::vec3(30, 110, 0), color),

    };
    centerUpWingX = 30;
    centerUpWingY = 30;

    std::vector<unsigned int> indiceUpWingDuck = {0, 1, 2};

    Mesh *triangle = new Mesh(name);
    triangle->InitFromData(verticeUpWingDuck, indiceUpWingDuck);
    meshes[name] = triangle;
}

void Tema1::generateDownWingDuck(glm::vec3 center, glm::vec3 color,
                                 string name) {
    vector<VertexFormat> verticeDownWingDuck{
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(64, 3, 0), color),
        VertexFormat(center + glm::vec3(30, -110, 0), color),

    };
    centerDownWingX = 30;
    centerDownWingY = 30;
    std::vector<unsigned int> indiceDownWingDuck = {0, 1, 2};

    Mesh *triangle = new Mesh(name);
    triangle->InitFromData(verticeDownWingDuck, indiceDownWingDuck);
    meshes[name] = triangle;
}

void Tema1::generateBaseDuck(glm::vec3 center, glm::vec3 color, string name) {
    vector<VertexFormat> verticeBaseDuck{
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(0, 60, 0), color),
        VertexFormat(center + glm::vec3(130, 30, 0), color),
    };
    centerDuckX = 30;
    centerDuckY = 30;
    // Vertex pentru toata rata
    vertexDuck.push_back(VertexFormat(center, color));
    vertexDuck.push_back(VertexFormat(center + glm::vec3(0, 60, 0), color));
    vertexDuck.push_back(VertexFormat(center + glm::vec3(130, 30, 0), color));

    std::vector<unsigned int> indiceBaseDuck = {0, 1, 2};

    // Indicii pentru toata rata
    indicesDuck.push_back(indiceStartDuck);
    indicesDuck.push_back(indiceStartDuck + 1);
    indicesDuck.push_back(indiceStartDuck + 2);
    indiceStartDuck += 3;
    Mesh *triangle = new Mesh(name);
    triangle->InitFromData(verticeBaseDuck, indiceBaseDuck);
    meshes[name] = triangle;
}

void Tema1::generateCircle(glm::vec3 center, glm::vec3 color, float radius,
                           string name) {
    vector<VertexFormat> verticeHead;
    std::vector<unsigned int> indiceHead;
    // Incrementul cat mai mic pentru un cerc mai detaliat
    float increment = pi / 500;
    // Indicii 0 si 1 ocupati
    int indiceCurrent = 2;
    verticeHead.push_back(VertexFormat(center, color));
    vertexDuck.push_back(VertexFormat(center, color));
    for (float i = 0.0f; i <= 5.0f * pi; i += increment) {
        verticeHead.push_back(VertexFormat(
            center + glm::vec3(radius * cos(i), radius * sin(i), 0), color));
        vertexDuck.push_back(VertexFormat(
            center + glm::vec3(radius * cos(i), radius * sin(i), 0), color));
        if (i == 0.0f) {
            // La primul pas am doar 2 vertecsi, mai asteptam unul
            // ca sa generam triunghiurile
            continue;
        }
        indiceHead.push_back(indiceCurrent);
        indiceHead.push_back(indiceCurrent - 1);
        indiceHead.push_back(0);

        // Indicii pentru toata rata
        indicesDuck.push_back(indiceCurrent);
        indicesDuck.push_back(indiceCurrent - 1);
        indicesDuck.push_back(0);
        indiceCurrent++;
    }
    indiceStartDuck = indiceCurrent;
    Mesh *circle = new Mesh(name);
    circle->InitFromData(verticeHead, indiceHead);
    meshes[name] = circle;
}

void Tema1::generateLife(glm::vec3 center, glm::vec3 color, float radius,
                         string name, int lifeNumber) {
    vector<VertexFormat> verticesLife;
    std::vector<unsigned int> indicesLife;
    // Incrementul cat mai mic pentru un cerc mai detaliat
    float increment = pi / 360;
    // Indicii 0 si 1 ocupati
    int indiceCurrent = 2;
    verticesLife.push_back(VertexFormat(center, color));
    for (float i = 0.0f; i <= 5.0f * pi; i += increment) {
        verticesLife.push_back(VertexFormat(
            center + glm::vec3(radius * cos(i), radius * sin(i), 0), color));
        if (i == 0.0f) {
            // La primul pas am doar 2 vertecsi, mai asteptam unul
            // ca sa generam triunghiurile
            continue;
        }
        if (lifeShow[lifeNumber]) {
            if (i == 0.0f) {
                // La primul pas am doar 2 vertecsi, mai asteptam unul
                // ca sa generam triunghiurile
                continue;
            }
            indicesLife.push_back(indiceCurrent);
            indicesLife.push_back(indiceCurrent - 1);
            indicesLife.push_back(0);
        } else {
            if (indiceCurrent % 3 != 0 && i == 0.0f) {
                // Nu am 3 vertecsi
                continue;
            }
            if (indiceCurrent > 360) {
                indicesLife.push_back(indiceCurrent - 1);
                indicesLife.push_back(indiceCurrent);
                indicesLife.push_back(indiceCurrent - 2);
            }
        }
        indiceCurrent++;
    }

    Mesh *life = new Mesh(name);
    if (!lifeShow[lifeNumber]) {
        life->SetDrawMode(GL_LINE_LOOP);
    }
    life->InitFromData(verticesLife, indicesLife);
    meshes[name] = life;
}

void Tema1::generateBullet(glm::vec3 center, glm::vec3 color, string name,
                           int bulletNumber) {
    int width = 15;
    int height = 30;
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(width, 0, 0), color),
        VertexFormat(center + glm::vec3(width, height, 0), color),
        VertexFormat(center + glm::vec3(0, height, 0), color)};

    Mesh *bullet = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!bulletShow[bulletNumber]) {
        bullet->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    bullet->InitFromData(vertices, indices);
    meshes[name] = bullet;
}

void Tema1::generateBulletBoss(glm::vec3 center, glm::vec3 color, string name,
                               int bulletNumber) {
    int diag = 30;
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),                               // 0
        VertexFormat(center + glm::vec3(-diag / 2, 0, 0), color),  // 1
        VertexFormat(center + glm::vec3(0, diag / 2, 0), color),   // 2
        VertexFormat(center + glm::vec3(diag / 2, 0, 0), color),   // 3
        VertexFormat(center + glm::vec3(0, -diag / 2, 0), color),  // 4
    };

    Mesh *bullet = new Mesh(name);
    std::vector<unsigned int> indices = {1, 2, 3, 4, 1, 3};

    bullet->InitFromData(vertices, indices);
    meshes[name] = bullet;
}

void Tema1::generateLevelOrKS(glm::vec3 center, glm::vec3 color, string name) {
    int width = 40;
    int height = 40;
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(width, 0, 0), color),
        VertexFormat(center + glm::vec3(width, height, 0), color),
        VertexFormat(center + glm::vec3(0, height, 0), color)};

    Mesh *level = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};
    indices.push_back(0);
    indices.push_back(2);
    level->InitFromData(vertices, indices);
    meshes[name] = level;
}

void Tema1::generateScoreBox(glm::vec3 center, glm::vec3 color, string name,
                             int maximumScore) {
    int height = 38;
    int width = maximumScore * 4 + 2;
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(width, 0, 0), color),
        VertexFormat(center + glm::vec3(width, height, 0), color),
        VertexFormat(center + glm::vec3(0, height, 0), color)};

    Mesh *scoreBox = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};
    scoreBox->SetDrawMode(GL_LINE_LOOP);
    scoreBox->InitFromData(vertices, indices);
    meshes[name] = scoreBox;
}

void Tema1::generateScore(glm::vec3 center, glm::vec3 color, string name,
                          int currentScore, int maximumScore) {
    currentScore =
        min(currentScore, maximumScore);  // daca a trecut de scorul maxim
    int height = 30;
    int width = currentScore > 2 ? currentScore * 4 - 8 : 0;
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(width, 0, 0), color),
        VertexFormat(center + glm::vec3(width, height, 0), color),
        VertexFormat(center + glm::vec3(0, height, 0), color)};

    Mesh *score = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};
    indices.push_back(0);
    indices.push_back(2);
    score->InitFromData(vertices, indices);
    meshes[name] = score;
}

void Tema1::generateGazon(glm::vec3 center, glm::vec3 color, string name) {
    int height = 150;
    int width = widthGame;
    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(width, 0, 0), color),
        VertexFormat(center + glm::vec3(width, height, 0), color),
        VertexFormat(center + glm::vec3(0, height, 0), color)};

    Mesh *gazon = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};
    indices.push_back(0);
    indices.push_back(2);
    gazon->InitFromData(vertices, indices);
    meshes[name] = gazon;
}

void Tema1::generateCursor(glm::vec3 center, glm::vec3 color) {
    int lenCursor = 4;
    vector<VertexFormat> vertexCursor{
        VertexFormat(center + glm::vec3(-3 * lenCursor, -lenCursor, 0), color),
        VertexFormat(center + glm::vec3(-3 * lenCursor, lenCursor, 0), color),
        VertexFormat(center + glm::vec3(3 * lenCursor, lenCursor, 0), color),
        VertexFormat(center + glm::vec3(3 * lenCursor, -lenCursor, 0), color),
        VertexFormat(center + glm::vec3(-lenCursor, -3 * lenCursor, 0), color),
        VertexFormat(center + glm::vec3(lenCursor, -3 * lenCursor, 0), color),
        VertexFormat(center + glm::vec3(-lenCursor, 3 * lenCursor, 0), color),
        VertexFormat(center + glm::vec3(lenCursor, 3 * lenCursor, 0), color),
    };

    std::vector<unsigned int> indicesCursor = {0, 1, 2, 3, 2, 0,
                                               4, 6, 5, 7, 6, 5};

    Mesh *cursor = new Mesh("cursor");
    cursor->InitFromData(vertexCursor, indicesCursor);
    meshes["cursor"] = cursor;
}

void Tema1::generateRange(glm::vec3 center, glm::vec3 color) {
    vector<VertexFormat> vertexRange{
        VertexFormat(center + glm::vec3(rangeAx, rangeAy, 0), color),
        VertexFormat(center + glm::vec3(rangeBx, rangeBy, 0), color),
        VertexFormat(center + glm::vec3(rangeCx, rangeCy, 0), color),
        VertexFormat(center + glm::vec3(rangeDx, rangeDy, 0), color),
    };

    std::vector<unsigned int> indicesRange = {2, 0, 1, 2, 3, 1};

    Mesh *range = new Mesh("range");
    range->SetDrawMode(GL_LINE_LOOP);
    range->InitFromData(vertexRange, indicesRange);
    meshes["range"] = range;
}

void Tema1::generatePower(glm::vec3 center, string name) {
    glm::vec3 colorContur = glm::vec3(0, 0, 0);
    glm::vec3 colorFill = glm::vec3(0.03f, 0.34f, 0.97f);
    glm::vec3 colorText = glm::vec3(1, 1, 1);

    int border = 25;
    int rama = border - 4;
    vector<VertexFormat> vertex{
        // Vertecsi pentru P
        VertexFormat(center, colorText),                          // 0
        VertexFormat(center + glm::vec3(-5, 10, 0), colorText),   // 1
        VertexFormat(center + glm::vec3(0, -14, 0), colorText),   // 2
        VertexFormat(center + glm::vec3(-5, -14, 0), colorText),  // 3
        VertexFormat(center + glm::vec3(0, 6, 0), colorText),     // 4
        VertexFormat(center + glm::vec3(0, -4, 0), colorText),    // 5
        VertexFormat(center + glm::vec3(10, -4, 0), colorText),   // 6
        VertexFormat(center + glm::vec3(10, 0, 0), colorText),    // 7
        VertexFormat(center + glm::vec3(6, 0, 0), colorText),     // 8
        VertexFormat(center + glm::vec3(10, 10, 0), colorText),   // 9
        VertexFormat(center + glm::vec3(6, 10, 0), colorText),    // 10
        VertexFormat(center + glm::vec3(6, 6, 0), colorText),     // 11
        VertexFormat(center + glm::vec3(0, 10, 0),
                     colorText),  // 12
                                  // Vertecsi pentru rama
        VertexFormat(center + glm::vec3(-border, -border, 0), colorText),  // 13
        VertexFormat(center + glm::vec3(border, -border, 0), colorText),   // 14
        VertexFormat(center + glm::vec3(-border, border, 0), colorText),   // 15
        VertexFormat(center + glm::vec3(border, border, 0), colorText),    // 16

        VertexFormat(center + glm::vec3(-rama, -rama, 0), colorText),  // 17
        VertexFormat(center + glm::vec3(rama, -rama, 0), colorText),   // 18
        VertexFormat(center + glm::vec3(-rama, rama, 0), colorText),   // 19
        VertexFormat(center + glm::vec3(rama, rama, 0), colorText),    // 20

        // Vertecsi pentru culoarea din interior
        VertexFormat(center + glm::vec3(-rama, -rama, 0), colorFill),  // 21
        VertexFormat(center + glm::vec3(rama, -rama, 0), colorFill),   // 22
        VertexFormat(center + glm::vec3(-rama, rama, 0), colorFill),   // 23
        VertexFormat(center + glm::vec3(rama, rama, 0), colorFill),    // 24
    };

    std::vector<unsigned int> indices = {
        // Indicii pentru P
        0, 1, 2, 2, 3, 1, 4, 0, 1, 0, 5, 6, 6, 0, 7, 7, 8, 9, 9, 8, 10, 1, 11,
        10, 10, 4, 1, 12, 4, 11,
        // Indicii pentru rama
        13, 19, 15, 15, 17, 19, 19, 15, 16, 15, 19, 20, 16, 14, 20, 18, 20, 16,
        20, 14, 16, 13, 18, 17, 18, 13, 14, 17, 14, 13, 16, 18, 14, 19, 17, 15,
        13, 15, 19, 13, 14, 18, 17, 14, 18,

        // Indicii pentru continut
        23, 21, 24, 22, 21, 24

    };
    Mesh *power = new Mesh("power");
    // power->SetDrawMode(GL_LINE_LOOP);

    power->InitFromData(vertex, indices);
    meshes["power"] = power;
}

void Tema1::generateDuck(glm::vec3 center) {
    // Generam componentele unei rate
    // glm::vec3 colorBase = glm::vec3(0.4f, 0.3f, 0.2f);
    glm::vec3 colorBase = glm::vec3(colorRandomR, colorRandomG, colorRandomB);
    glm::vec3 colorHead = glm::vec3(0, 0.55f, 0);
    glm::vec3 colorCioc = glm::vec3(0.8f, 0.8f, 0);

    // Marimea ratei (necesara coliziunii)
    widthOfDuck = 200;
    heightOfDuck = 180;

    // Limita de iesire din ecran
    barrierOfDuck = 20;

    // Centrul cercului va fi in varful triunghiului bazei ratei
    string duckHead = "duckHead";
    generateCircle(center + glm::vec3(130, 30, 0), colorHead, 32.0f, duckHead);
    // Baza ratei, triunghiul principal
    string baseDuck = "baseDuck";
    generateBaseDuck(center, colorBase, baseDuck);
    // Ciocul ratei
    string ciocDuck = "ciocDuck";
    generateCiocDuck(center + glm::vec3(155, 20, 0), colorCioc, ciocDuck);

    // Corpul ratei
    Mesh *duck = new Mesh("duck");
    duck->InitFromData(vertexDuck, indicesDuck);
    meshes["duck"] = duck;

    // Aripa de sus
    string upWingDuck = "upWingDuck";
    generateUpWingDuck(center + glm::vec3(10, 20, 0), colorBase, upWingDuck);

    // Aripa de jos
    string downWingDuck = "downWingDuck";
    generateDownWingDuck(center + glm::vec3(10, 30, 0), colorBase,
                         downWingDuck);
}

void Tema1::generateStartStats() {
    // Atributele pentru Boss Duck
    counterBulletBossDuck = 0;
    isBossDuck = false;

    spawnDuck();

    glm::vec3 center = glm::vec3(0, 0, 0);
    glm::vec3 centerScore = glm::vec3(0, 0, 0);
    glm::vec3 centerGazon = glm::vec3(0, 0, 0);

    glm::vec3 colorCursor = glm::vec3(1, 1, 1);
    glm::vec3 colorScore = glm::vec3(0.8f, 1, 1);
    glm::vec3 colorGazon = glm::vec3(0, 0.57f, 0.1f);
    updateCursor(center, colorCursor);

    // ScoreBox-ul
    string scoreBox = "scoreBox";
    generateScoreBox(centerScore, colorScore, scoreBox, 100);
    currentScore = 0;
    scoreEndGameX = widthGame - 420;
    scoreEndGameY = heightGame - 50;

    // Gazon
    string gazon = "gazon";
    generateGazon(centerGazon, colorGazon, gazon);

    // Abilitate
    string power = "power";
    generatePower(center, power);

    // Jucatorul porneste cu maximum 3 gloante si 3 vieti
    maximumLives = 3;
    maximumBullets = 3;
    maximumLevel = 5;
    // Jucatorul incepe cu maximum stats
    numberOfLives = maximumLives;
    numberOfBullets = maximumBullets;

    // Actualizam incrementul de scor pentru maxim 100
    float factor = (100 - scoreIncrement[3]) / ducksPerLevel;
    scoreIncrement[currentLevel - 1] = (int)(0.20f * factor);
    scoreIncrement[currentLevel] = (int)(0.25f * factor);
    scoreIncrement[currentLevel + 1] = (int)(0.57f * factor);
}

// Functii de update

void Tema1::updateLife() {
    glm::vec3 centerLife = glm::vec3(50, heightGame - 50, 0);
    glm::vec3 colorLife = glm::vec3(0.9f, 0.3f, 0.2f);
    for (int i = 0; i < maximumLives; i++) {
        string lifeNo = "life" + to_string(i + 1);
        generateLife(centerLife + glm::vec3(i * 50, 0, 0), colorLife, 20.0f,
                     lifeNo, i);
    }
}

void Tema1::updateBullets() {
    glm::vec3 centerBullet = glm::vec3(50, heightGame - 120, 0);
    glm::vec3 colorBullet = glm::vec3(0.1f, 0.8f, 0.8f);
    for (int i = 0; i < maximumBullets; i++) {
        string bulletNo = "glont" + to_string(i + 1);
        generateBullet(centerBullet + glm::vec3(i * 40, 0, 0), colorBullet,
                       bulletNo, i);
    }
}

void Tema1::updateLevel() {
    glm::vec3 centerLevel = glm::vec3(0, -50, 0);
    glm::vec3 colorLevel = glm::vec3(0.9f, 0.35f, 0.08f);
    for (int i = 0; i < maximumLevel; i++) {
        string levelNo = "level" + to_string(i + 1);
        generateLevelOrKS(centerLevel + glm::vec3(i * 50, 0, 0), colorLevel,
                          levelNo);
    }
}

void Tema1::updateKS() {
    glm::vec3 centerKS = glm::vec3(0, 0, 0);
    glm::vec3 colorKS = glm::vec3(0.7f, 0.68f, 0.08f);
    for (int i = 0; i < killingSpreeDuck / 3 && i < 3; i++) {
        string KSNo = "ks" + to_string(i + 1);
        generateLevelOrKS(centerKS + glm::vec3(i * 60, 0, 0), colorKS, KSNo);
    }
}

void Tema1::updateBossFight() {
    glm::vec3 centerBossFight = glm::vec3(50, heightGame - 160, 0);
    glm::vec3 colorBossFight = glm::vec3(0.9f, 0.2f, 0.2f);
    for (int i = 0; i < counterBulletBossDuck; i++) {
        string ctrBullet = "ctrBullet" + to_string(i + 1);
        generateBulletBoss(centerBossFight + glm::vec3(i * 40, 0, 0),
                           colorBossFight, ctrBullet, i);
    }
}

void Tema1::updateStats() {
    // Viata
    updateLife();

    // Gloante
    updateBullets();

    // Level
    updateLevel();

    // Killing Spree
    updateKS();

    // Update Boss Fight
    updateBossFight();

    // Scor
    glm::vec3 centerScore = glm::vec3(4, 4, 0);
    glm::vec3 colorScore = glm::vec3(0.8f, 1, 1);
    string score = "score";
    generateScore(centerScore, colorScore, score, currentScore, 100);
}

void Tema1::updateCursor(glm::vec3 center, glm::vec3 color) {
    // Generam cursorul
    generateCursor(center, color);
}

void Tema1::updateCurrentStats() {
    // Actualizam vietile
    for (int i = 0; i < numberOfLives; i++) {
        lifeShow[i] = true;
    }
    for (int i = numberOfLives; i < maximumLives; i++) {
        lifeShow[i] = false;
    }
    // Actualizam gloantele
    for (int i = 0; i < numberOfBullets; i++) {
        bulletShow[i] = true;
    }
    for (int i = numberOfBullets; i < maximumBullets; i++) {
        bulletShow[i] = false;
    }
    updateStats();
}

// Functii speciale

void Tema1::rangeHitDuck() {
    int OXDuck = widthOfDuck;
    int OYDuck = heightOfDuck;
    // Barierele pentru coliziune
    OXStart = -widthOfDuck / 6 - centerDuckX / 4;
    OXStop = -widthOfDuck / 6 + OXDuck - centerDuckX;
    OYStart = -heightOfDuck / 6 - 2.4f * centerDuckY;
    OYStop = -heightOfDuck / 6 + OYDuck - 1.5f * centerDuckY;

    // A = (OXStart, OYStart)
    // B = (OXStart, OYStop)
    // C = (OXStop, OYStart)
    // D = (OXStop, OYStop)

    float cosAngle = cos(-currentDuckAngle);
    float sinAngle = sin(-currentDuckAngle);

    int posXRange = translateXDuck + widthOfDuck / 6;
    int posYRange = translateYDuck + heightOfDuck / 6;

    rangeAx = posXRange + cosAngle * OXStart + sinAngle * OYStart;
    rangeAy = posYRange - sinAngle * OXStart + cosAngle * OYStart;

    rangeBx = posXRange + cosAngle * OXStart + sinAngle * OYStop;
    rangeBy = posYRange - sinAngle * OXStart + cosAngle * OYStop;

    rangeCx = posXRange + cosAngle * OXStop + sinAngle * OYStart;
    rangeCy = posYRange - sinAngle * OXStop + cosAngle * OYStart;

    rangeDx = posXRange + cosAngle * OXStop + sinAngle * OYStop;
    rangeDy = posYRange - sinAngle * OXStop + cosAngle * OYStop;
}

void Tema1::spawnDuck() {
    // Pozitia de inceput a primei rate
    translateXDuck = rand() % 800 + 100;
    translateYDuck = 50;

    // Directia primei rate
    int randomAngle = rand() % 125 + 35;
    currentDuckAngle = pi / 180 * (float)randomAngle;

    // Actualizam viteza aripilor
    speedWings = 0.5f + currentLevel / 100.f;

    glm::vec3 center = glm::vec3(0, 0, 0);
    generateDuck(center);
}

void Tema1::escapeDuck() {
    // Rata va scapa
    escapedDuck = true;
    currentDuckAngle = pi / 2;
    speedDuck += speedDuckRelease;
    killingSpreeDuck = 0;
    if (isBossDuck) {
        counterBulletBossDuck = 0;
    }
}

void Tema1::killDuck() {
    // Rata a murit
    killedDuck = false;  // sa nu cicleze aici
    // Animatie pentru aripi
    speedWings = 0;
    rotateUpWing = pi / 8;
    rotateDownWing = -pi / 8;
    currentDuckAngle = pi + pi / 2;
    speedDuck += speedDuckRelease;
}

void Tema1::endGame() {
    // Dispar vietile si gloantele
    maximumBullets = 0;
    maximumLives = 0;
    speedDuck = 0;
    currentDuckAngle = pi;
    resizeDuckX = 3.4f;
    resizeDuckY = 2.8f;
    translateXDuck = widthGame / 2 + 120;
    translateYDuck = heightGame / 2 + 50;
    // Pozitionam scorul sub rata
    scoreEndGameX = widthGame / 2 - 200;
    scoreEndGameY = 60;
    barrierWings = 0.11f;
    speedWings = 0.4f;
    killingSpreeDuck = 0;
    counterBulletBossDuck = 0;
    endOfGame = true;
}

void Tema1::randomVariables(float delta) {
    // Random move angle
    if ((int)(delta * 1000) % 3 == 0 && !escapedDuck && speedWings != 0 &&
        !endOfGame) {
        int sansa = rand() % 7;
        if (sansa == 2 && changeDirection > delta - 2) {
            currentDuckAngle += pi / 8;
            if (currentDuckAngle > 2 * pi) {
                currentDuckAngle = pi / 16;
            }
            if (currentDuckAngle < 0.0f) {
                currentDuckAngle = 2 * pi - pi / 16;
            }
            changeDirection = delta;
        }
    }
    // Random duck colors
    colorRandomR = (float)(rand() % 10) / 10;
    colorRandomG = (float)(rand() % 10) / 10;
    colorRandomB = (float)(rand() % 10) / 10;
}

void Tema1::checkSpeedAbility(float delta) {
    if (canReduceSpeed && !endOfGame) {
        // Se poate folosi abilitatea deci apare
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(widthGame - 100, 100);
        if (!reduceSpeedPower) {
            // Daca nu este in folosire acum
            modelMatrix *= transform::Translate(12, 12);
            modelMatrix *= transform::Scale(2 + rotateUpWing, 2 + rotateUpWing);
            modelMatrix *= transform::Translate(-12, -12);
        } else {
            modelMatrix *= transform::Translate(-12, -12);
            modelMatrix *= transform::Scale(2, 2);
        }
        RenderMesh2D(meshes["power"], shaders["VertexColor"], modelMatrix);
    }
    // Cresc timer pentru abilitate de speed limit
    if (reduceSpeedPower) {
        timerSpeedPower += 2 * delta;
    }
    if (timerSpeedPower > 5.0f) {
        reduceSpeedPower = false;
        speedBuffKS = 1;
        // Nu mai poate folosi abilitatea
        canReduceSpeed = false;
    }
}

void Tema1::bossDuck() {
    /*
        Ultima rata va muri doar daca
        e impuscata de 3 ori
        La fiecare glont, viteza scade
    */
    for (int i = 0; i < counterBulletBossDuck; i++) {
        string ctrBullet = "ctrBullet" + to_string(i + 1);
        modelMatrix = glm::mat3(1);
        // modelMatrix *= transform::Rotate(pi / 2);
        RenderMesh2D(meshes[ctrBullet], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::backgroundColor(float delta) {
    // Schimbam culoarea in functie de timp
    colorB += colorSign * delta / 10.0f;
    if (colorB > 1) {
        colorSign *= -1;
    }
    if (colorB < 0.30f) {
        colorSign *= -1;
    }
}

// Functii pentru randare

void Tema1::renderBullets() {
    for (int i = 0; i < maximumBullets; i++) {
        string glontNo = "glont" + to_string(i + 1);
        // Gloante
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes[glontNo], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::renderLives() {
    for (int i = 0; i < maximumLives; i++) {
        string lifeNo = "life" + to_string(i + 1);
        // Gloante
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes[lifeNo], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::renderLevels() {
    for (int i = 0; i < currentLevel; i++) {
        string levelNo = "level" + to_string(i + 1);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(scoreEndGameX, scoreEndGameY);
        RenderMesh2D(meshes[levelNo], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::renderKS() {
    for (int i = 0; i < killingSpreeDuck / 3 && i < 3; i++) {
        string KSNo = "ks" + to_string(i + 1);
        modelMatrix = glm::mat3(1);
        modelMatrix *=
            transform::Translate(scoreEndGameX + 250, scoreEndGameY - 50);
        modelMatrix *= transform::Translate(i * 60 - 20, 20);
        modelMatrix *= transform::Rotate(5 * timeKS);
        modelMatrix *= transform::Translate(-i * 60 - 20, -20);
        RenderMesh2D(meshes[KSNo], shaders["VertexColor"], modelMatrix);
    }
}

// Functii din framework

void Tema1::Init() {
    srand(time(0));
    glm::ivec2 resolution = window->GetResolution();
    // Lungimea ferestrei la initiere
    widthGame = resolution.x;
    heightGame = resolution.y;
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y,
                            0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    spawnDuck();
    generateStartStats();
}

void Tema1::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorR, colorG, colorB, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw-
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds) {
    // Random Moves and random colors
    randomVariables(deltaTimeSeconds);

    if (numberOfLives <= 0 || currentScore == 100) {
        // Se termina jocul
        endGame();
    }

    // Pentru abilitatea de incetinire
    checkSpeedAbility(deltaTimeSeconds);

    updateCurrentStats();
    timeOfDuck += deltaTimeSeconds;
    timeKS += deltaTimeSeconds;

    // Scapa rata
    if ((timeOfDuck > 12.0f - (float)(currentLevel)*1.5f ||
         numberOfBullets <= 0) &&
        !escapedDuck) {
        escapeDuck();
    }
    // A murit rata
    if (killedDuck) {
        killDuck();
    }

    // Cursor
    modelMatrix = glm::mat3(1);
    modelMatrix = transform::Translate(cursorX, cursorY);
    modelMatrix *= transform::Rotate(10 * rotateUpWing);
    RenderMesh2D(meshes["cursor"], shaders["VertexColor"], modelMatrix);

    // Vieti
    renderLives();

    // Gloante
    renderBullets();

    // Level
    renderLevels();

    // Killing Spree
    renderKS();

    // Boss Duck
    if (counterDuck == ducksPerLevel * (maximumLevel - 2) && !isBossDuck) {
        isBossDuck = true;
        counterBulletBossDuck = 0;
        resizeDuckX *= 1.2f;
        resizeDuckY *= 1.2f;
    }

    if (isBossDuck) {
        bossDuck();
    }

    // Background
    backgroundColor(deltaTimeSeconds);

    // Scor
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(scoreEndGameX, scoreEndGameY);
    RenderMesh2D(meshes["scoreBox"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(scoreEndGameX, scoreEndGameY);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

    if (!endOfGame) {
        // Gazon
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["gazon"], shaders["VertexColor"], modelMatrix);
        // PENTRU RANGE
        generateRange(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        rangeHitDuck();
        modelMatrix = glm::mat3(1);
        // RenderMesh2D(meshes["range"], shaders["VertexColor"], modelMatrix);
    }

    // Corpul ratei (fara aripi)
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(translateXDuck, translateYDuck);
    modelMatrix *= transform::Translate(centerDuckX, centerDuckY);
    modelMatrix *= transform::Rotate(currentDuckAngle);
    modelMatrix *= transform::Translate(-centerDuckX, -centerDuckY);
    modelMatrix *= transform::Scale(resizeDuckX, resizeDuckY);
    RenderMesh2D(meshes["duck"], shaders["VertexColor"], modelMatrix);

    // Aripa de sus
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(translateXDuck, translateYDuck);
    modelMatrix *= transform::Translate(centerUpWingX, centerUpWingY);
    modelMatrix *= transform::Rotate(currentDuckAngle);
    modelMatrix *= transform::Rotate(rotateUpWing);
    modelMatrix *= transform::Translate(-centerUpWingX, -centerUpWingY);
    modelMatrix *= transform::Scale(resizeDuckX, resizeDuckY);
    RenderMesh2D(meshes["upWingDuck"], shaders["VertexColor"], modelMatrix);

    // Aripa de jos
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(translateXDuck, translateYDuck);
    modelMatrix *= transform::Translate(centerDownWingX, centerDownWingY);
    modelMatrix *= transform::Rotate(currentDuckAngle);
    modelMatrix *= transform::Rotate(rotateDownWing);
    modelMatrix *= transform::Translate(-centerDownWingX, -centerDownWingY);
    modelMatrix *= transform::Scale(resizeDuckX, resizeDuckY);
    RenderMesh2D(meshes["downWingDuck"], shaders["VertexColor"], modelMatrix);

    /*
        Deplasarea ratei calculata in functie de unghiul de pornire
        al ratei fata de orizontala
        Impartirea miscarii pe cele 2 axe in functie de unghi
     */
    float deplasare = speedBuffKS * speedDuck * deltaTimeSeconds;
    // Aplicam incetinirea boss-ului
    if (counterBulletBossDuck != 0) {
        deplasare = deplasare / counterBulletBossDuck;
    }
    translateXDuck += deplasare * cos(currentDuckAngle);
    translateYDuck += deplasare * sin(currentDuckAngle);

    // Rotatia aripilor
    if (speedWings != 0) {
        rotateUpWing += sensRotatie * speedWings * deltaTimeSeconds;
        rotateDownWing -= sensRotatie * speedWings * deltaTimeSeconds;
        // Schimbam directia rotatiei aripilor
        if (rotateUpWing > barrierWings || rotateDownWing < -barrierWings) {
            sensRotatie = -sensRotatie;
            rotateUpWing = barrierWings;
            rotateDownWing = -barrierWings;
        }
        if (rotateUpWing < -barrierWings || rotateDownWing > barrierWings) {
            sensRotatie = -sensRotatie;
            rotateUpWing = -barrierWings;
            rotateDownWing = barrierWings;
        }
    }

    // Stabilim limitele ratei (sa nu poata iesi din ecran)
    // Formula de rotire rata: 2 (pi/2 - currentDuckAngle)
    if ((int)translateXDuck + barrierOfDuck >= widthGame) {
        // Daca rata trece de peretele din dreapta
        if (sin(currentDuckAngle) < 0) {
            // Se deplaseaza in jos pe OY - cadranul 4
            currentDuckAngle += 2 * (pi / 2 - currentDuckAngle);
        } else {
            // Se deplaseaza in sus pe OY - cadranul 1
            currentDuckAngle += 2 * (pi / 2 - currentDuckAngle);
        }
        translateXDuck -= centerDuckX / 5;
    } else {
        if ((int)translateXDuck - barrierOfDuck <= 0) {
            // Daca rata trece de peretele din stanga
            if (sin(currentDuckAngle) < 0) {
                // Se deplaseaza in jos pe OY - cadranul 3
                currentDuckAngle += 2 * (pi / 2 - currentDuckAngle);
                // => trecem in cadranul 4
            } else {
                // Se deplaseaza in sus pe OY - cadranul 2
                currentDuckAngle += 2 * (pi / 2 - currentDuckAngle);
                // => trecem in cadranul 1
            }
            translateXDuck += centerDuckX / 5;
        } else {
            if ((int)translateYDuck + barrierOfDuck >= heightGame) {
                // Daca rata nu scapa
                if (!escapedDuck) {
                    // Daca rata trece de peretele de sus
                    if (cos(currentDuckAngle) < 0) {
                        // Se deplaseaza in stanga pe OX - cadranul 2
                        currentDuckAngle += 2 * (pi - currentDuckAngle);
                        // => trecem in cadranul 3
                    } else {
                        // Se deplaseaza in dreapta pe OX - cadranul 1
                        currentDuckAngle += 2 * (pi - currentDuckAngle);
                        // => trecem in cadranul 4
                    }
                    translateYDuck -= centerDuckY / 5;
                } else {
                    // Repozitionam rata
                    escapedDuck = false;
                    numberOfBullets = maximumBullets;
                    numberOfLives--;
                    speedDuck -= speedDuckRelease;
                    // Respawnam rata
                    spawnDuck();
                    timeOfDuck = 0;
                }
            } else {
                if ((int)translateYDuck - barrierOfDuck <= 50) {
                    // Daca rata nu e moarta
                    if (speedWings > 0) {
                        // Daca rata trece de peretele de jos
                        if (cos(currentDuckAngle) < 0) {
                            // Se deplaseaza in stanga pe OX - cadranul 3
                            currentDuckAngle += 2 * (pi - currentDuckAngle);
                            // => trecem in cadranul 2
                        } else {
                            // Se deplaseaza in dreapta pe OX - cadranul 4
                            currentDuckAngle += 2 * (pi - currentDuckAngle);
                            // => trecem in cadranul 1
                        }
                        translateYDuck += centerDuckY / 5;
                    } else {
                        // Rata este moarta
                        speedDuck -= speedDuckRelease;
                        // Respawnam rata
                        spawnDuck();
                        timeOfDuck = 0;
                        if (killingSpreeDuck == 3) {
                            // Dam un glont in plus
                            maximumBullets++;
                        }
                        if (killingSpreeDuck == 6) {
                            // Dam o viata in plus
                            maximumLives++;
                            numberOfLives++;
                            // Poate folosi abilitatea P
                            canReduceSpeed = true;
                        }
                        if (killingSpreeDuck == 9) {
                            // Dam 2 gloante in plus
                            maximumBullets += 2;
                        }
                        // Resetam gloantele
                        numberOfBullets = maximumBullets;
                    }
                }
            }
        }
    }
}

void Tema1::FrameEnd() {}

void Tema1::OnInputUpdate(float deltaTime, int mods) {}

void Tema1::OnKeyPress(int key, int mods) {
    // Foloseste abilitatea de incetinit rata
    if (key == GLFW_KEY_P && canReduceSpeed && !endOfGame) {
        // Reduce viteza la 38% timp de cateva secunde
        reduceSpeedPower = true;
        timerSpeedPower = 0.0f;
        speedBuffKS = 0.38f;
    }
}

void Tema1::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    /*
        Verificam rezolutia curenta
        si scalam la rezolutia de baza
        ca sa functioneze  cursorul pentru fullscreen sau alte rezolutii
    */
    glm::ivec2 resolution = window->GetResolution();
    cursorX = mouseX * widthGame / resolution.x + deltaX;
    // OY incepe cu 0 invers
    cursorY = heightGame - (mouseY * heightGame / resolution.y) + deltaY;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Daca s-a terminat jocul sau a murit rata
    if (endOfGame || speedWings == 0 || escapedDuck) {
        return;
    }

    // Schimbam culoarea cursorului
    glm::vec3 center = glm::vec3(0, 0, 0);
    glm::vec3 colorCursor = glm::vec3(0.8f, 0.8f, 0);
    glm::vec3 colorCursorHit = glm::vec3(0.8f, 0, 0);

    // Calculam barierele pentru hitbox
    int barrierMinOx = min(rangeAx, rangeDx);
    int barrierMinOy = min(rangeAy, rangeDy);
    int barrierMaxOx = max(rangeAx, rangeDx);
    int barrierMaxOy = max(rangeAy, rangeDy);

    int centerOx = barrierMinOx + (barrierMaxOx - barrierMinOx) / 2;
    int centerOy = barrierMinOy + (barrierMaxOy - barrierMinOy) / 2;
    if (numberOfBullets > 0) {
        if (cursorX >= centerOx - widthOfDuck / 2 &&
            cursorX <= centerOx + widthOfDuck / 2) {
            if (cursorY >= centerOy - heightOfDuck / 2 &&
                cursorY <= centerOy + heightOfDuck / 2) {
                // Verificam daca suntem in Boss Fight
                if (isBossDuck) {
                    // Resetam timpul pentru a mai da timp jucatorului
                    timeOfDuck = 0;
                    updateCursor(center, colorCursorHit);
                    counterBulletBossDuck++;
                    numberOfBullets--;
                }
                if ((isBossDuck && counterBulletBossDuck == 3) || !isBossDuck) {
                    // Am lovit in hitbox
                    currentDuckAngle = pi + pi / 2;
                    // Crestem numarul de rate omorate
                    counterDuck++;
                    killingSpreeDuck++;
                    // Resetam timpul ratei
                    timeOfDuck = 0;
                    // Crestem scorul
                    currentScore += scoreIncrement[currentLevel - 1];

                    if (counterDuck != 0 && counterDuck % ducksPerLevel == 0) {
                        // au fost omorate 5 rate -> level up
                        currentLevel++;
                        speedDuck *= 1.6f;
                        // Crestem marimea ratei
                        resizeDuckX *= 1.12f;
                        resizeDuckY *= 1.12f;
                    }
                    updateCursor(center, colorCursorHit);
                    killedDuck = true;
                }
            }
        } else {
            // Scadem un glont daca nu a scapat rata
            if (!escapedDuck) {
                numberOfBullets--;
                updateCursor(center, colorCursor);
            }
        }
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
    updateCursor(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema1::OnWindowResize(int width, int height) {}
