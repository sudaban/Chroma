#include "World.h"

World::World() = default;

World::World(const std::string& name, int level)
    : m_name(name), m_level(level), m_width(0), m_height(0) {}

World::World(const World& copy)
    : m_name(copy.m_name), m_level(copy.m_level),
    m_width(copy.m_width), m_height(copy.m_height) {}


unsigned int World::Pack()
{
    size_t total = 0;
    total += 2; // version
    total += 4; // flags
    total += 2; // name length
    total += m_name.length();
    total += 4; // height
    total += 4; // width
    total += 4; // area size
    total += 4; // 4.31 undefined data-1
    total += 1; // 4.31 undefined data-2
    total += m_tiles.size() * 8; // tiles
    total += 4; // object size
    total += 4; // last object id
    total += 4; // active weather
    total += 4; // base weather


    std::vector<uint8_t> buffer(total);

    size_t offset = 0;

    auto write = [&](const void* data, size_t size) {
    memcpy(buffer.data() + offset, data, size);
    offset += size;
    };

    uint16_t version = 25;
    write(&version, sizeof(version));
    uint32_t flags = 0;
    write(&flags, sizeof(flags));
    write(m_name.data(), m_name.length());
    uint32_t height = m_height;
    write(&height, sizeof(height));
    uint32_t width = m_width;
    write(&width, sizeof(width));
    uint32_t area = width * height;
    write(&area, sizeof(area));
    uint32_t unk1 = 0;
    write(&unk1, sizeof(unk1));
    uint8_t unk2 = 0;
    write(&unk2, sizeof(unk2));
    
    for (int i = 0; i < m_tiles.size(); ++i)
    {
        Tile t = m_tiles[i];
        uint16_t fg = t.getForeground();
        uint16_t bg = t.getBackground();
        uint16_t pt = t.getParent();
        uint16_t fl = t.getFlags();

        write(&fg, sizeof(fg));
        write(&bg, sizeof(bg));
        write(&pt, sizeof(pt));
        write(&fl, sizeof(fl));

        //if (fl & 1 << 0)
        // tile extra
    }

    uint32_t object_size = 1; //todo
    write(&object_size, sizeof(object_size));
    uint32_t last_object_id = 0; //todo
    write(&last_object_id, sizeof(last_object_id));

    // todo objects

    uint32_t active_weather = m_active_weather;
    write(&active_weather, sizeof(active_weather));
    uint32_t base_weather = m_base_weather;
    write(&base_weather, sizeof(base_weather));

    return static_cast<unsigned int>(offset);
}

World::~World() = default; // later save or erase maybe

const std::string& World::get_name() const { //un-changeable
    return m_name;
}
