#include "SoundEffect.h"
#include "DxLib.h"

// 効果音を扱うクラス

SoundEffect::SoundEffect() {
    // コンストラクタ

    // 打撃攻撃ヒット音
    attackHitEffect1 = LoadSoundMem("resource/soundEffect/打撃ヒット1.mp3");

    // 攻撃空振り音
    attackMissEffect1 = LoadSoundMem("resource/soundEffect/攻撃空振り1.mp3");
}

SoundEffect::~SoundEffect() {
    // デストラクタ
    // 読み込んだ効果音の解放
    DeleteSoundMem(attackHitEffect1);
    DeleteSoundMem(attackMissEffect1);
}

void SoundEffect::PlaySoundEffect(SoundEffectType soundEffectType) {
    // 効果音の再生
    switch (soundEffectType) {
    case AttackHit:
        PlaySoundMem(attackHitEffect1, DX_PLAYTYPE_BACK);
        break;
    case AttackMiss:
        PlaySoundMem(attackMissEffect1, DX_PLAYTYPE_BACK);
        break;
        // 他の効果音に対する処理を追加する場合はここに追加します
    default:
        // デフォルトの効果音を再生するなどの処理
        break;
    }
}
