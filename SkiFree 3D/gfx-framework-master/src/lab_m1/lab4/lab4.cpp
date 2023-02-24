#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;
    translateXbonus = 6.5f;
    translateXbonus2 = 7.5f, translateZbonus2 = -1.5f;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    angularStepOYbonus = 0;
    time = 0;
    speedMoon = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(4.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateXbonus, 0.5f, -1.5f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(3.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateXbonus2 + 4 * sin(time), 1.0f, translateZbonus2 + 4 * cos(time));
    modelMatrix *= transform3D::RotateOY(angularStepOYbonus*2);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(3.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateXbonus2 + 4 * sin(time), 1.0f, translateZbonus2 + 4 * cos(time));
    modelMatrix *= transform3D::Translate(1.5f * sin(speedMoon), 0.5f, 1.5f * cos(speedMoon));
    modelMatrix *= transform3D::RotateOY(angularStepOYbonus);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds)
{
    time += deltaTimeSeconds;
    speedMoon += deltaTimeSeconds * 3;
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    ratio = miniViewportArea.width / miniViewportArea.height;

    translateXbonus += 0.5f * deltaTimeSeconds;
    translateXbonus2 += 0.5f * deltaTimeSeconds;
   // translateZbonus2 += 0.5f * sin(time);

    angularStepOYbonus += 0.2f * deltaTimeSeconds;
    // TODO(student): render the scene again, in the new viewport
    RenderScene();
    DrawCoordinateSystem();
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // CUBUL 1
    if (window->KeyHold(GLFW_KEY_W)) {
        translateX = translateX + 1 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        translateX = translateX - 1 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        translateZ = translateZ - 1 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        translateZ = translateZ + 1 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_R)) {
        translateY = translateY + 1 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_F)) {
        translateY = translateY - 1 * deltaTime;
    }

    // CUBUL 2
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX = scaleX + 0.1f * deltaTime;
        scaleY = scaleY + 0.1f * deltaTime;
        scaleZ = scaleZ + 0.1f * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX = scaleX - 0.1f * deltaTime;
        scaleY = scaleY - 0.1f * deltaTime;
        scaleZ = scaleZ - 0.1f * deltaTime;
    }

    // CUBUL 3
    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += 0.5f * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= 0.5f * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += 0.5f * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= 0.5f * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += 0.5f * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= 0.5f * deltaTime;
    }
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
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
    
    // TODO(student): Add viewport movement and scaling logic
    // miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height
    if (key == GLFW_KEY_I)
    {
        miniViewportArea.x -= 5;
    }
    if (key == GLFW_KEY_J)
    {
        miniViewportArea.x += 5;
    }
    if (key == GLFW_KEY_K)
    {
        miniViewportArea.y += 5;
    }
    if (key == GLFW_KEY_L)
    {
        miniViewportArea.y -= 5;
    }

    // SCALARE VIEWPORT
    if (key == GLFW_KEY_U)
    {
        //miniViewportArea.width -= 5 ;
        miniViewportArea.height -= 8;
        miniViewportArea.width -= ratio * 8;
        miniViewportArea.y += 4;
        miniViewportArea.x += ratio * 4;
    }
    if (key == GLFW_KEY_O)
    {
        miniViewportArea.height += 8;
        miniViewportArea.width += ratio * 8;
        miniViewportArea.y -= 4;
        miniViewportArea.x -= ratio * 4;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
