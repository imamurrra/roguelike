#include "Character.h"
#include "DungeonMap.h"
#include "DxLib.h"
#include "Effect.h"
#include "GameManager.h"
#include "SoundEffect.h"
#include <algorithm>
#include <vector>
#include<string>

Character::Character(int startX, int startY,
    Camera& camera, std::vector<Character*>& characters,
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
    GameManager& gameManager,
    Effect& effect,
    SoundEffect& soundEffect) :
    mapPosX(startX), mapPosY(startY), camera(camera), characters(characters),
    mapData(mapData), gameManager(gameManager), effect(effect), soundEffect(soundEffect) {
    image = NULL;
    spriteIndex = 0;
    animationTimer = 0;
    posX = targetPosX = mapPosX * GameManager::TILE_SIZE;
    posY = targetPosY = mapPosY * GameManager::TILE_SIZE;
    currentState = &Character::Idle;
    characterType = CharacterType::ENEMY;
    currentHealth = maxHealth = 10;
    isAttackReady = false;
    isAttackEnd = false;
    attackWaitTime = ATTACK_WAIT_TIME;
    survivalStatus = CharacterSurvivalStatus::Alive;
    dyingWaitingTime = DYING_WAITING_TIME;
    power = 1; // �U����
}

Character::~Character() {
    // �f�X�g���N�^
}

void Character::Draw() {
    // �L�����N�^�[�̉摜��`�悷��
    const int tileSize = GameManager::TILE_SIZE;

    // �|�ꂩ����ԂȂ�_�ł��ăL������\������
    if (survivalStatus == CharacterSurvivalStatus::Dying) {
        dyingWaitingTime--;
        if (dyingWaitingTime <= 0) {
            survivalStatus = CharacterSurvivalStatus::Dead; // �|�ꂽ��ԂɍX�V TODO:Draw()�ōX�V����͕̂ς��ȂƎv�����ǂƂ肠����
        }

        if (dyingWaitingTime % 2 == 0) {
            return;
        }
    }

    DrawRectGraph(
        posX - camera.GetOffsetX(),
        posY - camera.GetOffsetY(),
        tileSize * spriteIndex, direction * tileSize,
        tileSize, tileSize, image, TRUE, FALSE);

    // �A�j���[�V�����̍X�V
    if (survivalStatus == CharacterSurvivalStatus::Alive) {
        UpdateAnimation();
    }


    if (characterType == CharacterType::PLAYER) {
        char intStrX[32];
        sprintf_s(intStrX, "%d", mapPosX); // �����𕶎���ɕϊ�
        // �e�L�X�g����ʂɕ\��
        DrawString(100, 100, intStrX, GetColor(255, 255, 255));

        char intStrY[32];
        sprintf_s(intStrY, "%d", mapPosY); // �����𕶎���ɕϊ�
        // �e�L�X�g����ʂɕ\��
        DrawString(100, 120, intStrY, GetColor(255, 255, 255));

        char intStrdrawTargetPosX[32]; // �������i�[���邽�߂̕�����o�b�t�@
        sprintf_s(intStrdrawTargetPosX, "%d", targetPosX); // �����𕶎���ɕϊ�
        // �e�L�X�g����ʂɕ\��
        DrawString(100, 140, intStrdrawTargetPosX, GetColor(255, 255, 255));

        char intStrdrawTargetPosY[32]; // �������i�[���邽�߂̕�����o�b�t�@
        sprintf_s(intStrdrawTargetPosY, "%d", targetPosY); // �����𕶎���ɕϊ�
        // �e�L�X�g����ʂɕ\��
        DrawString(100, 160, intStrdrawTargetPosY, GetColor(255, 255, 255));

        char intStrdrawPosX[32]; // �������i�[���邽�߂̕�����o�b�t�@
        sprintf_s(intStrdrawPosX, "%d", posX); // �����𕶎���ɕϊ�
        // �e�L�X�g����ʂɕ\��
        DrawString(100, 180, intStrdrawPosX, GetColor(255, 255, 255));

        char intStrdrawPosY[32]; // �������i�[���邽�߂̕�����o�b�t�@
        sprintf_s(intStrdrawPosY, "%d", posY); // �����𕶎���ɕϊ�
        // �e�L�X�g����ʂɕ\��
        DrawString(100, 200, intStrdrawPosY, GetColor(255, 255, 255));

        std::string muki = "";
        switch (direction) {
        case UP:
            muki = "UP";
            break;
        case DOWN:
            muki = "DOWN";
            break;
        case LEFT:
            muki = "LEFT";
            break;
        case RIGHT:
            muki = "RIGHT";
            break;
        }
        DrawString(100, 220, muki.c_str(), GetColor(255, 255, 255));

        std::string status = "";
        if (currentState == &Character::Idle) {
            status = "�ҋ@��";
        } else if (currentState == &Character::Moving) {
            status = "�ړ���";
        }
        else if (currentState == &Character::NormalAttack) {
            status = "�ʏ�U����";
        }
        DrawString(100, 240, status.c_str(), GetColor(255, 255, 255));

        char intStrdrawPower[32]; // �������i�[���邽�߂̕�����o�b�t�@
        sprintf_s(intStrdrawPower, "�U����:%d", power); // �����𕶎���ɕϊ�
        // �e�L�X�g����ʂɕ\��
        DrawString(100, 260, intStrdrawPower, GetColor(255, 255, 255));
    }
}

