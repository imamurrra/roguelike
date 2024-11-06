#include "DungeonMap.h"
#include "DungeonRoom.h"
#include "DxLib.h"
#include "GameManager.h"
#include <algorithm>
#include <cmath>

DungeonMap::DungeonMap(Camera& camera) : camera(camera) {
    // 壁の画像を読み込む
    wallImage = LoadGraph("resource/map/wall.png");
    // 通路の画像を読み込む
    pathImage = LoadGraph("resource/map/path.png");

    // マップデータの初期化
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            dungeonMapData[x][y] = DungeonMapTile::WALL; // 壁を敷き詰める
        }
    }

    GenerateRooms();
}

DungeonMap::~DungeonMap() {
    // 読み込んだ画像を解放する
    DeleteGraph(wallImage);
    DeleteGraph(pathImage);
}

// 敵の生成位置を返すメソッドの実装
std::pair<int, int> DungeonMap::GetEnemySpawnPosition(int playerX, int playerY) const {
    // プレイヤーのいる小部屋を探す
    const DungeonRoom* playerRoom = nullptr;
    for (const auto& room : dungeonRooms) {
        if (room.ContainsPoint(playerX, playerY)) {
            playerRoom = &room;
            break;
        }
    }

    // プレイヤーのいる小部屋と異なるランダムな小部屋を選ぶ
    const DungeonRoom* enemyRoom = nullptr;
    do {
        int randomIndex = rand() % dungeonRooms.size();
        enemyRoom = &dungeonRooms[randomIndex];
    } while (enemyRoom == playerRoom);

    // ランダムな小部屋内のランダムな座標を選ぶ
    int enemyX = enemyRoom->x + rand() % enemyRoom->width;
    int enemyY = enemyRoom->y + rand() % enemyRoom->height;

    return std::make_pair(enemyX, enemyY);
}

bool DungeonMap::DungeonRoomCollides(const DungeonRoom& dungeonRoom) const {
    for (const DungeonRoom& other : dungeonRooms) {
        if (dungeonRoom.CollidesWith(other, minDistanceOtherRoom)) {
            return true;
        }
    }
    return false;
}

void DungeonMap::AddDungeonRoom(const DungeonRoom& dungeonRoom) {
    for (int x = dungeonRoom.x; x < dungeonRoom.x + dungeonRoom.width; ++x) {
        for (int y = dungeonRoom.y; y < dungeonRoom.y + dungeonRoom.height; ++y) {
            dungeonMapData[x][y] = DungeonMapTile::FLOOR; // 通路を設定
        }
    }
}

void DungeonMap::ConnectDungeonRooms() {
    for (size_t i = 0; i < dungeonRooms.size() - 1; i++) {
        const DungeonRoom& room1 = dungeonRooms[i];
        const DungeonRoom& room2 = dungeonRooms[i + 1];

        // room1のランダムな座標を取得
        int randX1 = room1.x + rand() % room1.width;
        int randY1 = room1.y + rand() % room1.height;

        // room2のランダムな座標を取得
        int randX2 = room2.x + rand() % room2.width;
        int randY2 = room2.y + rand() % room2.height;

        // room1からroom2への通路を作成する
        int currX = randX1;
        int currY = randY1;

        // 乱数によってギザギザな通路を作成するかどうかを決定する
        bool zigzagPath = (rand() % 10 == 0);

        while (currX != randX2 || currY != randY2) {
            // 次の座標に向かう方向を決定する
            int deltaX = (randX2 > currX) ? 1 : (randX2 < currX) ? -1 : 0;
            int deltaY = (randY2 > currY) ? 1 : (randY2 < currY) ? -1 : 0;

            if (zigzagPath) {
                // ギザギザな通路にする
                // 次の座標へ移動する（X方向とY方向を別々に移動）
                currX += deltaX;
                dungeonMapData[currX][currY] = DungeonMapTile::FLOOR;
                currY += deltaY;
            }
            else if (currX != randX2) {
                // X方向を更新する
                currX += deltaX;
            }
            else {
                // Y方向を更新する
                currY += deltaY;
            }

            // 通路を作成する
            dungeonMapData[currX][currY] = DungeonMapTile::FLOOR;
        }
    }
}

void DungeonMap::GenerateRooms() {
    int generateLimitTrialNum = 30; // 部屋の作成試行回数の上限 無限ループ対策用
    int generateNum = rand() % (MAX_ROOMS - MIN_ROOMS + 1) + MIN_ROOMS;

    while (dungeonRooms.size() < generateNum) {
        int width = rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1) + ROOM_MIN_SIZE + 1;
        int height = rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1) + ROOM_MIN_SIZE + 1;
        int x = rand() % (MAP_WIDTH - width - 1) + 1;
        int y = rand() % (MAP_HEIGHT - height - 1) + 1;
        DungeonRoom newDungeonRoom(x, y, width, height);

        // 重なりチェック
        if (!DungeonRoomCollides(newDungeonRoom)) {
            AddDungeonRoom(newDungeonRoom);
            dungeonRooms.push_back(newDungeonRoom);
            ++generateNum;
        }

        // プレイヤー用初期位置に最初の部屋の中央値を保存
        const DungeonRoom& firstRoom = dungeonRooms[0];
        playerInitPositionX = firstRoom.x + firstRoom.width / 2;
        playerInitPositionY = firstRoom.y + firstRoom.height / 2;

        generateLimitTrialNum--;
        if (generateLimitTrialNum <= 0) {
            break;
        }
    }

    ConnectDungeonRooms();
}

void DungeonMap::Draw() {
    const int tileSize = GameManager::TILE_SIZE;
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            // 壁の場合
            if (dungeonMapData[x][y] == DungeonMapTile::WALL) {
                DrawGraph((x * tileSize) - camera.GetOffsetX(), (y * tileSize) - camera.GetOffsetY(), wallImage, TRUE);
            }
            // 通路の場合
            else {
                DrawGraph((x * tileSize) - camera.GetOffsetX(), (y * tileSize) - camera.GetOffsetY(), pathImage, TRUE);
            }
        }
    }
}