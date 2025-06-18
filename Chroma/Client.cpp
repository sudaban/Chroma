#include "Client.h"

Client::Client(Player* player, ENetPacket* tank_data, const std::string& text_data)
	: m_player(player), m_tank_data(tank_data), m_text_data(text_data) {}


Client::~Client() = default;
