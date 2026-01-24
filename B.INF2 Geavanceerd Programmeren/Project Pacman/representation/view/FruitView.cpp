#include "FruitView.h"

FruitView::FruitView(std::shared_ptr<IEntityModel> e)
    : IEntityView(Assets::getSpriteInfo(Assets::SpriteDefinition::FruitBase), std::move(e)) {}
