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
    std::vector<uint8_t> Pack();
    ~World();

    const std::string& get_name() const;
    void set_name(const std::string& name);

    int get_level() const;
    void set_level(int level);

    uint32_t get_width() const;
    void set_width(uint32_t width);

    uint32_t get_height() const;
    void set_height(uint32_t height);

    const std::vector<Tile>& get_tiles() const;
    void set_tiles(const std::vector<Tile>& tiles);

    uint32_t get_active_weather() const;
    void set_active_weather(uint32_t active_weather);

    uint32_t get_base_weather() const;
    void set_base_weather(uint32_t base_weather);

private:
    std::string m_name = "";
    int m_level = 0;
    uint32_t m_width = 100, m_height = 60;
    std::vector<Tile> m_tiles = {};
    uint32_t m_active_weather = 0, m_base_weather = 0;
};


#endif
