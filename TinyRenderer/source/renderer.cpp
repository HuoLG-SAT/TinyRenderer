#include"../header/renderer.h"
#include"../header/camera.h"
#include"../header/shader.h"
#include"../header/timeUtility.h"
#include"../header/mathUtility.h"
#include"../header/gUIManager.h"
#include"../header/sceneManager.h"
#include"../header/lightManager.h"
#include"../header/resourceManager.h"
#include"../depend/stb_image/stb_image.h"
#include"../header/grid.h"
#include"../header/screenPostProcess.h"
#include"../header/skybox.h"
#include"../header/instance.h"
#include"../header/shadowMapManager.h"

Renderer::Window& Renderer::WindowI = Window::Instance;
Renderer::Camera& Renderer::CameraI = Camera::Instance;
Renderer::GUIManager& Renderer::GUIManagerI = GUIManager::Instance;
Renderer::SceneManager& Renderer::SceneManagerI = SceneManager::Instance;
Renderer::LightManager& Renderer::LightManagerI = LightManager::Instance;
Renderer::ResourceManager& Renderer::ResourceManagerI = ResourceManager::Instance;
Renderer::ShaderManager& Renderer::ShaderManagerI = ShaderManager::Instance;
Renderer::GridManager& Renderer::GridManagerI = GridManager::Instance;
Renderer::ScreenPostProcess& Renderer::ScreenPostProcessI = ScreenPostProcess::Instance;
Renderer::SkyBoxManager& Renderer::SkyBoxManagerI = SkyBoxManager::Instance;
Renderer::GPUInstanceManager& Renderer::GPUInstanceManagerI = GPUInstanceManager::Instance;
Renderer::ShadowMapManager& Renderer::ShadowMapManagerI = ShadowMapManager::Instance;

bool Renderer::Init()
{
    if (!WindowI.InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        std::cout << "Window Init Error!" << std::endl;
        return false;
    }
    if (!CameraI.InitCamera(WindowI.Width(), WindowI.Height()))
    {
        std::cout << "Camera Init Error!" << std::endl;
        return false;
    }
    if (!ShaderManagerI.LoadShader())
    {
        std::cout << "Shader Load Fail!" << std::endl;
        return false;
    }
    if (!ShadowMapManagerI.Init())
    {
        std::cout << "Shadow Map Manager Init Fail!" << std::endl;
        return false;
    }
    if (!ShaderManagerI.InitUniformBuffer())
    {
        std::cout << "Uniform Block Init Fail!" << std::endl;
        return false;
    }
    stbi_set_flip_vertically_on_load(false);
    if (!ResourceManagerI.InitResource())
    {
        std::cout << "Resource Load Fail!" << std::endl;
        return false;
    }
    if (!GUIManagerI.InitImGui())
    {
        std::cout << "ImGui Load Fail!" << std::endl;
        return false;
    }
    if (!ScreenPostProcessI.InitFrameBuffer() || !ScreenPostProcessI.InitPanelModel() || !ScreenPostProcessI.InitScreenPostProssShader())
    {
        std::cout << "Screen Post Process Init Fail!" << std::endl;
        return false;
    }
    if (!SkyBoxManagerI.InitSkyBox())
    {
        std::cout << "Sky Box Init Fail!" << std::endl;
        return false;
    }
    if (!GridManagerI.LoadGrid())
    {
        std::cout << "Grid Init Fail!" << std::endl;
        return false;
    }
    if (!GPUInstanceManagerI.Init())
    {
        std::cout << "GPU Instance Init Fail!" << std::endl;
        return false;
    }
    stbi_set_flip_vertically_on_load(true);
    return true;
}
void Renderer::UserInput()
{
    WindowI.ProcessUserInput();
    CameraI.ProcessUserInput();
}
void Renderer::Updata()
{
    TimeUtility::UpdataTime();
    MathUtility::Updata();
    CameraI.Updata();
    SceneManagerI.Updata();
    LightManagerI.Updata();
    ShaderManagerI.Updata();
}
void Renderer::Render()
{
    ShadowMapManagerI.ShadowMapping();
    if (ScreenPostProcessI.IsEnableScreenPostProcess())
    {
        ScreenPostProcessI.IsBindDefaultFramebuffer(false);
    }
    WindowI.ClearFrameBuffer();
    SceneManagerI.DrawGeometry();
    GPUInstanceManagerI.Draw();
    if (SkyBoxManagerI.IsEnableSkyBox())
    {
        SkyBoxManagerI.Draw();
    }
    GridManagerI.Draw();
    SceneManagerI.DrawTransparent();
    if (ScreenPostProcessI.IsEnableScreenPostProcess())
    {
        ScreenPostProcessI.Draw();
    }
    GUIManagerI.MarkStart();
    GUIManagerI.RenderGUI();
    WindowI.FlashWindow();
}
bool Renderer::IsClose()
{
    return WindowI.IsClose();
}