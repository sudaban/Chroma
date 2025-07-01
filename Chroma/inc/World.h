#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>
#include <cstdint>
#include <cstring>

#include "Tile.h"

class World {
public:
    World();
    World(const std::string& name, int level = 0);
    World(const World& copy);
    unsigned int Pack();
    ~World();

    const std::string& get_name() const;

private:
    std::string m_name = "";
    int m_level = 0;
    uint32_t m_width = 100, m_height = 60;
    std::vector<Tile> m_tiles = {};
    uint32_t m_active_weather = 0, m_base_weather = 0;
};


#endif
