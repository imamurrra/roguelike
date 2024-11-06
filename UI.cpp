#include "UI.h"
#include "DxLib.h"

#include <iostream>
#include <vector>
#include <string>

UI::UI() {
    // フォントの読み込み
    // 残り体力用のフォント
    customFontHandle1 = LoadFontDataToHandle("resource/font/源柔ゴシック_Bold_S24.dft", 1);

    // 行動ログ用のフォント
    customFontHandle2 = LoadFontDataToHandle("resource/font/源柔ゴシック_Bold_S16.dft", 1);
    customFontHandle3 = LoadFontDataToHandle("resource/font/源柔ゴシック_Bold_S22.dft", 1);

    // 体力と満腹度の枠画像の読み込み
    hpSatietyLevelFrame = LoadGraph("resource/ui/hp_satiety_level_frame.png");
    // 体力ゲージと満腹度ゲージ画像の読み込み
    hpGaugeHandle = LoadGraph("resource/ui/hp_gauge.png");
    satietyLevelGaugeHandle = LoadGraph("resource/ui/satiety_level_gauge.png");
}

UI::~UI() {
    // 読み込んだフォントの開放
    DeleteFontToHandle(customFontHandle1);

    // 読み込んだ画像の開放
    DeleteGraph(hpSatietyLevelFrame);
    DeleteGraph(hpGaugeHandle);
    DeleteGraph(satietyLevelGaugeHandle);
}

void UI::DrawPlayerHealthBar(int currentHealth, int maxHealth) {

    // ゲージの枠を表示
    DrawRectGraph(0, 0,
        0, 0, 186, 78,
        hpSatietyLevelFrame, TRUE, FALSE);

    // 体力のゲージの最大横幅
    const int maxGaugeWidth = 123;

    // 体力のゲージの長さを計算
    const int currentGaugeWidth = maxGaugeWidth * currentHealth / maxHealth;

    // 体力のゲージを表示
    DrawRectGraph(50, 14,
        0, 0, currentGaugeWidth, 78, // ゲージの長さを変更
        hpGaugeHandle, TRUE, FALSE);

    // 満腹度ゲージを表示
    DrawRectGraph(50, 47,
        0, 0, 186, 78,
        satietyLevelGaugeHandle, TRUE, FALSE);

    // "残り体力/最大体力"を表示
    char healthText[64];
    sprintf_s(healthText, "%d/%d", currentHealth, maxHealth);
    DrawStringToHandle(140, 20, healthText, GetColor(0, 255, 255), customFontHandle1);
}

void UI::DrawEnemyHealthBar(int posX, int posY, int currentHealth, int maxHealth) {
    const int healthBarWidth = 30; // 体力ゲージの幅
    const int healthBarHeight = 5; // 体力ゲージの高さ
    const int borderSize = 1; // 境界線のサイズ

    // 体力ゲージの背景を描画（黒色）
    DrawBox(posX - borderSize, posY - borderSize,
        posX + healthBarWidth + borderSize, posY + healthBarHeight + borderSize, GetColor(0, 0, 0), TRUE);

    // 体力ゲージの減少分を描画（赤色）
    DrawBox(posX, posY, posX + healthBarWidth, posY + healthBarHeight, GetColor(255, 0, 0), TRUE);

    // 体力ゲージを描画（緑色）
    float healthPercentage = static_cast<float>(currentHealth) / maxHealth; // 現在の体力の割合
    int healthBarFillWidth = static_cast<int>(healthPercentage * healthBarWidth); // 体力ゲージの幅を計算
    DrawBox(posX, posY, posX + healthBarFillWidth, posY + healthBarHeight, GetColor(0, 255, 0), TRUE);
}

// 行動ログの描画 古いログは画面上にスライドさせる
void UI::DrawActionLog(std::vector<ActionLogEntry>& actionLog) {
    const int baseY = 460; // 最下部のY座標
    const int lineSpacing = 20; // 各メッセージの間隔

    for (size_t i = 0; i < actionLog.size(); ++i) {
        // 目標のY座標を計算
        int targetY = baseY - static_cast<int>(i) * lineSpacing;

        // 現在のY座標が目標Y座標に達していない場合、Y座標を少しずつ目標に近づける
        if (actionLog[actionLog.size() - 1 - i].posY > targetY) {
            actionLog[actionLog.size() - 1 - i].posY--;  // Y座標を減少
        }

        // メッセージを描画
        DrawStringToHandle(10, actionLog[actionLog.size() - 1 - i].posY,
            actionLog[actionLog.size() - 1 - i].message.c_str(),
            GetColor(200, 200, 200), customFontHandle2);
    }
}
