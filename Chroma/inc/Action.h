#ifndef ACTION_H
#define ACTION_H

#include <Client.h>

class Action
{
public:
	static void RefreshItemsData(Client cli);
	static void EnterGame(Client cli);
	static void JoinRequest(Client cli);
};

#endif