#include "Tile.h"
#include <gui/Shape.h>
#include <td/ColorID.h>

Tile::Tile(TileType type, const gui::Rect& rect, ItemInBlock item)
    : _type(type)
    , _rect(rect)
    , _isSolid(type != TileType::AIR)
    , _hasItem(item != ItemInBlock::NONE)
    , _isHit(false)
    , _itemType(item)
{
}

void Tile::draw(const gui::Point& cameraOffset)
{
    if (_type == TileType::AIR)
        return;

    gui::Rect drawRect(_rect.left - cameraOffset.x,
        _rect.top - cameraOffset.y,
        _rect.right - cameraOffset.x,
        _rect.bottom - cameraOffset.y);

    gui::Shape shape;

    td::ColorID color;
    switch (_type) {
    case TileType::GROUND:
        color = td::ColorID::SaddleBrown;
        break;
    case TileType::BRICK:
        color = td::ColorID::Red;
        break;
    case TileType::QUESTION_BLOCK:
        color = _isHit ? td::ColorID::Gray : td::ColorID::Yellow;
        break;
    case TileType::PIPE_TOP_LEFT:
    case TileType::PIPE_TOP_RIGHT:
    case TileType::PIPE_BODY_LEFT:
    case TileType::PIPE_BODY_RIGHT:
        color = td::ColorID::Green;
        break;
    default:
        color = td::ColorID::White;
    }

    shape.createRect(drawRect);
    shape.drawFillAndWire(color, td::ColorID::Black);
}

void Tile::onHit()
{
    if (_type == TileType::QUESTION_BLOCK && !_isHit) {
        _isHit = true;
    }
}