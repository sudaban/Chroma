#include "inc/WorldManager.h"
#include <algorithm>
#include <filesystem>
#include "inc/Packet.h"
#include "inc/Log.h"

WorldManager::WorldManager() = default;
WorldManager::~WorldManager() = default;

World* WorldManager::add_world(const std::string& name, int level) {
    if (get_world(name)) return nullptr;

    auto world = std::make_unique<World>(name, level);
    World* ptr = world.get();
    m_worlds.emplace_back(std::move(world));
    return ptr;
}

World* WorldManager::get_world(const std::string& name) {
    for (auto& world : m_worlds) {
        if (world->get_name() == name) {
            return world.get();
        }
    }
    return nullptr;
}

void WorldManager::generate_new_world(const std::string& name)
{
    const uint32_t width = 100;
    const uint32_t height = 60;

    auto world = std::make_unique<World>();
    world->set_name(name);
    world->set_width(width);
    world->set_height(height);

    std::vector<Tile> tiles;
    tiles.reserve(width * height);

    const int dirt_start = 24;
    const int dirt_end = 52;
    const int lava_layer = 47;
    const int bedrock_layer = 51;

    const int main_door_x = rand() % (width - 5) + 5;
    const int main_door_y = dirt_start;

    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            Tile tile;
            const int index = y * width + x;
            const int rng = rand() % 100;

            // Foreground logic based on layer
            if (y > dirt_start && y < dirt_end)
            {
                if (y > lava_layer && rng < 30)
                {
                    tile.setForeground(4); // Lava
                }
                else
                {
                    if (y > 27 && rng < 3)
                        tile.setForeground(10); // Rock
                    else
                        tile.setForeground(2); // Dirt
                }
            }
            else if (x == main_door_x && y == main_door_y)
            {
                tile.setForeground(6); // Main Door
                tile.setBackground(4); // Cave Background
                // TODO: door tile extra
            }
            else if (y > bedrock_layer)
            {
                tile.setForeground(8); // Bedrock
            }
            // Place bedrock under the main door
            if (x == main_door_x && y == main_door_y + 1)
                tile.setForeground(8); // Bedrock
            // Set background for underground tiles
            if (y > dirt_start)
                tile.setBackground(14); // Cave Background
            tiles.push_back(tile);
        }
    }
    world->set_tiles(tiles);
    m_worlds.push_back(std::move(world));
}

bool WorldManager::join_world(Player* p, const std::string& name)
{
    World* w = get_world(name);
    if (!w)
    {
        std::string path = "worlds/" + name + ".bin";
        if (std::filesystem::exists(path))
        {
            // TODO: load from .bin file
            Logger("World file exists but loading not implemented", LogType::Warning);
            return false;
        }
        else
        {
            // Instead of basic flat world creation, use generate_new_world
            generate_new_world(name);
            w = get_world(name);
        }
    }

    if (!w) {
        Logger("World creation failed", LogType::Error);
        return false;
    }

    // w->add_player(p); // TODO

    std::vector<uint8_t> tp(60);

    size_t os = 0;
    uint32_t mtype = 4, ptype = 4, state = 8;
    
    memcpy(tp.data() + os, &mtype, sizeof(mtype));
    os += sizeof(mtype);
    memcpy(tp.data() + os, &ptype, sizeof(ptype));
    os += sizeof(ptype);
    memcpy(tp.data() + os, &state, sizeof(state));
    os += sizeof(state);
    
    std::vector<uint8_t> world_data = w->Pack();
    
    std::vector<uint8_t> full_data;
    full_data.reserve(tp.size() + world_data.size());
    full_data.insert(full_data.end(), tp.begin(), tp.end());
    full_data.insert(full_data.end(), world_data.begin(), world_data.end());
    
    ENetPacket* packet = enet_packet_create(full_data.data(), full_data.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(p->GetPeer(), 0, packet);
    
    return true;
}

bool WorldManager::delete_world(const std::string& name) {
    auto it = std::remove_if(m_worlds.begin(), m_worlds.end(),
        [&](const std::unique_ptr<World>& w) {
            return w->get_name() == name;
        });

    if (it != m_worlds.end()) {
        m_worlds.erase(it, m_worlds.end());
        return true;
    }
    return false;
}

const std::vector<std::unique_ptr<World>>& WorldManager::get_all_worlds() const { //un-changeable
    return m_worlds;
}
