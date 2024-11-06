#include "Camera.h"
#include "GameManager.h"
#include "DungeonMap.h"
#include "Player.h"
#include "UI.h"

// �G�̃N���X
#include "Skeleton.h"

#include "DxLib.h"
#include <vector>

GameManager::GameManager()
    : camera(nullptr), dungeonMap(nullptr), player(nullptr), characters(), effect(nullptr), soundEffect(nullptr) {
    actionLogScrollValue = 0;
}

GameManager::~GameManager() {
    // �C���X�^���X�����
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void GameManager::CreateEnemy(int startX, int startY, const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT]) {
    // �V�����G�̃C���X�^���X�𐶐����A���X�g�ɒǉ�����
    Skeleton* skeleton = new Skeleton(startX, startY, *camera, characters, mapData, *this, *effect, *soundEffect, *player);
    enemies.push_back(skeleton);
    // �L�����N�^�[���X�g�ɒǉ�
    characters.push_back(skeleton);
}

void GameManager::UpdateCharacters() {
    
    // �G�L�����̐����m�F
    // �폜����L�����N�^�[�̃C���f�b�N�X���i�[����x�N�^�[
    bool isDyingEnemy = false;
    std::vector<int> charactersToRemove;
    for (int i = 0; i < characters.size(); ++i) {
        auto character = characters[i];
        if (character->GetCharacterType() == CharacterType::ENEMY &&
            character->GetSurvivalStatus() == CharacterSurvivalStatus::Dead) {
            // �|��Ă���ꍇ�͍폜����
            charactersToRemove.push_back(i);
        }
        else if (character->GetCharacterType() == CharacterType::ENEMY &&
            character->GetSurvivalStatus() == CharacterSurvivalStatus::Dying) {
            isDyingEnemy = true; // �|�ꂩ����Ԃ̓G�L����������
        }
    }
    // �폜����L�����N�^�[��characters����폜����
    for (int i = (int)charactersToRemove.size() - 1; i >= 0; --i) {
        characters.erase(characters.begin() + charactersToRemove[i]);
    }

    allCharactersActionFinished = true;
    // �v���C���[�̍X�V
    if (player->CanTakeTurnAction() || player->GetCurrentState() != &Character::Idle) {
        player->Update();
        allCharactersActionFinished = false;
    }

    if (player->CanTakeTurnAction() || isDyingEnemy) {
        // �v���C���[�̍s�����I����Ă��Ȃ� or �|�ꂩ���̓G�L����������ꍇ�ȍ~�̃L�����͍X�V���Ȃ�
        return;
    }


    for (auto character : characters) {
        // �G�L�����̍X�V
        if (character->GetCharacterType() == CharacterType::ENEMY) {
            if (character->CanTakeTurnAction() || character->GetCurrentState() != &Character::Idle) {
                character->Update();
                allCharactersActionFinished = false;
            }

            if (character->CanTakeTurnAction()) {
                break;
            }
        }
    }
    
    // �S�L�����s�����I�����ꍇ�A�s���񐔂����Z�b�g
    if (allCharactersActionFinished) {
        player->resetTurnActionCount();
        for (auto character : characters) {
            character->resetTurnActionCount();
        }
    }
}

void GameManager::DrawCharacters() {
    // �v���C���[�ȊO�̃L�����̕\��
    for (auto character : characters) {
        if (character->GetCharacterType() != CharacterType::PLAYER) {
            character->Draw();
        }
  
    }

    // �G�̗̑̓o�[�̕`��
    for (auto character : characters) {
        if (character->GetCharacterType() == CharacterType::ENEMY) {
            ui.DrawEnemyHealthBar(character->GetPosX() - camera->GetOffsetX(), character->GetPosY() - camera->GetOffsetY(),
                character->GetCurrentHealth(), character->GetMaxHealth());
        }
    }

    // �v���C���[�̕\��
    player->Draw();

    // �v���C���[�̗̑̓o�[�̕`��
    ui.DrawPlayerHealthBar(player->GetCurrentHealth(), player->GetMaxHealth());

    // �s�����O�̕\��
    ui.DrawActionLog(actionLog);
}

void GameManager::HandleInput() {
    // shift�L�[��������Ă��邩�`�F�b�N
    if (CheckHitKey(KEY_INPUT_LSHIFT) != 0 or CheckHitKey(KEY_INPUT_RSHIFT) != 0) {
        doubleSpeedFlag = true;  // Shift�L�[��������Ă���ԁAdoubleSpeedFlag��true��(�{����Ԃ�L��)
    }
    else {
        doubleSpeedFlag = false; // Shift�L�[�������ꂽ��false��
    }
}