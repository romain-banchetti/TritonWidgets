#ifndef TWOBJECT_H_INCLUDED
#define TWOBJECT_H_INCLUDED

#include <iostream>
#include "twString.h"
#include <vector>
#include "tw.h"

typedef struct AEvent AEvent;
typedef struct Child Child;

class twObject
{
    public:
        enum Event{Over = 100, Clicked = 101};
        twObject();
        twObject(twObject *parent, tw::Object type);
        virtual ~twObject();
        void connect(int event, twObject *object = NULL, bool (twObject::*function)() = NULL);
        void connect(int event, bool (twObject::*function)() = NULL);
        void disconnect(twObject *object, int event = -1);
        std::vector<AEvent> events;
        twObject *getSender();
        twObject *m_sender;
        twString stringParameter;
        int intParameter;
        twString* pStringParameter;
        int* pIntParameter;
		int addChild(tw::Object type, twObject *child);
		void removeChild(twObject *child);
        std::vector<Child> *children;
        int sendEvent(int event);
        void setParent(twObject *parent);
        twObject *getParent();
        void setType(tw::Object type);
        tw::Object getType();
        int getChildId();
        int triggerEvent(int event);
        void freezeEvent(int event);
        void unfreezeEvent(int event);

    private:
        twObject *m_parent;
        tw::Object m_type;
        int m_childId;
        std::vector <int> frozenEvents;

};

struct Child
{
    tw::Object type;
    twObject *child;
};

struct AEvent
{
    int event;
    twObject *object;
    bool (twObject::*function)();
};

#endif // TWOBJECT_H_INCLUDED
