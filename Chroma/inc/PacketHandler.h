#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <enet/enet.h>
#include "Player.h"

class PacketHandler
{
public:
	void Text(Player* ply, ENetPacket* packet); // For action| and login
	void Tank(Player* ply, ENetPacket* packet); // Handle Tank packets (GameUpdatePacket) for client-server communication.
	void ProcessPacket(ENetPeer* peer, ENetPacket* packet);
};

#endif //!PACKETHANDLER_H