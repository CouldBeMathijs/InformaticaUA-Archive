#include "TextArea.h"
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>
#include <sstream>

TextArea::TextArea(const std::string& ph, const sf::Font& font) {
    box.setFillColor({30,30,30});
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1.f);

    placeholder.setFont(font);
    placeholder.setString(ph);
    placeholder.setCharacterSize(16);
    placeholder.setFillColor({100,100,100});

    contentText.setFont(font);
    contentText.setCharacterSize(16);
    contentText.setFillColor(sf::Color::White);
}

void TextArea::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    placeholder.setPosition(pos + sf::Vector2f{5,5});
    contentText.setPosition(pos + sf::Vector2f{5,5});
}

void TextArea::setSize(sf::Vector2f size) {
    box.setSize(size);
}

void TextArea::draw(sf::RenderTarget& target) {
    target.draw(box);

    if (content.empty() && !hasFocus) {
        target.draw(placeholder);
        return;
    }

    // Render selectie achtergrond
    // Render meerrijige selectie
    if (selectionStart != selectionEnd) {
        int selStart = std::min(selectionStart, selectionEnd);
        int selEnd = std::max(selectionStart, selectionEnd);

        float availW = box.getSize().x - 10.f;
        std::string wrappedText = wrapText(content, contentText, availW);
        std::istringstream stream(wrappedText);

        float lineHeight = contentText.getCharacterSize() + 4;
        float baseX = contentText.getPosition().x;
        float baseY = contentText.getPosition().y;
        float cursorY = baseY;

        int currentCharIndex = 0;
        std::string line;
        while (std::getline(stream, line)) {
            int lineStart = currentCharIndex;
            int lineEnd = currentCharIndex + line.size();

            // Check of deze lijn overlapt met selectie
            int drawStart = std::max(lineStart, selStart);
            int drawEnd = std::min(lineEnd, selEnd);
            if (drawStart < drawEnd) {
                sf::Text temp = contentText;
                temp.setString(line.substr(0, drawStart - lineStart));
                float highlightX = baseX + temp.getLocalBounds().width;

                temp.setString(line.substr(drawStart - lineStart, drawEnd - drawStart));
                float highlightW = temp.getLocalBounds().width;

                sf::RectangleShape highlight({highlightW, lineHeight});
                highlight.setFillColor(sf::Color(100, 100, 255, 100));
                highlight.setPosition({highlightX, cursorY});
                target.draw(highlight);
            }

            currentCharIndex += line.size() + 1; // +1 voor '\n'
            cursorY += lineHeight;
        }
    }

    // Render de gewrapte content
    target.draw(contentText);

    // Render cursor (in meerdere regels ondersteund)
    if (hasFocus) {
        float availW = box.getSize().x - 10.f;
        std::string wrappedText = wrapText(content, contentText, availW);

        std::istringstream stream(wrappedText);
        std::string line;
        float cursorY = contentText.getPosition().y;
        float lineHeight = contentText.getCharacterSize() + 4;
        int currentCharIndex = 0;
        float cursorX = contentText.getPosition().x;

        while (std::getline(stream, line)) {
            if (cursorIndex <= currentCharIndex + line.size()) {
                sf::Text temp = contentText;
                temp.setString(line.substr(0, cursorIndex - currentCharIndex));
                cursorX += temp.getLocalBounds().width;
                break;
            }
            currentCharIndex += line.size() + 1; // +1 voor '\n'
            cursorY += lineHeight;
        }

        sf::RectangleShape cursor({1, (float)contentText.getCharacterSize() + 2});
        cursor.setFillColor(sf::Color::White);
        cursor.setPosition({cursorX, cursorY});
        target.draw(cursor);
    }
}

