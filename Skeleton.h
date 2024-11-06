#pragma once

#include "Enemy.h"

class Skeleton : public Enemy {
public:
    Skeleton(int startX, int startY,
        Camera& camera, std::vector<Character*>& characters,
        const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
        GameManager& gameManager,
        Effect& effect,
        SoundEffect& soundEffect,
        Player& player);
    virtual ~Skeleton();
    
    void Update() override;
};
