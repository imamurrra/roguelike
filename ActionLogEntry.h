#pragma once
#include <string>

// �A�N�V�������O�̓��e
struct ActionLogEntry {
    std::string message; // ���b�Z�[�W
    int posY; // ���O��\�������ʂ�Y���W

    ActionLogEntry(const std::string& message, int posY)
        : message(message), posY(posY) {}
};