#include "EventSystem.h"
#include"C++HelperFunctions.h"
void EventSystem::AddObserver(Observer* observer, Events events)
{
	Observers.emplace(events, observer);
}

void EventSystem::RemoveObserver(Observer* observer)
{
	ListHelpers::RemoveFromMutiMap<Events, Observer*>(&Observers, &observer);

}

void EventSystem::AddEvent(Events NewEvent, const void* data)
{

	EventData eventData;
	eventData.entertity = data;
	eventData.event = NewEvent;

	EventList.push_back(eventData);


}

void EventSystem::AddEvent(Events NewEvent)
{
	EventData eventData;
	eventData.entertity = nullptr;
	eventData.event = NewEvent;

	EventList.push_back(eventData);
}



void EventSystem::Reset() 
{	
	Observers.clear();
	EventList.clear();
}

void EventSystem::Notify() 
{
	while (!EventList.empty())
	{
		for (auto i = Observers.begin(); i != Observers.end(); i++) {

			if (i->first == EventList.front().event) {
				i->second->OnNotify(EventList.front().entertity, EventList.front().event);
			}
		}
		EventList.pop_front();
	}
}

EventSystem* EventSystem::Instance()
{
	static EventSystem Instance;
	return &Instance;
}

void EventSystem::CleanUp()
{
	
	Observers.clear();
	EventList.clear();
	
	/*delete _Instance;
	_Instance = nullptr;*/
}
