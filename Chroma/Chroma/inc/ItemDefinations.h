#ifndef ITEMDEFINATIONS_H
#define ITEMDEFINATIONS_H

#include <string>
#include <cstdint>

struct Item 
{
    int itemID = 0;
    char itemProps1 = 0;
    char itemProps2 = 0;
    uint8_t itemCategory = 0;
    char hitSoundType = 0;
    std::string name;
    std::string texture;
    int textureHash = 0;
    char itemKind = 0;
    int val1 = 0;
    char textureX = 0;
    char textureY = 0;
    char spreadType = 0;
    char isStripeyWallpaper = 0;
    char collisionType = 0;
    uint8_t breakHits = 0;
    int restoreTime = 0;
    char clothingType = 0;
    int16_t rarity = 0;
    uint8_t maxAmount = 0;
    std::string extraFile;
    int extraFileHash = 0;
    int audioVolume = 0;
    std::string petName, petPrefix, petSuffix, petAbility;
    char seedBase = 0, seedOverlay = 0, treeBase = 0, treeLeaves = 0;
    int seedColor = 0, seedOverlayColor = 0, growTime = 0;
    int16_t val2 = 0, isRayman = 0;
    std::string extraOptions, texture2, extraOptions2, punchOptions, extraFieldUnk_4;
    int16_t value = 0, value2 = 0, unkValueShort1 = 0, unkValueShort2 = 0;
    int newValue = 0, newInt1 = 0, newInt2 = 0, newInt3 = 0, newInt4 = 0;
    uint8_t newValue1 = 0, newValue2 = 0, newValue3 = 0, newValue4 = 0;
    uint8_t newValue5 = 0, newValue6 = 0, newValue7 = 0, newValue8 = 0, newValue9 = 0;
    bool canPlayerSit = false;
    int sitPlayerOffsetX = 0, sitPlayerOffsetY = 0;
    int sitOverlayX = 0, sitOverlayY = 0;
    int sitOverlayOffsetX = 0, sitOverlayOffsetY = 0;
    std::string sitOverlayTexture;
    std::string rendererFile;
    uint32_t rendererHash;
    uint8_t newInt5[9] = {};
};

#endif