void TextArea::handleEvent(const sf::Event& evt) {
    if (evt.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f m{ float(evt.mouseButton.x), float(evt.mouseButton.y) };
        hasFocus = box.getGlobalBounds().contains(m);
    }

    if (!hasFocus) return;

    if (evt.type == sf::Event::KeyPressed && evt.key.control) {
        if (evt.key.code == sf::Keyboard::A) {
            selectionStart = 0;
            selectionEnd = content.size();
        } else if (evt.key.code == sf::Keyboard::C && selectionStart != selectionEnd) {
            int start = std::min(selectionStart, selectionEnd);
            int end = std::max(selectionStart, selectionEnd);
            sf::Clipboard::setString(content.substr(start, end - start));
        } else if (evt.key.code == sf::Keyboard::X && selectionStart != selectionEnd) {
            int start = std::min(selectionStart, selectionEnd);
            int end = std::max(selectionStart, selectionEnd);
            sf::Clipboard::setString(content.substr(start, end - start));
            content.erase(start, end - start);
            cursorIndex = start;
            selectionStart = selectionEnd = -1;
        } else if (evt.key.code == sf::Keyboard::V) {
            std::string pasted = sf::Clipboard::getString();
            if (selectionStart != selectionEnd) {
                int start = std::min(selectionStart, selectionEnd);
                int end = std::max(selectionStart, selectionEnd);
                content.erase(start, end - start);
                cursorIndex = start;
            }
            content.insert(cursorIndex, pasted);
            cursorIndex += pasted.size();
            selectionStart = selectionEnd = -1;
        }
        updateDisplayText();
        if (onTextChanged) onTextChanged(content);
        return;
    }

    if (evt.type == sf::Event::KeyPressed) {
        if (evt.key.code == sf::Keyboard::Left && cursorIndex > 0) {
            cursorIndex--;
            if (evt.key.shift) {
                if (selectionStart == -1) selectionStart = cursorIndex + 1;
                selectionEnd = cursorIndex;
            } else {
                selectionStart = selectionEnd = -1;
            }
        } else if (evt.key.code == sf::Keyboard::Right && cursorIndex < content.size()) {
            cursorIndex++;
            if (evt.key.shift) {
                if (selectionStart == -1) selectionStart = cursorIndex - 1;
                selectionEnd = cursorIndex;
            } else {
                selectionStart = selectionEnd = -1;
            }
        } else if (evt.key.code == sf::Keyboard::Up || evt.key.code == sf::Keyboard::Down) {
            float availW = box.getSize().x - 10.f;
            std::string wrapped = wrapText(content, contentText, availW);

            // Opsplitsen in regels en index-posities bijhouden
            std::vector<std::pair<std::string, int>> lines;
            int charIndex = 0;
            std::istringstream ss(wrapped);
            std::string line;
            while (std::getline(ss, line)) {
                lines.emplace_back(line, charIndex);
                charIndex += line.size() + 1; // +1 voor '\n'
            }

            // Huidige regel en offset bepalen
            int currentLine = 0;
            for (int i = 0; i < lines.size(); ++i) {
                int start = lines[i].second;
                int end = start + lines[i].first.size();
                if (cursorIndex >= start && cursorIndex <= end) {
                    currentLine = i;
                    break;
                }
            }
            int offset = cursorIndex - lines[currentLine].second;

            // Richting bepalen en nieuwe positie instellen
            if (evt.key.code == sf::Keyboard::Up && currentLine > 0) {
                int newStart = lines[currentLine - 1].second;
                int newLen = lines[currentLine - 1].first.size();
                cursorIndex = newStart + std::min(offset, newLen);
            } else if (evt.key.code == sf::Keyboard::Down && currentLine + 1 < lines.size()) {
                int newStart = lines[currentLine + 1].second;
                int newLen = lines[currentLine + 1].first.size();
                cursorIndex = newStart + std::min(offset, newLen);
            }

            selectionStart = selectionEnd = -1;
            updateDisplayText();
            if (onTextChanged) onTextChanged(content);
            return;
        } else if (evt.key.code == sf::Keyboard::BackSpace && cursorIndex > 0) {
            if (selectionStart != selectionEnd) {
                int start = std::min(selectionStart, selectionEnd);
                int end = std::max(selectionStart, selectionEnd);
                content.erase(start, end - start);
                cursorIndex = start;
                selectionStart = selectionEnd = -1;
            } else {
                content.erase(cursorIndex - 1, 1);
                cursorIndex--;
            }
        } else if (evt.key.code == sf::Keyboard::Delete && cursorIndex < content.size()) {
            if (selectionStart != selectionEnd) {
                int start = std::min(selectionStart, selectionEnd);
                int end = std::max(selectionStart, selectionEnd);
                content.erase(start, end - start);
                cursorIndex = start;
                selectionStart = selectionEnd = -1;
            } else {
                content.erase(cursorIndex, 1);
            }
        } else if (evt.key.code == sf::Keyboard::Enter || evt.key.code == sf::Keyboard::Return) {
            if (selectionStart != selectionEnd) {
                int start = std::min(selectionStart, selectionEnd);
                int end = std::max(selectionStart, selectionEnd);
                content.erase(start, end - start);
                cursorIndex = start;
            }
            content.insert(content.begin() + cursorIndex, '\n');
            cursorIndex++;
            selectionStart = selectionEnd = -1;
        }
        updateDisplayText();
        if (onTextChanged) onTextChanged(content);
        return;
    }

    if (evt.type == sf::Event::TextEntered) {
        if (evt.text.unicode >= 32 && evt.text.unicode != 127) {
            if (selectionStart != selectionEnd) {
                int start = std::min(selectionStart, selectionEnd);
                int end = std::max(selectionStart, selectionEnd);
                content.erase(start, end - start);
                cursorIndex = start;
            }
            content.insert(content.begin() + cursorIndex, static_cast<char>(evt.text.unicode));
            cursorIndex++;
            selectionStart = selectionEnd = -1;
        }
        updateDisplayText();
        if (onTextChanged) onTextChanged(content);
    }
}

void TextArea::updateDisplayText() {
    float availW = box.getSize().x - 10.f;
    contentText.setString(wrapText(content, contentText, availW));
}

std::string TextArea::wrapText(const std::string& str, const sf::Text& prototype, float maxWidth) {
    std::string wrapped;
    sf::Text meas = prototype;

    std::string currentLine, currentWord;

    auto flushWord = [&]() {
        if (currentWord.empty()) return;
        std::string trial = currentLine + currentWord;
        meas.setString(trial);
        float w = meas.getLocalBounds().width + meas.getLocalBounds().left;

        if (w > maxWidth && !currentLine.empty()) {
            wrapped += currentLine + '\n';
            currentLine = currentWord;
        } else {
            currentLine += currentWord;
        }
        currentWord.clear();
    };

    for (char c : str) {
        if (c == '\n') {
            flushWord();
            wrapped += currentLine + '\n';
            currentLine.clear();
        } else if (c == ' ') {
            currentWord += c;
            flushWord();
        } else {
            currentWord += c;
        }
    }

    flushWord();
    if (!currentLine.empty()) wrapped += currentLine;

    return wrapped;
}