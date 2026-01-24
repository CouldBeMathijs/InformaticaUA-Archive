// MainGui.cpp - verbeterde layout zonder overlapping
#include "MainGui.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ui/Card.h"
#include "ui/Button.h"
#include "ui/TextInput.h"
#include "ui/TextArea.h"
#include "ui/Checkbox.h"
#include "ui/Tabs.h"
#include "ui/Label.h"
#include "Application.h"
#include "../TokenMatcher.h"
#include "../Languages.h"
#include <filesystem>

TokenMatcher matcher;

MainGui::MainGui(sf::RenderWindow& win, const sf::Font& f)
        : window(win), font(f)
{
    using namespace std;
    using namespace sf;

    VideoMode desktop = VideoMode::getDesktopMode();
    window.create(desktop, "Talen in Automaten - TA-TOg");

    float margin = 20.f;
    Vector2f screenSize = static_cast<Vector2f>(Vector2u(desktop.width, desktop.height));

    // Titel + subtitel
    auto title = make_shared<Label>("Talen in Automaten - TA-TOg", font, 36);
    title->setPosition({margin, margin});
    elements.push_back(title);

    auto subtitle = make_shared<Label>("Language detection using automata and regular expressions", font, 20);
    subtitle->setPosition({margin, margin + 45});
    elements.push_back(subtitle);

    float topOffset = margin + 90;
    float cardHeaderHeight = 60.f;

    float availableHeight = screenSize.y - topOffset - 3 * margin;
    float leftCardHeight = availableHeight / 2.4f;
    float autoCardHeight = availableHeight / 1.9f;
    float cardWidth = screenSize.x * 0.5f - 1.5f * margin;

    // Tekstinvoer card
    auto textCard = make_shared<Card>(font, "Input field", "Input text here");
    textCard->setPosition({margin, topOffset});
    textCard->setSize({cardWidth, leftCardHeight});

    auto textArea = make_shared<TextArea>("// text here...", font);
    textArea->setPosition({margin + 20, topOffset + cardHeaderHeight});
    textArea->setSize({cardWidth - 40, leftCardHeight - cardHeaderHeight - 60});
    textCard->addChild(textArea);

    // Submit button for text input
    auto textSubmitBtn = make_shared<Button>("Submit", font);
    textSubmitBtn->setSize({100, 30});
    textSubmitBtn->setPosition({margin + 20, topOffset + leftCardHeight - 45});
    textSubmitBtn->onClick = [textArea]() {
        std::string outputPath = "output.png";
        if (std::filesystem::exists(outputPath)) {
            std::filesystem::remove(outputPath);
        }
        std::string text = textArea->getText();
        matcher.loadLanguageDFAs("languages");
        matcher.analyzeText(text);
        matcher.printReport();
        auto data = matcher.getScores();
        if (std::filesystem::exists("output.png")) std::filesystem::remove("output.png");
        Languages::exportToImage(data, "output.png");
        std::cout << "Text submitted and PNG exported.\n";
    };
    textCard->addChild(textSubmitBtn);

    elements.push_back(textCard);

    // Automata visualisatie card
    float visY = topOffset + leftCardHeight + margin;
    auto autoCard = make_shared<Card>(font, "Automaton Visualisation", "Select automata");
    autoCard->setPosition({margin, visY});
    autoCard->setSize({cardWidth, autoCardHeight});

    float checkY = visY + cardHeaderHeight;
    auto autoCheckboxes = std::make_shared<std::vector<std::shared_ptr<Checkbox>>>();
    vector<string> automaten = Application::getSupportedLanguages();

    for (const auto& a : automaten) {
        auto cb = std::make_shared<Checkbox>(font);
        cb->setLabel(a);
        cb->setSize({20, 20});
        cb->setPosition({margin + 30, checkY});
        checkY += 30;
        autoCard->addChild(cb);

        cb->setOnToggle([this, cb, autoCheckboxes, a, autoCard](bool checked) {
            if (checked) {
                for (auto& otherCb : *autoCheckboxes) {
                    if (otherCb != cb) {
                        otherCb->setChecked(false);
                    }
                }
                std::string imgPath = "images/" + a + ".png";
                std::string svgPath = "images/" + a + ".svg";
                currentSvgPath = svgPath;


                if (!languageTexture.loadFromFile(imgPath)) {
                    std::cerr << "Kan afbeelding niet laden: " << imgPath << "\n";
                    languageSprite = sf::Sprite();
                } else {
                    zoomLevel = 1.0f;
                    languageSprite.setTexture(languageTexture);

                    sf::Vector2u texSize = languageTexture.getSize();
                    float texWidth = static_cast<float>(texSize.x);
                    float texHeight = static_cast<float>(texSize.y);

                    sf::Vector2f cardPos = autoCard->getPosition();
                    sf::Vector2f cardSize = autoCard->getSize();

                    float maxW = cardSize.x * 0.6f;
                    float maxH = cardSize.y * 0.9f;

                    float scaleW = maxW / texWidth;
                    float scaleH = maxH / texHeight;

                    baseScale = std::min(scaleW, scaleH);  // kies de beperkende schaal
                    languageSprite.setScale(baseScale, baseScale);

                    // Afbeelding rechtsboven uitlijnen met marge
                    float marginRight = 20.f;
                    float marginTop = 20.f;
                    float scaledWidth = texWidth * baseScale;
                    float spriteX = cardPos.x + cardSize.x - scaledWidth - marginRight;
                    float spriteY = cardPos.y + marginTop;

                    languageSprite.setPosition(spriteX, spriteY);
                }
            } else {
                languageSprite = sf::Sprite();
            }
        });

        autoCheckboxes->push_back(cb);
    }
    elements.push_back(autoCard);

    // Rechterkant (results, talen, regex)
    float rightX = cardWidth + 2 * margin;
    float rightW = screenSize.x - rightX - margin;
    float resultHeight = leftCardHeight;
    float langHeight = availableHeight * 0.15f;
    float regexHeight = availableHeight - resultHeight - langHeight - 3.5f * margin;

    // Resultaat card
    auto resCard = make_shared<Card>(font, "Results", "Recognised results");
    resCard->setPosition({rightX, topOffset});
    resCard->setSize({rightW, resultHeight});
    elements.push_back(resCard);

    // Taal checkbox card
    float langY = topOffset + resultHeight + margin;
    auto langCard = make_shared<Card>(font, "Supported languages", "Detectable languages");
    langCard->setPosition({rightX, langY});
    langCard->setSize({rightW, langHeight});

    float checkLangY = langY + cardHeaderHeight;
    vector<string> talen = Application::getSupportedLanguages();

    std::string allLanguages;
    for (const auto& lang : talen) {
        if (!allLanguages.empty()) allLanguages += ", ";
        allLanguages += lang;
    }
    auto langLabel = std::make_shared<Label>(allLanguages, font, 16);
    langLabel->setPosition({rightX + 30, checkLangY});
    langCard->addChild(langLabel);

    elements.push_back(langCard);

    // Regex editor
    float regexY = langY + langHeight + margin;
    auto regexCard = make_shared<Card>(font, "Regex editor", "Name and expression");
    regexCard->setPosition({rightX, regexY});
    regexCard->setSize({rightW, regexHeight});

    // Name input field
    auto nameInput = make_shared<TextArea>("Name for the expression", font);
    nameInput->setSize({rightW - 40, 30});
    nameInput->setPosition({rightX + 20, regexY + cardHeaderHeight});
    regexCard->addChild(nameInput);

    // Regex input area
    auto regexInput = make_shared<TextArea>("// regex here...", font);
    regexInput->setPosition({rightX + 20, regexY + cardHeaderHeight + 40});
    regexInput->setSize({rightW - 40, regexHeight - cardHeaderHeight - 100});
    regexCard->addChild(regexInput);

    // Submit button for regex
    auto regexSubmitBtn = make_shared<Button>("Submit Regex", font);
    regexSubmitBtn->setSize({150, 30});
    regexSubmitBtn->setPosition({rightX + 20, regexY + regexHeight - 45});
    regexSubmitBtn->onClick = [nameInput, regexInput, this]() {
        Application::saveUserRegexByName(nameInput->getText(),
                                       regexInput->getText());
        this->refresh();
    };
    regexCard->addChild(regexSubmitBtn);

    elements.push_back(regexCard);

    refresh();
}

