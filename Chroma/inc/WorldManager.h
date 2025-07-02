#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include "World.h"
#include <vector>
#include "Player.h"
#include <memory>

class WorldManager {
public:
    WorldManager();
    ~WorldManager();

    World* add_world(const std::string& name, int level = 0);
    World* get_world(const std::string& name);
    bool delete_world(const std::string& name);
    bool join_world(Player* p, const std::string& name);
    void generate_new_world(const std::string& name);
    const std::vector<std::unique_ptr<World>>& get_all_worlds() const;

private:
    std::vector<std::unique_ptr<World>> m_worlds;
};

#endif