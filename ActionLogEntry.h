#pragma once
#include <string>

// アクションログの内容
struct ActionLogEntry {
    std::string message; // メッセージ
    int posY; // ログを表示する画面のY座標

    ActionLogEntry(const std::string& message, int posY)
        : message(message), posY(posY) {}
};