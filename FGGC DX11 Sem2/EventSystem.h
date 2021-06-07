#pragma once
#include <list>
#include<map>

#include"Observer.h"
using namespace std;
//struct to hold data related to an event
struct EventData
{
	const void* entertity;
	Events event;
};
/// <summary>
/// singleton to nofy all observers
/// </summary>
class EventSystem
{
public:
	//observer vector conrolls
	void AddObserver( Observer* observer, const  Events events);
	void RemoveObserver(  Observer* observer);

	//event list controlls
	void AddEvent(const Events NewEvent,  const void* data);
	void AddEvent(const Events NewEvent);
	//set lists to 0
	void Reset();
	//post all the event to relvent observers
	void Notify();
	//get singleton
	static EventSystem* Instance();

protected:

	EventSystem(){}
	~EventSystem() { CleanUp(); }

	
	void CleanUp();



private:
	

	multimap<Events, Observer*> Observers;
	list<EventData> EventList;


};

