#include <Assets/AssetManager.h>
#include <Core/Application.h>
#include <Events/Events.h>
#include <Events/EventArgs.h>

std::shared_ptr<Asset> AssetManager::LoadAsset(const AssetType& type, std::string path)
{
	if (this->IsAssetLoaded(path))
	{
		SC_ErrorEvent("Attempted to load already-loaded asset.");
		return nullptr;
	}

	std::shared_ptr<Asset> asset = nullptr;
	switch (type)
	{
		case AssetType::DX11_SHADER:
			asset = std::make_shared<ShaderAsset>();
			AssetDict.emplace(path, asset);
			asset->Load(path);
			break;
		default:
			SC_ErrorEvent("Could not load asset.");
			break;
	}

	return asset;
}

bool AssetManager::IsAssetLoaded(std::string path)
{
	// Debugging output
	std::cout << "Checking if asset is loaded: " << path << std::endl;
	std::cout << "Size of AssetDict: " << AssetDict.size() << std::endl;

	return AssetDict.find(path) != AssetDict.end();
}