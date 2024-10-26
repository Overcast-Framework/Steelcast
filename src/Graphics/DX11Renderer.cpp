#include <Graphics/DX11/DX11Renderer.h>
#include <SDL3/SDL.h>

bool DX11Renderer::Initialize(Window* window)
{
	SDL_PropertiesID id = SDL_GetWindowProperties(window->SDLWindow.get());
    // Check if we got a valid property ID
    if (id == 0) {
        std::cerr << "Failed to get window properties: " << SDL_GetError() << std::endl;
        return false;
    }

    HWND hwnd = (HWND)SDL_GetPointerProperty(id, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
    this->CreateDeviceAndSwapChain(hwnd, window->Size);
    this->CreateRenderTarget();
    this->CreateDepthStencil(window->Size);

    return true;
}

bool DX11Renderer::CreateDeviceAndSwapChain(HWND hwnd, SCVector2i size)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.Width = size.X;
    swapChainDesc.BufferDesc.Height = size.Y;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    DXCALL(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
        &d3dDevice, nullptr, &d3dContext
    ));

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = true;
    ID3D11RasterizerState* rasterizerState;
    d3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    d3dContext->RSSetState(rasterizerState);

    return true;
}

void DX11Renderer::CreateRenderTarget()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    DXCALL(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
    DXCALL(d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));
}

bool DX11Renderer::CreateDepthStencil(SCVector2i size)
{
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = size.X;
    depthStencilDesc.Height = size.Y;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;

    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    DXCALL(d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer));
    DXCALL(d3dDevice->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &depthStencilView));

    d3dContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

    return true;
}

bool DX11Renderer::CreateVertexBuffer(const void* vertexData, UINT vertexSize, UINT vertexCount, ComPtr<ID3D11Buffer>& buffer)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = vertexSize * vertexCount;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertexData;

    DXCALL(d3dDevice->CreateBuffer(&bufferDesc, &initData, &buffer));

    return true;
}

bool DX11Renderer::CreateIndexBuffer(const void* indexData, UINT indexSize, UINT indexCount, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = indexSize * indexCount;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indexData;

    HRESULT hr = d3dDevice->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create index buffer: " << std::hex << hr << std::endl;
        return false;
    }

    return true;
}

std::shared_ptr<DX11Mesh> DX11Renderer::CreateMesh(std::vector<SCVertex>& vertices, std::vector<unsigned int>& indices, std::shared_ptr<SCMaterial> material)
{
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;

    CreateVertexBuffer(vertices.data(), sizeof(SCVertex), vertices.size(), vertexBuffer);
    CreateIndexBuffer(indices.data(), sizeof(unsigned int), indices.size(), indexBuffer);

    std::shared_ptr<DX11Material> derivedMaterial = std::dynamic_pointer_cast<DX11Material>(material);

    if (!derivedMaterial)
    {
        std::cerr << "Invalid material for DX11Renderer" << std::endl;
        return nullptr;
    }

    // Create DX11Mesh with ComPtr objects
    auto mesh = std::make_shared<DX11Mesh>(vertexBuffer, indexBuffer, derivedMaterial);
    mesh->Indices = indices;
    mesh->Vertices = vertices;

    std::cout << mesh->IndexBuffer.Get() << std::endl;

    return mesh;
}

std::shared_ptr<DX11Shader> DX11Renderer::CreateShader(const wchar_t* vsPath, const wchar_t* psPath)
{
    std::shared_ptr<DX11Shader> shader = std::make_shared<DX11Shader>();
    shader->Initialize(this->d3dDevice.Get(), vsPath, psPath);
    return shader;
}

std::shared_ptr<DX11Shader> DX11Renderer::CreateShader(const wchar_t* shPath)
{
    std::shared_ptr<DX11Shader> shader = std::make_shared<DX11Shader>();
    shader->Initialize(this->d3dDevice.Get(), shPath);
    return shader;
}

void DX11Renderer::UploadMesh(std::shared_ptr<DX11Mesh> mesh)
{
    CreateVertexBuffer(mesh->Vertices.data(), sizeof(SCVertex), mesh->Vertices.size(), mesh->VertexBuffer);
    CreateIndexBuffer(mesh->Indices.data(), sizeof(unsigned int), mesh->Indices.size(), mesh->IndexBuffer);

    std::cout << "Uploaded mesh" << std::endl;
}

void DX11Renderer::DrawMesh(const std::weak_ptr<Mesh>& mesh)
{
    auto dx11Mesh = std::dynamic_pointer_cast<DX11Mesh>(mesh.lock());
    if (!dx11Mesh) {
        std::cerr << "Invalid mesh for DX11Renderer" << std::endl;
        return;
    }

    //std::cout << dx11Mesh.get() << std::endl;

    BindBuffer(DX11BufferType::VERTEX, dx11Mesh->VertexBuffer);
    BindBuffer(DX11BufferType::INDEX, dx11Mesh->IndexBuffer);

    if (dx11Mesh->Material->ConstantBuffer) {
        this->d3dContext->VSSetConstantBuffers(0, 1, dx11Mesh->Material->ConstantBuffer->GetBuffer().GetAddressOf());
    }

    this->d3dContext->IASetInputLayout(dx11Mesh->Material->Shader->inputLayout.Get());

    this->d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    this->d3dContext->VSSetShader(dx11Mesh->Material->Shader->vertexShader.Get(), nullptr, 0);
    this->d3dContext->PSSetShader(dx11Mesh->Material->Shader->pixelShader.Get(), nullptr, 0);

    this->d3dContext->DrawIndexed(dx11Mesh->Indices.size(), 0, 0);
}

std::shared_ptr<SCMaterial> DX11Renderer::CreateMaterial(std::shared_ptr<SCMaterialSpec> spec)
{
    auto dx11Spec = std::dynamic_pointer_cast<DX11MaterialSpec>(spec);
    if (!dx11Spec) {
        std::cerr << "Invalid material spec for DX11Renderer" << std::endl;
        return nullptr;
    }

    auto material = std::make_shared<DX11Material>();
    material->Shader = dx11Spec->shader;
    material->ConstantBuffer = std::move(dx11Spec->constBuffer);
    
    return material;
}

void DX11Renderer::BindBuffer(DX11BufferType bufferType, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer)
{
    if (!buffer) {
        std::cerr << "Index buffer is not initialized!" << std::endl;
        return;
    }

    switch (bufferType)
    {
    case DX11BufferType::VERTEX:
    {
        UINT stride = sizeof(SCVertex);
        UINT offset = 0;
        d3dContext->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
    }
    break;

    case DX11BufferType::INDEX:
        d3dContext->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        break;

    case DX11BufferType::CONSTANT:
        d3dContext->VSSetConstantBuffers(0, 1, buffer.GetAddressOf());
        break;

    default:
        std::cerr << "[ENGINE][RND/DX11]: Unknown buffer type!" << std::endl;
        break;
    }
}



void DX11Renderer::BeginFrame(SCVector2i size)
{
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    d3dContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);

    d3dContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    d3dContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(size.X);
    viewport.Height = static_cast<float>(size.Y);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    d3dContext->RSSetViewports(1, &viewport);
}

void DX11Renderer::EndFrame()
{
    swapChain->Present(1, 0);
}

void DX11Renderer::Resize(int width, int height)
{

}

void DX11Renderer::Render()
{

}

DX11Renderer::~DX11Renderer()
{

}