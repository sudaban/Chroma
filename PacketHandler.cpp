#include "PacketHandler.h"
#include "GameUpdatePacket.h"
#include "Log.h"

void PacketHandler::Text(ENetPeer* peer)
{

}

void PacketHandler::Tank(ENetPeer* peer)
{

}

void PacketHandler::ProcessPacket(ENetPeer* peer, ENetPacket* packet)
{
	switch (*(int*)packet->data)
	{
	case NET_MESSAGE_GENERIC_TEXT: case NET_MESSAGE_GAME_MESSAGE:
	{
		Text(peer);
		break;
	}
	case NET_MESSAGE_GAME_PACKET:
	{
		Tank(peer);
		break;
	}
	default:
	{
		Logger("Unhandled packet: " + std::to_string(*(int*)packet->data), LogType::Debug);
		break;
	}
	}
	enet_packet_destroy(packet);
}