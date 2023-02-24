#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema3/camera.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
     public:
         struct ViewportArea
         {
             ViewportArea() : x(0), y(0), width(1), height(1) {}
             ViewportArea(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };
         Tema3();
         ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture, int typeObject, int isSpotty, int positionObject);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void updateCamera();
        void RenderScene();
        void RenderObjects();
        void RenderTree(int i);
        void RenderRock(int i);
        void RenderStalp(int i);
        void RenderGift(int i);
        void GenerateObjects(bool isFirst);

        // Coliziuni
        bool checkPointCollision(glm::vec3 point, glm::vec3 other, float radius);
        std::pair<int, int> checkPointInObject(glm::vec3 point, float radius);
        void PointInObject(glm::vec3 point);

        bool CheckObjectInQuad(int position);
        std::unordered_map<std::string, Texture2D *> mapTextures;

        //void generateCar(glm::vec3 center, glm::vec3 color, std::string name);
        //void generateSki(glm::vec3 center, glm::vec3 color, std::string name);
    protected:
        implemented::CameraTema3 *cameraPrincipala;
        bool cameraCurrent = 0;
        glm::mat4 projectionMatrixGame;
        // Variabile pentru miscarea planului sincron cu obiectul
        float movingSpeed = 0.0f;
        float movingAcceleration = 2.5f;
        float anglePlane = 30.0f;
        float angleMovingCar = 30.0f;
        float movingSpeedAnt = 0.0f;
        float sizeOfCar = 0.1f;
        float distanceToTarget = 2.0f;
        int widthGame;

        float sizeOfPlan = 50.0f;
        float ratioPlan = 1;

        float savedPOZ = 0;
        float savedPOY = 0;
        float savedPOX = 0;


        float proximityRender = 20.0f;
        float proximityGenerate = 20.0f;
        float proximityObject = 1.0f;

        int noGifts = 0;
        std::vector<int> allGifts;
        float timeSurvived = 0;
        std::vector<float> allTimes;
        int noRound = 1;

        glm::vec3 posPlan, posCar, posSki1, posSki2;
        std::vector<glm::vec3> posObjects, posStalpi, posLightPoint, colorLightPoint;
        std::vector<int> typeObjects;
        int noObjects = 60;


        // Pentru lumina directionala
        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        // Pentru faruri
        float cutOff = 30.0f;
        float angleX, angleZ;
    };
}   // namespace m1
