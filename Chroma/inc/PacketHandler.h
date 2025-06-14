#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <enet/enet.h>

class PacketHandler
{
public:
	void Text(ENetPeer* peer); // For action| and login
	void Tank(ENetPeer* peer); // Handles Tank packets (GameUpdatePacket) for client-server communication.
	void ProcessPacket(ENetPeer* peer, ENetPacket* packet);
};

#endif //!PACKETHANDLER_H