#include "Enemy.h"
#include "Camera.h"
#include "DungeonMap.h"
#include "DxLib.h"
#include "Player.h"
#include <vector>
#include <queue>
#include <utility>
#include <cmath>

Enemy::Enemy(int startX, int startY,
    Camera& camera, std::vector<Character*>& characters,
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
    GameManager& gameManager,
    Effect& effect,
    SoundEffect& soundEffect,
    Player& player) :
    Character(startX, startY, camera, characters, mapData, gameManager, effect, soundEffect), player(player) {
    // 敵の初期化処理
    image = LoadGraph("resource/character/enemy/enemy1.png");

    characterType = CharacterType::ENEMY;
    // turnActionCountLimit = 3; // 一ターンの行動回数

    maxHealth = 15; // 最大体力
    currentHealth = 15; // 現在体力

    currentEnemyMode = EnemyMode::RandomMovement; // ランダム移動状態
    //currentEnemyMode = EnemyMode::PlayerDetection; // プレイヤー追跡状態
    characterName = "敵キャラ";
}

Enemy::~Enemy() {
    // 敵の破棄処理
    DeleteGraph(image);
}

void Enemy::Update() {
    // 敵の行動を更新する処理をここに追加

    // プレイヤーの位置
    int playerX = player.GetMapPosX();
    int playerY = player.GetMapPosY();

    // 攻撃or移動を行う処理
    // 攻撃 > プレイヤーの追跡判定 > 移動 の順で処理する
    if (currentState == &Character::Idle) {
        // プレイヤーに攻撃が当たる距離か
        if ((playerX == mapPosX && std::abs(playerY - mapPosY) == 1) ||
            (playerY == mapPosY && std::abs(playerX - mapPosX) == 1)) {

            // プレイヤーと敵との距離
            int playerEnemyDistanceX = playerX - mapPosX;
            int playerEnemyDistanceY = playerY - mapPosY;

            // 向きを設定
            if (playerEnemyDistanceX > 0) {
                direction = RIGHT;
            }
            else if (playerEnemyDistanceX < 0) {
                direction = LEFT;
            }
            else if (playerEnemyDistanceY > 0) {
                direction = DOWN;
            }
            else if (playerEnemyDistanceY < 0) {
                direction = UP;
            }
            currentState = &Character::NormalAttack; // 攻撃を行う
        }
        else {
            // 移動処理
            int dx = 0;
            int dy = 0;

            // 上、下、左、右の順で移動する場合
            int dxs[] = { 0, 0, -1, 1 };
            int dys[] = { -1, 1, 0, 0 };

            // ランダム移動状態の場合
            if (currentEnemyMode == EnemyMode::RandomMovement) {
                // 敵の前方と周囲マスを調べてプレイヤーが存在するか確認
                int searchDistance = 8;
                int searchRange = 2;
                if (IsSearchPlayer(playerX, playerY, searchDistance, searchRange)) {
                    // プレイヤーを見つけたのなら追跡状態に更新
                    currentEnemyMode = EnemyMode::PlayerDetection;
                }
            }

            if (currentState == &Character::Idle) {
                // プレイヤーを目標として追跡する必要があるかどうかを判断
                if (currentEnemyMode == EnemyMode::PlayerDetection) {
                    // プレイヤーを追跡する処理

                    // オープンリストとクローズドリストの初期化
                    std::priority_queue < Node*, std::vector<Node*>, CompareNodePtr> openSet;
                    std::vector<std::vector<bool>> closedSet(DungeonMap::MAP_WIDTH, std::vector<bool>(DungeonMap::MAP_HEIGHT, false));

                    // スタートノードを生成してオープンリストに追加
                    Node* startNode = new Node(mapPosX, mapPosY, 0, ManhattanDistance(mapPosX, mapPosY, playerX, playerY));
                    openSet.push(startNode);

                    // 探索結果の経路
                    std::vector<Node> path;

                    while (!openSet.empty()) {
                        // オープンリストからf値が最小のノードを取り出す
                        Node* current = openSet.top();
                        openSet.pop();

                        if (current->x == playerX && current->y == playerY) {
                            while (current != nullptr) {
                                path.push_back(*current);
                                current = current->parent;
                            }
                            std::reverse(path.begin(), path.end());
                            int nextMapPosX = path[1].x;
                            int nextMapPosY = path[1].y;
                            // プレイヤーに到達したら移動方向を決定
                            dx = nextMapPosX - mapPosX > 0 ? 1 : (nextMapPosX - mapPosX < 0 ? -1 : 0);
                            dy = nextMapPosY - mapPosY > 0 ? 1 : (nextMapPosY - mapPosY < 0 ? -1 : 0);
                            break;
                        }

                        closedSet[current->x][current->y] = true;

                        // 4方向の移動を考慮
                        for (int i = 0; i < 4; ++i) {
                            int nx = current->x + dxs[i];
                            int ny = current->y + dys[i];

                            // マップの範囲内かつ通行可能かつ未訪問かどうかを確認
                            if (nx >= 0 && nx < DungeonMap::MAP_WIDTH && ny >= 0 && ny < DungeonMap::MAP_HEIGHT &&
                                mapData[nx][ny] != DungeonMapTile::WALL && !closedSet[nx][ny]) {

                                // 移動コストの計算
                                int ng = current->g + 1;

                                if (!(playerX == nx && playerY == ny) && IsCharacterCollision(nx, ny)) {
                                    // 別キャラがいる場合移動コストを加算
                                    ng += 5;
                                }

                                // オープンリストに追加
                                Node* nextNode = new Node(nx, ny, ng, ManhattanDistance(nx, ny, playerX, playerY), current);
                                openSet.push(nextNode);
                            }
                        }
                    }
                }
                else {
                    // ランダムに移動する
                    if (rand() % 2 == 0) {
                        dx = rand() % 3 - 1; // -1, 0, 1 のいずれかを選択
                    }
                    else {
                        dy = rand() % 3 - 1; // -1, 0, 1 のいずれかを選択
                    }
                }
                // currentState を適切な移動方向に設定
                if (dx == -1) {
                    currentState = &Character::MoveLeft;
                }
                else if (dx == 1) {
                    currentState = &Character::MoveRight;
                }
                else if (dy == -1) {
                    currentState = &Character::MoveUp;
                }
                else if (dy == 1) {
                    currentState = &Character::MoveDown;
                }
                else {
                    turnActionCount++; // 一ターンに行動した回数を加算
                    currentState = &Character::Idle; // 移動しない場合は Idle 状態に戻す
                }
            }
        }
    }



    (this->*currentState)();
}

