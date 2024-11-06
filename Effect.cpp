#include "Effect.h"
#include "Camera.h"
#include "DxLib.h"

Effect::Effect(Camera& camera) : camera(camera) {
    // �R���X�g���N�^
    
    // �G�t�F�N�g�X�v���C�g�摜�̓ǂݍ��݂ƃT�C�Y�A�X�v���C�g����ݒ�
    // �Ō��̃G�t�F�N�g
    hittingEffect1.sprite = LoadGraph("resource/effect/�Ō��G�t�F�N�g1_S80.png");
    hittingEffect1.size = 80;
    hittingEffect1.numSprites = 6;
    hittingEffect1.correctionValuePosX = 24;
    hittingEffect1.correctionValuePosY = 24;
}

Effect::~Effect() {
    // �f�X�g���N�^
    // �ǂݍ��񂾃X�v���C�g�摜�̉��
    DeleteGraph(hittingEffect1.sprite);
}

void Effect::AddEffect(int posX, int posY) {
    // �G�t�F�N�g����\������z��ɒǉ�����֐�
    EffectInfo effectInfo = hittingEffect1;
    effectInfo.posX = posX - effectInfo.correctionValuePosX;
    effectInfo.posY = posY - effectInfo.correctionValuePosY;
    effects.push_back(effectInfo);
}

void Effect::Draw() {
    // �폜����G�t�F�N�g�̃C���f�b�N�X���i�[����z��
    std::vector<int> effectToRemove;

    // �G�t�F�N�g��`��
    for (int i = 0; i < effects.size(); ++i) {
        auto& effect = effects[i];
        DrawRectGraph(
            effect.posX - camera.GetOffsetX(),
            effect.posY - camera.GetOffsetY(),
            effect.size * effect.spriteIndex, 0,
            effect.size, effect.size,
            effect.sprite, TRUE, FALSE);

        // �^�C�}�[���C���N�������g
        effect.animationTimer++;
        if (effect.animationTimer >= effect.animationInterval) {
            // �X�v���C�g�摜�̃C���f�b�N�X���X�V
            effect.spriteIndex++;
            effect.animationTimer = 0;
            if (effect.spriteIndex >= effect.numSprites) {
                // �X�v���C�g�摜�̃C���f�b�N�X���ő�l�ɒB�����ꍇ�A�폜�z��ɒǉ�
                effectToRemove.push_back(i);
            }
        }
    }

    // effectToRemove�ɓo�^���ꂽ�C���f�b�N�X�̃G�t�F�N�g��z�񂩂�폜
    for (int index : effectToRemove) {
        // �C���f�b�N�X���͈͓��ɂ��邩�m�F
        if (index >= 0 && index < effects.size()) {
            // �C���f�b�N�X�ɑΉ�����G�t�F�N�g���폜
            effects.erase(effects.begin() + index);
        }
    }
}