#include "Character.h"
#include "DungeonMap.h"
#include "DxLib.h"
#include "Effect.h"
#include "GameManager.h"
#include "SoundEffect.h"
#include <algorithm>
#include <vector>
#include<string>

Character::Character(int startX, int startY,
    Camera& camera, std::vector<Character*>& characters,
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
    GameManager& gameManager,
    Effect& effect,
    SoundEffect& soundEffect) :
    mapPosX(startX), mapPosY(startY), camera(camera), characters(characters),
    mapData(mapData), gameManager(gameManager), effect(effect), soundEffect(soundEffect) {
    image = NULL;
    spriteIndex = 0;
    animationTimer = 0;
    posX = targetPosX = mapPosX * GameManager::TILE_SIZE;
    posY = targetPosY = mapPosY * GameManager::TILE_SIZE;
    currentState = &Character::Idle;
    characterType = CharacterType::ENEMY;
    currentHealth = maxHealth = 10;
    isAttackReady = false;
    isAttackEnd = false;
    attackWaitTime = ATTACK_WAIT_TIME;
    survivalStatus = CharacterSurvivalStatus::Alive;
    dyingWaitingTime = DYING_WAITING_TIME;
    power = 1; // 攻撃力
}

Character::~Character() {
    // デストラクタ
}

void Character::Draw() {
    // キャラクターの画像を描画する
    const int tileSize = GameManager::TILE_SIZE;

    // 倒れかけ状態なら点滅してキャラを表示する
    if (survivalStatus == CharacterSurvivalStatus::Dying) {
        dyingWaitingTime--;
        if (dyingWaitingTime <= 0) {
            survivalStatus = CharacterSurvivalStatus::Dead; // 倒れた状態に更新 TODO:Draw()で更新するのは変だなと思うけどとりあえず
        }

        if (dyingWaitingTime % 2 == 0) {
            return;
        }
    }

    DrawRectGraph(
        posX - camera.GetOffsetX(),
        posY - camera.GetOffsetY(),
        tileSize * spriteIndex, direction * tileSize,
        tileSize, tileSize, image, TRUE, FALSE);

    // アニメーションの更新
    if (survivalStatus == CharacterSurvivalStatus::Alive) {
        UpdateAnimation();
    }


    if (characterType == CharacterType::PLAYER) {
        char intStrX[32];
        sprintf_s(intStrX, "%d", mapPosX); // 整数を文字列に変換
        // テキストを画面に表示
        DrawString(100, 100, intStrX, GetColor(255, 255, 255));

        char intStrY[32];
        sprintf_s(intStrY, "%d", mapPosY); // 整数を文字列に変換
        // テキストを画面に表示
        DrawString(100, 120, intStrY, GetColor(255, 255, 255));

        char intStrdrawTargetPosX[32]; // 整数を格納するための文字列バッファ
        sprintf_s(intStrdrawTargetPosX, "%d", targetPosX); // 整数を文字列に変換
        // テキストを画面に表示
        DrawString(100, 140, intStrdrawTargetPosX, GetColor(255, 255, 255));

        char intStrdrawTargetPosY[32]; // 整数を格納するための文字列バッファ
        sprintf_s(intStrdrawTargetPosY, "%d", targetPosY); // 整数を文字列に変換
        // テキストを画面に表示
        DrawString(100, 160, intStrdrawTargetPosY, GetColor(255, 255, 255));

        char intStrdrawPosX[32]; // 整数を格納するための文字列バッファ
        sprintf_s(intStrdrawPosX, "%d", posX); // 整数を文字列に変換
        // テキストを画面に表示
        DrawString(100, 180, intStrdrawPosX, GetColor(255, 255, 255));

        char intStrdrawPosY[32]; // 整数を格納するための文字列バッファ
        sprintf_s(intStrdrawPosY, "%d", posY); // 整数を文字列に変換
        // テキストを画面に表示
        DrawString(100, 200, intStrdrawPosY, GetColor(255, 255, 255));

        std::string muki = "";
        switch (direction) {
        case UP:
            muki = "UP";
            break;
        case DOWN:
            muki = "DOWN";
            break;
        case LEFT:
            muki = "LEFT";
            break;
        case RIGHT:
            muki = "RIGHT";
            break;
        }
        DrawString(100, 220, muki.c_str(), GetColor(255, 255, 255));

        std::string status = "";
        if (currentState == &Character::Idle) {
            status = "待機中";
        } else if (currentState == &Character::Moving) {
            status = "移動中";
        }
        else if (currentState == &Character::NormalAttack) {
            status = "通常攻撃中";
        }
        DrawString(100, 240, status.c_str(), GetColor(255, 255, 255));

        char intStrdrawPower[32]; // 整数を格納するための文字列バッファ
        sprintf_s(intStrdrawPower, "攻撃力:%d", power); // 整数を文字列に変換
        // テキストを画面に表示
        DrawString(100, 260, intStrdrawPower, GetColor(255, 255, 255));
    }
}

