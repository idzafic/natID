#pragma once
#include <gui/Types.h>

enum class TileType {
    AIR = 0,
    GROUND,
    BRICK,
    QUESTION_BLOCK,
    PIPE_TOP_LEFT,
    PIPE_TOP_RIGHT,
    PIPE_BODY_LEFT,
    PIPE_BODY_RIGHT
};
enum class ItemInBlock {
    NONE,
    COIN,
    MUSHROOM,
    FIRE_FLOWER
};
class Tile {
protected:
    TileType _type;
    gui::Rect _rect;
    bool _isSolid;
    bool _hasItem;
    bool _isHit;
    ItemInBlock _itemType;

public:
    Tile(TileType type, const gui::Rect& rect, ItemInBlock item = ItemInBlock::NONE);

    void draw(const gui::Point& cameraOffset);
    void onHit();

    TileType getType() const { return _type; }
    const gui::Rect& getRect() const { return _rect; }
    bool isSolid() const { return _isSolid; }
    bool hasItem() const { return _hasItem && !_isHit; }
    bool isHit() const { return _isHit; }
    ItemInBlock getItemType() const { return _itemType; }
};