void Character::Update() {
    (this->*currentState)();
}

void Character::Idle() {
    // �������Ȃ���� �ҋ@���
}

void Character::NormalAttack() {
    // �ʏ�U�� �ڂ̑O1�}�X����U�� 
    int targetX = mapPosX; // �U���Ώۂ�X���W
    int targetY = mapPosY; // �U���Ώۂ�Y���W

    // �L���������ݍ��ދ���
    int steppingDistance = GameManager::TILE_SIZE / 2;
    
    // �{���̈ʒu
    int originalPosX = mapPosX * GameManager::TILE_SIZE;
    int originalPosY = mapPosY * GameManager::TILE_SIZE;

    // ���ݍ��ޖڕW���W
    int steppingTargetPosX = originalPosX;
    int steppingTargetPosY = originalPosY;

    // posX or posY�𓥂ݍ��ޖڕW���W�܂ŃC���N�������g or �f�N�������g������
    int moveX = 0;
    int moveY = 0;
    if (!isAttackReady) {
        // �L�����N�^�[�������Ă�������ɉ����āA�U���Ώۂ̍��W���v�Z����
        switch (direction) {
        case UP:
            targetY -= 1;
            steppingTargetPosY -= steppingDistance;
            break;
        case DOWN:
            targetY += 1;
            steppingTargetPosY += steppingDistance;
            break;
        case LEFT:
            targetX -= 1;
            steppingTargetPosX -= steppingDistance;
            break;
        case RIGHT:
            targetX += 1;
            steppingTargetPosX += steppingDistance;
            break;
        }

        // ���ݍ��݈ړ����I������U���t���O��L���ɂ���
        if (MoveToTarget(steppingTargetPosX, steppingTargetPosY)) {
            isAttackReady = true;
        }
    }

    // �U����ɓG�����݂��邩�ǂ������`�F�b�N����
    if (isAttackReady && !isAttackEnd) {
        bool isEnemyInRange = false;
        Character* targetCharacter = nullptr;
        for (Character* character : characters) {
            // �v���C���[�͓G�ɁA�G�̓v���C���[�̏ꍇ�����U�����q�b�g����
            if (character->GetCharacterType() != characterType && character->GetMapPosX() == targetX && character->GetMapPosY() == targetY) {
                isEnemyInRange = true;
                targetCharacter = character;
                break;
            }
        }

        if (isEnemyInRange && targetCharacter != nullptr) {
            // �U���ΏۂɃ_���[�W��^����
            targetCharacter->ReceiveDamage(power);
            
            // �s�����O�ɁuA��B��n�_���[�W�^�����v�̓��e�Ń��O��ǉ�
            std::string logMessage = characterName + "��" + targetCharacter->characterName + "��" + std::to_string(power) + "�_���[�W�^����";
            gameManager.AddToActionLog(logMessage);

            // �U���q�b�g���ƃG�t�F�N�g���Đ�
            effect.AddEffect(targetCharacter->GetPosX(), targetCharacter->GetPosY());
            soundEffect.PlaySoundEffect(SoundEffectType::AttackHit);
        }
        else {
            // �U����U�艹���Đ�
            soundEffect.PlaySoundEffect(SoundEffectType::AttackMiss);
        }
        isAttackEnd = true;


    }

    // ���b�ҋ@
    if (attackWaitTime > 0) {
        attackWaitTime--;
        return;
    }

    if (isAttackReady && isAttackEnd) {
        // ���̈ʒu�ɖ߂�
        // ���̈ʒu�ɖ߂�����ҋ@��Ԃɖ߂�
        if (MoveToTarget(originalPosX, originalPosY)) {
            currentState = &Character::Idle;
            // �t���O�����Z�b�g
            isAttackReady = false;
            isAttackEnd = false;
            ResetAttackWaitTime();
            turnActionCount++; // ��^�[���ɍs�������񐔂����Z
        }
    }

}
 
