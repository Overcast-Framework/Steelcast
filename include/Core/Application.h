#pragma once
#include <memory>
#include <Core/Window.h>
#include <Events/EventSystem.h>
#include <Graphics/Renderer.h>
#include <Assets/AssetManager.h>
#include <Graphics/DX11/DX11Renderer.h>

enum class RendererAPI
{
	None,
	DirectX11
};

struct AppSettings
{
	std::string Title;
	SCVector2i Size;
	RendererAPI RenderAPI;

	AppSettings(std::string title, SCVector2i size, RendererAPI api) : Title(title), Size(size), RenderAPI(api) {}
};

class Application
{
private:
	static Application* instance;
	std::unique_ptr<Window> AppWindow;
	std::unique_ptr<EventSystem> EventSys;
	std::unique_ptr<AssetManager> AssetMan;
	std::unique_ptr<Renderer> m_Renderer;
public:
	RendererAPI RenderAPI;

	void Run();
	void Init();
	void Close();

	static Application& Get() { return *instance; }
	Window& GetWindow() { return *AppWindow; }
	EventSystem& GetEventSys() { return *EventSys; }
	Renderer& GetRenderer() { return *m_Renderer; }
	AssetManager& GetAssetManager() { return *AssetMan; }
	DX11Renderer& GetDX11Renderer();

	virtual ~Application();
	Application(const AppSettings& settings);
};
