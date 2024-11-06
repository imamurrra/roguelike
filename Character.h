#pragma once

class GameManager;
class Effect;
class SoundEffect;

#include "Camera.h"
#include "DungeonMap.h"
#include <vector>
#include <string>

// 向いている方向
enum Direction {
    DOWN = 0,
    LEFT = 1,
    RIGHT = 2,
    UP = 3,
};

// キャラクターのタイプ
enum CharacterType {
    PLAYER,
    ENEMY,
};

// キャラクターのステータス
enum CharacterSurvivalStatus {
    Alive, // 生存状態
    Dying, // 倒れかけ
    Dead, // 倒された
};

const int MOVEING_POS_SPEED = 2; // 移動速度
const int ATTACK_WAIT_TIME = 10; // 攻撃判定発生後の待機時間
const int DYING_WAITING_TIME = 30; // 体力が0になった時、消えるまで点滅する時間

class Character {
public:
    Character(int startX, int startY,
        Camera& camera, std::vector<Character*>& characters,
        const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
        GameManager& gameManager,
        Effect& effect,
        SoundEffect& soundEffect);
    virtual ~Character(); // 純粋仮想デストラクタ

    void Draw();
    virtual void Update();
    virtual void Move(int dx, int dy);

    // キャラの行動関数
    void Idle(); // 何もしない状態
    void NormalAttack(); //　通常攻撃処理
    void Moving();
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    void UpdateAnimation();
    bool IsWall(int x, int y) const;
    bool IsCharacterCollision(int x, int y) const;
    bool IsInMapRange(int x, int y) const;
    bool CheckMoveValidity(int x, int y) const;

    // Get関数
    int GetMapPosX() const { return mapPosX; }
    int GetMapPosY() const { return mapPosY; }
    int GetPosX() const { return posX; }
    int GetPosY() const { return posY; }
    int GetCurrentHealth() const { return currentHealth; }
    int GetMaxHealth() const { return maxHealth; }
    Direction GetDirection() const { return direction; }
    bool CanTakeTurnAction() const { // キャラが行動可能かを返す
        return turnActionCount < turnActionCountLimit;
    }
    void (Character::* GetCurrentState())() {
        return currentState;
    }
    CharacterType GetCharacterType() const {
        return characterType;
    }
    CharacterSurvivalStatus GetSurvivalStatus() const { return survivalStatus; }
    std::string GetCharacterName() const {
        return characterName;
    }

    void resetTurnActionCount() { turnActionCount = 0; } // 一ターンに行動した回数のリセット
    void SetCurrentState(void(Character::* state)()) {
        currentState = state;
    }
    void SetDirection(Direction newDirection) { direction = newDirection; }
    void ReceiveDamage(int receiveDamage); // ダメージを受ける


protected:
    int mapPosX; // 二次元配列マップ上のキャラの座標
    int mapPosY; // 二次元配列マップ上のキャラの座標
    int posX; // 現在の描画位置
    int posY; // 現在の描画位置
    int targetPosX; // 目標の描画位置
    int targetPosY; // 目標の描画位置
    int image; // キャラクターの画像
    int spriteIndex = 0; // スプライト画像のインデックス
    int animationTimer = 0; // アニメーションのタイマー
    int spriteIncrement = 1;
    int turnActionCountLimit = 1; // 一ターンに行動出来る回数
    int turnActionCount = 0; // 実際に一ターンに行動した回数
    Direction direction = DOWN; // 向いている方向
    int currentHealth; // 現在の体力(残り体力)
    int maxHealth; // 最大体力
    bool isAttackReady; // 攻撃許可
    bool isAttackEnd; // 攻撃終了
    int attackWaitTime; // 攻撃時の待機時間
    CharacterSurvivalStatus survivalStatus; // キャラクターの状態
    int dyingWaitingTime; // 倒れるまでの経過時間
    int power; // 攻撃力
    std::string characterName; // キャラクターの名前


    Camera& camera;
    Effect& effect;
    GameManager& gameManager;
    SoundEffect& soundEffect;
    std::vector<Character*>& characters;
    void(Character::* currentState)();
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT];
    CharacterType characterType;

private:
    bool MoveToTarget(int targetX, int targetY); // 目標地点まで移動。目標に着いていたらtrueを返す
    void ResetAttackWaitTime() {
        attackWaitTime = ATTACK_WAIT_TIME;
    }
};
