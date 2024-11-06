#include "DxLib.h"
#include "Camera.h"
#include "DungeonMap.h"
#include "Effect.h"
#include "GameManager.h"
#include "Player.h"
#include "SoundEffect.h"
#include <iostream>
#include <vector>

int WINAPI main(HINSTANCE, HINSTANCE, LPSTR, int) {
    // DX���C�u��������������
    if (DxLib_Init() == -1) {
        return -1; // ���������s���̓G���[�R�[�h��Ԃ��ďI��
    }

    // �E�B���h�E���쐬
    const int width = 32 * 20 * 1; // 640
    const int height = 32 * 15 * 1; // 480
    SetGraphMode(width, height, 32); // �E�B���h�E�T�C�Y���w��
    ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɂ���
    SetMainWindowText("�s�v�c�̃_���W����"); // �E�B���h�E�̃^�C�g����ݒ�

    if (SetGraphMode(width, height, 32) != 0) {
        DxLib_End(); // DX���C�u�����I������
        return -1;   // �O���t�B�b�N���[�h�̐ݒ莸�s���̓G���[�R�[�h��Ԃ��ďI��
    }

    // �Q�[���}�l�[�W���̃C���X�^���X���쐬
    GameManager gameManager;

    // �J�����̃C���X�^���X���쐬
    Camera camera(width, height);
    gameManager.SetCamera(camera);

    // �G�t�F�N�g�̃C���X�^���X���쐬
    Effect effect(camera);
    gameManager.SetEffect(effect);

    // ���ʉ��̃C���X�^���X���쐬
    SoundEffect soundEffect;
    gameManager.SetSoundEffect(soundEffect);


    // �_���W�����}�b�v�N���X�̃C���X�^���X���쐬
    srand(time(NULL)); // �[�������̃V�[�h��ݒ�
    DungeonMap dungeonMap(camera);
    const auto& dungeonMapData = dungeonMap.GetMapData();

    // �v���C���[
    Player* player; // Player �N���X�̃|�C���^�[�������o�[�Ƃ��Đ錾
    player = new Player(dungeonMap.GetPlayerInitPositionX(), dungeonMap.GetPlayerInitPositionY(),
        camera,
        gameManager.GetCharacters(),
        dungeonMap.GetMapData(),
        gameManager,
        effect,
        soundEffect);
    gameManager.SetPlayer(player); // �Q�[���}�l�[�W���Ƀv���C���[�̏���ݒ�
    gameManager.SetCharacter(player);

    // �G�̐���
    std::pair<int, int> enemySpawnPosition = dungeonMap.GetEnemySpawnPosition(player->GetMapPosX(), player->GetMapPosY());
    gameManager.CreateEnemy(enemySpawnPosition.first, enemySpawnPosition.second, dungeonMap.GetMapData());
    enemySpawnPosition = dungeonMap.GetEnemySpawnPosition(player->GetMapPosX(), player->GetMapPosY());
    gameManager.CreateEnemy(enemySpawnPosition.first, enemySpawnPosition.second, dungeonMap.GetMapData());

    // ���C�����[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen(); // ��ʂ��N���A

        // �v���C���[�̕`��ʒu���擾���ăJ�����ɐݒ�
        // �U�����͖����ɂ���
        if (player->GetCurrentState() != &Character::NormalAttack) {
            camera.SetPosition(player->GetPosX(), player->GetPosY());
        }


        // �_���W�����}�b�v�̕`��
        dungeonMap.Draw();

        // �L�����̕`��
        gameManager.DrawCharacters();
        // �L�����̍X�V
        gameManager.UpdateCharacters();

        // �G�t�F�N�g�̕`��
        effect.Draw();

        // �s�����O�̃��Z�b�g
        gameManager.ResetActionLog();

        // �Q�[���}�l�[�W���p�̃L�[���͏���
        gameManager.HandleInput();


        ScreenFlip();  // ����ʂ�\��ʂɔ��f
        WaitTimer(16); // �t���[�����[�g����
    }

    // DX���C�u�����I������
    DxLib_End();
    return 0;
}
