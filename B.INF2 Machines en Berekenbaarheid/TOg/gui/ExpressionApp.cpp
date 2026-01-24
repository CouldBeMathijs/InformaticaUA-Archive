#include "ExpressionApp.h"

#include "../utils/Tokenizer.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

ExpressionApp::ExpressionApp()
    : window_(sf::VideoMode(1200, 700), "MBTOg Expression GUI", sf::Style::Default)
    , cfg_("config/expression_grammar.json")
    , parser_(cfg_)
    , env_(expr::OperatorEnvironment::createDefault())
    , cykOps_("config/operators.json")
    , cykCfg_("config/expression_grammar.json")
    , pda_("config/pda_config.json") {
    // PDA loads its own grammar
    pda_.loadGrammarFromJSON("config/expression_grammar.json");

    cfg_.setOperatorConfig(&cykOps_);
    parser_.setOperatorConfig(&cykOps_);
    parser_.slr(null_out);
    expr::loadBinaryOperatorsFromFile(env_, "config/operators.json");

    cykCfg_.toCNF();
    cykIsInCnf_ = true;

    if (!font_.loadFromFile("resources/arial.ttf"))
        throw std::runtime_error("Kon font resources/arial.ttf niet laden");

    window_.setKeyRepeatEnabled(true);

    // --- Background card ---
    cardShadow_.setPosition(46.f, 36.f);
    cardShadow_.setSize({1108.f, 630.f});
    cardShadow_.setFillColor(sf::Color(0, 0, 0, 90));

    card_.setPosition(40.f, 30.f);
    card_.setSize({1108.f, 630.f});
    card_.setFillColor(sf::Color(24, 26, 31));
    card_.setOutlineThickness(1.f);
    card_.setOutlineColor(sf::Color(70, 75, 90));

    // --- Input label ---
    inputLabel_.setFont(font_);
    inputLabel_.setCharacterSize(18);
    inputLabel_.setFillColor(sf::Color(220, 225, 235));
    inputLabel_.setString("Expressie");
    inputLabel_.setPosition(70.f, 70.f);

    // --- Input box ---
    inputBox_.setPosition(70.f, 105.f);
    inputBox_.setSize({740.f, 46.f});
    inputBox_.setFillColor(sf::Color(18, 20, 24));
    inputBox_.setOutlineThickness(2.f);
    inputBox_.setOutlineColor(sf::Color(95, 105, 125));

    inputInner_.setPosition(inputBox_.getPosition() + sf::Vector2f(2.f, 2.f));
    inputInner_.setSize(inputBox_.getSize() - sf::Vector2f(4.f, 4.f));
    inputInner_.setFillColor(sf::Color(245, 246, 248));

    inputText_.setFont(font_);
    inputText_.setCharacterSize(20);
    inputText_.setFillColor(sf::Color(20, 20, 20));
    inputText_.setPosition(inputBox_.getPosition().x + 12.f, inputBox_.getPosition().y + 10.f);

    selectionRect_.setFillColor(sf::Color(120, 160, 255, 120));
    caretRect_.setFillColor(sf::Color(30, 110, 255));
    caretRect_.setSize({2.f, 24.f});

    // --- Buttons ---
    auto initButton = [&](Button& b, sf::Vector2f pos, sf::Vector2f size, const std::string& txt) {
        b.box.setPosition(pos);
        b.box.setSize(size);
        b.box.setFillColor(sf::Color(55, 60, 75));
        b.box.setOutlineThickness(1.f);
        b.box.setOutlineColor(sf::Color(90, 95, 115));

        b.label.setFont(font_);
        b.label.setCharacterSize(18);
        b.label.setFillColor(sf::Color(235, 238, 245));
        b.label.setString(txt);

        const sf::FloatRect lb = b.label.getLocalBounds();
        b.label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        b.label.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
    };

    initButton(evalButton_, {70.f, 170.f}, {120.f, 44.f}, "Evaluate");
    initButton(cykButton_, {205.f, 170.f}, {120.f, 44.f}, "CYK Check");
    initButton(astButton_, {340.f, 170.f}, {120.f, 44.f}, "AST Image");
    initButton(pdaButton_, {475.f, 170.f}, {130.f, 44.f}, "PDA Diagram");
    initButton(pdaSimButton_, {620.f, 170.f}, {130.f, 44.f}, "PDA Validate");

    // --- Result text ---
    resultText_.setFont(font_);
    resultText_.setCharacterSize(22);
    resultText_.setFillColor(sf::Color(230, 235, 245));
    resultText_.setPosition(70.f, 240.f);

    // --- Status panel ---
    statusPanel_.setPosition(70.f, 290.f);
    statusPanel_.setSize({250.f, 340.f});
    statusPanel_.setFillColor(sf::Color(18, 20, 24));
    statusPanel_.setOutlineThickness(1.f);
    statusPanel_.setOutlineColor(sf::Color(70, 75, 90));

    statusText_.setFont(font_);
    statusText_.setCharacterSize(14);
    statusText_.setPosition(statusPanel_.getPosition().x + 12.f,
                            statusPanel_.getPosition().y + 12.f);

    // --- AST panel (right side, top) ---
    astLabel_.setFont(font_);
    astLabel_.setCharacterSize(14);
    astLabel_.setFillColor(sf::Color(220, 225, 235));
    astLabel_.setString("AST Diagram (wheel=zoom, drag=pan)");
    astLabel_.setPosition(340.f, 290.f);

    astPanel_.setPosition(340.f, 315.f);
    astPanel_.setSize({290.f, 315.f});
    astPanel_.setFillColor(sf::Color(18, 20, 24));
    astPanel_.setOutlineThickness(2.f);
    astPanel_.setOutlineColor(sf::Color(70, 75, 90));

    astPanelInner_.setPosition(astPanel_.getPosition() + sf::Vector2f(6.f, 6.f));
    astPanelInner_.setSize(astPanel_.getSize() - sf::Vector2f(12.f, 12.f));
    astPanelInner_.setFillColor(sf::Color(12, 13, 16));

    {
        auto     inner = astPanelInner_.getSize();
        unsigned w     = static_cast<unsigned>(std::max(1.f, inner.x));
        unsigned h     = static_cast<unsigned>(std::max(1.f, inner.y));
        if (!astViewportTex_.create(w, h))
            throw std::runtime_error("Failed to create AST viewport render texture.");
        astViewportSprite_.setTexture(astViewportTex_.getTexture(), true);
        astViewportSprite_.setPosition(astPanelInner_.getPosition());
    }

    // --- PDA panel (right side, bottom) ---
    pdaLabel_.setFont(font_);
    pdaLabel_.setCharacterSize(14);
    pdaLabel_.setFillColor(sf::Color(220, 225, 235));
    pdaLabel_.setString("PDA Diagram (wheel=zoom, drag=pan)");
    pdaLabel_.setPosition(650.f, 290.f);

    pdaPanel_.setPosition(650.f, 315.f);
    pdaPanel_.setSize({390.f, 315.f});
    pdaPanel_.setFillColor(sf::Color(18, 20, 24));
    pdaPanel_.setOutlineThickness(2.f);
    pdaPanel_.setOutlineColor(sf::Color(70, 75, 90));

    pdaPanelInner_.setPosition(pdaPanel_.getPosition() + sf::Vector2f(6.f, 6.f));
    pdaPanelInner_.setSize(pdaPanel_.getSize() - sf::Vector2f(12.f, 12.f));
    pdaPanelInner_.setFillColor(sf::Color(12, 13, 16));

    {
        auto     inner = pdaPanelInner_.getSize();
        unsigned w     = static_cast<unsigned>(std::max(1.f, inner.x));
        unsigned h     = static_cast<unsigned>(std::max(1.f, inner.y));
        if (!pdaViewportTex_.create(w, h))
            throw std::runtime_error("Failed to create PDA viewport render texture.");
        pdaViewportSprite_.setTexture(pdaViewportTex_.getTexture(), true);
        pdaViewportSprite_.setPosition(pdaPanelInner_.getPosition());
    }

    parser_.setDebug(false);

    // initial state
    focusInput(false);
    setStatus("Tip: Ctrl+A/C/V/X work. Type expression, press Evaluate!", true);
    applyLayout(static_cast<float>(window_.getSize().x), static_cast<float>(window_.getSize().y));
}

