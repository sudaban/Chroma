#include "PacketHandler.h"
#include "GameUpdatePacket.h"
#include "Log.h"
#include "Text.h"

void PacketHandler::Text(Player* ply, ENetPacket* packet)
{
    if (!ply || !packet || !packet->data || packet->dataLength < 5)
    {
        Logger("Invalid packet or player reference.", LogType::Error);
        return;
    }
    std::string raw(reinterpret_cast<char*>(packet->data + 4), packet->dataLength - 4);

    if (raw.empty()) 
    {
        Logger("Received empty action packet.", LogType::Warning);
        return;
    }

    std::string erased_action = Text::to_erase("action|", raw);

    if (raw.starts_with("requestedName"))
    {
        ply->Login(ply, raw, true, false);
    }
    else if (raw.starts_with("tankIDName"))
    {
        ply->Login(ply, raw, false, false);
    }
    else
    {
        Logger("Unknown action received: " + erased_action, LogType::Error);
    }
}


void PacketHandler::Tank(Player* ply, ENetPacket* packet) {}

void PacketHandler::ProcessPacket(ENetPeer* peer, ENetPacket* packet)
{
	Player* ply = (Player*)peer->data;
	switch (*(int*)packet->data)
	{
	case NET_MESSAGE_GENERIC_TEXT: case NET_MESSAGE_GAME_MESSAGE:
	{
		Logger("Received text, processing...", LogType::Info);
		Text(ply, packet);
		break;
	}
	case NET_MESSAGE_GAME_PACKET:
	{
		Logger("Received tank, processing...", LogType::Info);
		Tank(ply, packet);
		break;
	}
	default:
	{
		Logger("unknown packet: " + std::to_string(*(int*)packet->data), LogType::Debug);
		break;
	}
	}
	enet_packet_destroy(packet);
}