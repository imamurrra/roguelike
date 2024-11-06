#pragma once

#include <vector>

class Camera;

// �G�t�F�N�g�����܂Ƃ߂�\����
struct EffectInfo {
    int sprite = -1;                // �X�v���C�g�摜�̃n���h��
    int posX = 0;                   // x���W
    int posY = 0;                   // y���W
    int size = 0;                   // �`��T�C�Y 
    int spriteIndex = 0;            // �X�v���C�g�摜�̃C���f�b�N�X
    int animationTimer = 0;         // �A�j���[�V�����̃^�C�}�[
    int numSprites = 0;             // �X�v���C�g��
    int animationInterval = 4;      // �A�j���[�V�����̍X�V�Ԋu
    int correctionValuePosX = 0;    // �G�t�F�N�g�̕`��ʒu�̕␳�l
    int correctionValuePosY = 0;    // �G�t�F�N�g�̕`��ʒu�̕␳�l
};

class Effect
{
public:
    Effect(Camera& camera);
    ~Effect();

    void Draw();
    // �G�t�F�N�g����ǉ�����֐�
    void AddEffect(int posX, int posY);


private:
    Camera& camera;
    EffectInfo hittingEffect1; // �Ō��̃G�t�F�N�g

    std::vector<EffectInfo> effects;

};
