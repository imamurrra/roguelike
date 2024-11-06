#pragma once


class Camera;
class Effect;
class Player;
class SoundEffect;

#include "ActionLogEntry.h"
#include "Character.h"
#include "DungeonMap.h"
#include "Enemy.h"
#include "UI.h"

#include <functional>
#include <vector>
#include <string>
#include <chrono> // タイマー処理に必要なライブラリ



class GameManager {
public:
    GameManager();
    ~GameManager();

    Camera& GetCamera() const { return *camera; }
    std::vector<Character*>& GetCharacters() { return characters; }
    DungeonMap& GetDungeonMap() const { return *dungeonMap; }
    void CreateEnemy(int startX, int startY, const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT]);
    void DrawCharacters();
    void HandleInput();
    void UpdateCharacters();
    void SetEffect(Effect& e) {
        effect = &e;
    }
    void SetCamera(Camera& c) {
        camera = &c;
    }
    void SetDungeonMap(DungeonMap& dm) {
        dungeonMap = &dm;
    }
    void SetSoundEffect(SoundEffect& s) {
        soundEffect = &s;
    }

    void SetCharacter(Character* c) {
        characters.push_back(c);
    }
    void SetPlayer(Player* p) {
        player = p;
    }
    bool GetDoubleSpeedFlag() {
        return doubleSpeedFlag;
    }
    // 行動ログにメッセージを追加
    void AddToActionLog(const std::string& message) {
        const int baseY = 460; // 初期のY座標

        // 6件を超える場合、先頭のメッセージを削除
        if (actionLog.size() >= 6) {
            actionLog.erase(actionLog.begin());
        }

        // 新しいメッセージを追加
        actionLog.push_back({ message, baseY });

        lastLogTime = std::chrono::steady_clock::now(); // ログタイマーをリセット
    }
    // 行動ログの内容をリセット
    void ResetActionLog() {
        if (!actionLog.empty()) {
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastLogTime);

            // 最後のメッセージから5秒経過した場合、ログをリセット
            if (duration.count() >= 5) {
                actionLog.clear();
            }
        }
    }

    static constexpr int TILE_SIZE = 32;


private:
    Camera* camera;
    DungeonMap* dungeonMap;
    Effect* effect;
    Player* player;
    SoundEffect* soundEffect;
    std::vector<Character*> characters;
    std::vector<Enemy*> enemies;
    std::vector<ActionLogEntry> actionLog; // 行動ログを管理,ログメッセージと表示する座標を保持する
    int actionLogScrollValue; // 行動ログメッセージのスクロール値
    std::chrono::steady_clock::time_point lastLogTime = std::chrono::steady_clock::now(); // ログをリセットするまでのタイマー

    bool allCharactersActionFinished = true; // 全キャラが行動を終えたかのフラグ
    bool doubleSpeedFlag = false;
    UI ui;
};