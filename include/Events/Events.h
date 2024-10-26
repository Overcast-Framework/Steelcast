#pragma once
#include <iostream>

enum class EventType
{
	EVENT_NONE = 0,
	ERROR_EVENT,
	APP_PROC_START,
	APP_PROC_STOP,
	APP_RENDER_LOOP
};