void ExpressionApp::run() {
    while (window_.isOpen()) {
        handleEvents();

        if (caretBlinkClock_.getElapsedTime().asSeconds() >= 0.5f) {
            caretVisible_ = !caretVisible_;
            caretBlinkClock_.restart();
        }

        window_.clear(sf::Color(14, 15, 18));
        render();
        window_.display();
    }
}

void ExpressionApp::handleEvents() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window_.close();
                break;

            case sf::Event::TextEntered:
                if (inputFocused_)
                    handleTextEntered(event.text);
                break;

            case sf::Event::KeyPressed:
                if (inputFocused_)
                    handleKeyPressed(event.key);
                break;

            case sf::Event::MouseWheelScrolled: {
                sf::Vector2f mp(static_cast<float>(event.mouseWheelScroll.x),
                                static_cast<float>(event.mouseWheelScroll.y));

                if (isMouseOverAstPanel(mp) && astHasImage_) {
                    sf::Vector2f       m     = mp - astPanelInner_.getPosition();
                    const sf::Vector2u vpSz  = astViewportTex_.getSize();
                    const sf::Vector2u imgSz = astImageTexture_.getSize();
                    if (vpSz.x == 0 || vpSz.y == 0 || imgSz.x == 0 || imgSz.y == 0)
                        break;

                    constexpr float    pad  = 10.f;
                    const float        fitX = (vpSz.x - 2.f * pad) / static_cast<float>(imgSz.x);
                    const float        fitY = (vpSz.y - 2.f * pad) / static_cast<float>(imgSz.y);
                    const float        baseScale = std::min(fitX, fitY);
                    const float        oldZoom   = astZoom_;
                    const float        oldScale  = baseScale * oldZoom;
                    const float        oldImgW   = imgSz.x * oldScale;
                    const float        oldImgH   = imgSz.y * oldScale;
                    const sf::Vector2f oldCenter((vpSz.x - oldImgW) * 0.5f,
                                                 (vpSz.y - oldImgH) * 0.5f);

                    const sf::Vector2f oldImgPos = oldCenter + astPan_;
                    const sf::Vector2f imgCoord  = (m - oldImgPos) / oldScale;

                    const float        factor    = (event.mouseWheelScroll.delta > 0.f) ? 1.15f
                                                                                        : (1.f / 1.15f);
                    astZoom_                     = std::clamp(astZoom_ * factor, 0.10f, 8.0f);

                    const float        newScale  = baseScale * astZoom_;
                    const float        newImgW   = imgSz.x * newScale;
                    const float        newImgH   = imgSz.y * newScale;
                    const sf::Vector2f newCenter((vpSz.x - newImgW) * 0.5f,
                                                 (vpSz.y - newImgH) * 0.5f);

                    const sf::Vector2f newImgPos = m - imgCoord * newScale;
                    astPan_                      = newImgPos - newCenter;

                    updateAstPreview();
                    break;
                }

                if (isMouseOverPdaPanel(mp) && pdaHasImage_) {
                    sf::Vector2f       m     = mp - pdaPanelInner_.getPosition();
                    const sf::Vector2u vpSz  = pdaViewportTex_.getSize();
                    const sf::Vector2u imgSz = pdaImageTexture_.getSize();
                    if (vpSz.x == 0 || vpSz.y == 0 || imgSz.x == 0 || imgSz.y == 0)
                        break;

                    constexpr float    pad  = 10.f;
                    const float        fitX = (vpSz.x - 2.f * pad) / static_cast<float>(imgSz.x);
                    const float        fitY = (vpSz.y - 2.f * pad) / static_cast<float>(imgSz.y);
                    const float        baseScale = std::min(fitX, fitY);
                    const float        oldZoom   = pdaZoom_;
                    const float        oldScale  = baseScale * oldZoom;
                    const float        oldImgW   = imgSz.x * oldScale;
                    const float        oldImgH   = imgSz.y * oldScale;
                    const sf::Vector2f oldCenter((vpSz.x - oldImgW) * 0.5f,
                                                 (vpSz.y - oldImgH) * 0.5f);

                    const sf::Vector2f oldImgPos = oldCenter + pdaPan_;
                    const sf::Vector2f imgCoord  = (m - oldImgPos) / oldScale;

                    const float        factor    = (event.mouseWheelScroll.delta > 0.f) ? 1.15f
                                                                                        : (1.f / 1.15f);
                    pdaZoom_                     = std::clamp(pdaZoom_ * factor, 0.10f, 8.0f);

                    const float        newScale  = baseScale * pdaZoom_;
                    const float        newImgW   = imgSz.x * newScale;
                    const float        newImgH   = imgSz.y * newScale;
                    const sf::Vector2f newCenter((vpSz.x - newImgW) * 0.5f,
                                                 (vpSz.y - newImgH) * 0.5f);

                    const sf::Vector2f newImgPos = m - imgCoord * newScale;
                    pdaPan_                      = newImgPos - newCenter;

                    updatePdaPreview();
                    break;
                }

                break;
            }

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mp(static_cast<float>(event.mouseButton.x),
                                    static_cast<float>(event.mouseButton.y));
                    handleMousePressed(mp);
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mp(static_cast<float>(event.mouseButton.x),
                                    static_cast<float>(event.mouseButton.y));
                    handleMouseReleased(mp);
                }
                break;

            case sf::Event::MouseMoved: {
                sf::Vector2f mp(static_cast<float>(event.mouseMove.x),
                                static_cast<float>(event.mouseMove.y));
                handleMouseMoved(mp);
                break;
            }

            case sf::Event::Resized: {
                sf::View view(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width),
                                            static_cast<float>(event.size.height)));
                window_.setView(view);

                applyLayout(static_cast<float>(event.size.width),
                            static_cast<float>(event.size.height));
                break;
            }

            default:
                break;
        }
    }
}

