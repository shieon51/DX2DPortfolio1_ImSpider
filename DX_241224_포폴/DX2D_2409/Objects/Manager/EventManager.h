#pragma once

class EventManager : public Singleton<EventManager>
{
private:
    friend class Singleton;

    EventManager() = default;
    ~EventManager() = default;

public:
    void AddEvent(string key, Event event);
    void AddIntParamEvent(string key, IntParamEvent event);    

    void ExcuteEvent(string key);
    void ExcuteIntParamEvent(string key, int param);

private:
    unordered_map<string, vector<IntParamEvent>> intParamEvents;
    unordered_map<string, vector<Event>> events;
};