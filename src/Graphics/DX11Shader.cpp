#include <Graphics/DX11/DX11Shader.h>

bool DX11Shader::Initialize(ID3D11Device* device, const wchar_t* vsPath, const wchar_t* psPath)
{
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    if (!CompileShaderFromFile(vsPath, "VS_Main", "vs_5_0", &vsBlob) ||
        !CompileShaderFromFile(psPath, "PS_Main", "ps_5_0", &psBlob))
    {
        return false;
    }

    HRESULT hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
    if (FAILED(hr)) { std::cerr << "Failed to create vertex shader." << std::endl; return false; }

    hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
    if (FAILED(hr)) { std::cerr << "Failed to create pixel shader." << std::endl; return false; }

    const D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
    if (FAILED(hr)) { std::cerr << "Failed to create input layout." << std::endl; return false; }

    vsBlob->Release();
    psBlob->Release();

    return true;
}

bool DX11Shader::Initialize(ID3D11Device* device, const wchar_t* shPath)
{
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    if (!CompileShaderFromFile(shPath, "VS_Main", "vs_5_0", &vsBlob) ||
        !CompileShaderFromFile(shPath, "PS_Main", "ps_5_0", &psBlob))
    {
        return false;
    }

    HRESULT hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
    if (FAILED(hr)) { std::cerr << "Failed to create vertex shader." << std::endl; return false; }

    hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
    if (FAILED(hr)) { std::cerr << "Failed to create pixel shader." << std::endl; return false; }

    const D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
    if (FAILED(hr)) { std::cerr << "Failed to create input layout." << std::endl; return false; }

    vsBlob->Release();
    psBlob->Release();

    return true;
}

void DX11Shader::SetShaders(ID3D11DeviceContext* context)
{
    context->IASetInputLayout(inputLayout.Get());
    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);
}

bool DX11Shader::CompileShaderFromFile(const wchar_t* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** blob)
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    flags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
        flags, 0, blob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            std::cerr << "Shader compilation error: " << static_cast<char*>(errorBlob->GetBufferPointer()) << std::endl;
            errorBlob->Release();
        }
        return false;
    }

    if (errorBlob) errorBlob->Release();
    return true;
}