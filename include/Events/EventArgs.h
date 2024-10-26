#pragma once
#include <string>
#include <source_location>
#include <Core/Application.h>
#include <Events/EventSystem.h>
#define SC_ErrorEvent(x) Application::Get().GetEventSys().FireEvent(std::make_unique<Event>(EventType::ERROR_EVENT, std::make_unique<ErrorEventArgs>(x)));

class AppEventArgs : public EventArgs
{
public:
    Application* App;

    AppEventArgs(Application* app = nullptr) : App(app) {}
};

class ErrorEventArgs : public EventArgs
{
public:
    std::string Error;
    std::source_location Location;

    ErrorEventArgs(const std::string& error, std::source_location location = std::source_location::current())
        : Error(error), Location(location) {}
};