void MainGui::draw() {
    for (auto& e : elements) e->draw(window);

    if (languageSprite.getTexture()) {
        window.draw(languageSprite);
    }

    // Zoek de "Results" kaart
    sf::Vector2f resultPos;
    sf::Vector2f resultSize;
    float cardHeaderHeight = 60.f;
    for (const auto& e : elements) {
        auto card = std::dynamic_pointer_cast<Card>(e);
        if (card && card->getTitle() == "Results") {
            resultPos = card->getPosition();
            resultSize = card->getSize();
            break;
        }
    }

    std::string outputPath = "output.png";
    if (std::filesystem::exists(outputPath)) {
        static sf::Texture outputTexture;
        static bool textureLoaded = false;

        if (!textureLoaded) {
            if (!outputTexture.loadFromFile(outputPath)) {
                std::filesystem::remove(outputPath);
                std::cerr << "Kon output.png niet laden.\n";
                return;
            }
            textureLoaded = true;
        }

        sf::Vector2u texSize = outputTexture.getSize();
        float texWidth = static_cast<float>(texSize.x);
        float texHeight = static_cast<float>(texSize.y);

        float margin = 10.f;
        float maxWidth = resultSize.x - 2 * margin;
        float maxHeight = resultSize.y - cardHeaderHeight - 2 * margin;

        float scaleX = maxWidth / texWidth;
        float scaleY = maxHeight / texHeight;
        float scale = std::min(scaleX, scaleY);

        outputSprite.setTexture(outputTexture);
        outputSprite.setScale(scale, scale);
        outputSprite.setPosition(resultPos.x + margin, resultPos.y + cardHeaderHeight + margin);

        window.draw(outputSprite);
    }
}

