#include "IDirectionalEntityView.h"

IDirectionalEntityView::IDirectionalEntityView(Assets::TextureLocation m, std::shared_ptr<IEntityModel> n,
                                               const int amountOfTextures)
    : IEntityView(m, std::move(n)) {
    this->m_amountOfTextures = amountOfTextures;
}

void IDirectionalEntityView::update() {
    m_currentSprite.top =
        m_topBase + 50 * (getCurrentTextureOffset() +
                          m_amountOfTextures * static_cast<unsigned int>(getCoupledEntity()->getDirection()));
    IEntityView::update();
}
