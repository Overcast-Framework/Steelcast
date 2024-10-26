#pragma once
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class DX11Shader
{
public:
    bool Initialize(ID3D11Device* device, const wchar_t* vsPath, const wchar_t* psPath);
    bool Initialize(ID3D11Device* device, const wchar_t* shPath);
    void SetShaders(ID3D11DeviceContext* context);

    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> inputLayout;
private:

    bool CompileShaderFromFile(const wchar_t* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** blob);
};