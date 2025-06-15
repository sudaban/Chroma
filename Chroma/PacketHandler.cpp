#include "PacketHandler.h"
#include <Packet.h>
#include "Log.h"
#include "Text.h"

void PacketHandler::Text(Client cli, ENetPacket* packet)
{
    Player* ply = cli.GetPlayer();

    if (!ply || !packet || !packet->data || packet->dataLength < 5)
    {
        Logger("Invalid packet or player reference.", LogType::Error);
        return;
    }

    std::string raw(reinterpret_cast<char*>(packet->data + 4), packet->dataLength - 4);

    if (raw.empty())
    {
        Logger("Received empty text packet.", LogType::Warning);
        return;
    }

    std::string action = Text::to_erase("action|", raw);

    if (action.starts_with("requestedName"))
    {
        ply->Login(raw, true, false);
    }
    else if (action.starts_with("tankIDName"))
    {
        ply->Login(raw, false, false);
    }
    else if (action.starts_with("protocol"))
    {
        ply->Login(raw, false, true);
    }
    else
    {
        Logger("Unhandled Action Received: " + action, LogType::Error);
    }
}


void PacketHandler::Tank(Client cli, ENetPacket* packet)
{
    uint32_t packetType = *(uint32_t*)packet->data;
    switch (packetType)
    {
    default:
    {
        Logger("Unhandled Tank Packet Received: " + Packet::GetTankPacketName(packetType), LogType::Debug);
        break;
    }
    }
}

void PacketHandler::ProcessPacket(Client cli, ENetPacket* packet)
{

    uint32_t packetType = *(uint32_t*)packet->data;

    switch (packetType)
    {
    case NET_MESSAGE_GENERIC_TEXT: case NET_MESSAGE_GAME_MESSAGE:
    {
        Text(cli, packet);
        break;
    }
    case NET_MESSAGE_GAME_PACKET:
    {
        Tank(cli, packet);
        break;
    }
    default:
    {
        Logger("Unknown Packet Received: " + Packet::GetTankPacketName(packetType), LogType::Debug);
        break;
    }
    }
    enet_packet_destroy(packet);
}