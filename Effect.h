#pragma once

#include <vector>

class Camera;

// エフェクト情報をまとめる構造体
struct EffectInfo {
    int sprite = -1;                // スプライト画像のハンドル
    int posX = 0;                   // x座標
    int posY = 0;                   // y座標
    int size = 0;                   // 描画サイズ 
    int spriteIndex = 0;            // スプライト画像のインデックス
    int animationTimer = 0;         // アニメーションのタイマー
    int numSprites = 0;             // スプライト数
    int animationInterval = 4;      // アニメーションの更新間隔
    int correctionValuePosX = 0;    // エフェクトの描画位置の補正値
    int correctionValuePosY = 0;    // エフェクトの描画位置の補正値
};

class Effect
{
public:
    Effect(Camera& camera);
    ~Effect();

    void Draw();
    // エフェクト情報を追加する関数
    void AddEffect(int posX, int posY);


private:
    Camera& camera;
    EffectInfo hittingEffect1; // 打撃のエフェクト

    std::vector<EffectInfo> effects;

};
