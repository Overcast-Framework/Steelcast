#pragma once
#include <functional>
#include <map>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <Events/Events.h>

class EventArgs {
public:
	virtual ~EventArgs() = default;
};

class Event {
public:
	EventType Type;
	std::unique_ptr<EventArgs> Args;

	virtual ~Event() = default;
	Event(EventType type, std::unique_ptr<EventArgs> args) : Type(type), Args(std::move(args)) {}

	Event(const Event&) = delete;
	Event& operator=(const Event&) = delete;

	Event(Event&&) noexcept = default;
	Event& operator=(Event&&) noexcept = default;
};

class EventQueue
{
public:
	void PushEvent(std::unique_ptr<Event> event);
	std::unique_ptr<Event> PopEvent();
private:
	std::queue<std::unique_ptr<Event>> queue_;
	std::mutex mutex_;
	std::condition_variable condition_;
};

using ECallback = std::function<void(const EventArgs&)>;
class EventSystem
{
public:
	void Register(EventType type, ECallback cb);
	void FireEvent(std::unique_ptr<Event> event);
	void Launch();
	void Halt();
private:
	static std::map<EventType, std::vector<ECallback>> EventCallbacks;
	static void EventHandler(EventQueue& queue);
	std::thread HandlerThread;
	EventQueue Queue;
	std::mutex mutex_;
	bool running = true;
};

void EventHandler(EventQueue& queue);