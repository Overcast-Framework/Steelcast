#pragma once
#include <Graphics/Renderer.h>
#include <Graphics/Vertex.h>
#include <Graphics/Mesh.h>
#include <Graphics/DX11/DX11Shader.h>
#include <Graphics/DX11/DX11Material.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include <memory>
#include <wrl.h>
#include <Graphics/DX11/DX11ConstantBuffer.h>

#define DXCALL(hr) if (FAILED(hr)) { std::cerr << "Error at " << __FILE__ << ":" << __LINE__ << " - " << std::hex << hr << std::endl; }

using namespace Microsoft::WRL;

enum class DX11BufferType
{
    VERTEX,
    INDEX,
    CONSTANT
};

class DX11Mesh : public Mesh
{
public:
    ComPtr<ID3D11Buffer> VertexBuffer;
    ComPtr<ID3D11Buffer> IndexBuffer;
    std::shared_ptr<DX11Material> Material;

    DX11Mesh(ComPtr<ID3D11Buffer> vb, ComPtr<ID3D11Buffer> ib, std::shared_ptr<DX11Material> mat)
        : VertexBuffer(vb), IndexBuffer(ib), Material(mat) {}
};

class DX11Renderer : public Renderer
{
public:
    DX11Renderer() = default;
    ~DX11Renderer();

    // Renderer base class functions
    bool Initialize(Window* hwnd) override;
    void Render() override;
    void DrawMesh(const std::weak_ptr<Mesh>&) override;
    void Resize(int width, int height) override;
    void BeginFrame(SCVector2i size) override;
    void EndFrame() override;
    std::shared_ptr<SCMaterial> CreateMaterial(std::shared_ptr<SCMaterialSpec> spec) override;

    // DX11Renderer-Specific
    bool CreateVertexBuffer(const void* vertexData, UINT vertexSize, UINT vertexCount, ComPtr<ID3D11Buffer>& buffer);
    bool CreateIndexBuffer(const void* indexData, UINT indexSize, UINT indexCount, ComPtr<ID3D11Buffer>& buffer);

    template<typename T>
    std::shared_ptr<DX11ConstantBuffer<T>> CreateConstantBuffer(const T& data)
    {
        std::shared_ptr<DX11ConstantBuffer<T>> cBuffer = std::make_shared<DX11ConstantBuffer<T>>(d3dDevice.Get());
        cBuffer->UpdateBuffer(d3dContext.Get(), data);
        return cBuffer;
    }

    template<typename T>
    void UpdateConstantBuffer(std::shared_ptr<DX11ConstantBuffer<T>> cb, const T& data)
    {
        if (cb)
        {
            cb->UpdateBuffer(d3dContext.Get(), data);
        }
    }

    void BindBuffer(DX11BufferType bufferType, ComPtr<ID3D11Buffer>& buffer);

    void UploadMesh(std::shared_ptr<DX11Mesh> mesh);
    std::shared_ptr<DX11Mesh> CreateMesh(std::vector<SCVertex>& vertices, std::vector<unsigned int>& indices, std::shared_ptr<SCMaterial> material);
    std::shared_ptr<DX11Shader> CreateShader(const wchar_t* vsPath, const wchar_t* psPath);
    std::shared_ptr<DX11Shader> CreateShader(const wchar_t* shPath);
    
private:
    bool CreateDeviceAndSwapChain(HWND hwnd, SCVector2i size);
    void CreateRenderTarget();
    bool CreateDepthStencil(SCVector2i size);

    ComPtr<ID3D11Device> d3dDevice;
    ComPtr<ID3D11DeviceContext> d3dContext;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    ComPtr<ID3D11DepthStencilView> depthStencilView;
};