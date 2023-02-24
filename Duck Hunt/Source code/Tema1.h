#pragma once

#include <string>

#include "components/simple_scene.h"

namespace m1 {
class Tema1 : public gfxc::SimpleScene {
   public:
    Tema1();
    ~Tema1();

    void Init() override;

   private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;
    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button,
                           int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX,
                       int offsetY) override;
    void OnWindowResize(int width, int height) override;

    // Functii de generare
    void generateDuck(glm::vec3 center);
    void generateBaseDuck(glm::vec3 center, glm::vec3 color, std::string name);
    void generateCiocDuck(glm::vec3 center, glm::vec3 color, std::string name);
    void generateCircle(glm::vec3 center, glm::vec3 color, float radius,
                        std::string name);
    void generateUpWingDuck(glm::vec3 center, glm::vec3 color,
                            std::string name);
    void generateDownWingDuck(glm::vec3 center, glm::vec3 color,
                              std::string name);
    void generateCursor(glm::vec3 center, glm::vec3 color);
    void generateRange(glm::vec3 center, glm::vec3 color);
    void generateLife(glm::vec3 center, glm::vec3 color, float radius,
                      std::string name, int lifeNumber);
    void generateBullet(glm::vec3 center, glm::vec3 color, std::string name,
                        int bulletNumber);
    void generateBulletBoss(glm::vec3 center, glm::vec3 color, std::string name,
                            int bulletNumber);
    void generateScoreBox(glm::vec3 center, glm::vec3 color, std::string name,
                          int maximumScore);
    void generateScore(glm::vec3 center, glm::vec3 color, std::string name,
                       int currentScore, int maximumScore);
    void generatePower(glm::vec3 center, std::string name);
    void generateStartStats();
    void generateGazon(glm::vec3 center, glm::vec3 color, std::string name);
    void generateLevelOrKS(glm::vec3 center, glm::vec3 color, std::string name);

    // Functii de update
    void updateCurrentStats();
    void updateStats();
    void updateLife();
    void updateBullets();
    void updateLevel();
    void updateKS();
    void updateBossFight();
    void updateCursor(glm::vec3 center, glm::vec3 color);

    // Functii speciale
    void rangeHitDuck();
    void escapeDuck();
    void killDuck();
    void spawnDuck();
    void bossDuck();
    void backgroundColor(float delta);
    void randomVariables(float delta);
    void checkSpeedAbility(float delta);
    void endGame();

    // Functii pentru randare
    void renderBullets();
    void renderLives();
    void renderLevels();
    void renderKS();

   protected:
    glm::mat3 modelMatrix;
    std::vector<VertexFormat> vertexDuck;
    std::vector<unsigned int> indicesDuck;
    int indiceStartDuck = 0;
    float pi = 3.141519f;
    float translateXDuck = 0, translateYDuck = 0;
    float speedWings;
    float rotateUpWing = 0, rotateDownWing = 0;
    float centerUpWingX, centerUpWingY;
    float centerDuckX, centerDuckY;
    float currentDuckAngle;
    float centerDownWingX, centerDownWingY;
    float sensRotatie = 1;
    int heightGame, widthGame;
    int widthOfDuck, heightOfDuck, barrierOfDuck;
    float colorR = 0, colorG = 0.17f, colorB = 0.51f, colorSign = 1;
    int cursorX = 0;
    int cursorY = 0;
    int OXStart, OXStop, OYStart, OYStop;
    int rangeAx, rangeAy;
    int rangeBx, rangeBy;
    int rangeCx, rangeCy;
    int rangeDx, rangeDy;
    bool lifeShow[10];
    bool bulletShow[10];
    int numberOfLives, numberOfBullets, currentScore,
        scoreIncrement[4] = {4, 8, 15, 20};
    int ducksPerLevel = 5;
    int maximumBullets, maximumLives, maximumLevel;
    int currentLevel = 1;
    float speedDuck = 260.0f, speedDuckRelease = 250.0f;
    int counterDuck = 0, killingSpreeDuck = 0;
    float timeOfDuck = 0.0f, timeKS = 0.0f;
    bool escapedDuck = false, killedDuck = false;
    float resizeDuckX = 1, resizeDuckY = 1;
    int scoreEndGameX, scoreEndGameY;
    float barrierWings = 0.18f;
    bool endOfGame = false;
    float changeDirection = 0;
    float colorRandomR = 0.4f, colorRandomG = 0.3f, colorRandomB = 0.2f;
    bool reduceSpeedPower = false, canReduceSpeed = false;
    bool isBossDuck = false;
    int counterBulletBossDuck;
    float timerSpeedPower;
    float speedBuffKS = 1;
};
}  // namespace m1
