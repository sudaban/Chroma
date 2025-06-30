#ifndef WORLD_H
#define WORLD_H

#pragma once
#include <string>
#include <vector>
#include "Tile.h"

class World {
public:
    World();
    World(const std::string& name, int level = 0);
    World(const World& copy);
    ~World();

    const std::string& get_name() const;

private:
    std::string m_name = "";
    int m_level = 0;
    uint32_t m_width = 0, m_height = 0;
    std::vector<Tile> m_tiles = {};
};


#endif