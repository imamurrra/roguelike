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

    // �v���C���[�̗̑̓Q�[�W��`��
    void DrawPlayerHealthBar(int currentHealth, int maxHealth);
    // �G�̗̑̓Q�[�W�̕`��
    void DrawEnemyHealthBar(int posX, int posY, int currentHealth, int maxHealth);
    // �s�����O��\��
    void DrawActionLog(std::vector<ActionLogEntry>& actionLog);

private:
    // �t�H���g
    int customFontHandle1; // �c��̗͗p�̃t�H���g
    int customFontHandle2; // �s�����O�p�̃t�H���g
    int customFontHandle3; // �s�����O�p�̃t�H���g


    int hpSatietyLevelFrame; // �̗͂Ɩ����x�̃Q�[�W�g
    int hpGaugeHandle; // �̗̓Q�[�W
    int satietyLevelGaugeHandle; // �����x�Q�[�W
};

