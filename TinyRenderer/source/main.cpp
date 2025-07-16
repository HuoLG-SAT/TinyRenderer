#include"../header/renderer.h"
#include"../header/camera.h"
#include"../header/shader.h"
#include"../header/timeUtility.h"
#include"../header/gUIManager.h"

int main()
{
    if (!Renderer::Init())
    {
        std::cout << "Renderer Init Fail!" << std::endl;
        return -1;
    }

    while (!Renderer::IsClose())
    {
        Renderer::UserInput();
        Renderer::Updata();
        Renderer::Render();
    }

    return 0;
}