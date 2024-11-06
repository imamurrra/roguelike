#include "DxLib.h"
#include "Camera.h"
#include "DungeonMap.h"
#include "Effect.h"
#include "GameManager.h"
#include "Player.h"
#include "SoundEffect.h"
#include <iostream>
#include <vector>

int WINAPI main(HINSTANCE, HINSTANCE, LPSTR, int) {
    // DXライブラリ初期化処理
    if (DxLib_Init() == -1) {
        return -1; // 初期化失敗時はエラーコードを返して終了
    }

    // ウィンドウを作成
    const int width = 32 * 20 * 1; // 640
    const int height = 32 * 15 * 1; // 480
    SetGraphMode(width, height, 32); // ウィンドウサイズを指定
    ChangeWindowMode(TRUE); // ウィンドウモードにする
    SetMainWindowText("不思議のダンジョン"); // ウィンドウのタイトルを設定

    if (SetGraphMode(width, height, 32) != 0) {
        DxLib_End(); // DXライブラリ終了処理
        return -1;   // グラフィックモードの設定失敗時はエラーコードを返して終了
    }

    // ゲームマネージャのインスタンスを作成
    GameManager gameManager;

    // カメラのインスタンスを作成
    Camera camera(width, height);
    gameManager.SetCamera(camera);

    // エフェクトのインスタンスを作成
    Effect effect(camera);
    gameManager.SetEffect(effect);

    // 効果音のインスタンスを作成
    SoundEffect soundEffect;
    gameManager.SetSoundEffect(soundEffect);


    // ダンジョンマップクラスのインスタンスを作成
    srand(time(NULL)); // 擬似乱数のシードを設定
    DungeonMap dungeonMap(camera);
    const auto& dungeonMapData = dungeonMap.GetMapData();

    // プレイヤー
    Player* player; // Player クラスのポインターをメンバーとして宣言
    player = new Player(dungeonMap.GetPlayerInitPositionX(), dungeonMap.GetPlayerInitPositionY(),
        camera,
        gameManager.GetCharacters(),
        dungeonMap.GetMapData(),
        gameManager,
        effect,
        soundEffect);
    gameManager.SetPlayer(player); // ゲームマネージャにプレイヤーの情報を設定
    gameManager.SetCharacter(player);

    // 敵の生成
    std::pair<int, int> enemySpawnPosition = dungeonMap.GetEnemySpawnPosition(player->GetMapPosX(), player->GetMapPosY());
    gameManager.CreateEnemy(enemySpawnPosition.first, enemySpawnPosition.second, dungeonMap.GetMapData());
    enemySpawnPosition = dungeonMap.GetEnemySpawnPosition(player->GetMapPosX(), player->GetMapPosY());
    gameManager.CreateEnemy(enemySpawnPosition.first, enemySpawnPosition.second, dungeonMap.GetMapData());

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen(); // 画面をクリア

        // プレイヤーの描画位置を取得してカメラに設定
        // 攻撃時は無効にする
        if (player->GetCurrentState() != &Character::NormalAttack) {
            camera.SetPosition(player->GetPosX(), player->GetPosY());
        }


        // ダンジョンマップの描画
        dungeonMap.Draw();

        // キャラの描画
        gameManager.DrawCharacters();
        // キャラの更新
        gameManager.UpdateCharacters();

        // エフェクトの描画
        effect.Draw();

        // 行動ログのリセット
        gameManager.ResetActionLog();

        // ゲームマネージャ用のキー入力処理
        gameManager.HandleInput();


        ScreenFlip();  // 裏画面を表画面に反映
        WaitTimer(16); // フレームレート調整
    }

    // DXライブラリ終了処理
    DxLib_End();
    return 0;
}
