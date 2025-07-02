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

bool WorldManager::join_world(Player* p, const std::string& name)
{
    World* w = get_world(name);
    if (!w)
    {
        std::string path = "worlds/" + name + ".bin";
        if (std::filesystem::exists(path))
        {
            auto world = std::make_unique<World>();
            world->set_name(name);
            world->set_width(100);
            world->set_height(60);
            w = world.get(); // point
            m_worlds.push_back(std::move(world)); // pushhhhhhhhhh
        }
        else
        {
            // create new world
            auto world = std::make_unique<World>();
            world->set_name(name);
            world->set_width(100);
            world->set_height(60);
            w = world.get();
            m_worlds.push_back(std::move(world));
        }
    }

    if (!w) {
        Logger("World creation failed", LogType::Error);
        return false;
    }

   // w->add_player(p); // to-do

    std::vector<uint8_t> world_data = w->Pack();

    TankPacket t;
    t.Type = PACKET_SEND_MAP_DATA;
    t.State = 8;
    t.PacketLength = (uint32_t)world_data.size();
    Logger("packet length: " + std::to_string(t.PacketLength), LogType::Debug);
    p->SendPacket(t, world_data.data(), t.PacketLength);
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
