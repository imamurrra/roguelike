#pragma once
#include "Character.h"
#include "DungeonMap.h"
#include <vector>

class Player;

// �񋓌^�Ƃ��Ē�`����Node
struct Node {
    int x;
    int y;
    int g; // �J�n�n�_����̈ړ��R�X�g
    int h; // �S�[���܂ł̐���ړ��R�X�g
    Node* parent; // �e�m�[�h�ւ̃|�C���^

    Node(int x, int y, int g, int h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    // f�l���v�Z����֐�
    int f() const { return g + h; }
};

// �I�[�v�����X�g�̗D��x��r�p�̊֐��I�u�W�F�N�g
struct CompareNodePtr {
    bool operator()(Node* a, Node* b) const {
        return a->f() > b->f();
    }
};

// �G�̏��
enum EnemyMode {
    RandomMovement, // �����_���ړ����
    PlayerDetection, // �v���C���[�𔭌����
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
    virtual ~Enemy(); // �������z�f�X�g���N�^�̃I�[�o�[���C�h
    void Update(); // �G�̍s�����X�V


private:
    EnemyMode currentEnemyMode;
    Player& player;

    int ManhattanDistance(int x1, int y1, int x2, int y2);
    bool IsSearchPlayer(int playerX, int playerY, int searchDistance, int searchRange); // �v���C���[�̑{��
};