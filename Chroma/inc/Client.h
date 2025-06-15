#ifndef CLIENT_H
#define CLIENT_H

#include <Player.h>

class Client
{
public:

	Client(Player* player, ENetPacket* data);
	~Client();

	inline Player* GetPlayer() const { return m_player; }

private:
	Player* m_player;
	ENetPacket* m_data;
};

#endif
