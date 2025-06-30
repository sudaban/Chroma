#include "inc/World.h"

World::World() = default;

World::World(const std::string& name, int level)
    : m_name(name), m_level(level), m_width(0), m_height(0) {}

World::World(const World& copy)
    : m_name(copy.m_name), m_level(copy.m_level),
    m_width(copy.m_width), m_height(copy.m_height) {}

World::~World() = default; // later save or erase maybe

const std::string& World::get_name() const { //un-changeable
    return m_name;
}