int Enemy::ManhattanDistance(int x1, int y1, int x2, int y2) {
    // マンハッタン距離を返す(水平方向の距離と垂直方向の距離の合計)
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool Enemy::IsSearchPlayer(int playerX, int playerY, int searchDistance, int searchRange) {
    // プレイヤーを探す

    // 前方のsearchDistanceマス以内にプレイヤーが存在するか確認
    int dx = 0;
    int dy = 0;
    switch (direction) {
    case DOWN:
        dy = 1;
        break;
    case LEFT:
        dx = -1;
        break;
    case RIGHT:
        dx = 1;
        break;
    case UP:
        dy = -1;
        break;
    }

    for (int i = 1; i <= searchDistance; ++i) {
        int checkX = mapPosX + i * dx;
        int checkY = mapPosY + i * dy;

        // 前方が壁ならfalseを返す
        if (mapData[checkX][checkY] == DungeonMapTile::WALL) {
            break;
        }

        // プレイヤーが存在する場合trueを返す
        if (checkX == playerX && checkY == playerY) {
            return true;
        }
    }
    
    // 周囲マスsearchRangeの範囲を調べてプレイヤーが存在するか確認
    for (int i = -searchRange; i <= searchRange; ++i) {
        for (int j = -searchRange; j <= searchRange; ++j) {
            // 自分自身の位置は調べない
            if (i == 0 && j == 0) {
                continue;
            }

            int checkX = mapPosX + i;
            int checkY = mapPosY + j;

            // マップの範囲内かつプレイヤーが存在する場合trueを返す
            if (checkX >= 0 && checkX < DungeonMap::MAP_WIDTH && checkY >= 0 && checkY < DungeonMap::MAP_HEIGHT &&
                checkX == playerX && checkY == playerY) {
                return true;
            }
        }
    }

    // プレイヤーが見つからなかった場合はfalseを返す
    return false;
}
