#include "World.h"
#include "inc/Log.h"

World::World() = default;

World::World(const std::string& name, int level)
    : m_name(name), m_level(level), m_width(0), m_height(0) {
}

World::World(const World& copy)
    : m_name(copy.m_name), m_level(copy.m_level),
    m_width(copy.m_width), m_height(copy.m_height) {
}

World::~World() = default; // later save or erase maybe

const std::string& World::get_name() const {
    return m_name;
}

void World::set_name(const std::string& name) {
    m_name = name;
}

int World::get_level() const {
    return m_level;
}

void World::set_level(int level) {
    m_level = level;
}

uint32_t World::get_width() const {
    return m_width;
}

void World::set_width(uint32_t width) {
    m_width = width;
}

uint32_t World::get_height() const {
    return m_height;
}

void World::set_height(uint32_t height) {
    m_height = height;
}

const std::vector<Tile>& World::get_tiles() const {
    return m_tiles;
}

void World::set_tiles(const std::vector<Tile>& tiles) {
    m_tiles = tiles;
}

uint32_t World::get_active_weather() const {
    return m_active_weather;
}

void World::set_active_weather(uint32_t active_weather) {
    m_active_weather = active_weather;
}

uint32_t World::get_base_weather() const {
    return m_base_weather;
}

void World::set_base_weather(uint32_t base_weather) {
    m_base_weather = base_weather;
}


std::vector<uint8_t> World::Pack()
{
    size_t total = 0;
    total += 2; // version
    total += 4; // flags
    total += 2; // name length
    total += m_name.length();
    total += 4; // height
    total += 4; // width
    total += 4; // area size
    total += 4; // undefined data-1
    total += 1; // undefined data-2
    total += m_tiles.size() * 8; // tiles
    total += 4; // object size
    total += 4; // last object id
    total += 4; // active weather
    total += 4; // base weather

    std::vector<uint8_t> buffer(total);
    size_t offset = 0;

    auto safe_write = [&](const void* data, size_t size) {
        if (offset + size > buffer.size()) {
            Logger("World::Pack failed: buffer overflow detected", LogType::Error);
            return false;
        }
        memcpy(buffer.data() + offset, data, size);
        offset += size;
        return true;
        };

    uint16_t version = 25;
    uint32_t flags = 0;
    uint32_t height = m_height;
    uint32_t width = m_width;
    uint32_t area = width * height;
    uint32_t unk1 = 0;
    uint8_t unk2 = 0;

    if (!safe_write(&version, sizeof(version))) return {};
    if (!safe_write(&flags, sizeof(flags))) return {};
    uint16_t name_length = static_cast<uint16_t>(m_name.length());
    if (!safe_write(&name_length, sizeof(name_length))) return {};
    if (!safe_write(m_name.data(), name_length)) return {};

    if (!safe_write(&height, sizeof(height))) return {};
    if (!safe_write(&width, sizeof(width))) return {};
    if (!safe_write(&area, sizeof(area))) return {};
    if (!safe_write(&unk1, sizeof(unk1))) return {};
    if (!safe_write(&unk2, sizeof(unk2))) return {};

    for (const auto& t : m_tiles) {
        uint16_t fg = t.getForeground();
        uint16_t bg = t.getBackground();
        uint16_t pt = t.getParent();
        uint16_t fl = t.getFlags();

        if (!safe_write(&fg, sizeof(fg))) return {};
        if (!safe_write(&bg, sizeof(bg))) return {};
        if (!safe_write(&pt, sizeof(pt))) return {};
        if (!safe_write(&fl, sizeof(fl))) return {};
    }

    uint32_t object_size = 0; // placeholder
    uint32_t last_object_id = -1;
    if (!safe_write(&object_size, sizeof(object_size))) return {};
    if (!safe_write(&last_object_id, sizeof(last_object_id))) return {};

    uint32_t active_weather = m_active_weather;
    uint32_t base_weather = m_base_weather;
    if (!safe_write(&active_weather, sizeof(active_weather))) return {};
    if (!safe_write(&base_weather, sizeof(base_weather))) return {};

    return buffer;
}


