#ifndef TILE_H
#define TILE_H

#include <vector>
#include "TileExtra.h"

class Tile
{
public:
    Tile();
    ~Tile();

    uint16_t getBackground() const;
    uint16_t getForeground() const;
    uint16_t getParent() const;
    uint16_t getFlags() const;

    void setBackground(uint16_t bg);
    void setForeground(uint16_t fg);
    void setParent(uint16_t pt);
    void setFlags(uint16_t flags);

    std::vector<TileExtra>& getTileExtras();
    const std::vector<TileExtra>& getTileExtras() const;

private:
    uint16_t m_bg;
    uint16_t m_fg;
    uint16_t m_pt; // parent tile
    uint16_t m_flags;
    std::vector<TileExtra> m_tile_extras;
};

#endif // !TILE_H
