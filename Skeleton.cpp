#include "Skeleton.h"
#include "DxLib.h"

Skeleton::Skeleton(int startX, int startY,
    Camera& camera, std::vector<Character*>& characters,
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
    GameManager& gameManager,
    Effect& effect,
    SoundEffect& soundEffect,
    Player& player)
    : Enemy(startX, startY, camera, characters, mapData, gameManager, effect, soundEffect, player) {
    
    // Skeleton固有の初期化
    image = LoadGraph("resource/character/enemy/skeleton.png");
    characterName = "スケルトン";
}

Skeleton::~Skeleton() {
    // Skeleton固有の破棄処理があればここに追加
}

void Skeleton::Update() {
    // Enemyの基本的なUpdateを使う
    Enemy::Update();
}