void ExpressionApp::applyLayout(float w, float h) {
    constexpr float baseW = 1200.f;
    constexpr float baseH = 700.f;

    float           s     = std::min(w / baseW, h / baseH);
    s                     = std::clamp(s, 0.75f, 2.25f);

    auto X                = [&](float v) { return v * s; };
    auto Y                = [&](float v) { return v * s; };
    auto P                = [&](float x, float y) { return sf::Vector2f(X(x), Y(y)); };
    auto S                = [&](float x, float y) { return sf::Vector2f(X(x), Y(y)); };

    cardShadow_.setPosition(P(46.f, 36.f));
    cardShadow_.setSize(S(1108.f, 630.f));

    card_.setPosition(P(40.f, 30.f));
    card_.setSize(S(1108.f, 630.f));

    inputLabel_.setCharacterSize(static_cast<unsigned>(std::round(18.f * s)));
    inputLabel_.setPosition(P(70.f, 70.f));

    inputBox_.setPosition(P(70.f, 105.f));
    inputBox_.setSize(S(740.f, 46.f));

    inputInner_.setPosition(inputBox_.getPosition() + sf::Vector2f(X(2.f), Y(2.f)));
    inputInner_.setSize(inputBox_.getSize() - sf::Vector2f(X(4.f), Y(4.f)));

    inputText_.setCharacterSize(static_cast<unsigned>(std::round(20.f * s)));
    inputText_.setPosition(inputBox_.getPosition().x + X(12.f),
                           inputBox_.getPosition().y + Y(10.f));

    selectionRect_.setSize(S(0.f, 26.f));
    caretRect_.setSize(S(2.f, 24.f));

    auto reCenterButtonLabel = [&](Button& b) {
        const sf::FloatRect lb = b.label.getLocalBounds();
        b.label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        sf::Vector2f pos  = b.box.getPosition();
        sf::Vector2f size = b.box.getSize();
        b.label.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
    };

    auto setButton = [&](Button& b, sf::Vector2f pos, sf::Vector2f size) {
        b.box.setPosition(pos);
        b.box.setSize(size);
        b.label.setCharacterSize(static_cast<unsigned>(std::round(18.f * s)));
        reCenterButtonLabel(b);
    };

    setButton(evalButton_, P(70.f, 170.f), S(120.f, 44.f));
    setButton(cykButton_, P(205.f, 170.f), S(120.f, 44.f));
    setButton(astButton_, P(340.f, 170.f), S(120.f, 44.f));
    setButton(pdaButton_, P(475.f, 170.f), S(130.f, 44.f));
    setButton(pdaSimButton_, P(620.f, 170.f), S(130.f, 44.f));

    resultText_.setCharacterSize(static_cast<unsigned>(std::round(22.f * s)));
    resultText_.setPosition(P(70.f, 240.f));

    statusPanel_.setPosition(P(70.f, 290.f));
    statusPanel_.setSize(S(250.f, 340.f));

    statusText_.setCharacterSize(static_cast<unsigned>(std::round(14.f * s)));
    statusText_.setPosition(statusPanel_.getPosition().x + X(12.f),
                            statusPanel_.getPosition().y + Y(12.f));

    astLabel_.setCharacterSize(static_cast<unsigned>(std::round(14.f * s)));
    astLabel_.setPosition(P(340.f, 290.f));

    astPanel_.setPosition(P(340.f, 315.f));
    astPanel_.setSize(S(290.f, 315.f));

    astPanelInner_.setPosition(astPanel_.getPosition() + sf::Vector2f(X(6.f), Y(6.f)));
    astPanelInner_.setSize(astPanel_.getSize() - sf::Vector2f(X(12.f), Y(12.f)));

    pdaLabel_.setCharacterSize(static_cast<unsigned>(std::round(14.f * s)));
    pdaLabel_.setPosition(P(650.f, 290.f));

    pdaPanel_.setPosition(P(650.f, 315.f));
    pdaPanel_.setSize(S(390.f, 315.f));

    pdaPanelInner_.setPosition(pdaPanel_.getPosition() + sf::Vector2f(X(6.f), Y(6.f)));
    pdaPanelInner_.setSize(pdaPanel_.getSize() - sf::Vector2f(X(12.f), Y(12.f)));

    rebuildAstViewportTexture();
    rebuildPdaViewportTexture();
    updateWrappedStatus();
}

