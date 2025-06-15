#include <cstring>
#include <iostream>
#include "EnetServer.h"
#include "Log.h"
#include "Player.h"
#include "PacketHandler.h"
#include "Client.h"
#include "VariantSender.h"

ENetServer::ENetServer(const std::string& address, uint16_t port, size_t maxClients, size_t channels)
    : m_address(address), m_port(port), m_maxClients(maxClients), m_channels(channels)
{
    if (enet_initialize() != 0)
    {
        Logger("Failed to initialize ENet!", LogType::Error);
        throw std::runtime_error("Failed to initialize ENet");
    }

    ENetAddress addressStruct;
    enet_address_set_host(&addressStruct, m_address.c_str());
    addressStruct.port = m_port;

    m_host = enet_host_create(&addressStruct, static_cast<enet_uint32>(m_maxClients), static_cast<enet_uint32>(m_channels), 0, 0);
    if (m_host == nullptr)
    {
        Logger("Failed to create ENet host, port might be in use.", LogType::Error);
        throw std::runtime_error("Failed to create ENet host");
    }
   // m_host->usingNewPacketForServer = true; // Growtopia 5.19 Protocol (You need mod enet)
    m_host->checksum = enet_crc32;
    enet_host_compress_with_range_coder(m_host);

    Logger("ENetServer running on " + m_address + ":" + std::to_string(m_port), LogType::Info);
}

ENetServer::~ENetServer()
{
    Stop();

    if (m_host)
    {
        enet_host_destroy(m_host);
        m_host = nullptr;
    }

    enet_deinitialize();
}

bool ENetServer::Start()
{
    if (running_)
        return false;

    running_ = true;
    m_thread = std::thread(&ENetServer::Run, this);
    return true;
}

void ENetServer::Stop()
{
    if (!running_)
        return;

    running_ = false;

    if (m_host)
    {
        for (size_t i = 0; i < m_host->peerCount; ++i)
        {
            ENetPeer* peer = &m_host->peers[i];
            if (peer->state == ENET_PEER_STATE_CONNECTED)
            {
                enet_peer_disconnect_later(peer, 0);
            }
        }
        enet_host_flush(m_host);
    }

    if (m_thread.joinable())
        m_thread.join();

    Logger("Server stopped.", LogType::Info);
}

void ENetServer::sendPacket(ENetPeer* peer, int packetType, const void* data, int dataLength)
{
    if (!peer || dataLength < 0)
        return;

    size_t packetSize = 4 + dataLength + 1;
    std::vector<char> buffer(packetSize);

    std::memcpy(buffer.data(), &packetType, 4);
    if (data && dataLength > 0)
        std::memcpy(buffer.data() + 4, data, dataLength);

    buffer[4 + dataLength] = 0;

    ENetPacket* packet = enet_packet_create(buffer.data(), buffer.size(), ENET_PACKET_FLAG_RELIABLE);
    if (!packet)
        return;

    if (enet_peer_send(peer, 0, packet) != 0)
        enet_packet_destroy(packet);
}

void ENetServer::Run()
{
    ENetEvent event;

    while (running_)
    {
        // timeout 10 ms
        while (enet_host_service(m_host, &event, 10) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                if (event.peer->data != nullptr) 
                    break;
                Player* player = new Player(event.peer);
                event.peer->data = player;

                Client cli(player, event.packet);

                Logger("new cli. ID: " + std::to_string(event.peer->connectID), LogType::Info);
                sendPacket(event.peer, 1, NULL, 1);
                VariantSender::OnConsoleMessage(player, "Welcome!");
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
            {
                if (event.peer == nullptr || event.peer->data == nullptr)
                    break;
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                if (event.peer == nullptr || event.peer->data == nullptr)
                    break;

                Logger("cli dc, ID: " + std::to_string(event.peer->connectID), LogType::Debug);
                delete static_cast<Player*>(event.peer->data);
                event.peer->data = nullptr;
                break;
            }
            case ENET_EVENT_TYPE_NONE:
            default:
                break;
            }
        }
    }
}
