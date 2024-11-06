#include "Camera.h"

#include "DxLib.h"

Camera::Camera(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight) {

}

void Camera::SetPosition(int x, int y) {
    // �\���̈�̒��S���W���v�Z
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // �v���C���[�̈ʒu�𒆐S�ɂ��āA�I�t�Z�b�g���v�Z
    offsetX = x - centerX;
    offsetY = y - centerY;
}
