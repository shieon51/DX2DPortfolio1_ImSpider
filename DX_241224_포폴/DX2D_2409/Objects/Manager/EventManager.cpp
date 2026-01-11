#include "Framework.h"

void EventManager::AddEvent(string key, Event event)
{
    events[key].push_back(event);
}

void EventManager::AddIntParamEvent(string key, IntParamEvent event)
{
    intParamEvents[key].push_back(event);
}

void EventManager::ExcuteEvent(string key)
{
    for (Event event : events[key])
    {
        event();
    }
}

void EventManager::ExcuteIntParamEvent(string key, int param)
{
    for (IntParamEvent event : intParamEvents[key])
    {
        event(param);
    }
}
