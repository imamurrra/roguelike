#pragma once
#include "Character.h"
#include "DungeonMap.h"
#include <vector>

class Player;

// 列挙型として定義したNode
struct Node {
    int x;
    int y;
    int g; // 開始地点からの移動コスト
    int h; // ゴールまでの推定移動コスト
    Node* parent; // 親ノードへのポインタ

    Node(int x, int y, int g, int h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    // f値を計算する関数
    int f() const { return g + h; }
};

// オープンリストの優先度比較用の関数オブジェクト
struct CompareNodePtr {
    bool operator()(Node* a, Node* b) const {
        return a->f() > b->f();
    }
};

// 敵の状態
enum EnemyMode {
    RandomMovement, // ランダム移動状態
    PlayerDetection, // プレイヤーを発見状態
};

class Enemy : public Character {
public:
    Enemy(int startX, int startY,
        Camera& camera, std::vector<Character*>& characters,
        const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
        GameManager& gameManager,
        Effect& effect,
        SoundEffect& soundEffect,
        Player& player);
    virtual ~Enemy(); // 純粋仮想デストラクタのオーバーライド
    void Update(); // 敵の行動を更新


private:
    EnemyMode currentEnemyMode;
    Player& player;

    int ManhattanDistance(int x1, int y1, int x2, int y2);
    bool IsSearchPlayer(int playerX, int playerY, int searchDistance, int searchRange); // プレイヤーの捜査
};