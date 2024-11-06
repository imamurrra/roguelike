#pragma once
#include "Camera.h"
#include "Character.h"
#include "DungeonMap.h"

class Player : public Character {
public:
    Player(int startX, int startY,
        Camera& camera, std::vector<Character*>& characters,
        const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
        GameManager& gameManager,
        Effect& effect,
        SoundEffect& soundEffect);
    ~Player();

    void Update();
    // ƒL[‚Ì“ü—Íˆ—
    void HandleInput();

};
