#include "PacketHandler.h"
#include "Packet.h"
#include "Log.h"
#include "Text.h"
#include "Action.h"

PacketHandler::PacketHandler(ENetPacket* tank_packet, const std::string& text_packet)
{
    m_tank_packet = tank_packet;
    m_text_packet = text_packet;
}

void PacketHandler::Text(Client cli)
{
    Player* ply = cli.GetPlayer();

    if (!ply || !m_tank_packet || !m_tank_packet->data || m_tank_packet->dataLength < 5)
    {
        Logger("Invalid tank packet or player reference.", LogType::Error);
        return;
    }

    if (m_text_packet.empty())
    {
        Logger("Received empty text packet.", LogType::Error);
        return;
    }

    std::string action = Text::to_erase("action|", m_text_packet);

    if (m_text_packet.starts_with("requestedName"))
    {
        ply->Login(m_text_packet, true, false);
    }
    else if (m_text_packet.starts_with("tankIDName"))
    {
        ply->Login(m_text_packet, false, false);
    }
    else if (m_text_packet.starts_with("protocol"))
    {
        ply->Login(m_text_packet, false, true);
    }
    else if (action.starts_with("refresh_item_data"))
    {
        Action::RefreshItemsData(cli);
    }
    else if (action.starts_with("enter_game"))
    {
        Action::EnterGame(cli);
    }
    else if (action.starts_with("join_request"))
    {
        Action::JoinRequest(cli);
    }
    else if (action == "quit")
    {
        enet_peer_disconnect_later(ply->GetPeer(), 0);
    }
    else
    {
        Logger("Unhandled Action Received: " + action, LogType::Warning);
    }
}


void PacketHandler::Tank(Client cli)
{
    uint32_t packetType = *(uint32_t*)m_tank_packet->data;
    switch (packetType)
    {
    default:
    {
        Logger("Unhandled Tank Packet Received: " + Packet::GetTankPacketName(packetType), LogType::Warning);
        break;
    }
    }
}

void PacketHandler::ProcessPacket(Client cli)
{

    uint32_t packetType = *(uint32_t*)m_tank_packet->data;

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
        Logger("Unknown Packet Received: " + Packet::GetTankPacketName(packetType), LogType::Warning);
        break;
    }
    }
    enet_packet_destroy(m_tank_packet);
}