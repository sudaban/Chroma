#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <enet/enet.h>

class Variant {
public:
    Variant(int delay = 0, int netID = -1)
        : index(0) {
        data.resize(61, 0); // initialize with zeros

        int32_t messageType = 0x4;
        int32_t packetType = 0x1;
        int32_t charState = 0x8;

        write(0, messageType);
        write(4, packetType);
        write(8, netID);
        write(16, charState);
        write(24, delay);
    }

    void add(const std::string& str) {
        appendHeader(0x2);
        writeValue<int32_t>(str.size());
        appendBytes(reinterpret_cast<const uint8_t*>(str.data()), str.size());
        finalizeAdd();
    }

    void add(int value) {
        appendHeader(0x9);
        writeValue<int32_t>(value);
        finalizeAdd();
    }

    void add(unsigned int value) {
        appendHeader(0x5);
        writeValue<uint32_t>(value);
        finalizeAdd();
    }

    void add(float value) {
        appendHeader(0x1);
        writeValue<float>(value);
        finalizeAdd();
    }

    void add(float a, float b) {
        appendHeader(0x3);
        writeValue<float>(a);
        writeValue<float>(b);
        finalizeAdd();
    }

    void add(float a, float b, float c) {
        appendHeader(0x4);
        writeValue<float>(a);
        writeValue<float>(b);
        writeValue<float>(c);
        finalizeAdd();
    }

    void send(ENetPeer* peer) const {
        ENetPacket* packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }

private:
    std::vector<uint8_t> data;
    uint8_t index = 0;

    template<typename T>
    void write(size_t offset, const T& value) {
        std::memcpy(data.data() + offset, &value, sizeof(T));
    }

    void appendHeader(uint8_t type) {
        data.push_back(index);
        data.push_back(type);
    }

    template<typename T>
    void writeValue(const T& value) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
        data.insert(data.end(), bytes, bytes + sizeof(T));
    }

    void appendBytes(const uint8_t* bytes, size_t size) {
        data.insert(data.end(), bytes, bytes + size);
    }

    void finalizeAdd() {
        index++;
        if (data.size() >= 61)
            data[60] = index;
    }
};