void ExpressionApp::rebuildAstViewportTexture() {
    const auto inner = astPanelInner_.getSize();
    const auto w     = static_cast<unsigned>(std::max(1.f, inner.x));
    const auto h     = static_cast<unsigned>(std::max(1.f, inner.y));
    astViewportTex_.create(w, h);
    astViewportSprite_.setTexture(astViewportTex_.getTexture(), true);
    astViewportSprite_.setPosition(astPanelInner_.getPosition());
}

void ExpressionApp::rebuildPdaViewportTexture() {
    const auto inner = pdaPanelInner_.getSize();
    const auto w     = static_cast<unsigned>(std::max(1.f, inner.x));
    const auto h     = static_cast<unsigned>(std::max(1.f, inner.y));
    pdaViewportTex_.create(w, h);
    pdaViewportSprite_.setTexture(pdaViewportTex_.getTexture(), true);
    pdaViewportSprite_.setPosition(pdaPanelInner_.getPosition());
}

void ExpressionApp::render() {
    window_.draw(cardShadow_);
    window_.draw(card_);

    window_.draw(inputLabel_);
    window_.draw(inputBox_);
    window_.draw(inputInner_);

    if (inputFocused_ && hasSelection()) {
        const std::size_t  a        = std::min(selAnchor_, selCaret_);
        const std::size_t  b        = std::max(selAnchor_, selCaret_);

        const sf::Vector2f pA       = inputText_.findCharacterPos(static_cast<std::uint32_t>(a));
        const sf::Vector2f pB       = inputText_.findCharacterPos(static_cast<std::uint32_t>(b));

        float              left     = pA.x - textScrollX_;
        float              right    = pB.x - textScrollX_;

        const float        boxLeft  = inputBox_.getPosition().x + 10.f;
        const float        boxRight = inputBox_.getPosition().x + inputBox_.getSize().x - 10.f;

        left                        = std::clamp(left, boxLeft, boxRight);
        right                       = std::clamp(right, boxLeft, boxRight);

        float top                   = inputText_.getPosition().y;
        selectionRect_.setPosition({left, top + 3.f});
        selectionRect_.setSize({std::max(0.f, right - left), 26.f});
        window_.draw(selectionRect_);
    }

    sf::Text t = inputText_;
    t.move(-textScrollX_, 0.f);
    window_.draw(t);

    if (inputFocused_ && caretVisible_) {
        sf::Vector2f cp   = inputText_.findCharacterPos(static_cast<std::uint32_t>(caretIndex_));
        float        cx   = cp.x - textScrollX_;

        float        minX = inputBox_.getPosition().x + 10.f;
        float        maxX = inputBox_.getPosition().x + inputBox_.getSize().x - 10.f;
        cx                = std::clamp(cx, minX, maxX);

        caretRect_.setPosition({cx, inputText_.getPosition().y + 3.f});
        window_.draw(caretRect_);
    }

    window_.draw(evalButton_.box);
    window_.draw(evalButton_.label);
    window_.draw(cykButton_.box);
    window_.draw(cykButton_.label);
    window_.draw(astButton_.box);
    window_.draw(astButton_.label);
    window_.draw(pdaButton_.box);
    window_.draw(pdaButton_.label);
    window_.draw(pdaSimButton_.box);
    window_.draw(pdaSimButton_.label);

    window_.draw(resultText_);
    window_.draw(statusPanel_);
    window_.draw(statusText_);

    window_.draw(astLabel_);
    window_.draw(astPanel_);
    window_.draw(astPanelInner_);
    updateAstPreview();
    window_.draw(astViewportSprite_);

    window_.draw(pdaLabel_);
    window_.draw(pdaPanel_);
    window_.draw(pdaPanelInner_);
    updatePdaPreview();
    window_.draw(pdaViewportSprite_);
}

void ExpressionApp::setStatus(const std::string& msg, bool good) {
    statusGood_ = good;
    statusRaw_  = msg;

    statusPanel_.setOutlineColor(good ? sf::Color(90, 140, 110) : sf::Color(160, 90, 90));
    statusText_.setFillColor(good ? sf::Color(190, 245, 210) : sf::Color(255, 190, 190));

    updateWrappedStatus();
}

void ExpressionApp::updateWrappedStatus() {
    float maxW = statusPanel_.getSize().x - 24.f;
    statusText_.setString(wrapTextToWidth(font_, statusText_.getCharacterSize(), statusRaw_, maxW));

    const float maxH = statusPanel_.getSize().y - 24.f;
    sf::Text    tmp  = statusText_;
    while (tmp.getLocalBounds().height > maxH) {
        std::string s   = tmp.getString();
        auto        pos = s.find('\n');
        if (pos == std::string::npos)
            break;
        s = "…\n" + s.substr(pos + 1);
        tmp.setString(s);
        statusText_.setString(s);
    }
}

std::string ExpressionApp::wrapTextToWidth(const sf::Font& font, unsigned charSize,
                                           const std::string& text, float maxWidth) {
    std::stringstream in(text);
    std::string       line;
    std::string       out;

    auto              measure = [&](const std::string& s) -> float {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(charSize);
        t.setString(s);
        return t.getLocalBounds().width;
    };

    bool firstPara = true;
    while (std::getline(in, line)) {
        if (!firstPara)
            out += "\n";
        firstPara = false;

        std::stringstream words(line);
        std::string       word;
        std::string       cur;

        while (words >> word) {
            std::string candidate = cur.empty() ? word : (cur + " " + word);
            if (measure(candidate) <= maxWidth) {
                cur = candidate;
            } else {
                if (!cur.empty())
                    out += cur + "\n";
                if (measure(word) > maxWidth) {
                    std::string chunk;
                    for (char c : word) {
                        std::string cand = chunk + c;
                        if (measure(cand) <= maxWidth)
                            chunk = cand;
                        else {
                            if (!chunk.empty())
                                out += chunk + "\n";
                            chunk = std::string(1, c);
                        }
                    }
                    cur = chunk;
                } else {
                    cur = word;
                }
            }
        }
        out += cur;
    }
    return out;
}

void ExpressionApp::focusInput(bool focus) {
    inputFocused_ = focus;
    inputBox_.setOutlineColor(focus ? sf::Color(60, 140, 255) : sf::Color(95, 105, 125));
    caretVisible_ = true;
    caretBlinkClock_.restart();
    ensureCaretVisible();
}