void Character::Update() {
    (this->*currentState)();
}

void Character::Idle() {
    // 何もしない状態 待機状態
}

void Character::NormalAttack() {
    // 通常攻撃 目の前1マス先を攻撃 
    int targetX = mapPosX; // 攻撃対象のX座標
    int targetY = mapPosY; // 攻撃対象のY座標

    // キャラが踏み込む距離
    int steppingDistance = GameManager::TILE_SIZE / 2;
    
    // 本来の位置
    int originalPosX = mapPosX * GameManager::TILE_SIZE;
    int originalPosY = mapPosY * GameManager::TILE_SIZE;

    // 踏み込む目標座標
    int steppingTargetPosX = originalPosX;
    int steppingTargetPosY = originalPosY;

    // posX or posYを踏み込む目標座標までインクリメント or デクリメントさせる
    int moveX = 0;
    int moveY = 0;
    if (!isAttackReady) {
        // キャラクターが向いている方向に応じて、攻撃対象の座標を計算する
        switch (direction) {
        case UP:
            targetY -= 1;
            steppingTargetPosY -= steppingDistance;
            break;
        case DOWN:
            targetY += 1;
            steppingTargetPosY += steppingDistance;
            break;
        case LEFT:
            targetX -= 1;
            steppingTargetPosX -= steppingDistance;
            break;
        case RIGHT:
            targetX += 1;
            steppingTargetPosX += steppingDistance;
            break;
        }

        // 踏み込み移動を終えたら攻撃フラグを有効にする
        if (MoveToTarget(steppingTargetPosX, steppingTargetPosY)) {
            isAttackReady = true;
        }
    }

    // 攻撃先に敵が存在するかどうかをチェックする
    if (isAttackReady && !isAttackEnd) {
        bool isEnemyInRange = false;
        Character* targetCharacter = nullptr;
        for (Character* character : characters) {
            // プレイヤーは敵に、敵はプレイヤーの場合だけ攻撃がヒットする
            if (character->GetCharacterType() != characterType && character->GetMapPosX() == targetX && character->GetMapPosY() == targetY) {
                isEnemyInRange = true;
                targetCharacter = character;
                break;
            }
        }

        if (isEnemyInRange && targetCharacter != nullptr) {
            // 攻撃対象にダメージを与える
            targetCharacter->ReceiveDamage(power);
            
            // 行動ログに「AはBにnダメージ与えた」の内容でログを追加
            std::string logMessage = characterName + "は" + targetCharacter->characterName + "に" + std::to_string(power) + "ダメージ与えた";
            gameManager.AddToActionLog(logMessage);

            // 攻撃ヒット音とエフェクトを再生
            effect.AddEffect(targetCharacter->GetPosX(), targetCharacter->GetPosY());
            soundEffect.PlaySoundEffect(SoundEffectType::AttackHit);
        }
        else {
            // 攻撃空振り音を再生
            soundEffect.PlaySoundEffect(SoundEffectType::AttackMiss);
        }
        isAttackEnd = true;


    }

    // 数秒待機
    if (attackWaitTime > 0) {
        attackWaitTime--;
        return;
    }

    if (isAttackReady && isAttackEnd) {
        // 元の位置に戻る
        // 元の位置に戻ったら待機状態に戻す
        if (MoveToTarget(originalPosX, originalPosY)) {
            currentState = &Character::Idle;
            // フラグもリセット
            isAttackReady = false;
            isAttackEnd = false;
            ResetAttackWaitTime();
            turnActionCount++; // 一ターンに行動した回数を加算
        }
    }

}
 
void Character::MoveUp() {
    Move(0, -1);
}

void Character::MoveDown() {
    Move(0, 1);
}

void Character::MoveLeft() {
    Move(-1, 0);
}

void Character::MoveRight() {
    Move(1, 0);
}

void Character::Move(int dx, int dy) {
    int newX = mapPosX + dx;
    int newY = mapPosY + dy;

    // 移動が有効かどうかをチェック
    if (CheckMoveValidity(newX, newY)) {
        // 移動
        mapPosX = newX;
        mapPosY = newY;

        targetPosX = mapPosX * GameManager::TILE_SIZE;
        targetPosY = mapPosY * GameManager::TILE_SIZE;

        // 向きを設定
        if (dx > 0) {
            direction = RIGHT;
        }
        else if (dx < 0) {
            direction = LEFT;
        }
        else if (dy > 0) {
            direction = DOWN;
        }
        else if (dy < 0) {
            direction = UP;
        }

        // 移動中状態に更新
        currentState = &Character::Moving;
    }
    else {
        // 無効な移動の場合は待機状態にするなどの処理を行う
        currentState = &Character::Idle;
    }

    turnActionCount++; // 一ターンに行動した回数を加算
}

