#include "DungeonMap.h"
#include "DungeonRoom.h"
#include "DxLib.h"
#include "GameManager.h"
#include <algorithm>
#include <cmath>

DungeonMap::DungeonMap(Camera& camera) : camera(camera) {
    // �ǂ̉摜��ǂݍ���
    wallImage = LoadGraph("resource/map/wall.png");
    // �ʘH�̉摜��ǂݍ���
    pathImage = LoadGraph("resource/map/path.png");

    // �}�b�v�f�[�^�̏�����
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            dungeonMapData[x][y] = DungeonMapTile::WALL; // �ǂ�~���l�߂�
        }
    }

    GenerateRooms();
}

DungeonMap::~DungeonMap() {
    // �ǂݍ��񂾉摜���������
    DeleteGraph(wallImage);
    DeleteGraph(pathImage);
}

// �G�̐����ʒu��Ԃ����\�b�h�̎���
std::pair<int, int> DungeonMap::GetEnemySpawnPosition(int playerX, int playerY) const {
    // �v���C���[�̂��鏬������T��
    const DungeonRoom* playerRoom = nullptr;
    for (const auto& room : dungeonRooms) {
        if (room.ContainsPoint(playerX, playerY)) {
            playerRoom = &room;
            break;
        }
    }

    // �v���C���[�̂��鏬�����ƈقȂ郉���_���ȏ�������I��
    const DungeonRoom* enemyRoom = nullptr;
    do {
        int randomIndex = rand() % dungeonRooms.size();
        enemyRoom = &dungeonRooms[randomIndex];
    } while (enemyRoom == playerRoom);

    // �����_���ȏ��������̃����_���ȍ��W��I��
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
            dungeonMapData[x][y] = DungeonMapTile::FLOOR; // �ʘH��ݒ�
        }
    }
}

void DungeonMap::ConnectDungeonRooms() {
    for (size_t i = 0; i < dungeonRooms.size() - 1; i++) {
        const DungeonRoom& room1 = dungeonRooms[i];
        const DungeonRoom& room2 = dungeonRooms[i + 1];

        // room1�̃����_���ȍ��W���擾
        int randX1 = room1.x + rand() % room1.width;
        int randY1 = room1.y + rand() % room1.height;

        // room2�̃����_���ȍ��W���擾
        int randX2 = room2.x + rand() % room2.width;
        int randY2 = room2.y + rand() % room2.height;

        // room1����room2�ւ̒ʘH���쐬����
        int currX = randX1;
        int currY = randY1;

        // �����ɂ���ăM�U�M�U�ȒʘH���쐬���邩�ǂ��������肷��
        bool zigzagPath = (rand() % 10 == 0);

        while (currX != randX2 || currY != randY2) {
            // ���̍��W�Ɍ��������������肷��
            int deltaX = (randX2 > currX) ? 1 : (randX2 < currX) ? -1 : 0;
            int deltaY = (randY2 > currY) ? 1 : (randY2 < currY) ? -1 : 0;

            if (zigzagPath) {
                // �M�U�M�U�ȒʘH�ɂ���
                // ���̍��W�ֈړ�����iX������Y������ʁX�Ɉړ��j
                currX += deltaX;
                dungeonMapData[currX][currY] = DungeonMapTile::FLOOR;
                currY += deltaY;
            }
            else if (currX != randX2) {
                // X�������X�V����
                currX += deltaX;
            }
            else {
                // Y�������X�V����
                currY += deltaY;
            }

            // �ʘH���쐬����
            dungeonMapData[currX][currY] = DungeonMapTile::FLOOR;
        }
    }
}

void DungeonMap::GenerateRooms() {
    int generateLimitTrialNum = 30; // �����̍쐬���s�񐔂̏�� �������[�v�΍��p
    int generateNum = rand() % (MAX_ROOMS - MIN_ROOMS + 1) + MIN_ROOMS;

    while (dungeonRooms.size() < generateNum) {
        int width = rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1) + ROOM_MIN_SIZE + 1;
        int height = rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1) + ROOM_MIN_SIZE + 1;
        int x = rand() % (MAP_WIDTH - width - 1) + 1;
        int y = rand() % (MAP_HEIGHT - height - 1) + 1;
        DungeonRoom newDungeonRoom(x, y, width, height);

        // �d�Ȃ�`�F�b�N
        if (!DungeonRoomCollides(newDungeonRoom)) {
            AddDungeonRoom(newDungeonRoom);
            dungeonRooms.push_back(newDungeonRoom);
            ++generateNum;
        }

        // �v���C���[�p�����ʒu�ɍŏ��̕����̒����l��ۑ�
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
            // �ǂ̏ꍇ
            if (dungeonMapData[x][y] == DungeonMapTile::WALL) {
                DrawGraph((x * tileSize) - camera.GetOffsetX(), (y * tileSize) - camera.GetOffsetY(), wallImage, TRUE);
            }
            // �ʘH�̏ꍇ
            else {
                DrawGraph((x * tileSize) - camera.GetOffsetX(), (y * tileSize) - camera.GetOffsetY(), pathImage, TRUE);
            }
        }
    }
}