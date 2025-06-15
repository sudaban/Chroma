#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <enet/enet.h>
#include "Client.h"

class PacketHandler
{
public:
	void Text(Client cli, ENetPacket* packet); // Handle text packets
	void Tank(Client cli, ENetPacket* packet); // Handle tank packets a.k.a. GameUpdatePacket for client-server communication.
	void ProcessPacket(Client cli, ENetPacket* packet);
};

#endif //!PACKETHANDLER_H