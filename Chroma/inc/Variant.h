#ifndef VARIANT_HH
#define VARIANT_HH

#include <cstdint>
#include <string>
#include <vector>
#include <cstring>
#include <variant>
#include <stdexcept>
#include <memory>
#include "Log.h"

class Variant
{
public:
    enum ArgType : uint8_t {
        TYPE_STRING = 0x2,
        TYPE_UINT = 0x5,
        TYPE_INT = 0x9,
        TYPE_FLOAT1 = 0x1,
        TYPE_FLOAT2 = 0x3,
        TYPE_FLOAT3 = 0x4
    };

    using ArgValue = std::variant<std::string, uint32_t, int32_t, std::vector<float>>;

    struct VariantData
    {
        uint32_t type = 0;
        uint32_t packetType = 0;
        int32_t netID = -1;
        uint32_t state = 0;
        uint32_t delay = 0;
        uint8_t argsCount = 0;
        std::vector<ArgValue> args;
        std::vector<uint8_t> packet;
    };

    static VariantData from(const std::vector<uint8_t>& buffer)
    {
        if (buffer.size() < 61)
        {
            Logger("Invalid buffer size!", LogType::Error);
            exit(EXIT_FAILURE);
        }

        VariantData v;
        memcpy(&v.type, &buffer[0], 4);
        memcpy(&v.packetType, &buffer[4], 4);
        memcpy(&v.netID, &buffer[8], 4);
        memcpy(&v.state, &buffer[16], 4);
        memcpy(&v.delay, &buffer[24], 4);

        v.argsCount = buffer[60];

        size_t pos = 62;
        for (int i = 0; i < v.argsCount; ++i)
        {
            if (pos >= buffer.size()) break;

            uint8_t argType = buffer[pos++];
            switch (argType)
            {
            case TYPE_STRING:
            {
                uint32_t len;
                memcpy(&len, &buffer[pos], 4);
                pos += 4;
                std::string str(reinterpret_cast<const char*>(&buffer[pos]), len);
                pos += len + 1;
                v.args.emplace_back(str);
                break;
            }
            case TYPE_UINT:
            {
                uint32_t val;
                memcpy(&val, &buffer[pos], 4);
                pos += 5;
                v.args.emplace_back(val);
                break;
            }
            case TYPE_INT:
            {
                int32_t val;
                memcpy(&val, &buffer[pos], 4);
                pos += 5;
                v.args.emplace_back(val);
                break;
            }
            case TYPE_FLOAT1:
            case TYPE_FLOAT2:
            case TYPE_FLOAT3:
            {
                int count = (argType == TYPE_FLOAT1) ? 1 : (argType == TYPE_FLOAT2) ? 2 : 3;
                std::vector<float> floats;
                for (int j = 0; j < count; ++j) {
                    float f;
                    memcpy(&f, &buffer[pos], 4);
                    pos += 4;
                    floats.push_back(f);
                }
                pos += 1;
                v.args.emplace_back(floats);
                break;
            }
            default:
                Logger("Invalid argument type!", LogType::Error);
                exit(EXIT_FAILURE);
            }
        }

        v.packet = buffer;
        return v;
    }

    static VariantData call(const std::vector<ArgValue>& args, int32_t netID = -1, uint32_t delay = 0)
    {
        std::vector<uint8_t> packet;
        packet.resize(61);
        write_u32(packet, 0, 4);
        write_u32(packet, 4, 1);
        write_i32(packet, 8, netID);
        write_u32(packet, 16, 8);
        write_u32(packet, 24, delay);
        packet[60] = static_cast<uint8_t>(args.size());

        size_t pos = 61;

        for (size_t i = 0; i < args.size(); ++i)
        {
            packet.push_back(static_cast<uint8_t>(i));

            if (auto str = std::get_if<std::string>(&args[i]))
            {
                packet.push_back(TYPE_STRING);
                write_u32(packet, pos + 1, str->length());
                packet.insert(packet.end(), str->begin(), str->end());
                packet.push_back(0);
                pos = packet.size();
            }
            else if (auto ui = std::get_if<uint32_t>(&args[i]))
            {
                packet.push_back(TYPE_UINT);
                write_u32(packet, pos + 1, *ui);
                packet.resize(packet.size() + 4);
                pos += 6;
            }
            else if (auto si = std::get_if<int32_t>(&args[i]))
            {
                packet.push_back(TYPE_INT);
                write_i32(packet, pos + 1, *si);
                packet.resize(packet.size() + 4);
                pos += 6;
            }
            else if (auto floats = std::get_if<std::vector<float>>(&args[i]))
            {
                uint8_t code = floats->size() == 1 ? TYPE_FLOAT1 : floats->size() == 2 ? TYPE_FLOAT2 : TYPE_FLOAT3;
                packet.push_back(code);
                for (float f : *floats) write_float(packet, packet.size(), f);
                packet.push_back(0);
                pos = packet.size();
            }
        }

        return
        {
            .type = 4,
            .packetType = 1,
            .netID = netID,
            .state = 8,
            .delay = delay,
            .argsCount = static_cast<uint8_t>(args.size()),
            .args = args,
            .packet = packet
        };
    }

private:
    static void write_u32(std::vector<uint8_t>& buf, size_t offset, uint32_t val)
    {
        if (offset + 4 > buf.size()) buf.resize(offset + 4);
        memcpy(&buf[offset], &val, 4);
    }

    static void write_i32(std::vector<uint8_t>& buf, size_t offset, int32_t val)
    {
        if (offset + 4 > buf.size()) buf.resize(offset + 4);
        memcpy(&buf[offset], &val, 4);
    }

    static void write_float(std::vector<uint8_t>& buf, size_t offset, float val)
    {
        if (offset + 4 > buf.size()) buf.resize(offset + 4);
        memcpy(&buf[offset], &val, 4);
    }
};


#endif
