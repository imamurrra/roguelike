#pragma once
class Camera {
public:
    Camera(int screenWidth, int screenHeight);
    // カメラの位置を設定
    void SetPosition(int x, int y);
    // カメラのオフセットを取得
    int GetOffsetX() const { return offsetX; }
    int GetOffsetY() const { return offsetY; }

private:
    int screenWidth;  // ウィンドウの幅
    int screenHeight; // ウィンドウの高さ
    int offsetX = 0;      // カメラのX方向オフセット
    int offsetY = 0;      // カメラのY方向オフセット
};
