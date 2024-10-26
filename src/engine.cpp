#include <Steelcast.h>

int main()
{
	const AppSettings settings("Steelcast Window", { 800,600 }, RendererAPI::DirectX11);
	Application app(settings);
	app.Run();
}