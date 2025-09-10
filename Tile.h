#include <SFML/Graphics.hpp>
#include <vector>
#include "Textures.h"
//
// Created by monic on 8/1/2025.
//

#ifndef TILE_H
#define TILE_H

struct Tile {
    sf::Sprite sprite;
    sf::Sprite flagSprite;
    sf::Sprite overlay;
    bool isMine;
    bool hasFlag;
    bool isRevealed;
    int adjacentMines;
};

#endif //TILE_H
