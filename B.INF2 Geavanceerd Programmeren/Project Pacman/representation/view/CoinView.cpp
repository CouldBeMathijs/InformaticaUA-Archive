#include "CoinView.h"

CoinView::CoinView(std::shared_ptr<IEntityModel> e)
    : IEntityView(Assets::getSpriteInfo(Assets::SpriteDefinition::CoinBase), std::move(e)) {}
