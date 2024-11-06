#pragma once

enum SoundEffectType {
    AttackHit, // �U���q�b�g��
    AttackMiss // �U����U�艹
};

class SoundEffect
{
public:
    SoundEffect();
    ~SoundEffect();

    void PlaySoundEffect(SoundEffectType soundEffectType);

private:
    int attackHitEffect1; // �Ō��U���q�b�g���̉�
    int attackMissEffect1; // �U����U�莞�̉�
};

