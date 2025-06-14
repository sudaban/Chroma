#ifndef ENETSERVER_H
#define ENETSERVER_H

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <enet/enet.h>

class ENetServer
{
public:
    ENetServer(const std::string& address = "0.0.0.0", uint16_t port = 17091, size_t maxClients = 1024, size_t channels = 2);
    ~ENetServer();

    bool Start();
    void Stop();

    bool IsRunning() const noexcept { return running_; }
    void sendPacket(ENetPeer* peer, int packetType, const void* data, int dataLength);
private:
    void Run();

    std::string m_address;
    uint16_t m_port;
    size_t m_maxClients;
    size_t m_channels;

    ENetHost* m_host = nullptr;

    std::thread m_thread;
    std::atomic<bool> running_{ false };
};
#endif