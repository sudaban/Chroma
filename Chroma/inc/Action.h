#ifndef ACTIONS_H
#define ACTIONS_H

#include <Client.h>

class Action
{
public:
	static void RefreshItemsData(Client cli);
	static void EnterGame(Client cli);
};

#endif