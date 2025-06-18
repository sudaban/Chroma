#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <enet/enet.h>
#include "Client.h"

class PacketHandler
{
public:
	PacketHandler(ENetPacket* tank_packet, const std::string& text_packet);
	void Text(Client cli); // Handle text packets
	void Tank(Client cli); // Handle tank packets a.k.a. GameUpdatePacket for client-server communication.
	void ProcessPacket(Client cli);
private:
	ENetPacket* m_tank_packet;
	std::string m_text_packet;
};

#endif //!PACKETHANDLER_H