bool ExpressionApp::hasSelection() const { return selAnchor_ != selCaret_; }

void ExpressionApp::clearSelection() { selAnchor_ = selCaret_ = caretIndex_; }

void ExpressionApp::selectAll() {
    selAnchor_  = 0;
    selCaret_   = inputBuffer_.size();
    caretIndex_ = selCaret_;
    ensureCaretVisible();
}

std::string ExpressionApp::getSelectedText() const {
    if (!hasSelection())
        return "";
    std::size_t a = std::min(selAnchor_, selCaret_);
    std::size_t b = std::max(selAnchor_, selCaret_);
    return inputBuffer_.substr(a, b - a);
}

void ExpressionApp::deleteSelectionIfAny() {
    if (!hasSelection())
        return;
    std::size_t a = std::min(selAnchor_, selCaret_);
    std::size_t b = std::max(selAnchor_, selCaret_);
    inputBuffer_.erase(a, b - a);
    caretIndex_ = a;
    clearSelection();
}

void ExpressionApp::insertTextAtCaret(const std::string& s) {
    deleteSelectionIfAny();
    inputBuffer_.insert(caretIndex_, s);
    caretIndex_ += s.size();
    clearSelection();
    inputText_.setString(inputBuffer_);
    ensureCaretVisible();
}

void ExpressionApp::backspace() {
    if (hasSelection()) {
        deleteSelectionIfAny();
    } else if (caretIndex_ > 0 && !inputBuffer_.empty()) {
        inputBuffer_.erase(caretIndex_ - 1, 1);
        caretIndex_--;
    }
    clearSelection();
    inputText_.setString(inputBuffer_);
    ensureCaretVisible();
}

void ExpressionApp::del() {
    if (hasSelection())
        deleteSelectionIfAny();
    else if (caretIndex_ < inputBuffer_.size())
        inputBuffer_.erase(caretIndex_, 1);
    clearSelection();
    inputText_.setString(inputBuffer_);
    ensureCaretVisible();
}

std::size_t ExpressionApp::findWordLeft(const std::string& s, std::size_t pos) {
    if (pos == 0)
        return 0;
    std::size_t i = std::min(pos, s.size());
    while (i > 0 && std::isspace(static_cast<unsigned char>(s[i - 1])))
        --i;
    while (i > 0 && !std::isspace(static_cast<unsigned char>(s[i - 1])))
        --i;
    return i;
}

std::size_t ExpressionApp::findWordRight(const std::string& s, std::size_t pos) {
    std::size_t i = std::min(pos, s.size());
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
        ++i;
    while (i < s.size() && !std::isspace(static_cast<unsigned char>(s[i])))
        ++i;
    return i;
}

void ExpressionApp::moveCaretLeft(bool ctrl, bool shift) {
    const auto next = ctrl ? findWordLeft(inputBuffer_, caretIndex_)
                           : (caretIndex_ == 0 ? 0 : caretIndex_ - 1);

    caretIndex_     = next;
    if (shift)
        selCaret_ = caretIndex_;
    else
        clearSelection();
    ensureCaretVisible();
}

void ExpressionApp::moveCaretRight(bool ctrl, bool shift) {
    const auto next = ctrl ? findWordRight(inputBuffer_, caretIndex_)
                           : std::min(inputBuffer_.size(), caretIndex_ + 1);

    caretIndex_     = next;
    if (shift)
        selCaret_ = caretIndex_;
    else
        clearSelection();
    ensureCaretVisible();
}

void ExpressionApp::moveCaretHome(bool shift) {
    caretIndex_ = 0;
    if (shift)
        selCaret_ = caretIndex_;
    else
        clearSelection();
    ensureCaretVisible();
}

void ExpressionApp::moveCaretEnd(bool shift) {
    caretIndex_ = inputBuffer_.size();
    if (shift)
        selCaret_ = caretIndex_;
    else
        clearSelection();
    ensureCaretVisible();
}

void ExpressionApp::handleTextEntered(const sf::Event::TextEvent& text) {
    const sf::Uint32 code = text.unicode;
    if (code >= 32 && code != 127) {
        if (code < 128)
            insertTextAtCaret(std::string(1, static_cast<char>(code)));
    }
}

void ExpressionApp::handleKeyPressed(const sf::Event::KeyEvent& key) {
    const bool ctrl  = key.control;
    const bool shift = key.shift;

    caretVisible_    = true;
    caretBlinkClock_.restart();

    if (ctrl && key.code == sf::Keyboard::A) {
        selectAll();
        return;
    }
    if (ctrl && key.code == sf::Keyboard::C) {
        sf::Clipboard::setString(getSelectedText());
        return;
    }
    if (ctrl && key.code == sf::Keyboard::X) {
        sf::Clipboard::setString(getSelectedText());
        deleteSelectionIfAny();
        inputText_.setString(inputBuffer_);
        ensureCaretVisible();
        return;
    }
    if (ctrl && key.code == sf::Keyboard::V) {
        sf::String  clip   = sf::Clipboard::getString();
        std::string pasted = clip.toAnsiString();
        if (!pasted.empty())
            insertTextAtCaret(pasted);
        return;
    }

    switch (key.code) {
        case sf::Keyboard::Left:
            moveCaretLeft(ctrl, shift);
            break;
        case sf::Keyboard::Right:
            moveCaretRight(ctrl, shift);
            break;
        case sf::Keyboard::Home:
            moveCaretHome(shift);
            break;
        case sf::Keyboard::End:
            moveCaretEnd(shift);
            break;
        case sf::Keyboard::BackSpace:
            backspace();
            break;
        case sf::Keyboard::Delete:
            del();
            break;

        case sf::Keyboard::Return:
            onEvaluateClicked();
            break;

        case sf::Keyboard::Escape:
            focusInput(false);
            break;

        default:
            break;
    }
}

