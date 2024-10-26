#pragma once
#include <iostream>
#include <map>
#include <Assets/Assets.h>

enum class AssetType
{
	DX11_SHADER,
	MODEL,
	TEXTURE,
	SCRIPT
};

class AssetManager
{
public:
	AssetManager() : AssetDict() {}
	~AssetManager() = default;

	std::shared_ptr<Asset> LoadAsset(const AssetType& type, std::string path);
	std::shared_ptr<Asset> GetAsset(std::string path);
	std::vector<std::shared_ptr<Asset>> GetAllAssets();
	std::vector<std::shared_ptr<Asset>> GetAssetsOfType(const AssetType& type);
	bool IsAssetLoaded(std::string path);
	AssetType GetAssetType(std::string path);
private:
	std::map<std::string, std::shared_ptr<Asset>> AssetDict;
};