#include "Player.h"
#include "Camera.h"
#include "DxLib.h"
#include "DungeonMap.h"
#include "GameManager.h"

Player::Player(int startX, int startY,
    Camera& camera,
    std::vector<Character*>& characters,
    const int(&mapData)[DungeonMap::MAP_WIDTH][DungeonMap::MAP_HEIGHT],
    GameManager& gameManager,
    Effect& effect,
    SoundEffect& soundEffect) :
    Character(startX, startY, camera, characters, mapData, gameManager, effect, soundEffect) {
    // �v���C���[�̉摜��ǂݍ���
    image = LoadGraph("resource/character/player/player1.png");

    characterType = CharacterType::PLAYER;
    turnActionCountLimit = 1; // ��^�[���̍s����
    maxHealth = 20; // �ő�̗�
    currentHealth = 12; // ���ݑ̗�
    power = 5; // �U����
    characterName = "�v���C���[";
}

Player::~Player() {
    // �v���C���[�̉摜���������
    DeleteGraph(image);
}

void Player::Update() {

    HandleInput();

    (this->*currentState)();
}

void Player::HandleInput() {
    static bool keyPressed = false; // �L�[��������Ă��邩�ǂ����������t���O
    bool isDoubleSpeed = gameManager.GetDoubleSpeedFlag(); // �{����Ԃ�


    if ((!keyPressed || isDoubleSpeed) && currentState == &Character::Idle) {
        // �L�[�������ꂽ�ꍇ�Ƀv���C���[�̈ړ��������s��
        // �ړ��ł��Ȃ��Ă������͕ς���
        if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)) {
            // �ړ����L�����ǂ������`�F�b�N
            if (CheckMoveValidity(mapPosX, mapPosY - 1)) {
                currentState = &Character::MoveUp;
            }
            direction = UP; // �ړ��o���Ȃ��Ă������͕ς��� ����������������ɐݒ�
            keyPressed = !isDoubleSpeed;  // �{����Ԃ̏ꍇ�AkeyPressed �����Z�b�g���Ȃ�
        } else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)) {
            // �ړ����L�����ǂ������`�F�b�N
            if (CheckMoveValidity(mapPosX, mapPosY + 1)) {
                currentState = &Character::MoveDown;
            }
            direction = DOWN; // �����������牺�����ɐݒ�
            keyPressed = !isDoubleSpeed;
        } else if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A)) {
            // �ړ����L�����ǂ������`�F�b�N
            if (CheckMoveValidity(mapPosX - 1, mapPosY)) {
                currentState = &Character::MoveLeft;
            }
            direction = LEFT; // �����������獶�����ɐݒ�
            keyPressed = !isDoubleSpeed;
        }else if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D)) {
            // �ړ����L�����ǂ������`�F�b�N
            if (CheckMoveValidity(mapPosX + 1, mapPosY)) {
                currentState = &Character::MoveRight;
            }
            direction = RIGHT; // �E����������E�����ɐݒ�
            keyPressed = !isDoubleSpeed;
        } else if (CheckHitKey(KEY_INPUT_RETURN) || CheckHitKey(KEY_INPUT_P)) {
            // Enter �L�[�܂��� P �L�[�������ꂽ��ʏ�U����Ԃɂ���
            currentState = &Character::NormalAttack;
            keyPressed = true;
        }
    }

    // �L�[�������ꂽ��ړ��L�[���̓t���O�����Z�b�g
    if (!CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN) &&
        !CheckHitKey(KEY_INPUT_LEFT) && !CheckHitKey(KEY_INPUT_RIGHT) &&
        !CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S) &&
        !CheckHitKey(KEY_INPUT_A) && !CheckHitKey(KEY_INPUT_D) &&
        !CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_P)) {
        keyPressed = false;
    }
}
