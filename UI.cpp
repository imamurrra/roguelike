#include "UI.h"
#include "DxLib.h"

#include <iostream>
#include <vector>
#include <string>

UI::UI() {
    // �t�H���g�̓ǂݍ���
    // �c��̗͗p�̃t�H���g
    customFontHandle1 = LoadFontDataToHandle("resource/font/���_�S�V�b�N_Bold_S24.dft", 1);

    // �s�����O�p�̃t�H���g
    customFontHandle2 = LoadFontDataToHandle("resource/font/���_�S�V�b�N_Bold_S16.dft", 1);
    customFontHandle3 = LoadFontDataToHandle("resource/font/���_�S�V�b�N_Bold_S22.dft", 1);

    // �̗͂Ɩ����x�̘g�摜�̓ǂݍ���
    hpSatietyLevelFrame = LoadGraph("resource/ui/hp_satiety_level_frame.png");
    // �̗̓Q�[�W�Ɩ����x�Q�[�W�摜�̓ǂݍ���
    hpGaugeHandle = LoadGraph("resource/ui/hp_gauge.png");
    satietyLevelGaugeHandle = LoadGraph("resource/ui/satiety_level_gauge.png");
}

UI::~UI() {
    // �ǂݍ��񂾃t�H���g�̊J��
    DeleteFontToHandle(customFontHandle1);

    // �ǂݍ��񂾉摜�̊J��
    DeleteGraph(hpSatietyLevelFrame);
    DeleteGraph(hpGaugeHandle);
    DeleteGraph(satietyLevelGaugeHandle);
}

void UI::DrawPlayerHealthBar(int currentHealth, int maxHealth) {

    // �Q�[�W�̘g��\��
    DrawRectGraph(0, 0,
        0, 0, 186, 78,
        hpSatietyLevelFrame, TRUE, FALSE);

    // �̗͂̃Q�[�W�̍ő剡��
    const int maxGaugeWidth = 123;

    // �̗͂̃Q�[�W�̒������v�Z
    const int currentGaugeWidth = maxGaugeWidth * currentHealth / maxHealth;

    // �̗͂̃Q�[�W��\��
    DrawRectGraph(50, 14,
        0, 0, currentGaugeWidth, 78, // �Q�[�W�̒�����ύX
        hpGaugeHandle, TRUE, FALSE);

    // �����x�Q�[�W��\��
    DrawRectGraph(50, 47,
        0, 0, 186, 78,
        satietyLevelGaugeHandle, TRUE, FALSE);

    // "�c��̗�/�ő�̗�"��\��
    char healthText[64];
    sprintf_s(healthText, "%d/%d", currentHealth, maxHealth);
    DrawStringToHandle(140, 20, healthText, GetColor(0, 255, 255), customFontHandle1);
}

void UI::DrawEnemyHealthBar(int posX, int posY, int currentHealth, int maxHealth) {
    const int healthBarWidth = 30; // �̗̓Q�[�W�̕�
    const int healthBarHeight = 5; // �̗̓Q�[�W�̍���
    const int borderSize = 1; // ���E���̃T�C�Y

    // �̗̓Q�[�W�̔w�i��`��i���F�j
    DrawBox(posX - borderSize, posY - borderSize,
        posX + healthBarWidth + borderSize, posY + healthBarHeight + borderSize, GetColor(0, 0, 0), TRUE);

    // �̗̓Q�[�W�̌�������`��i�ԐF�j
    DrawBox(posX, posY, posX + healthBarWidth, posY + healthBarHeight, GetColor(255, 0, 0), TRUE);

    // �̗̓Q�[�W��`��i�ΐF�j
    float healthPercentage = static_cast<float>(currentHealth) / maxHealth; // ���݂̗̑͂̊���
    int healthBarFillWidth = static_cast<int>(healthPercentage * healthBarWidth); // �̗̓Q�[�W�̕����v�Z
    DrawBox(posX, posY, posX + healthBarFillWidth, posY + healthBarHeight, GetColor(0, 255, 0), TRUE);
}

// �s�����O�̕`�� �Â����O�͉�ʏ�ɃX���C�h������
void UI::DrawActionLog(std::vector<ActionLogEntry>& actionLog) {
    const int baseY = 460; // �ŉ�����Y���W
    const int lineSpacing = 20; // �e���b�Z�[�W�̊Ԋu

    for (size_t i = 0; i < actionLog.size(); ++i) {
        // �ڕW��Y���W���v�Z
        int targetY = baseY - static_cast<int>(i) * lineSpacing;

        // ���݂�Y���W���ڕWY���W�ɒB���Ă��Ȃ��ꍇ�AY���W���������ڕW�ɋ߂Â���
        if (actionLog[actionLog.size() - 1 - i].posY > targetY) {
            actionLog[actionLog.size() - 1 - i].posY--;  // Y���W������
        }

        // ���b�Z�[�W��`��
        DrawStringToHandle(10, actionLog[actionLog.size() - 1 - i].posY,
            actionLog[actionLog.size() - 1 - i].message.c_str(),
            GetColor(200, 200, 200), customFontHandle2);
    }
}
