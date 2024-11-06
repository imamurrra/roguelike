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

    int GetPlayerInitPositionX() const { return playerInitPositionX; } // �v���C���[�p�����ʒu���W��Ԃ�
    int GetPlayerInitPositionY() const { return playerInitPositionY; } // �v���C���[�p�����ʒu���W��Ԃ�
    // �G�̐����ʒu��Ԃ�
    std::pair<int, int> GetEnemySpawnPosition(int playerX, int playerY) const;
    bool DungeonRoomCollides(const DungeonRoom& dungeonRoom) const; // ���������m�̏Փ˔���
    void AddDungeonRoom(const DungeonRoom& dungeonRoom); // �}�b�v�ɕ�����ǉ�
    void ConnectDungeonRooms();
    void Draw();
    void GenerateRooms(); // �_���W�����}�b�v���̏������𐶐�
    // �_���W�����}�b�v�f�[�^�ւ̎Q�Ƃ�Ԃ� const ���\�b�h
    const int(&GetMapData() const)[MAP_WIDTH][MAP_HEIGHT]{
        return dungeonMapData;
    }

private:
    int wallImage;   // �ǂ̉摜
    int pathImage;   // �ʘH�̉摜
    int dungeonMapData[MAP_WIDTH][MAP_HEIGHT];
    int playerInitPositionX;
    int playerInitPositionY;
    int minDistanceOtherRoom = 2; // �������m�Ƃ̍Œ዗��
    std::vector<DungeonRoom> dungeonRooms;

    Camera& camera;
};

// �񋓑̂����J
enum DungeonMapTile;