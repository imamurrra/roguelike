#include "SoundEffect.h"
#include "DxLib.h"

// ���ʉ��������N���X

SoundEffect::SoundEffect() {
    // �R���X�g���N�^

    // �Ō��U���q�b�g��
    attackHitEffect1 = LoadSoundMem("resource/soundEffect/�Ō��q�b�g1.mp3");

    // �U����U�艹
    attackMissEffect1 = LoadSoundMem("resource/soundEffect/�U����U��1.mp3");
}

SoundEffect::~SoundEffect() {
    // �f�X�g���N�^
    // �ǂݍ��񂾌��ʉ��̉��
    DeleteSoundMem(attackHitEffect1);
    DeleteSoundMem(attackMissEffect1);
}

void SoundEffect::PlaySoundEffect(SoundEffectType soundEffectType) {
    // ���ʉ��̍Đ�
    switch (soundEffectType) {
    case AttackHit:
        PlaySoundMem(attackHitEffect1, DX_PLAYTYPE_BACK);
        break;
    case AttackMiss:
        PlaySoundMem(attackMissEffect1, DX_PLAYTYPE_BACK);
        break;
        // ���̌��ʉ��ɑ΂��鏈����ǉ�����ꍇ�͂����ɒǉ����܂�
    default:
        // �f�t�H���g�̌��ʉ����Đ�����Ȃǂ̏���
        break;
    }
}
