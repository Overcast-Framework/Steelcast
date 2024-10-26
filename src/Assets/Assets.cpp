#include <Assets/Assets.h>
#include <Core/Application.h>
#include <Graphics/DX11/DX11Renderer.h>
#include <iostream>

void ShaderAsset::Load(const std::string& path)
{
	auto rend = Application::Get().GetDX11Renderer();
	auto p = (wchar_t*)path.c_str();
	std::cout << (char*)p << std::endl;
	this->ShaderObj = rend.CreateShader(p);
}

std::shared_ptr<SCMaterial> ShaderAsset::PackMat()
{
	auto rend = Application::Get().GetDX11Renderer();
	auto spec = std::make_shared<DX11MaterialSpec>(ShaderObj, rend.CreateConstantBuffer(nullptr));
	auto mat = rend.CreateMaterial(spec);
	return mat;
}