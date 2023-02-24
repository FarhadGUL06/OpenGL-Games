#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
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
        Tema2();
        ~Tema2();

        void Init() override;

    private:

        // Functii de generare
        void generateShapes();
        void generateGround(glm::vec3 center, glm::vec3 color, std::string name);
        void generateCar(glm::vec3 center, glm::vec3 color, std::string name);
        void generatePista(glm::vec3 center, glm::vec3 color, std::string name);
        void generateLinie(glm::vec3 center, glm::vec3 color, std::string name, float offset);
        void generateTree(glm::vec3 center, std::string name);

        // Functii de randare
        void renderMultipleTrees(int typeOfRender);
        void renderMultipleEnemies(int typeOfRender, float deltaTimeSeconds, glm::mat4 modelMatrixPista);
        void renderAbilities(int typeOfRender);
        void RenderScene(int typeOfRender, float deltaTimeSeconds);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int typeOfRender, int isEnemy, const glm::vec3& color);
        
        // Functii de start
        void gameInitAbility();
        void gameStart();
        
        // Functii de update
        void updateCamera();
        void updateStats(float deltaTimeSeconds);
        void updateMinimapPos();
        void updateBackgroundColor(float delta);

        // Functii de verificare coliziuni
        float triangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c);
        bool checkPointInTriangle(glm::vec3 point, glm::vec3 a, glm::vec3 b, glm::vec3 c);
        bool checkPointInside(glm::vec3 point);
        bool checkPointCollision(glm::vec3 point, glm::vec3 enemy, float radius);
        bool checkPointInEnemies(glm::vec3 point);
        bool checkPointInAbilities(glm::vec3 point);

        // Functii standard
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
    protected:
        implemented::CameraTema2 *cameraPrincipala, *cameraMinimap;
        glm::mat4 projectionMatrixGame, projectionMatrixMinimap;
        glm::ivec2 resolution;
        bool renderCameraTarget;
        bool orto = false;
        bool persp = false;
        float fov = 60;
        float top = 2.f;
        float bottom = -2.f;
        float left1 = 5.f;
        float right1 = -5.f;

        float translateCarX, translateCarY, translateCarZ;
        glm::vec3 posCar;
        std::vector<glm::vec3> posTree;
        float rotateCar;
        float speedCar, speedCarEnemy;
        float rotateSpeed = 0.5f;
        float distanceToTarget = 3.5f;
        float PI = 3.1415f;
        float miniMapSize = 20.0f;
        float sizeOfCar = 0.7f;
        std::vector<int> posVertex;
        int numberEnemy = 10;
        int numberTriangles = 100;
        int frecvLinie = 2;
        int frecvLinieSecundara = 20;

        bool isAbilitySpeed = false;
        bool isAbilitySlowEnemy = false;
        float abilityTime = 0;

        float colorR = 0, colorG = 0.17f, colorB = 0.51f, colorSign = 1;

        // Culoare standard
        glm::vec3 colorStandard = glm::vec3(0, 0, 0);

        GLenum polygonMode;
        ViewportArea miniViewportArea;
        glm::vec3 colorPista = glm::vec3(0.15f, 0.15f, 0.15f);
        std::vector<VertexFormat> vertexPista{
            VertexFormat(glm::vec3(-22.23569248947097, 0, 25.205326184592725), colorPista),
            VertexFormat(glm::vec3(-27.715446478893888, 0, 25.205326184592725), colorPista),
            VertexFormat(glm::vec3(-32.73081453700978, 0, 25.205326184592725), colorPista),
            VertexFormat(glm::vec3(-37.467551036341455, 0, 23.626414018148832), colorPista),
            VertexFormat(glm::vec3(-40.81112974175205, 0, 21.304484361613692), colorPista),
            VertexFormat(glm::vec3(-42.85442783950297, 0, 18.889677518817148), colorPista),
            VertexFormat(glm::vec3(-45, 0, 15), colorPista),
            VertexFormat(glm::vec3(-46.29088373117497, 0, 10.623607941552052), colorPista),
            VertexFormat(glm::vec3(-47.21965559378902, 0, 5.515362697174747), colorPista),
            VertexFormat(glm::vec3(-47.591164338834645, 0, 0.7786261978430629), colorPista),
            VertexFormat(glm::vec3(-46.569515289959185, 0, -4.236741860272837), colorPista),
            VertexFormat(glm::vec3(-43.96895407463983, 0, -6.651548703069382), colorPista),
            VertexFormat(glm::vec3(-40, 0, -10), colorPista),
            VertexFormat(glm::vec3(-37.00316510503443, 0, -11.481162388662472), colorPista),
            VertexFormat(glm::vec3(-32.86631654442071, 0, -13.192415012566467), colorPista),
            VertexFormat(glm::vec3(-29.108604272814972, 0, -14.45323234902745), colorPista),
            VertexFormat(glm::vec3(-25.48639400862016, 0, -15.660635770425722), colorPista),
            VertexFormat(glm::vec3(-19.263622529105998, 0, -16.217898887994156), colorPista),
            VertexFormat(glm::vec3(-12.672423587435562, 0, -14.418764787282166), colorPista),
            VertexFormat(glm::vec3(-7.93260580521454, 0, -11.29540801613966), colorPista),
            VertexFormat(glm::vec3(-4.960535844849567, 0, -8.509092428297494), colorPista),
            VertexFormat(glm::vec3(-1.2454483943933512, 0, -6.465794330546571), colorPista),
            VertexFormat(glm::vec3(5, 0, -5), colorPista),
            VertexFormat(glm::vec3(9.156796466884053, 0, -5.351268095409704), colorPista),
            VertexFormat(glm::vec3(13.05763828986308, 0, -6.837303075592193), colorPista),
            VertexFormat(glm::vec3(15.100936387613999, 0, -9.066355545865926), colorPista),
            VertexFormat(glm::vec3(17.980129161717567, 0, -12.224179878753716), colorPista),
            VertexFormat(glm::vec3(21.880970984696592, 0, -14.731863907811666), colorPista),
            VertexFormat(glm::vec3(26.13576198823312, 0, -16.73029563729254), colorPista),
            VertexFormat(glm::vec3(30.95984643234617, 0, -15.595216944560054), colorPista),
            VertexFormat(glm::vec3(34.17590272842153, 0, -13.98718879652237), colorPista),
            VertexFormat(glm::vec3(37.48654891555794, 0, -11.433261737874282), colorPista),
            VertexFormat(glm::vec3(40.2296557563281, 0, -6.32540762057811), colorPista),
            VertexFormat(glm::vec3(40, 0, 0), colorPista),
            VertexFormat(glm::vec3(38.432447826168335, 0, 5.687508544174002), colorPista),
            VertexFormat(glm::vec3(36.16229044070337, 0, 11.268312116775377), colorPista),
            VertexFormat(glm::vec3(33.13541392675009, 0, 13.727649284362423), colorPista),
            VertexFormat(glm::vec3(26.797891225660404, 0, 16.943705580437793), colorPista),
            VertexFormat(glm::vec3(21.122497761997995, 0, 20.159761876513162), colorPista),
            VertexFormat(glm::vec3(16.29841331788495, 0, 21.767790024550845), colorPista),
            VertexFormat(glm::vec3(10.628222132162685, 0, 21.22713533112081), colorPista),
            VertexFormat(glm::vec3(5, 0, 20), colorPista),
            VertexFormat(glm::vec3(-1.6352756149942873, 0, 18.856192433337124), colorPista),
            VertexFormat(glm::vec3(-6.949457972095642, 0, 19.83727225310968), colorPista),
            VertexFormat(glm::vec3(-12.34539698084471, 0, 22.53524175748422), colorPista),
            VertexFormat(glm::vec3(-16.842012821468934, 0, 23.925104835495343), colorPista)
        };

        std::vector<glm::vec3> forwardEnemies;
        std::vector<glm::vec3> posEnemies;
        std::vector<glm::vec3> posEnemiesCollision;
        std::vector<glm::vec3> posAbilitiesCollision;
        std::vector<VertexFormat> vertexR, vertexA;
        bool showAbility[5];
        bool isInLap = true;
        int counterLaps = 0;

    };
}   // namespace m1
