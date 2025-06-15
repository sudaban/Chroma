#include "Client.h"

Client::Client(Player* player, ENetPacket* data)
{
	m_player = player;
	m_data = data;
}

Client::~Client()
{

}