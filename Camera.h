#pragma once
class Camera {
public:
    Camera(int screenWidth, int screenHeight);
    // �J�����̈ʒu��ݒ�
    void SetPosition(int x, int y);
    // �J�����̃I�t�Z�b�g���擾
    int GetOffsetX() const { return offsetX; }
    int GetOffsetY() const { return offsetY; }

private:
    int screenWidth;  // �E�B���h�E�̕�
    int screenHeight; // �E�B���h�E�̍���
    int offsetX = 0;      // �J������X�����I�t�Z�b�g
    int offsetY = 0;      // �J������Y�����I�t�Z�b�g
};
