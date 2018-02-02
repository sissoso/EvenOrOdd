#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_


#include <unistd.h>
#include <stdlib.h>

#include "EventHandler.h"
#include "Protocol.h"
#include "Server.h"
#include "Client.h"
#include "Global.h"
#include "MThread.h"

using namespace std;

namespace networkingLab {

class TestHandler: public EventHandler {
public:
	Command lastCmdRecv;
	Command lastCmdSent;

	TestHandler();
	~TestHandler();
	 void onMessageRecvFromServer(Command cmd);
	 void onMessageSentToServer(Command cmd);

};

class TestRunner {
public:
	TestRunner();
	virtual ~TestRunner();
	void print(string str);
	bool test();
};

} /* namespace networkingLab */

#endif /* TESTRUNNER_H_ */
