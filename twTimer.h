#ifndef TWTIMER_H_INCLUDED
#define TWTIMER_H_INCLUDED

#include "twObject.h"

class twTimer : public twObject
{
    public:
        enum Event{TimeOut};
        twTimer(twObject *parent, int interval);
        virtual ~twTimer();

    private:
		UINT_PTR m_timer;
		twObject *m_parent;
};

#endif // TWTIMER_H_INCLUDED