std::size_t ExpressionApp::caretFromMouseX(const float mouseX) const {
    constexpr float pad = 12.f;
    float           x   = mouseX - (inputBox_.getPosition().x + pad) + textScrollX_;
    if (x <= 0.f)
        return 0;

    sf::Text measureText = inputText_;
    measureText.setString(inputBuffer_);

    std::size_t best     = inputBuffer_.size();
    float       bestDist = std::numeric_limits<float>::infinity();

    for (std::size_t i = 0; i <= inputBuffer_.size(); ++i) {
        sf::Vector2f p  = measureText.findCharacterPos(static_cast<std::uint32_t>(i));
        float        px = p.x - measureText.getPosition().x;
        float        d  = std::fabs(px - x);

        if (d < bestDist) {
            bestDist = d;
            best     = i;
        }
    }
    return best;
}

void ExpressionApp::ensureCaretVisible() {
    constexpr float padLeft  = 12.f;
    constexpr float padRight = 12.f;

    inputText_.setString(inputBuffer_);
    sf::Vector2f cp        = inputText_.findCharacterPos(static_cast<std::uint32_t>(caretIndex_));
    float        caretX    = cp.x - inputText_.getPosition().x;

    float        viewLeft  = textScrollX_;
    float        viewRight = textScrollX_ + (inputBox_.getSize().x - (padLeft + padRight));

    if (caretX < viewLeft)
        textScrollX_ = caretX;
    else if (caretX > viewRight)
        textScrollX_ = caretX - (inputBox_.getSize().x - (padLeft + padRight));
    textScrollX_ = std::max(0.f, textScrollX_);
}

void ExpressionApp::handleMousePressed(sf::Vector2f mousePos) {
    evalButton_.pressed   = evalButton_.contains(mousePos);
    cykButton_.pressed    = cykButton_.contains(mousePos);
    astButton_.pressed    = astButton_.contains(mousePos);
    pdaButton_.pressed    = pdaButton_.contains(mousePos);
    pdaSimButton_.pressed = pdaSimButton_.contains(mousePos);

    if (isMouseOverAstPanel(mousePos) && astHasImage_) {
        astDragging_ = true;
        astDragLast_ = mousePos;
    }

    if (isMouseOverPdaPanel(mousePos) && pdaHasImage_) {
        pdaDragging_ = true;
        pdaDragLast_ = mousePos;
    }

    if (inputBox_.getGlobalBounds().contains(mousePos)) {
        focusInput(true);

        caretIndex_ = caretFromMouseX(mousePos.x);
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
            selAnchor_ = selCaret_ = caretIndex_;
        } else {
            selCaret_ = caretIndex_;
        }

        selecting_    = true;
        caretVisible_ = true;
        caretBlinkClock_.restart();
        ensureCaretVisible();
    } else {
        if (!isMouseOverAstPanel(mousePos) && !isMouseOverPdaPanel(mousePos))
            focusInput(false);
        selecting_ = false;
    }
}

void ExpressionApp::handleMouseReleased(const sf::Vector2f mousePos) {
    if (evalButton_.pressed && evalButton_.contains(mousePos))
        onEvaluateClicked();
    if (cykButton_.pressed && cykButton_.contains(mousePos))
        onCykClicked();
    if (astButton_.pressed && astButton_.contains(mousePos))
        onAstImageClicked();
    if (pdaButton_.pressed && pdaButton_.contains(mousePos))
        onPdaImageClicked();
    if (pdaSimButton_.pressed && pdaSimButton_.contains(mousePos))
        onPdaValidateClicked();

    evalButton_.pressed   = false;
    cykButton_.pressed    = false;
    astButton_.pressed    = false;
    pdaButton_.pressed    = false;
    pdaSimButton_.pressed = false;

    selecting_            = false;
    astDragging_          = false;
    pdaDragging_          = false;
}

void ExpressionApp::handleMouseMoved(sf::Vector2f mousePos) {
    evalButton_.hovered    = evalButton_.contains(mousePos);
    cykButton_.hovered     = cykButton_.contains(mousePos);
    astButton_.hovered     = astButton_.contains(mousePos);
    pdaButton_.hovered     = pdaButton_.contains(mousePos);
    pdaSimButton_.hovered  = pdaSimButton_.contains(mousePos);

    auto updateButtonStyle = [&](Button& b) {
        sf::Color base  = sf::Color(55, 60, 75);
        sf::Color hover = sf::Color(70, 76, 95);
        sf::Color press = sf::Color(40, 44, 58);

        if (b.pressed)
            b.box.setFillColor(press);
        else if (b.hovered)
            b.box.setFillColor(hover);
        else
            b.box.setFillColor(base);

        b.box.setOutlineColor(b.hovered ? sf::Color(120, 130, 160) : sf::Color(90, 95, 115));
    };

    updateButtonStyle(evalButton_);
    updateButtonStyle(cykButton_);
    updateButtonStyle(astButton_);
    updateButtonStyle(pdaButton_);
    updateButtonStyle(pdaSimButton_);

    if (inputFocused_ && selecting_) {
        if (inputBox_.getGlobalBounds().contains(mousePos)) {
            caretIndex_   = caretFromMouseX(mousePos.x);
            selCaret_     = caretIndex_;
            caretVisible_ = true;
            caretBlinkClock_.restart();
            ensureCaretVisible();
        }
    }

    if (astDragging_ && astHasImage_) {
        const sf::Vector2f delta = mousePos - astDragLast_;
        astDragLast_             = mousePos;
        astPan_ += delta;
    }

    if (pdaDragging_ && pdaHasImage_) {
        sf::Vector2f delta = mousePos - pdaDragLast_;
        pdaDragLast_       = mousePos;
        pdaPan_ += delta;
    }
}

bool ExpressionApp::parseToAst(const std::string& input, std::unique_ptr<expr::ASTNode>& outAst,
                               std::string& errorMessage) const {
    const std::vector<std::string> tokens = Tokenize::tokenizeForParser(input, cykOps_);
    if (tokens.empty()) {
        errorMessage = "Cannot tokenize the input.";
        return false;
    }

    std::ostringstream parseLog;
    auto               ast = parser_.parse(tokens, parseLog);
    if (!ast) {
        errorMessage = "Parse error:\n" + parseLog.str();
        return false;
    }

    outAst = std::move(ast);
    return true;
}

bool ExpressionApp::evaluateAst(const expr::ASTNode& ast, double& result,
                                std::string& errorMessage) const {
    try {
        result = ast.evaluateWith(env_);
        return true;
    } catch (const std::exception& e) {
        errorMessage = std::string("Evaluation error: ") + e.what();
        return false;
    }
}

