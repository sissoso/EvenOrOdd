#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <string>
#include "Protocol.h"
using namespace std;

namespace networkingLab {

class EventHandler {
public:
	virtual void onMessageRecvFromServer(Command cmd)=0;
	virtual void onMessageSentToServer(Command cmd)=0;
	virtual ~EventHandler() {

	}
};
}

#endif /* EVENTHANDLER_H_ */
