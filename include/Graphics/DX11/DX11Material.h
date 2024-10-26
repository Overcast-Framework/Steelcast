#pragma once
#include <iostream>
#include <Graphics/DX11/DX11ConstantBuffer.h>
#include <Graphics/DX11/DX11Shader.h>
#include <Graphics/Renderer.h>
#include <Graphics/Mesh.h>

class DX11Material : public SCMaterial {
public:
    std::shared_ptr<DX11ConstantBufferBase> ConstantBuffer;
    std::shared_ptr<DX11Shader> Shader;

    DX11Material() : ConstantBuffer(nullptr), Shader(nullptr) {}
};

class DX11MaterialSpec : public SCMaterialSpec
{
public:
    std::shared_ptr<DX11Shader> shader;
    std::shared_ptr<DX11ConstantBufferBase> constBuffer;

    DX11MaterialSpec() : constBuffer(nullptr), shader(nullptr) {}
    DX11MaterialSpec(std::shared_ptr<DX11Shader> shad, std::shared_ptr<DX11ConstantBufferBase> constantBuffer) : shader(shad), constBuffer(constantBuffer) {}
};