void ExpressionApp::onEvaluateClicked() {
    if (inputBuffer_.empty()) {
        resultText_.setString("");
        setStatus("No expression filled in.", false);
        return;
    }

    std::unique_ptr<expr::ASTNode> ast;
    std::string                    err;

    if (!parseToAst(inputBuffer_, ast, err)) {
        resultText_.setString("");
        setStatus(err, false);
        lastAst_.reset();
        astHasImage_ = false;
        return;
    }

    lastAst_      = std::move(ast);

    double result = 0.0;
    if (evaluateAst(*lastAst_, result, err)) {
        std::ostringstream oss;
        oss << "Result: " << result;
        resultText_.setString(oss.str());
        setStatus("OK. Press 'AST Image' or 'PDA Sim' to visualize", true);
    } else {
        resultText_.setString("");
        setStatus(err, false);
    }
}

void ExpressionApp::onCykClicked() {
    if (inputBuffer_.empty()) {
        setStatus("No word given for CYK.", false);
        return;
    }

    std::string message;
    if (runCykAnalysis(inputBuffer_, message))
        setStatus("CYK: " + message + " (see console)", cykLastAccepted_);
    else
        setStatus("CYK error: " + message, false);
}

bool ExpressionApp::runCykAnalysis(const std::string& word, std::string& message) {
    try {
        if (!cykIsInCnf_) {
            cykCfg_.toCNF();
            cykIsInCnf_ = true;
        }

        const std::vector<std::string> cykTokens = Tokenize::tokenizeToCykSymbols(word, cykOps_);
        if (cykTokens.empty()) {
            message          = "Could not tokenize input for CYK.";
            cykLastAccepted_ = false;
            return false;
        }

        auto pre         = CFG::preprocessImplicitMulForCYK_NUM_LPAREN(cykTokens);
        cykLastAccepted_ = cykCfg_.acceptsTokens(pre);

        if (cykLastAccepted_)
            message = "word \"" + word + "\" is ACCEPTED by the CFG.";
        else
            message = "word \"" + word + "\" is REJECTED by the CFG.";

        return true;
    } catch (const std::exception& e) {
        message          = e.what();
        cykLastAccepted_ = false;
        return false;
    }
}

bool ExpressionApp::isMouseOverAstPanel(sf::Vector2f p) const {
    return astPanel_.getGlobalBounds().contains(p) || astPanelInner_.getGlobalBounds().contains(p);
}

bool ExpressionApp::isMouseOverPdaPanel(sf::Vector2f p) const {
    return pdaPanel_.getGlobalBounds().contains(p) || pdaPanelInner_.getGlobalBounds().contains(p);
}

void ExpressionApp::onAstImageClicked() {
    if (inputBuffer_.empty()) {
        astHasImage_ = false;
        setStatus("No expression: AST image not generated.", false);
        return;
    }

    if (!lastAst_) {
        setStatus("No AST available. First click 'Evaluate' to parse.", false);
        return;
    }

    try {
        const std::string filename = "ast_diagram.png";
        lastAst_->exportToImage(filename);

        if (!astImageTexture_.loadFromFile(filename)) {
            astHasImage_ = false;
            setStatus("AST image generated, but SFML could not load it", false);
            return;
        }

        astImageSprite_.setTexture(astImageTexture_, true);
        astZoom_     = 1.f;
        astPan_      = {0.f, 0.f};
        astHasImage_ = true;

        setStatus("AST image generated. Wheel to zoom, drag to pan.", true);
    } catch (const std::exception& e) {
        astHasImage_ = false;
        setStatus(std::string("Failed to generate AST image: ") + e.what(), false);
    }
}

void ExpressionApp::onPdaImageClicked() {
    if (inputBuffer_.empty()) {
        pdaHasImage_ = false;
        setStatus("No expression: PDA diagram not generated.", false);
        return;
    }

    try {
        const std::string filename = "pda_diagram.png";
        pda_.exportToImage(filename);

        if (!pdaImageTexture_.loadFromFile(filename)) {
            pdaHasImage_ = false;
            setStatus("PDA diagram generated, but SFML could not load it", false);
            return;
        }

        pdaImageSprite_.setTexture(pdaImageTexture_, true);
        pdaZoom_     = 1.f;
        pdaPan_      = {0.f, 0.f};
        pdaHasImage_ = true;

        setStatus("PDA diagram generated. Wheel to zoom, drag to pan.", true);
    } catch (const std::exception& e) {
        pdaHasImage_ = false;
        setStatus(std::string("Failed to generate PDA diagram: ") + e.what(), false);
    }
}

