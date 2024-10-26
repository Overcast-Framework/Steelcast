#pragma once
#include <Core/Window.h>
#include <Graphics/Mesh.h>
#include <Math/Vector.h>
#include <memory>

class SCMaterialSpec
{
public:
    virtual ~SCMaterialSpec() = default;
};

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual bool Initialize(Window* hwnd) = 0; 
    virtual void Render() = 0;              
    virtual void DrawMesh(const std::weak_ptr<Mesh>&) = 0;
    virtual void BeginFrame(SCVector2i size) = 0;
    virtual void EndFrame() = 0;
    virtual void Resize(int width, int height) = 0; 
    virtual std::shared_ptr<SCMaterial> CreateMaterial(std::shared_ptr<SCMaterialSpec> spec) = 0;

protected:
    Renderer() = default;
};