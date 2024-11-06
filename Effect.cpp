#include "Effect.h"
#include "Camera.h"
#include "DxLib.h"

Effect::Effect(Camera& camera) : camera(camera) {
    // コンストラクタ
    
    // エフェクトスプライト画像の読み込みとサイズ、スプライト数を設定
    // 打撃のエフェクト
    hittingEffect1.sprite = LoadGraph("resource/effect/打撃エフェクト1_S80.png");
    hittingEffect1.size = 80;
    hittingEffect1.numSprites = 6;
    hittingEffect1.correctionValuePosX = 24;
    hittingEffect1.correctionValuePosY = 24;
}

Effect::~Effect() {
    // デストラクタ
    // 読み込んだスプライト画像の解放
    DeleteGraph(hittingEffect1.sprite);
}

void Effect::AddEffect(int posX, int posY) {
    // エフェクト情報を表示する配列に追加する関数
    EffectInfo effectInfo = hittingEffect1;
    effectInfo.posX = posX - effectInfo.correctionValuePosX;
    effectInfo.posY = posY - effectInfo.correctionValuePosY;
    effects.push_back(effectInfo);
}

void Effect::Draw() {
    // 削除するエフェクトのインデックスを格納する配列
    std::vector<int> effectToRemove;

    // エフェクトを描画
    for (int i = 0; i < effects.size(); ++i) {
        auto& effect = effects[i];
        DrawRectGraph(
            effect.posX - camera.GetOffsetX(),
            effect.posY - camera.GetOffsetY(),
            effect.size * effect.spriteIndex, 0,
            effect.size, effect.size,
            effect.sprite, TRUE, FALSE);

        // タイマーをインクリメント
        effect.animationTimer++;
        if (effect.animationTimer >= effect.animationInterval) {
            // スプライト画像のインデックスを更新
            effect.spriteIndex++;
            effect.animationTimer = 0;
            if (effect.spriteIndex >= effect.numSprites) {
                // スプライト画像のインデックスが最大値に達した場合、削除配列に追加
                effectToRemove.push_back(i);
            }
        }
    }

    // effectToRemoveに登録されたインデックスのエフェクトを配列から削除
    for (int index : effectToRemove) {
        // インデックスが範囲内にあるか確認
        if (index >= 0 && index < effects.size()) {
            // インデックスに対応するエフェクトを削除
            effects.erase(effects.begin() + index);
        }
    }
}