void MainGui::handleEvent(const sf::Event& evt) {
    for (auto& e : elements) e->handleEvent(evt);

    if (evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = { static_cast<float>(evt.mouseButton.x), static_cast<float>(evt.mouseButton.y) };

        if (languageSprite.getGlobalBounds().contains(mousePos)) {
            if (!currentSvgPath.empty()) {
                openSvgInBrowser(currentSvgPath);
            }
        }

        if (outputSprite.getGlobalBounds().contains(mousePos)) {
            std::string imgPath = "output.png";
#ifdef _WIN32
            std::string cmd = "start \"\" \"" + imgPath + "\"";
#elif __APPLE__
            std::string cmd = "open \"" + imgPath + "\"";
#else
            std::string cmd = "xdg-open \"" + imgPath + "\"";
#endif
            std::system(cmd.c_str());
        }
    }

    if (evt.type == sf::Event::MouseWheelScrolled) {
        if (evt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            float oldZoom = zoomLevel;
            float zoomFactor = 1.1f;
            if (evt.mouseWheelScroll.delta > 0) {
                zoomLevel *= zoomFactor;
            } else {
                zoomLevel /= zoomFactor;
                if (zoomLevel < 0.1f) zoomLevel = 0.1f;
            }

            sf::Vector2f mousePos{ float(evt.mouseWheelScroll.x), float(evt.mouseWheelScroll.y) };

            sf::Vector2f spritePos = languageSprite.getPosition();
            sf::Vector2f spriteScale = languageSprite.getScale();

            float baseScaleX = spriteScale.x / oldZoom;
            float baseScaleY = spriteScale.y / oldZoom;

            float newScaleX = baseScaleX * zoomLevel;
            float newScaleY = baseScaleY * zoomLevel;

            sf::Vector2f mouseToSprite = mousePos - spritePos;
            sf::Vector2f newSpritePos = mousePos - (mouseToSprite * (newScaleX / spriteScale.x));

            languageSprite.setScale(newScaleX, newScaleY);
            languageSprite.setPosition(newSpritePos);
        }
    }
}

void MainGui::refresh() {
    updateSupportedLanguages();
}

