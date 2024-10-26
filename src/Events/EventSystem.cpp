#include <Events/EventSystem.h>
#include <Events/EventArgs.h>

std::map<EventType, std::vector<ECallback>> EventSystem::EventCallbacks;

void EventQueue::PushEvent(std::unique_ptr<Event> event)
{
	std::lock_guard<std::mutex> lock(mutex_);
	queue_.push(std::move(event));
	condition_.notify_one();
}

std::unique_ptr<Event> EventQueue::PopEvent()
{
	std::unique_lock<std::mutex> lock(mutex_);
	condition_.wait(lock, [this] { return !queue_.empty(); });

	auto event = std::move(queue_.front());
	queue_.pop();
	return event;
}

void EventSystem::Register(EventType type, ECallback cb)
{
	std::lock_guard<std::mutex> lock(mutex_);
	EventCallbacks[type].push_back(std::move(cb));
}

void EventSystem::Launch()
{
	HandlerThread = std::thread(EventSystem::EventHandler, std::ref(Queue));
}

void EventSystem::FireEvent(std::unique_ptr<Event> event)
{
	Queue.PushEvent(std::move(event));
}

void EventSystem::Halt()
{
	running = false;
	Queue.PushEvent(nullptr); // to unblock the thread if waiting
	if (HandlerThread.joinable()) {
		HandlerThread.join();
	}
}

void EventSystem::EventHandler(EventQueue& queue)
{
	while (true)
	{
		auto event = queue.PopEvent();
		if (event == nullptr) { // handle shutdown signal
			break;
		}

		auto callbacksIt = EventCallbacks.find(event->Type);
		if (callbacksIt != EventCallbacks.end()) {
			for (const auto& callback : callbacksIt->second) {
				callback(*event->Args);
			}
		}
	}
}