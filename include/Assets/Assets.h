#pragma once
#include <iostream>
#include <Graphics/DX11/DX11Shader.h>
#include <Graphics/DX11/DX11Material.h>

class Asset { 
public:
	virtual ~Asset() = default;
	virtual void Load(const std::string& path) = 0;
};

class ModelAsset : public Asset
{
public:
	ModelAsset() = default;
	void Load(const std::string& path) override;
};

class ShaderAsset : public Asset
{
public:
	ShaderAsset() = default;
	std::shared_ptr<DX11Shader> ShaderObj;
	void Load(const std::string& path) override;
	std::shared_ptr<SCMaterial> PackMat();
};