void MainGui::updateSupportedLanguages() {
    auto talen = Application::getSupportedLanguages();

    for (auto& elem : elements) {
        auto card = std::dynamic_pointer_cast<Card>(elem);
        if (card && card->getTitle() == "Supported languages") {
            float maxWidth = card->getSize().x - 60.f;
            float labelX = card->getPosition().x + 30.f;
            float labelY = card->getPosition().y + 60.f;

            std::string wrappedText;
            std::string currentLine;
            sf::Text tempText("", font, 16);

            for (const auto& lang : talen) {
                std::string word = lang + ", ";
                std::string testLine = currentLine + word;

                tempText.setString(testLine);
                float testWidth = tempText.getLocalBounds().width;

                if (testWidth > maxWidth) {
                    wrappedText += currentLine + "\n";
                    currentLine = word;
                } else {
                    currentLine = testLine;
                }
            }

            wrappedText += currentLine;

            for (auto& child : card->getChildren()) {
                auto label = std::dynamic_pointer_cast<Label>(child);
                if (label) {
                    label->setText(wrappedText);
                    label->setPosition({labelX, labelY});
                    break;
                }
            }
        }

        if (card && card->getTitle() == "Automaton Visualisation") {
            std::cout << "Updating automaton visualisation card\n";

            card->removeChildrenOfType<Checkbox>();

            float margin = 20.f;
            float checkStartX = card->getPosition().x + margin;
            float checkStartY = card->getPosition().y + 60.f;
            float checkX = checkStartX;
            float checkY = checkStartY;

            float checkboxHeight = 30.f;
            float checkboxSpacing = 10.f;
            float maxY = card->getPosition().y + card->getSize().y - margin;

            auto autoCheckboxes = std::make_shared<std::vector<std::shared_ptr<Checkbox>>>();

            for (const auto& a : talen) {
                if (checkY + checkboxHeight > maxY) {
                    checkY = checkStartY;
                    checkX += 150.f; // volgende kolom (pas dit eventueel aan voor jouw layout)
                }

                auto cb = std::make_shared<Checkbox>(font);
                cb->setLabel(a);
                cb->setSize({20, 20});
                cb->setPosition({checkX, checkY});
                checkY += checkboxHeight + checkboxSpacing;

                cb->setOnToggle([this, cb, autoCheckboxes, a, card, margin](bool checked) {
                    if (checked) {
                        for (auto& otherCb : *autoCheckboxes) {
                            if (otherCb != cb) otherCb->setChecked(false);
                        }

                        std::string imgPath = "images/" + a + ".png";
                        std::string svgPath = "images/" + a + ".svg";
                        currentSvgPath = svgPath;


                        if (!languageTexture.loadFromFile(imgPath)) {
                            std::cerr << "Kan afbeelding niet laden: " << imgPath << "\n";
                            languageSprite = sf::Sprite();
                        } else {
                            zoomLevel = 1.0f;
                            languageSprite.setTexture(languageTexture);

                            sf::Vector2u texSize = languageTexture.getSize();
                            float texWidth = static_cast<float>(texSize.x);
                            float texHeight = static_cast<float>(texSize.y);

                            sf::Vector2f cardPos = card->getPosition();
                            sf::Vector2f cardSize = card->getSize();

                            float maxW = cardSize.x * 0.6f;
                            float maxH = cardSize.y * 0.9f;

                            float scaleW = maxW / texWidth;
                            float scaleH = maxH / texHeight;

                            baseScale = std::min(scaleW, scaleH);  // kies de beperkende schaal
                            languageSprite.setScale(baseScale, baseScale);

                            // Afbeelding rechtsboven uitlijnen met marge
                            float marginRight = 20.f;
                            float marginTop = 20.f;
                            float scaledWidth = texWidth * baseScale;
                            float spriteX = cardPos.x + cardSize.x - scaledWidth - marginRight;
                            float spriteY = cardPos.y + marginTop;

                            languageSprite.setPosition(spriteX, spriteY);
                }
                    } else {
                        languageSprite = sf::Sprite();
                    }
                });

                card->addChild(cb);
                autoCheckboxes->push_back(cb);
            }
        }
    }
}

void MainGui::updateSpriteScale() {
    if (languageTexture.getSize().x == 0 || languageTexture.getSize().y == 0) return;

    float baseScale;
    sf::Vector2u texSize = languageTexture.getSize();

    float imageSize = 300.f;

    if (texSize.x > texSize.y)
        baseScale = imageSize / texSize.x;
    else
        baseScale = imageSize / texSize.y;

    languageSprite.setScale(baseScale * zoomLevel, baseScale * zoomLevel);
}

void MainGui::openSvgInBrowser(const std::string& svgPath) {
#ifdef _WIN32
    std::string cmd = "start \"\" \"" + svgPath + "\"";
#elif __APPLE__
    std::string cmd = "open \"" + svgPath + "\"";
#else
    std::string cmd = "xdg-open \"" + svgPath + "\"";
#endif
    std::system(cmd.c_str());
}
