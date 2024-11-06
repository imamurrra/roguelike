#include "Player.h"
#include "Camera.h"
#include "DxLib.h"
#include "DungeonMap.h"
#include "GameManager.h"

Player::Player(int startX, int startY,
    Camera& camera,
    std::vector<Character*>& characters,
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
    GameManager& gameManager,
    Effect& effect,
    SoundEffect& soundEffect) :
    Character(startX, startY, camera, characters, mapData, gameManager, effect, soundEffect) {
    // プレイヤーの画像を読み込む
    image = LoadGraph("resource/character/player/player1.png");

    characterType = CharacterType::PLAYER;
    turnActionCountLimit = 1; // 一ターンの行動回数
    maxHealth = 20; // 最大体力
    currentHealth = 12; // 現在体力
    power = 5; // 攻撃力
    characterName = "プレイヤー";
}

Player::~Player() {
    // プレイヤーの画像を解放する
    DeleteGraph(image);
}

void Player::Update() {

    HandleInput();

    (this->*currentState)();
}

void Player::HandleInput() {
    static bool keyPressed = false; // キーが押されているかどうかを示すフラグ
    bool isDoubleSpeed = gameManager.GetDoubleSpeedFlag(); // 倍速状態か


    if ((!keyPressed || isDoubleSpeed) && currentState == &Character::Idle) {
        // キーが押された場合にプレイヤーの移動処理を行う
        // 移動できなくても向きは変える
        if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)) {
            // 移動が有効かどうかをチェック
            if (CheckMoveValidity(mapPosX, mapPosY - 1)) {
                currentState = &Character::MoveUp;
            }
            direction = UP; // 移動出来なくても向きは変える 上を押したら上向きに設定
            keyPressed = !isDoubleSpeed;  // 倍速状態の場合、keyPressed をリセットしない
        } else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)) {
            // 移動が有効かどうかをチェック
            if (CheckMoveValidity(mapPosX, mapPosY + 1)) {
                currentState = &Character::MoveDown;
            }
            direction = DOWN; // 下を押したら下向きに設定
            keyPressed = !isDoubleSpeed;
        } else if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A)) {
            // 移動が有効かどうかをチェック
            if (CheckMoveValidity(mapPosX - 1, mapPosY)) {
                currentState = &Character::MoveLeft;
            }
            direction = LEFT; // 左を押したら左向きに設定
            keyPressed = !isDoubleSpeed;
        }else if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D)) {
            // 移動が有効かどうかをチェック
            if (CheckMoveValidity(mapPosX + 1, mapPosY)) {
                currentState = &Character::MoveRight;
            }
            direction = RIGHT; // 右を押したら右向きに設定
            keyPressed = !isDoubleSpeed;
        } else if (CheckHitKey(KEY_INPUT_RETURN) || CheckHitKey(KEY_INPUT_P)) {
            // Enter キーまたは P キーが押されたら通常攻撃状態にする
            currentState = &Character::NormalAttack;
            keyPressed = true;
        }
    }

    // キーが離されたら移動キー入力フラグをリセット
    if (!CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN) &&
        !CheckHitKey(KEY_INPUT_LEFT) && !CheckHitKey(KEY_INPUT_RIGHT) &&
        !CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S) &&
        !CheckHitKey(KEY_INPUT_A) && !CheckHitKey(KEY_INPUT_D) &&
        !CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_P)) {
        keyPressed = false;
    }
}
