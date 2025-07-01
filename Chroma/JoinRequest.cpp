#include "Action.h"
#include "Text.h"
#include "Log.h"

void Action::JoinRequest(Client cli)
{
	std::string data = cli.GetTextData();
	Player* player = cli.GetPlayer();
	WorldManager* wm = cli.GetWorldManager();

	std::string requested = Text::parser("name", data);
	Logger("Requested World: " + requested, LogType::Debug);
	wm->join_world(player, requested);
}