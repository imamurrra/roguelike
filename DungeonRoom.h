#pragma once



// �_���W�����}�b�v���ɑ��݂��鏬��������
class DungeonRoom
{
public:
    DungeonRoom(int startX, int startY, int roomWidth, int roomHeight)
        : x(startX), y(startY), width(roomWidth), height(roomHeight) {}

    int x, y, width, height;

    // �����̏Փ˃`�F�b�N
    bool CollidesWith(const DungeonRoom& other, int distanceOtherRoom) const {
        return (x < other.x + other.width + distanceOtherRoom &&
            x + width + distanceOtherRoom > other.x &&
            y < other.y + other.height + distanceOtherRoom &&
            y + height + distanceOtherRoom > other.y);
    }
    // ���W�������̋��E���Ɋ܂܂�邩�ǂ����𔻒�
    bool ContainsPoint(int x, int y) const {
        
        return x >= this->x && x < this->x + this->width &&
            y >= this->y && y < this->y + this->height;
    }

};