void Character::MoveUp() {
    Move(0, -1);
}

void Character::MoveDown() {
    Move(0, 1);
}

void Character::MoveLeft() {
    Move(-1, 0);
}

void Character::MoveRight() {
    Move(1, 0);
}

void Character::Move(int dx, int dy) {
    int newX = mapPosX + dx;
    int newY = mapPosY + dy;

    // �ړ����L�����ǂ������`�F�b�N
    if (CheckMoveValidity(newX, newY)) {
        // �ړ�
        mapPosX = newX;
        mapPosY = newY;

        targetPosX = mapPosX * GameManager::TILE_SIZE;
        targetPosY = mapPosY * GameManager::TILE_SIZE;

        // ������ݒ�
        if (dx > 0) {
            direction = RIGHT;
        }
        else if (dx < 0) {
            direction = LEFT;
        }
        else if (dy > 0) {
            direction = DOWN;
        }
        else if (dy < 0) {
            direction = UP;
        }

        // �ړ�����ԂɍX�V
        currentState = &Character::Moving;
    }
    else {
        // �����Ȉړ��̏ꍇ�͑ҋ@��Ԃɂ���Ȃǂ̏������s��
        currentState = &Character::Idle;
    }

    turnActionCount++; // ��^�[���ɍs�������񐔂����Z
}

void Character::Moving() {
    int moveX = 0;
    int moveY = 0;

    int moveingPosSpeed = MOVEING_POS_SPEED;
    // �{����Ԃ̏���
    if (gameManager.GetDoubleSpeedFlag()) {
        moveingPosSpeed *= 2;
    }

    // ���ۂ̕\�����W�ƖڕW�\�����W�ɍ�������΂��̍���moveingPosSpeed�̕��������߂�
    moveX = (targetPosX - posX > 0) ? (std::min)(moveingPosSpeed, targetPosX - posX) :
        (std::max)(-moveingPosSpeed, targetPosX - posX);
    moveY = (targetPosY - posY > 0) ? (std::min)(moveingPosSpeed, targetPosY - posY) :
        (std::max)(-moveingPosSpeed, targetPosY - posY);

    // ���݂̈ʒu����ړ��ʂ������ĐV�����ʒu���v�Z
    posX += moveX;
    posY += moveY;

    // �\���ʒu��␳
    if (abs(posX - targetPosX) <= moveingPosSpeed) {
        posX = targetPosX;
    }
    if (abs(posY - targetPosY) <= moveingPosSpeed) {
        posY = targetPosY;
    }

    // �ړ����I������ҋ@��Ԃɖ߂�
    if (posX == targetPosX && posY == targetPosY) {
        currentState = &Character::Idle;
    }
}

