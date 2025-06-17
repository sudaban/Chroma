#include "PacketHandler.h"
#include "Packet.h"
#include "Log.h"
#include "Text.h"

PacketHandler::PacketHandler(ENetPacket* p)
{
    m_packet = p;
}

void PacketHandler::Text(Client cli)
{
    Player* ply = cli.GetPlayer();

    if (!ply || !m_packet || !m_packet->data || m_packet->dataLength < 5)
    {
        Logger("Invalid m_packet or player reference.", LogType::Error);
        return;
    }

    std::string raw(reinterpret_cast<char*>(m_packet->data + 4), m_packet->dataLength - 4);

    if (raw.empty())
    {
        Logger("Received empty text m_packet.", LogType::Warning);
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
    else if (action.find("refresh_item_data\n") != std::string::npos)
    {
        Logger("Cli requested item data", LogType::Info);
    }
    else
    {
        Logger("Unhandled Action Received: " + action, LogType::Error);
    }
}


void PacketHandler::Tank(Client cli)
{
    uint32_t packetType = *(uint32_t*)m_packet->data;
    switch (packetType)
    {
    default:
    {
        Logger("Unhandled Tank Packet Received: " + Packet::GetTankPacketName(packetType), LogType::Debug);
        break;
    }
    }
}

void PacketHandler::ProcessPacket(Client cli)
{

    uint32_t packetType = *(uint32_t*)m_packet->data;

    switch (packetType)
    {
    case NET_MESSAGE_GENERIC_TEXT: case NET_MESSAGE_GAME_MESSAGE:
    {
        Text(cli);
        break;
    }
    case NET_MESSAGE_GAME_PACKET:
    {
        Tank(cli);
        break;
    }
    default:
    {
        Logger("Unknown Packet Received: " + Packet::GetTankPacketName(packetType), LogType::Debug);
        break;
    }
    }
    enet_packet_destroy(m_packet);
}