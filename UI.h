#pragma once

#include "Camera.h"
#include "ActionLogEntry.h"

#include <iostream>
#include <vector>
#include <string>

class UI {
public:
    UI();
    ~UI();

    // プレイヤーの体力ゲージを描画
    void DrawPlayerHealthBar(int currentHealth, int maxHealth);
    // 敵の体力ゲージの描画
    void DrawEnemyHealthBar(int posX, int posY, int currentHealth, int maxHealth);
    // 行動ログを表示
    void DrawActionLog(std::vector<ActionLogEntry>& actionLog);

private:
    // フォント
    int customFontHandle1; // 残り体力用のフォント
    int customFontHandle2; // 行動ログ用のフォント
    int customFontHandle3; // 行動ログ用のフォント


    int hpSatietyLevelFrame; // 体力と満腹度のゲージ枠
    int hpGaugeHandle; // 体力ゲージ
    int satietyLevelGaugeHandle; // 満腹度ゲージ
};

