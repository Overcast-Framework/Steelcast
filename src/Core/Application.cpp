#include <Core/Application.h>
#include <Events/EventArgs.h>
#include <Graphics/Camera.h>

Application* Application::instance = nullptr;

Application::Application(const AppSettings& settings)
{
    if (instance == nullptr)
    {
        instance = this;
        AppWindow = std::make_unique<Window>(settings.Title, settings.Size);
        RenderAPI = settings.RenderAPI;
    }
    else
    {
        std::cerr << "Application instance already exists." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

Application::~Application()
{
    Close();
}

void Application::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (RenderAPI == RendererAPI::DirectX11)
    {
        m_Renderer = std::make_unique<DX11Renderer>();
        m_Renderer->Initialize(AppWindow.get());
    }

    std::cout << "Application Initialized." << std::endl;
}

void Application::Close()
{
    EventSys->Halt();
    SDL_DestroyWindow(AppWindow->SDLWindow.get());
    SDL_Quit();
}

struct MatrixBuffer
{
    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX MVP;
    float padding;
};

DX11Renderer& Application::GetDX11Renderer()
{
    DX11Renderer* dx11Renderer = dynamic_cast<DX11Renderer*>(m_Renderer.get());

    if (!dx11Renderer) {
        SC_ErrorEvent("Failed to cast Renderer to DX11Renderer");
    }

    return *dx11Renderer;
}

std::vector<SCVertex> GetCubeVertices()
{
    std::vector<SCVertex> vertices;

    // Define the 24 vertices for the cube (4 vertices per face * 6 faces)
    // Positions
    SCVector3f p0(-0.5f, -0.5f, -0.5f);
    SCVector3f p1(0.5f, -0.5f, -0.5f);
    SCVector3f p2(0.5f, 0.5f, -0.5f);
    SCVector3f p3(-0.5f, 0.5f, -0.5f);
    SCVector3f p4(-0.5f, -0.5f, 0.5f);
    SCVector3f p5(0.5f, -0.5f, 0.5f);
    SCVector3f p6(0.5f, 0.5f, 0.5f);
    SCVector3f p7(-0.5f, 0.5f, 0.5f);

    // Normals
    SCVector3f nFront(0.0f, 0.0f, -1.0f);
    SCVector3f nBack(0.0f, 0.0f, 1.0f);
    SCVector3f nLeft(-1.0f, 0.0f, 0.0f);
    SCVector3f nRight(1.0f, 0.0f, 0.0f);
    SCVector3f nTop(0.0f, 1.0f, 0.0f);
    SCVector3f nBottom(0.0f, -1.0f, 0.0f);

    // Texture coordinates
    SCVector2f uv00(0.0f, 0.0f);
    SCVector2f uv01(1.0f, 0.0f);
    SCVector2f uv10(1.0f, 1.0f);
    SCVector2f uv11(0.0f, 1.0f);

    // Front face
    vertices.push_back(SCVertex(p0, nFront, uv00));
    vertices.push_back(SCVertex(p1, nFront, uv01));
    vertices.push_back(SCVertex(p2, nFront, uv10));
    vertices.push_back(SCVertex(p3, nFront, uv11));

    // Back face
    vertices.push_back(SCVertex(p5, nBack, uv00));
    vertices.push_back(SCVertex(p4, nBack, uv01));
    vertices.push_back(SCVertex(p7, nBack, uv10));
    vertices.push_back(SCVertex(p6, nBack, uv11));

    // Left face
    vertices.push_back(SCVertex(p4, nLeft, uv00));
    vertices.push_back(SCVertex(p0, nLeft, uv01));
    vertices.push_back(SCVertex(p3, nLeft, uv10));
    vertices.push_back(SCVertex(p7, nLeft, uv11));

    // Right face
    vertices.push_back(SCVertex(p1, nRight, uv00));
    vertices.push_back(SCVertex(p5, nRight, uv01));
    vertices.push_back(SCVertex(p6, nRight, uv10));
    vertices.push_back(SCVertex(p2, nRight, uv11));

    // Top face
    vertices.push_back(SCVertex(p3, nTop, uv00));
    vertices.push_back(SCVertex(p2, nTop, uv01));
    vertices.push_back(SCVertex(p6, nTop, uv10));
    vertices.push_back(SCVertex(p7, nTop, uv11));

    // Bottom face
    vertices.push_back(SCVertex(p4, nBottom, uv00));
    vertices.push_back(SCVertex(p5, nBottom, uv01));
    vertices.push_back(SCVertex(p1, nBottom, uv10));
    vertices.push_back(SCVertex(p0, nBottom, uv11));

    return vertices;
}

std::vector<unsigned int> GetCubeIndices()
{
    std::vector<unsigned int> indices;

    // Define the indices for each face (two triangles per face)
    // Front face
    indices.push_back(0); // p0
    indices.push_back(1); // p1
    indices.push_back(2); // p2
    indices.push_back(2); // p2
    indices.push_back(3); // p3
    indices.push_back(0); // p0

    // Back face
    indices.push_back(6); // p6
    indices.push_back(7); // p7
    indices.push_back(4); // p4
    indices.push_back(4); // p4
    indices.push_back(5); // p5
    indices.push_back(6); // p6

    // Left face
    indices.push_back(8); // p4
    indices.push_back(9); // p0
    indices.push_back(10); // p3
    indices.push_back(10); // p3
    indices.push_back(11); // p7
    indices.push_back(8); // p4

    // Right face
    indices.push_back(12); // p1
    indices.push_back(13); // p5
    indices.push_back(14); // p6
    indices.push_back(14); // p6
    indices.push_back(15); // p2
    indices.push_back(12); // p1

    // Top face
    indices.push_back(16); // p3
    indices.push_back(17); // p2
    indices.push_back(18); // p6
    indices.push_back(18); // p6
    indices.push_back(19); // p7
    indices.push_back(16); // p3

    // Bottom face
    indices.push_back(20); // p4
    indices.push_back(21); // p5
    indices.push_back(22); // p1
    indices.push_back(22); // p1
    indices.push_back(23); // p0
    indices.push_back(20); // p4

    return indices;
}

void Application::Run()
{
    Init();

    EventSys = std::make_unique<EventSystem>();
    AssetMan = std::make_unique<AssetManager>();
    EventSys->Launch();

    std::vector<SCVertex> vertices = {
        // Front face
        { SCVector3f(-0.5f, -0.5f,  0.5f), SCVector3f(0.0f, 0.0f, 1.0f), SCVector2f(0.0f, 0.0f) }, // 0
        { SCVector3f(0.5f, -0.5f,  0.5f), SCVector3f(0.0f, 0.0f, 1.0f), SCVector2f(1.0f, 0.0f) }, // 1
        { SCVector3f(0.5f,  0.5f,  0.5f), SCVector3f(0.0f, 0.0f, 1.0f), SCVector2f(1.0f, 1.0f) }, // 2
        { SCVector3f(-0.5f,  0.5f,  0.5f), SCVector3f(0.0f, 0.0f, 1.0f), SCVector2f(0.0f, 1.0f) }, // 3

        // Back face
        { SCVector3f(-0.5f, -0.5f, -0.5f), SCVector3f(0.0f, 0.0f, -1.0f), SCVector2f(0.0f, 0.0f) }, // 4
        { SCVector3f(0.5f, -0.5f, -0.5f), SCVector3f(0.0f, 0.0f, -1.0f), SCVector2f(1.0f, 0.0f) }, // 5
        { SCVector3f(0.5f,  0.5f, -0.5f), SCVector3f(0.0f, 0.0f, -1.0f), SCVector2f(1.0f, 1.0f) }, // 6
        { SCVector3f(-0.5f,  0.5f, -0.5f), SCVector3f(0.0f, 0.0f, -1.0f), SCVector2f(0.0f, 1.0f) }, // 7

        // Top face
        { SCVector3f(-0.5f,  0.5f, -0.5f), SCVector3f(0.0f, 1.0f, 0.0f), SCVector2f(0.0f, 0.0f) }, // 8
        { SCVector3f(0.5f,  0.5f, -0.5f), SCVector3f(0.0f, 1.0f, 0.0f), SCVector2f(1.0f, 0.0f) }, // 9
        { SCVector3f(0.5f,  0.5f,  0.5f), SCVector3f(0.0f, 1.0f, 0.0f), SCVector2f(1.0f, 1.0f) }, // 10
        { SCVector3f(-0.5f,  0.5f,  0.5f), SCVector3f(0.0f, 1.0f, 0.0f), SCVector2f(0.0f, 1.0f) }, // 11

        // Bottom face
        { SCVector3f(-0.5f, -0.5f, -0.5f), SCVector3f(0.0f, -1.0f, 0.0f), SCVector2f(0.0f, 0.0f) }, // 12
        { SCVector3f(0.5f, -0.5f, -0.5f), SCVector3f(0.0f, -1.0f, 0.0f), SCVector2f(1.0f, 0.0f) }, // 13
        { SCVector3f(0.5f, -0.5f,  0.5f), SCVector3f(0.0f, -1.0f, 0.0f), SCVector2f(1.0f, 1.0f) }, // 14
        { SCVector3f(-0.5f, -0.5f,  0.5f), SCVector3f(0.0f, -1.0f, 0.0f), SCVector2f(0.0f, 1.0f) }, // 15

        // Left face
        { SCVector3f(-0.5f, -0.5f, -0.5f), SCVector3f(-1.0f, 0.0f, 0.0f), SCVector2f(0.0f, 0.0f) }, // 16
        { SCVector3f(-0.5f, -0.5f,  0.5f), SCVector3f(-1.0f, 0.0f, 0.0f), SCVector2f(1.0f, 0.0f) }, // 17
        { SCVector3f(-0.5f,  0.5f,  0.5f), SCVector3f(-1.0f, 0.0f, 0.0f), SCVector2f(1.0f, 1.0f) }, // 18
        { SCVector3f(-0.5f,  0.5f, -0.5f), SCVector3f(-1.0f, 0.0f, 0.0f), SCVector2f(0.0f, 1.0f) }, // 19

        // Right face
        { SCVector3f(0.5f, -0.5f, -0.5f), SCVector3f(1.0f, 0.0f, 0.0f), SCVector2f(0.0f, 0.0f) }, // 20
        { SCVector3f(0.5f, -0.5f,  0.5f), SCVector3f(1.0f, 0.0f, 0.0f), SCVector2f(1.0f, 0.0f) }, // 21
        { SCVector3f(0.5f,  0.5f,  0.5f), SCVector3f(1.0f, 0.0f, 0.0f), SCVector2f(1.0f, 1.0f) }, // 22
        { SCVector3f(0.5f,  0.5f, -0.5f), SCVector3f(1.0f, 0.0f, 0.0f), SCVector2f(0.0f, 1.0f) }  // 23
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2,  2, 3, 0,

        // Back face
        4, 5, 6,  6, 7, 4,

        // Top face
        8, 9, 10, 10, 11, 8,

        // Bottom face
        12, 13, 14, 14, 15, 12,

        // Left face
        16, 17, 18, 18, 19, 16,

        // Right face
        20, 21, 22, 22, 23, 20
    };

    // Cast the raw pointer
    DX11Renderer* dx11Renderer = dynamic_cast<DX11Renderer*>(m_Renderer.get());

    if (!dx11Renderer) {
        std::cerr << "Failed to cast Renderer to DX11Renderer" << std::endl;
        return;
    }

    DXCamera3D camera(
        XMFLOAT3(0.0f, 1.0f, -3.0f), // Position
        XMFLOAT3(0.0f, 0.0f, -1.0f),  // Target
        XMFLOAT3(0.0f, 1.0f, 0.0f)   // Up
    );

    constexpr float fovY = XMConvertToRadians(45.0f);

    camera.FOV = fovY;

    auto Proj = camera.ComputeProjectionMatrix(800/600);

    //Proj = XMMatrixTranspose(Proj);

    auto View = camera.ComputeViewMatrix();

    //View = XMMatrixTranspose(View);
    
    MatrixBuffer buffer;

    auto world = XMMatrixIdentity();

    buffer.World = world;
    buffer.View = View;
    buffer.MVP = camera.ComputeMVPMatrix(800 / 600, world);

    auto cbuf = dx11Renderer->CreateConstantBuffer<MatrixBuffer>(buffer);
    auto shAsset = AssetMan->LoadAsset(AssetType::DX11_SHADER, "resources/shaders/hlsl/sm5/basic.hlsl");
    auto shader = std::dynamic_pointer_cast<ShaderAsset>(shAsset);
    auto material = std::dynamic_pointer_cast<DX11Material>(shader->PackMat());
    material->ConstantBuffer = cbuf;
    auto vert = GetCubeVertices();
    auto indc = GetCubeIndices();
    auto mesh = dx11Renderer->CreateMesh(vertices, indices, material);

    std::cout << mesh->IndexBuffer.Get() << std::endl;

    bool running = true;
    float frameTime = 0;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;
    while (running)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency());

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                XMVECTOR forwardVector = XMVector3Normalize(XMLoadFloat3(&camera.Target) - XMLoadFloat3(&camera.Position));
                XMVECTOR rightVector = XMVector3Normalize(XMVector3Cross(forwardVector, XMLoadFloat3(&camera.Up)));

                XMFLOAT3 forward;
                XMFLOAT3 back;
                XMFLOAT3 left;
                XMFLOAT3 right;
                XMStoreFloat3(&forward, forwardVector * deltaTime * 2.f);
                XMStoreFloat3(&right, rightVector * deltaTime * 2.f);
                XMStoreFloat3(&back, -forwardVector * deltaTime * 2.f);
                XMStoreFloat3(&left, -rightVector * deltaTime * 2.f);

                switch (event.key.key)
                {
                    case SDLK_W:
                        
                        camera.Position = AddFloat3(camera.Position, forward);
                        std::cout << "X: " << camera.Position.x << "Y: " << camera.Position.y << "Z: " << camera.Position.z << std::endl;
                        break;
                    case SDLK_S:
                        std::cout << "hello?" << std::endl;
                        camera.Position = AddFloat3(camera.Position, back);
                        std::cout << "X: " << camera.Position.x << "Y: " << camera.Position.y << "Z: " << camera.Position.z << std::endl;
                        break;
                    case SDLK_A:
                        std::cout << "hello?" << std::endl;
                        camera.Position = AddFloat3(camera.Position, right);
                        std::cout << "X: " << camera.Position.x << "Y: " << camera.Position.y << "Z: " << camera.Position.z << std::endl;
                        break;
                    case SDLK_D:
                        std::cout << "hello?" << std::endl;
                        camera.Position = AddFloat3(camera.Position, left);
                        std::cout << "X: " << camera.Position.x << "Y: " << camera.Position.y << "Z: " << camera.Position.z << std::endl;
                        break;
                    case SDLK_Q:
                        camera.Position = AddFloat3(camera.Position, XMFLOAT3(0,1,0));
                        break;
                    case SDLK_E:
                        camera.Position = AddFloat3(camera.Position, XMFLOAT3(0, -1, 0));
                        break;
                }

                XMVECTOR newTarget = XMLoadFloat3(&camera.Position) + forwardVector;
                XMStoreFloat3(&camera.Target, newTarget);
            }
        }

        auto View = camera.ComputeViewMatrix();
        world = XMMatrixTranslation(1, 0, 1) * XMMatrixScaling(1,1,1);
        buffer.World = world;
        buffer.View = View;
        buffer.MVP = camera.ComputeMVPMatrix(static_cast<float>(AppWindow->GetSize().X) / static_cast<float>(AppWindow->GetSize().Y), world);
        dx11Renderer->UpdateConstantBuffer(cbuf, buffer);

        dx11Renderer->BeginFrame(AppWindow->GetSize());
        dx11Renderer->DrawMesh(mesh);
        dx11Renderer->EndFrame();

        frameTime += 4*deltaTime;
    }

    Close();
}