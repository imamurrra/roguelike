#include "Camera.h"

#include "DxLib.h"

Camera::Camera(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight) {

}

void Camera::SetPosition(int x, int y) {
    // 表示領域の中心座標を計算
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // プレイヤーの位置を中心にして、オフセットを計算
    offsetX = x - centerX;
    offsetY = y - centerY;
}
