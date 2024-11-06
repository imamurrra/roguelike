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
    
    // Skeleton�ŗL�̏�����
    image = LoadGraph("resource/character/enemy/skeleton.png");
    characterName = "�X�P���g��";
}

Skeleton::~Skeleton() {
    // Skeleton�ŗL�̔j������������΂����ɒǉ�
}

void Skeleton::Update() {
    // Enemy�̊�{�I��Update���g��
    Enemy::Update();
}