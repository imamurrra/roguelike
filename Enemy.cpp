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
    // �G�̏���������
    image = LoadGraph("resource/character/enemy/enemy1.png");

    characterType = CharacterType::ENEMY;
    // turnActionCountLimit = 3; // ��^�[���̍s����

    maxHealth = 15; // �ő�̗�
    currentHealth = 15; // ���ݑ̗�

    currentEnemyMode = EnemyMode::RandomMovement; // �����_���ړ����
    //currentEnemyMode = EnemyMode::PlayerDetection; // �v���C���[�ǐՏ��
    characterName = "�G�L����";
}

Enemy::~Enemy() {
    // �G�̔j������
    DeleteGraph(image);
}

void Enemy::Update() {
    // �G�̍s�����X�V���鏈���������ɒǉ�

    // �v���C���[�̈ʒu
    int playerX = player.GetMapPosX();
    int playerY = player.GetMapPosY();

    // �U��or�ړ����s������
    // �U�� > �v���C���[�̒ǐՔ��� > �ړ� �̏��ŏ�������
    if (currentState == &Character::Idle) {
        // �v���C���[�ɍU���������鋗����
        if ((playerX == mapPosX && std::abs(playerY - mapPosY) == 1) ||
            (playerY == mapPosY && std::abs(playerX - mapPosX) == 1)) {

            // �v���C���[�ƓG�Ƃ̋���
            int playerEnemyDistanceX = playerX - mapPosX;
            int playerEnemyDistanceY = playerY - mapPosY;

            // ������ݒ�
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
            currentState = &Character::NormalAttack; // �U�����s��
        }
        else {
            // �ړ�����
            int dx = 0;
            int dy = 0;

            // ��A���A���A�E�̏��ňړ�����ꍇ
            int dxs[] = { 0, 0, -1, 1 };
            int dys[] = { -1, 1, 0, 0 };

            // �����_���ړ���Ԃ̏ꍇ
            if (currentEnemyMode == EnemyMode::RandomMovement) {
                // �G�̑O���Ǝ��̓}�X�𒲂ׂăv���C���[�����݂��邩�m�F
                int searchDistance = 8;
                int searchRange = 2;
                if (IsSearchPlayer(playerX, playerY, searchDistance, searchRange)) {
                    // �v���C���[���������̂Ȃ�ǐՏ�ԂɍX�V
                    currentEnemyMode = EnemyMode::PlayerDetection;
                }
            }

            if (currentState == &Character::Idle) {
                // �v���C���[��ڕW�Ƃ��ĒǐՂ���K�v�����邩�ǂ����𔻒f
                if (currentEnemyMode == EnemyMode::PlayerDetection) {
                    // �v���C���[��ǐՂ��鏈��

                    // �I�[�v�����X�g�ƃN���[�Y�h���X�g�̏�����
                    std::priority_queue < Node*, std::vector<Node*>, CompareNodePtr> openSet;
                    std::vector<std::vector<bool>> closedSet(DungeonMap::MAP_WIDTH, std::vector<bool>(DungeonMap::MAP_HEIGHT, false));

                    // �X�^�[�g�m�[�h�𐶐����ăI�[�v�����X�g�ɒǉ�
                    Node* startNode = new Node(mapPosX, mapPosY, 0, ManhattanDistance(mapPosX, mapPosY, playerX, playerY));
                    openSet.push(startNode);

                    // �T�����ʂ̌o�H
                    std::vector<Node> path;

                    while (!openSet.empty()) {
                        // �I�[�v�����X�g����f�l���ŏ��̃m�[�h�����o��
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
                            // �v���C���[�ɓ��B������ړ�����������
                            dx = nextMapPosX - mapPosX > 0 ? 1 : (nextMapPosX - mapPosX < 0 ? -1 : 0);
                            dy = nextMapPosY - mapPosY > 0 ? 1 : (nextMapPosY - mapPosY < 0 ? -1 : 0);
                            break;
                        }

                        closedSet[current->x][current->y] = true;

                        // 4�����̈ړ����l��
                        for (int i = 0; i < 4; ++i) {
                            int nx = current->x + dxs[i];
                            int ny = current->y + dys[i];

                            // �}�b�v�͈͓̔����ʍs�\�����K�₩�ǂ������m�F
                            if (nx >= 0 && nx < DungeonMap::MAP_WIDTH && ny >= 0 && ny < DungeonMap::MAP_HEIGHT &&
                                mapData[nx][ny] != DungeonMapTile::WALL && !closedSet[nx][ny]) {

                                // �ړ��R�X�g�̌v�Z
                                int ng = current->g + 1;

                                if (!(playerX == nx && playerY == ny) && IsCharacterCollision(nx, ny)) {
                                    // �ʃL����������ꍇ�ړ��R�X�g�����Z
                                    ng += 5;
                                }

                                // �I�[�v�����X�g�ɒǉ�
                                Node* nextNode = new Node(nx, ny, ng, ManhattanDistance(nx, ny, playerX, playerY), current);
                                openSet.push(nextNode);
                            }
                        }
                    }
                }
                else {
                    // �����_���Ɉړ�����
                    if (rand() % 2 == 0) {
                        dx = rand() % 3 - 1; // -1, 0, 1 �̂����ꂩ��I��
                    }
                    else {
                        dy = rand() % 3 - 1; // -1, 0, 1 �̂����ꂩ��I��
                    }
                }
                // currentState ��K�؂Ȉړ������ɐݒ�
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
                    turnActionCount++; // ��^�[���ɍs�������񐔂����Z
                    currentState = &Character::Idle; // �ړ����Ȃ��ꍇ�� Idle ��Ԃɖ߂�
                }
            }
        }
    }



    (this->*currentState)();
}

int Enemy::ManhattanDistance(int x1, int y1, int x2, int y2) {
    // �}���n�b�^��������Ԃ�(���������̋����Ɛ��������̋����̍��v)
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool Enemy::IsSearchPlayer(int playerX, int playerY, int searchDistance, int searchRange) {
    // �v���C���[��T��

    // �O����searchDistance�}�X�ȓ��Ƀv���C���[�����݂��邩�m�F
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

        // �O�����ǂȂ�false��Ԃ�
        if (mapData[checkX][checkY] == DungeonMapTile::WALL) {
            break;
        }

        // �v���C���[�����݂���ꍇtrue��Ԃ�
        if (checkX == playerX && checkY == playerY) {
            return true;
        }
    }
    
    // ���̓}�XsearchRange�͈̔͂𒲂ׂăv���C���[�����݂��邩�m�F
    for (int i = -searchRange; i <= searchRange; ++i) {
        for (int j = -searchRange; j <= searchRange; ++j) {
            // �������g�̈ʒu�͒��ׂȂ�
            if (i == 0 && j == 0) {
                continue;
            }

            int checkX = mapPosX + i;
            int checkY = mapPosY + j;

            // �}�b�v�͈͓̔����v���C���[�����݂���ꍇtrue��Ԃ�
            if (checkX >= 0 && checkX < DungeonMap::MAP_WIDTH && checkY >= 0 && checkY < DungeonMap::MAP_HEIGHT &&
                checkX == playerX && checkY == playerY) {
                return true;
            }
        }
    }

    // �v���C���[��������Ȃ������ꍇ��false��Ԃ�
    return false;
}
