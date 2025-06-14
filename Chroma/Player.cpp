#include "Player.h"
#include "Log.h"
#include "Text.h"

Player::Player(ENetPeer* peer)
{
	if (peer)
		this->m_peer = peer;
	else return;
}
Player::~Player() {/*destructor*/ }

void Player::Login(Player* player, const std::string& data, bool guest, bool new_login)
{
	Logger(data, LogType::Debug);

	if (guest) 
	{
		player->m_guest = true;
	}

	if (new_login)
	{
		player->m_growid = Text::parser("growId", data);
		player->m_pass = Text::parser("password", data);
		Logger("growId (logged): " + player->m_growid, LogType::Debug);

		if (player->m_growid == "_register_" && player->m_pass == "_register_") {
			player->m_guest = true; // NYRU.net login
		}
	}
	else
	{
		player->m_growid = Text::parser("tankIDName", data);
		player->m_pass = Text::parser("tankIDPass", data);
		Logger("tankIDName (logged): " + player->m_growid, LogType::Debug);
	}

	std::string requestedName = Text::parser("requestedName", data);
	if (requestedName.empty()) {
		requestedName = "CrazyShit_";
	}
	player->m_req_name = requestedName + std::to_string(rand() % 900 + 100);
	Logger("req name (logging): " + player->m_req_name, LogType::Debug);

	player->m_game_version = Text::parser("game_version", data);
	player->m_country = Text::parser("country", data);
	player->m_rid = Text::parser("rid", data);
	player->m_mac = Text::parser("mac", data);
	player->m_platform_id = Text::parser("platformID", data);
}

void GlobalMessage(ENetHost* server, const std::string& message) 
{
	for (ENetPeer* currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer) 
	{

		if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == nullptr)
			continue;

		Player* player = static_cast<Player*>(currentPeer->data);
		// There will be console message packet
	}
}