void Character::UpdateAnimation() {
    // �A�j���[�V�����̍X�V�Ԋu
    const int animationInterval = 20;
    const int numSprites = 3;

    // �^�C�}�[���C���N�������g
    animationTimer++;
    if (gameManager.GetDoubleSpeedFlag()) { // �{����Ԃ�
        animationTimer++; // �{���ŃA�j���[�V��������
    }

    if (animationTimer >= animationInterval) {
        // �X�v���C�g�摜�̃C���f�b�N�X���X�V
        spriteIndex += spriteIncrement;
        if (spriteIndex == numSprites - 1 || spriteIndex == 0) {
            spriteIncrement = -spriteIncrement; // �������t�ɂ���
        }
        animationTimer = 0;
    }
}

// �ړ��悪�ǂ��ǂ������`�F�b�N����֐�
bool Character::IsWall(int x, int y) const {
    return mapData[x][y] != DungeonMapTile::FLOOR;
}

// �ړ���ɑ��̃L�����N�^�[�����Ȃ����ǂ������`�F�b�N����֐�
bool Character::IsCharacterCollision(int x, int y) const {
    for (const Character* character : characters) {
        if (character != this && character->GetMapPosX() == x && character->GetMapPosY() == y) {
            return true; // �Փ˂���
        }
    }
    return false; // �Փ˂Ȃ�
}

// �ړ��悪�}�b�v�͈͓̔����ǂ������`�F�b�N����֐�
bool Character::IsInMapRange(int x, int y) const {
    return x >= 0 && x < DungeonMap::MAP_WIDTH && y >= 0 && y < DungeonMap::MAP_HEIGHT;
}

// �ړ����L�����ǂ������`�F�b�N����֐�
bool Character::CheckMoveValidity(int x, int y) const {
    // �ړ��悪�}�b�v�͈̔͊O�̏ꍇ�͖���
    if (!IsInMapRange(x, y)) {
        return false;
    }

    // �ړ��悪�ǂ̏ꍇ�͖���
    if (IsWall(x, y)) {
        return false;
    }

    // �ړ���ɑ��̃L�����N�^�[������ꍇ�͖���
    if (IsCharacterCollision(x, y)) {
        return false;
    }

    // ��L�̏��������ׂĖ������ꍇ�͗L��
    return true;
}

bool Character::MoveToTarget(int targetX, int targetY) {
    // �ڕW���W�܂ł̈ړ��ʂ��v�Z
    int moveingPosSpeed = MOVEING_POS_SPEED;
    if (gameManager.GetDoubleSpeedFlag()) { // �{����Ԃ�
        moveingPosSpeed *= 2;
    }

    int moveX = (targetX - posX > 0) ? (std::min)(moveingPosSpeed, targetX - posX) :
        (std::max)(-moveingPosSpeed, targetX - posX);
    int moveY = (targetY - posY > 0) ? (std::min)(moveingPosSpeed, targetY - posY) :
        (std::max)(-moveingPosSpeed, targetY - posY);

    // ���݂̈ʒu����ړ��ʂ������ĐV�����ʒu���v�Z
    posX += moveX;
    posY += moveY;

    // �\���ʒu��␳
    if (abs(posX - targetX) <= moveingPosSpeed) {
        posX = targetX;
    }
    if (abs(posY - targetY) <= moveingPosSpeed) {
        posY = targetY;
    }

    // �ړ����I������true��Ԃ�
    return (posX == targetX && posY == targetY);
}

void Character::ReceiveDamage(int receiveDamage) {
    // �_���[�W���󂯂�
    currentHealth -= receiveDamage;
    if (currentHealth <= 0) {
        currentHealth = 0;
        // �|�ꂩ���̏�ԂɍX�V
        survivalStatus = CharacterSurvivalStatus::Dying;
    }
}