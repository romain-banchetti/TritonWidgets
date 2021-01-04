#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "twObject.h"

using namespace std;

twObject::twObject()
{
    intParameter = -1;
    pStringParameter = NULL;
    pIntParameter = NULL;
	children = new vector<Child>;
	m_parent = NULL;
	m_type = tw::None;
	m_childId = -1;
}

twObject::twObject(twObject *parent, tw::Object type)
{
    intParameter = -1;
    pStringParameter = NULL;
    pIntParameter = NULL;
	children = new vector<Child>;
	setType(type);
	setParent(parent);
}

twObject::~twObject()
{
    for (int i = int(children->size()) - 1; i >= 0; i--)
    {
        if (children->at(i).type != tw::Menu && children->at(i).type != tw::Timer)
            delete children->at(i).child;
    }
    delete children;
    if (getParent())
        getParent()->removeChild(this);
}

void twObject::connect(int event, twObject *object, bool (twObject::*function)())
{
    AEvent event2;
    event2.event = event;
    event2.object = object;
    event2.function = function;
    events.push_back(event2);
    m_sender = NULL;
}

void twObject::connect(int event, bool (twObject::*function)())
{
    AEvent event2;
    event2.event = event;
    if (getParent() == NULL)
		event2.object = this;
	else
		event2.object = getParent();
    event2.function = function;
    events.push_back(event2);
    m_sender = NULL;
}

void twObject::disconnect(twObject *object, int event)
{
    if (event == -1)
    {
		for (int i = events.size() - 1; i >= 0; i--)
		{
			if (events[i].object == object)
			{
				events.erase(events.begin() + i);
			}
		}
    }
    else
    {
        for (unsigned int i = 0; i < events.size(); i++)
        {
            if (events[i].object == object && events[i].event == event)
                events.erase(events.begin() + i);
        }
    }
}

twObject *twObject::getSender()
{
	return m_sender;
}

int twObject::addChild(tw::Object type, twObject *child)
{
    Child child2;
    child2.type = type;
    child2.child = child;
    children->push_back(child2);
    return children->size()-1;
}

void twObject::removeChild(twObject *child)
{
	for (unsigned int i=0;i<children->size();i++)
	{
		if (children->at(i).child == child)
            children->erase(children->begin()+i);
	}
}

int twObject::sendEvent(int event)
{
    for (unsigned int i = 0; i < events.size(); i++)
    {
        if (events[i].event == event)
        {
            events[i].object->m_sender = this;
            return (events[i].object->*events[i].function)();
        }
    }
    return 0;
}

void twObject::setParent(twObject *parent)
{
    if (getParent())
    {
        if (getParent() == parent)
            return;
        getParent()->removeChild(this);
    }
    m_parent = parent;
    if (getType() != tw::None)
        m_childId = getParent()->addChild(getType(), this);
}

twObject *twObject::getParent()
{
    return m_parent;
}

int twObject::triggerEvent(int event)
{
	int toReturn = 1;
    for (unsigned int i = 0; i < frozenEvents.size(); i++)
        if (frozenEvents[i] == event)
            return 0;
    int eventsSize = events.size();
    for (unsigned int i = 0; i < events.size(); i++)
    {
        if (events[i].event == event)
        {
            events[i].object->m_sender = this;
            if (toReturn == 0)
				(events[i].object->*events[i].function)();
			else
				toReturn = (events[i].object->*events[i].function)();
        }
        if ((int)events.size() != eventsSize)
            break;
    }
    return toReturn;
}

void twObject::setType(tw::Object type)
{
    m_type = type;
}

tw::Object twObject::getType()
{
    return m_type;
}

int twObject::getChildId()
{
    return m_childId;
}

void twObject::freezeEvent(int event)
{
    frozenEvents.push_back(event);
}

void twObject::unfreezeEvent(int event)
{
    for (unsigned int i = 0; i < frozenEvents.size(); i++)
    {
        if (frozenEvents[i] == event)
        {
            frozenEvents.erase(frozenEvents.begin() + i);
            return;
        }
    }
}
