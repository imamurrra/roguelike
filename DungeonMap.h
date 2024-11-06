#pragma once

#include "Camera.h"
#include "DungeonRoom.h"
#include <vector>

enum DungeonMapTile {
    FLOOR,
    WALL,
};

class DungeonMap
{
public:
    DungeonMap(Camera& camera);
    ~DungeonMap();

    static constexpr int MAP_WIDTH = 35;
    static constexpr int MAP_HEIGHT = 30;
    //static constexpr int MAP_WIDTH = 35;
    //static constexpr int MAP_HEIGHT = 25;
    static constexpr int ROOM_MAX_SIZE = 9;
    static constexpr int ROOM_MIN_SIZE = 4;
    static constexpr int MAX_ROOMS = 9;
    static constexpr int MIN_ROOMS = 3;

    int GetPlayerInitPositionX() const { return playerInitPositionX; } // プレイヤー用初期位置座標を返す
    int GetPlayerInitPositionY() const { return playerInitPositionY; } // プレイヤー用初期位置座標を返す
    // 敵の生成位置を返す
    std::pair<int, int> GetEnemySpawnPosition(int playerX, int playerY) const;
    bool DungeonRoomCollides(const DungeonRoom& dungeonRoom) const; // 小部屋同士の衝突判定
    void AddDungeonRoom(const DungeonRoom& dungeonRoom); // マップに部屋を追加
    void ConnectDungeonRooms();
    void Draw();
    void GenerateRooms(); // ダンジョンマップ内の小部屋を生成
    // ダンジョンマップデータへの参照を返す const メソッド
    const int(&GetMapData() const)[MAP_WIDTH][MAP_HEIGHT]{
        return dungeonMapData;
    }

private:
    int wallImage;   // 壁の画像
    int pathImage;   // 通路の画像
    int dungeonMapData[MAP_WIDTH][MAP_HEIGHT];
    int playerInitPositionX;
    int playerInitPositionY;
    int minDistanceOtherRoom = 2; // 部屋同士との最低距離
    std::vector<DungeonRoom> dungeonRooms;

    Camera& camera;
};

// 列挙体を公開
enum DungeonMapTile;