#ifndef CLIENT_H
#define CLIENT_H

#include <Player.h>

class Client
{
public:

	Client(Player* player, ENetPacket* tank_data, const std::string& text_data);
	~Client();

	inline Player* GetPlayer() const { return m_player; }
	inline ENetPacket* GetTankData() const { return m_tank_data; }
	inline std::string GetTextData() const { return m_text_data; }

private:
	Player* m_player;
	ENetPacket* m_tank_data;
	std::string m_text_data;
};

#endif