void ExpressionApp::onPdaValidateClicked() {
    if (inputBuffer_.empty()) {
        setStatus("No expression for PDA simulation.", false);
        return;
    }

    setStatus("Running PDA simulation on input...", true);

    // Tokenize input to CYK symbols (same as CYK uses)
    auto tokens = Tokenize::tokenizeToCykSymbols(inputBuffer_, cykOps_);

    if (tokens.empty()) {
        setStatus("Could not tokenize input for PDA simulation.", false);
        return;
    }

    // Run PDA simulation with tokens
    bool accepted = pda_.simulateCykSymbols(tokens);

    // Get simulation steps
    auto steps = pda_.getLastSimulation();

    // Format token display
    std::ostringstream tokenDisplay;
    tokenDisplay << "[";
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i > 0)
            tokenDisplay << " ";
        tokenDisplay << tokens[i];
    }
    tokenDisplay << "]";

    if (accepted) {
        std::ostringstream msg;
        msg << "PDA ACCEPTED: " << tokenDisplay.str() << " in " << steps.size() << " steps!";
        setStatus(msg.str(), true);

        // Show detailed trace in console
        std::cout << "\n=== PDA SIMULATION TRACE ===" << std::endl;
        std::cout << "Input tokens: " << tokenDisplay.str() << std::endl;
        for (const auto& step : steps) {
            std::cout << "Step " << step.stepNumber << ": "
                      << "State=" << step.currentState << " | Stack=[";
            for (size_t i = 0; i < step.stackContent.size(); ++i) {
                if (i > 0)
                    std::cout << "|";
                std::cout << step.stackContent[i];
            }
            std::cout << "] | Remaining=\"" << step.inputRemaining << "\" | Action=" << step.action
                      << std::endl;
        }
        std::cout << "=========================\n" << std::endl;

    } else {
        std::string errorMsg = pda_.getErrorMessage();
        if (errorMsg.empty())
            errorMsg = "PDA rejected after " + std::to_string(steps.size()) + " steps.";
        setStatus("PDA REJECTED: " + errorMsg + " Input was: " + tokenDisplay.str(), false);

        // Show trace in console even for rejection
        std::cout << "\n=== PDA SIMULATION TRACE (REJECTED) ===" << std::endl;
        std::cout << "Input tokens: " << tokenDisplay.str() << std::endl;
        for (const auto& step : steps) {
            std::cout << "Step " << step.stepNumber << ": "
                      << "State=" << step.currentState << " | Stack=[";
            for (size_t i = 0; i < step.stackContent.size(); ++i) {
                if (i > 0)
                    std::cout << "|";
                std::cout << step.stackContent[i];
            }
            std::cout << "] | Remaining=\"" << step.inputRemaining << "\" | Action=" << step.action
                      << std::endl;
        }
        std::cout << "=====================================\n" << std::endl;
    }

    // Generate simulation visualization (always, even for rejection)
    try {
        pda_.exportSimulationToImage("pda_simulation.png");
        if (pdaImageTexture_.loadFromFile("pda_simulation.png")) {
            pdaImageSprite_.setTexture(pdaImageTexture_, true);
            pdaZoom_     = 1.f;
            pdaPan_      = {0.f, 0.f};
            pdaHasImage_ = true;

            // Update status to mention visualization
            std::string currentStatus = statusRaw_;
            currentStatus += " Check PDA panel for step visualization.";
            setStatus(currentStatus, accepted);
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not generate PDA simulation visualization: " << e.what()
                  << std::endl;
    }
}

std::string ExpressionApp::formatTokens(const std::vector<std::string>& tokens) {
    std::string result;
    for (const auto& t : tokens) {
        if (!result.empty())
            result += " ";
        result += t;
    }
    return "[" + result + "]";
}

void ExpressionApp::updatePdaPreview() {
    pdaViewportTex_.clear(sf::Color(12, 13, 16));

    if (!pdaHasImage_) {
        sf::Text hint;
        hint.setFont(font_);
        hint.setCharacterSize(12);
        hint.setFillColor(sf::Color(150, 155, 165));
        hint.setString("No PDA diagram.\nPress 'PDA Diagram'");
        hint.setPosition(12.f, 12.f);
        pdaViewportTex_.draw(hint);
        pdaViewportTex_.display();
        pdaViewportSprite_.setTexture(pdaViewportTex_.getTexture(), true);
        pdaViewportSprite_.setPosition(pdaPanelInner_.getPosition());
        return;
    }

    sf::Vector2u imgSz = pdaImageTexture_.getSize();
    sf::Vector2u vpSz  = pdaViewportTex_.getSize();

    if (imgSz.x == 0 || imgSz.y == 0 || vpSz.x == 0 || vpSz.y == 0) {
        pdaViewportTex_.display();
        return;
    }

    float pad        = 10.f;
    float fitX       = (vpSz.x - 2.f * pad) / static_cast<float>(imgSz.x);
    float fitY       = (vpSz.y - 2.f * pad) / static_cast<float>(imgSz.y);
    float baseScale  = std::min(fitX, fitY);

    float finalScale = baseScale * pdaZoom_;

    pdaImageSprite_.setScale(finalScale, finalScale);

    float        imgW = imgSz.x * finalScale;
    float        imgH = imgSz.y * finalScale;

    sf::Vector2f center((vpSz.x - imgW) * 0.5f, (vpSz.y - imgH) * 0.5f);
    sf::Vector2f pos = center + pdaPan_;

    pdaImageSprite_.setPosition(pos);

    pdaViewportTex_.draw(pdaImageSprite_);
    pdaViewportTex_.display();

    pdaViewportSprite_.setTexture(pdaViewportTex_.getTexture(), true);
    pdaViewportSprite_.setPosition(pdaPanelInner_.getPosition());
}

void ExpressionApp::updateAstPreview() {
    astViewportTex_.clear(sf::Color(12, 13, 16));

    if (!astHasImage_) {
        sf::Text hint;
        hint.setFont(font_);
        hint.setCharacterSize(12);
        hint.setFillColor(sf::Color(150, 155, 165));
        hint.setString("No AST image.\nPress 'AST Image'");
        hint.setPosition(12.f, 12.f);
        astViewportTex_.draw(hint);
        astViewportTex_.display();
        astViewportSprite_.setTexture(astViewportTex_.getTexture(), true);
        astViewportSprite_.setPosition(astPanelInner_.getPosition());
        return;
    }

    sf::Vector2u imgSz = astImageTexture_.getSize();
    sf::Vector2u vpSz  = astViewportTex_.getSize();

    if (imgSz.x == 0 || imgSz.y == 0 || vpSz.x == 0 || vpSz.y == 0) {
        astViewportTex_.display();
        return;
    }

    constexpr float pad        = 10.f;
    const float     fitX       = (vpSz.x - 2.f * pad) / static_cast<float>(imgSz.x);
    const float     fitY       = (vpSz.y - 2.f * pad) / static_cast<float>(imgSz.y);
    const float     baseScale  = std::min(fitX, fitY);

    float           finalScale = baseScale * astZoom_;

    astImageSprite_.setScale(finalScale, finalScale);

    const float        imgW = imgSz.x * finalScale;
    const float        imgH = imgSz.y * finalScale;

    const sf::Vector2f center((vpSz.x - imgW) * 0.5f, (vpSz.y - imgH) * 0.5f);
    const sf::Vector2f pos = center + astPan_;

    astImageSprite_.setPosition(pos);

    astViewportTex_.draw(astImageSprite_);
    astViewportTex_.display();

    astViewportSprite_.setTexture(astViewportTex_.getTexture(), true);
    astViewportSprite_.setPosition(astPanelInner_.getPosition());
}
