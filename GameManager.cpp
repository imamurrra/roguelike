#include "Camera.h"
#include "GameManager.h"
#include "DungeonMap.h"
#include "Player.h"
#include "UI.h"

// 敵のクラス
#include "Skeleton.h"

#include "DxLib.h"
#include <vector>

GameManager::GameManager()
    : camera(nullptr), dungeonMap(nullptr), player(nullptr), characters(), effect(nullptr), soundEffect(nullptr) {
    actionLogScrollValue = 0;
}

GameManager::~GameManager() {
    // インスタンスを解放
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void GameManager::CreateEnemy(int startX, int startY, const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT]) {
    // 新しい敵のインスタンスを生成し、リストに追加する
    Skeleton* skeleton = new Skeleton(startX, startY, *camera, characters, mapData, *this, *effect, *soundEffect, *player);
    enemies.push_back(skeleton);
    // キャラクターリストに追加
    characters.push_back(skeleton);
}

void GameManager::UpdateCharacters() {
    
    // 敵キャラの生存確認
    // 削除するキャラクターのインデックスを格納するベクター
    bool isDyingEnemy = false;
    std::vector<int> charactersToRemove;
    for (int i = 0; i < characters.size(); ++i) {
        auto character = characters[i];
        if (character->GetCharacterType() == CharacterType::ENEMY &&
            character->GetSurvivalStatus() == CharacterSurvivalStatus::Dead) {
            // 倒れている場合は削除する
            charactersToRemove.push_back(i);
        }
        else if (character->GetCharacterType() == CharacterType::ENEMY &&
            character->GetSurvivalStatus() == CharacterSurvivalStatus::Dying) {
            isDyingEnemy = true; // 倒れかけ状態の敵キャラがいる
        }
    }
    // 削除するキャラクターをcharactersから削除する
    for (int i = (int)charactersToRemove.size() - 1; i >= 0; --i) {
        characters.erase(characters.begin() + charactersToRemove[i]);
    }

    allCharactersActionFinished = true;
    // プレイヤーの更新
    if (player->CanTakeTurnAction() || player->GetCurrentState() != &Character::Idle) {
        player->Update();
        allCharactersActionFinished = false;
    }

    if (player->CanTakeTurnAction() || isDyingEnemy) {
        // プレイヤーの行動が終わっていない or 倒れかけの敵キャラがいる場合以降のキャラは更新しない
        return;
    }


    for (auto character : characters) {
        // 敵キャラの更新
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
    
    // 全キャラ行動を終えた場合、行動回数をリセット
    if (allCharactersActionFinished) {
        player->resetTurnActionCount();
        for (auto character : characters) {
            character->resetTurnActionCount();
        }
    }
}

void GameManager::DrawCharacters() {
    // プレイヤー以外のキャラの表示
    for (auto character : characters) {
        if (character->GetCharacterType() != CharacterType::PLAYER) {
            character->Draw();
        }
  
    }

    // 敵の体力バーの描画
    for (auto character : characters) {
        if (character->GetCharacterType() == CharacterType::ENEMY) {
            ui.DrawEnemyHealthBar(character->GetPosX() - camera->GetOffsetX(), character->GetPosY() - camera->GetOffsetY(),
                character->GetCurrentHealth(), character->GetMaxHealth());
        }
    }

    // プレイヤーの表示
    player->Draw();

    // プレイヤーの体力バーの描画
    ui.DrawPlayerHealthBar(player->GetCurrentHealth(), player->GetMaxHealth());

    // 行動ログの表示
    ui.DrawActionLog(actionLog);
}

void GameManager::HandleInput() {
    // shiftキーが押されているかチェック
    if (CheckHitKey(KEY_INPUT_LSHIFT) != 0 or CheckHitKey(KEY_INPUT_RSHIFT) != 0) {
        doubleSpeedFlag = true;  // Shiftキーが押されている間、doubleSpeedFlagをtrueに(倍速状態を有効)
    }
    else {
        doubleSpeedFlag = false; // Shiftキーが離されたらfalseに
    }
}