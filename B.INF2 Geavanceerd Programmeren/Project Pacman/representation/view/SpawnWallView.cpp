#include "SpawnWallView.h"

SpawnWallView::SpawnWallView(const std::shared_ptr<IEntityModel>& entity_model)
    : IEntityView(Assets::getSpriteInfo(Assets::SpriteDefinition::SpawnWallBase), entity_model) {}