void Character::Moving() {
    int moveX = 0;
    int moveY = 0;

    int moveingPosSpeed = MOVEING_POS_SPEED;
    // 倍速状態の処理
    if (gameManager.GetDoubleSpeedFlag()) {
        moveingPosSpeed *= 2;
    }

    // 実際の表示座標と目標表示座標に差があればその差をmoveingPosSpeedの分だけ埋める
    moveX = (targetPosX - posX > 0) ? (std::min)(moveingPosSpeed, targetPosX - posX) :
        (std::max)(-moveingPosSpeed, targetPosX - posX);
    moveY = (targetPosY - posY > 0) ? (std::min)(moveingPosSpeed, targetPosY - posY) :
        (std::max)(-moveingPosSpeed, targetPosY - posY);

    // 現在の位置から移動量を加えて新しい位置を計算
    posX += moveX;
    posY += moveY;

    // 表示位置を補正
    if (abs(posX - targetPosX) <= moveingPosSpeed) {
        posX = targetPosX;
    }
    if (abs(posY - targetPosY) <= moveingPosSpeed) {
        posY = targetPosY;
    }

    // 移動を終えたら待機状態に戻す
    if (posX == targetPosX && posY == targetPosY) {
        currentState = &Character::Idle;
    }
}

void Character::UpdateAnimation() {
    // アニメーションの更新間隔
    const int animationInterval = 20;
    const int numSprites = 3;

    // タイマーをインクリメント
    animationTimer++;
    if (gameManager.GetDoubleSpeedFlag()) { // 倍速状態か
        animationTimer++; // 倍速でアニメーション処理
    }

    if (animationTimer >= animationInterval) {
        // スプライト画像のインデックスを更新
        spriteIndex += spriteIncrement;
        if (spriteIndex == numSprites - 1 || spriteIndex == 0) {
            spriteIncrement = -spriteIncrement; // 方向を逆にする
        }
        animationTimer = 0;
    }
}

// 移動先が壁かどうかをチェックする関数
bool Character::IsWall(int x, int y) const {
    return mapData[x][y] != DungeonMapTile::FLOOR;
}

// 移動先に他のキャラクターがいないかどうかをチェックする関数
bool Character::IsCharacterCollision(int x, int y) const {
    for (const Character* character : characters) {
        if (character != this && character->GetMapPosX() == x && character->GetMapPosY() == y) {
            return true; // 衝突あり
        }
    }
    return false; // 衝突なし
}

// 移動先がマップの範囲内かどうかをチェックする関数
bool Character::IsInMapRange(int x, int y) const {
    return x >= 0 && x < DungeonMap::MAP_WIDTH && y >= 0 && y < DungeonMap::MAP_HEIGHT;
}

// 移動が有効かどうかをチェックする関数
bool Character::CheckMoveValidity(int x, int y) const {
    // 移動先がマップの範囲外の場合は無効
    if (!IsInMapRange(x, y)) {
        return false;
    }

    // 移動先が壁の場合は無効
    if (IsWall(x, y)) {
        return false;
    }

    // 移動先に他のキャラクターがいる場合は無効
    if (IsCharacterCollision(x, y)) {
        return false;
    }

    // 上記の条件をすべて満たす場合は有効
    return true;
}

bool Character::MoveToTarget(int targetX, int targetY) {
    // 目標座標までの移動量を計算
    int moveingPosSpeed = MOVEING_POS_SPEED;
    if (gameManager.GetDoubleSpeedFlag()) { // 倍速状態か
        moveingPosSpeed *= 2;
    }

    int moveX = (targetX - posX > 0) ? (std::min)(moveingPosSpeed, targetX - posX) :
        (std::max)(-moveingPosSpeed, targetX - posX);
    int moveY = (targetY - posY > 0) ? (std::min)(moveingPosSpeed, targetY - posY) :
        (std::max)(-moveingPosSpeed, targetY - posY);

    // 現在の位置から移動量を加えて新しい位置を計算
    posX += moveX;
    posY += moveY;

    // 表示位置を補正
    if (abs(posX - targetX) <= moveingPosSpeed) {
        posX = targetX;
    }
    if (abs(posY - targetY) <= moveingPosSpeed) {
        posY = targetY;
    }

    // 移動を終えたらtrueを返す
    return (posX == targetX && posY == targetY);
}

void Character::ReceiveDamage(int receiveDamage) {
    // ダメージを受ける
    currentHealth -= receiveDamage;
    if (currentHealth <= 0) {
        currentHealth = 0;
        // 倒れかけの状態に更新
        survivalStatus = CharacterSurvivalStatus::Dying;
    }
}