#pragma once

class GameManager;
class Effect;
class SoundEffect;

#include "Camera.h"
#include "DungeonMap.h"
#include <vector>
#include <string>

// �����Ă������
enum Direction {
    DOWN = 0,
    LEFT = 1,
    RIGHT = 2,
    UP = 3,
};

// �L�����N�^�[�̃^�C�v
enum CharacterType {
    PLAYER,
    ENEMY,
};

// �L�����N�^�[�̃X�e�[�^�X
enum CharacterSurvivalStatus {
    Alive, // �������
    Dying, // �|�ꂩ��
    Dead, // �|���ꂽ
};

const int MOVEING_POS_SPEED = 2; // �ړ����x
const int ATTACK_WAIT_TIME = 10; // �U�����蔭����̑ҋ@����
const int DYING_WAITING_TIME = 30; // �̗͂�0�ɂȂ������A������܂œ_�ł��鎞��

class Character {
public:
    Character(int startX, int startY,
        Camera& camera, std::vector<Character*>& characters,
        const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
        GameManager& gameManager,
        Effect& effect,
        SoundEffect& soundEffect);
    virtual ~Character(); // �������z�f�X�g���N�^

    void Draw();
    virtual void Update();
    virtual void Move(int dx, int dy);

    // �L�����̍s���֐�
    void Idle(); // �������Ȃ����
    void NormalAttack(); //�@�ʏ�U������
    void Moving();
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    void UpdateAnimation();
    bool IsWall(int x, int y) const;
    bool IsCharacterCollision(int x, int y) const;
    bool IsInMapRange(int x, int y) const;
    bool CheckMoveValidity(int x, int y) const;

    // Get�֐�
    int GetMapPosX() const { return mapPosX; }
    int GetMapPosY() const { return mapPosY; }
    int GetPosX() const { return posX; }
    int GetPosY() const { return posY; }
    int GetCurrentHealth() const { return currentHealth; }
    int GetMaxHealth() const { return maxHealth; }
    Direction GetDirection() const { return direction; }
    bool CanTakeTurnAction() const { // �L�������s���\����Ԃ�
        return turnActionCount < turnActionCountLimit;
    }
    void (Character::* GetCurrentState())() {
        return currentState;
    }
    CharacterType GetCharacterType() const {
        return characterType;
    }
    CharacterSurvivalStatus GetSurvivalStatus() const { return survivalStatus; }
    std::string GetCharacterName() const {
        return characterName;
    }

    void resetTurnActionCount() { turnActionCount = 0; } // ��^�[���ɍs�������񐔂̃��Z�b�g
    void SetCurrentState(void(Character::* state)()) {
        currentState = state;
    }
    void SetDirection(Direction newDirection) { direction = newDirection; }
    void ReceiveDamage(int receiveDamage); // �_���[�W���󂯂�


protected:
    int mapPosX; // �񎟌��z��}�b�v��̃L�����̍��W
    int mapPosY; // �񎟌��z��}�b�v��̃L�����̍��W
    int posX; // ���݂̕`��ʒu
    int posY; // ���݂̕`��ʒu
    int targetPosX; // �ڕW�̕`��ʒu
    int targetPosY; // �ڕW�̕`��ʒu
    int image; // �L�����N�^�[�̉摜
    int spriteIndex = 0; // �X�v���C�g�摜�̃C���f�b�N�X
    int animationTimer = 0; // �A�j���[�V�����̃^�C�}�[
    int spriteIncrement = 1;
    int turnActionCountLimit = 1; // ��^�[���ɍs���o�����
    int turnActionCount = 0; // ���ۂɈ�^�[���ɍs��������
    Direction direction = DOWN; // �����Ă������
    int currentHealth; // ���݂̗̑�(�c��̗�)
    int maxHealth; // �ő�̗�
    bool isAttackReady; // �U������
    bool isAttackEnd; // �U���I��
    int attackWaitTime; // �U�����̑ҋ@����
    CharacterSurvivalStatus survivalStatus; // �L�����N�^�[�̏��
    int dyingWaitingTime; // �|���܂ł̌o�ߎ���
    int power; // �U����
    std::string characterName; // �L�����N�^�[�̖��O


    Camera& camera;
    Effect& effect;
    GameManager& gameManager;
    SoundEffect& soundEffect;
    std::vector<Character*>& characters;
    void(Character::* currentState)();
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT];
    CharacterType characterType;

private:
    bool MoveToTarget(int targetX, int targetY); // �ڕW�n�_�܂ňړ��B�ڕW�ɒ����Ă�����true��Ԃ�
    void ResetAttackWaitTime() {
        attackWaitTime = ATTACK_WAIT_TIME;
    }
};
