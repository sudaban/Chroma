#include "inc/WorldManager.h"
#include <algorithm>

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
