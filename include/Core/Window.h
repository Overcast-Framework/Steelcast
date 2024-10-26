#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <Math/Vector.h>

class Window
{
public:
    std::string Title;
    SCVector2i Size;
    std::shared_ptr<SDL_Window> SDLWindow;

    Window(const std::string& title, const SCVector2i& size)
        : Title(title), Size(size)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDLWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
            Title.c_str(),                      
            Size.X,                            
            Size.Y,                            
            SDL_WINDOW_HIGH_PIXEL_DENSITY      
        ), SDL_DestroyWindow);                  

        if (!SDLWindow) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }
    }

    ~Window() {
        SDL_Quit();
    }

    void SetTitle(const std::string& title) {
        Title = title;
        SDL_SetWindowTitle(SDLWindow.get(), Title.c_str());
    }

    void SetSize(const SCVector2i& size) {
        Size = size;
        SDL_SetWindowSize(SDLWindow.get(), Size.X, Size.Y);
    }

    SCVector2i GetSize() const {
        int width, height;
        SDL_GetWindowSize(SDLWindow.get(), &width, &height);
        return SCVector2i(width, height);
    }
};