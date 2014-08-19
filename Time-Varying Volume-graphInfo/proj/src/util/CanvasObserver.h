#ifndef CANVASOBSERVER_H
#define CANVASOBSERVER_H

#include "observer.h"

class IUpdatable : public Observer
{
public:
	virtual void udpate() = 0;
};

#endif // CANVASOBSERVER_H