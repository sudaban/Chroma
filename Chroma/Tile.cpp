#include "Tile.h"

Tile::Tile()
    : m_bg(0), m_fg(0), m_pt(0), m_flags(0)
{

}

Tile::~Tile() = default; 

uint16_t Tile::getBackground() const
{
    return m_bg;
}

uint16_t Tile::getForeground() const
{
    return m_fg;
}

uint16_t Tile::getParent() const
{
    return m_pt;
}

uint16_t Tile::getFlags() const
{
    return m_flags;
}

void Tile::setBackground(uint16_t bg)
{
    m_bg = bg;
}

void Tile::setForeground(uint16_t fg)
{
    m_fg = fg;
}

void Tile::setParent(uint16_t pt)
{
    m_pt = pt;
}

void Tile::setFlags(uint16_t flags)
{
    m_flags = flags;
}
