#pragma once



// ダンジョンマップ内に存在する小さい部屋
class DungeonRoom
{
public:
    DungeonRoom(int startX, int startY, int roomWidth, int roomHeight)
        : x(startX), y(startY), width(roomWidth), height(roomHeight) {}

    int x, y, width, height;

    // 部屋の衝突チェック
    bool CollidesWith(const DungeonRoom& other, int distanceOtherRoom) const {
        return (x < other.x + other.width + distanceOtherRoom &&
            x + width + distanceOtherRoom > other.x &&
            y < other.y + other.height + distanceOtherRoom &&
            y + height + distanceOtherRoom > other.y);
    }
    // 座標が部屋の境界内に含まれるかどうかを判定
    bool ContainsPoint(int x, int y) const {
        
        return x >= this->x && x < this->x + this->width &&
            y >= this->y && y < this->y + this->height;
    }

};

