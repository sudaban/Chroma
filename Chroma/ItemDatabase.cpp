#include "ItemDatabase.h"
#include <string>
#include <fstream>
#include "Log.h"
#include "Packet.h"
#include <filesystem>
#include <cstring>

int ItemDatabase::items_data_size = 0;
uint8_t* ItemDatabase::items_data = nullptr;
uint32_t ItemDatabase::items_data_hash = 0;

uint32_t ItemDatabase::proton_hash(const void* data, uintmax_t data_len)
{
    if (!data) return 0;
    uint32_t hash = 0x55555555;

    for (uintmax_t i = 0; i < data_len; ++i)
        hash = (hash >> 27) + (hash << 5) + static_cast<const uint8_t*>(data)[i];

    return hash;
}

bool ItemDatabase::Decode(const std::string& path)
{
    const std::string secret = "PBG892FXX982ABC*";

    auto now = std::chrono::high_resolution_clock::now();

    if (!std::filesystem::exists(path))
    {
        Logger(path + " is not found!", LogType::Error);
        return false;
    }

    items_data_size = std::filesystem::file_size(path);
    items_data = new uint8_t[items_data_size];
   
    std::vector<uint8_t> buffer(items_data_size);

    {
        std::ifstream file(path, std::ios::binary);
        if (!file || !file.read(reinterpret_cast<char*>(buffer.data()), items_data_size)) {
            Logger("Failed to read: " + path, LogType::Error);
            return false;
        }
    }

    items_data_hash = proton_hash(buffer.data(), buffer.size());
   
    int pos = 0;

    auto read = [&](auto& val) {
        memcpy(&val, &buffer[pos], sizeof(val));
        pos += sizeof(val);
    };

    auto read_string = [&](std::string& out, bool decrypt = false, int itemID = 0) {
        int16_t len;
        read(len);
        out.clear();
        for (int i = 0; i < len; ++i) {
            char ch = buffer[pos++];
            out += decrypt ? (ch ^ secret[(i + itemID) % secret.size()]) : ch;
        }
    };

    int16_t version;
    int32_t itemCount;
    read(version);
    read(itemCount);

    for (int i = 0; i < itemCount; ++i) {
        Item item{};
        read(item.itemID);
        read(item.itemProps1);
        read(item.itemProps2);
        read(item.itemCategory);
        read(item.hitSoundType);
        read_string(item.name, true, item.itemID);
        read_string(item.texture);
        read(item.textureHash);
        read(item.itemKind);
        read(item.val1);
        read(item.textureX);
        read(item.textureY);
        read(item.spreadType);
        read(item.isStripeyWallpaper);
        read(item.collisionType);
        read(item.breakHits);
        read(item.restoreTime);
        read(item.clothingType);
        read(item.rarity);
        read(item.maxAmount);
        read_string(item.extraFile);
        read(item.extraFileHash);
        read(item.audioVolume);
        read_string(item.petName);
        read_string(item.petPrefix);
        read_string(item.petSuffix);
        read_string(item.petAbility);
        read(item.seedBase);
        read(item.seedOverlay);
        read(item.treeBase);
        read(item.treeLeaves);
        read(item.seedColor);
        read(item.seedOverlayColor);
        pos += 4;
        read(item.growTime);
        read(item.val2);
        read(item.isRayman);
        read_string(item.extraOptions);
        read_string(item.texture2);
        read_string(item.extraOptions2);

        int16_t len4;
        read(len4);
        item.extraFieldUnk_4.assign(reinterpret_cast<const char*>(&buffer[pos]), len4);
        pos += len4;
        pos += 4;
        read(item.value);
        read(item.value2);
        read(item.unkValueShort1);
        int diff = static_cast<int>(16 - item.value);
        pos += (diff > 0) ? diff : 0;
        read(item.unkValueShort2);
        pos += 50;

        if (version >= 11) read_string(item.punchOptions);
        if (version >= 12) {
            read(item.newValue);
            read(item.newValue1);
            read(item.newValue2);
            read(item.newValue3);
            read(item.newValue4);
            read(item.newValue5);
            read(item.newValue6);
            read(item.newValue7);
            read(item.newValue8);
            read(item.newValue9);
        }
        if (version >= 13) read(item.newInt1);
        if (version >= 14) read(item.newInt2);
        if (version >= 15) {
            read(item.canPlayerSit);
            read(item.sitPlayerOffsetX);
            read(item.sitPlayerOffsetY);
            read(item.sitOverlayX);
            read(item.sitOverlayY);
            read(item.sitOverlayOffsetX);
            read(item.sitOverlayOffsetY);
            read_string(item.sitOverlayTexture);
        }
        if (version >= 16) read_string(item.rendererFile);
        if (version >= 17) read(item.newInt3);
        if (version >= 18) read(item.rendererHash);
        if (version >= 19) {
            for (int j = 0; j < 9; ++j)
                read(item.newInt5[j]);
        }
        if (version >= 21) {
            read(item.newInt4);
        }

        if (item.itemID != i) {
            if (item.itemID == 99999) item.itemID = 5550; // bypass shitty items.dat protection (not really needed)
            else
            {
                Logger("Unordered item with ItemID: " + std::to_string(item.itemID), LogType::Error);
                return false;
            }
        }

        items.push_back(std::move(item));
    }
    auto time_taken = std::chrono::high_resolution_clock::now() - now;

    Logger(std::to_string(items.size()) + " items loaded in " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(time_taken).count()) + "ms.", LogType::Info);

    return true;
}