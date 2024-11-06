#pragma once

enum SoundEffectType {
    AttackHit, // UŒ‚ƒqƒbƒg‰¹
    AttackMiss // UŒ‚‹óU‚è‰¹
};

class SoundEffect
{
public:
    SoundEffect();
    ~SoundEffect();

    void PlaySoundEffect(SoundEffectType soundEffectType);

private:
    int attackHitEffect1; // ‘ÅŒ‚UŒ‚ƒqƒbƒg‚Ì‰¹
    int attackMissEffect1; // UŒ‚‹óU‚è‚Ì‰¹
};

