#ifndef MBTOG_EXPRESSIONAPP_H
#define MBTOG_EXPRESSIONAPP_H

#include "../CFG.h"
#include "../PDA.h"
#include "../SLRParser.h"
#include "../ast/AST.h"
#include "../ast/ExpressionOperatorConfig.h"
#include "../ast/OperatorEnvironment.h"
#include "../config/OperatorConfig.h"

#include <SFML/Graphics.hpp>
#include <string>

/**
 * @brief Main GUI application for expression parsing, validation, and visualization.
 *
 * This class implements an interactive graphical application using SFML that allows:
 *  - Editing and evaluating mathematical expressions
 *  - Parsing expressions into an AST
 *  - Validating expressions using CYK and PDA techniques
 *  - Visualizing AST and PDA structures
 */
class ExpressionApp {
  public:
    /**
     * @brief Constructs the expression application.
     */
    ExpressionApp();

    /**
     * @brief Runs the main application loop.
     */
    void run();

  private:
    /**
     * @brief Handles SFML window events.
     */
    void handleEvents();

    /**
     * @brief Renders the complete user interface.
     */
    void render();

    /** @name GUI Helpers */
    ///@{
    void               setStatus(const std::string& msg, bool good);
    void               updateWrappedStatus();
    static std::string wrapTextToWidth(const sf::Font& font, unsigned charSize,
                                       const std::string& text, float maxWidth);
    ///@}

    /** @name Input Editing */
    ///@{
    void               focusInput(bool focus);
    void               handleTextEntered(const sf::Event::TextEvent& text);
    void               handleKeyPressed(const sf::Event::KeyEvent& key);
    void               handleMousePressed(sf::Vector2f mousePos);
    void               handleMouseReleased(sf::Vector2f mousePos);
    void               handleMouseMoved(sf::Vector2f mousePos);
    void               handleMouseWheel(float delta, sf::Vector2f mousePos);

    std::size_t        caretFromMouseX(float mouseX) const;
    void               ensureCaretVisible();

    bool               hasSelection() const;
    void               clearSelection();
    void               selectAll();
    void               deleteSelectionIfAny();
    void               insertTextAtCaret(const std::string& s);
    void               backspace();
    void               del();
    void               moveCaretLeft(bool ctrl, bool shift);
    void               moveCaretRight(bool ctrl, bool shift);
    void               moveCaretHome(bool shift);
    void               moveCaretEnd(bool shift);

    static std::size_t findWordLeft(const std::string& s, std::size_t pos);
    static std::size_t findWordRight(const std::string& s, std::size_t pos);

    std::string        getSelectedText() const;
    ///@}

    /** @name User Actions */
    ///@{
    void onEvaluateClicked();
    void onCykClicked();
    void onAstImageClicked();
    void onPdaImageClicked();
    void onPdaValidateClicked();
    bool runCykAnalysis(const std::string& word, std::string& message);
    ///@}

    /**
     * @brief Formats a vector of tokens for display.
     */
    static std::string formatTokens(const std::vector<std::string>& tokens);

    /** @name Internal Helpers */
    ///@{
    bool parseToAst(const std::string& input, std::unique_ptr<expr::ASTNode>& outAst,
                    std::string& errorMessage) const;

    bool evaluateAst(const expr::ASTNode& ast, double& result, std::string& errorMessage) const;

    void updateAstPreview();
    void updatePdaPreview();
    bool isMouseOverAstPanel(sf::Vector2f p) const;
    bool isMouseOverPdaPanel(sf::Vector2f p) const;

    void applyLayout(float w, float h);
    void rebuildAstViewportTexture();
    void rebuildPdaViewportTexture();
    ///@}

  private:
    sf::RenderWindow window_; ///< Main application window
    sf::Font         font_;   ///< UI font

    /** @name Layout Panel */
    ///@{
    sf::RectangleShape card_;
    sf::RectangleShape cardShadow_;
    ///@}

    /** @name AST Viewer (fixed panel with zoom/pan) */
    ///@{
    sf::RectangleShape             astPanel_;
    sf::RectangleShape             astPanelInner_;
    sf::Text                       astLabel_;

    sf::RenderTexture              astViewportTex_;
    sf::Sprite                     astViewportSprite_;

    sf::Texture                    astImageTexture_;
    sf::Sprite                     astImageSprite_;

    std::unique_ptr<expr::ASTNode> lastAst_;

    bool                           astHasImage_ = false;
    bool                           astDragging_ = false;
    sf::Vector2f                   astDragLast_{0.f, 0.f};
    sf::Vector2f                   astPan_{0.f, 0.f};
    float                          astZoom_ = 1.f;
    ///@}

    /** @name PDA Viewer (fixed panel with zoom/pan) */
    ///@{
    sf::RectangleShape pdaPanel_;
    sf::RectangleShape pdaPanelInner_;
    sf::Text           pdaLabel_;

    sf::RenderTexture  pdaViewportTex_;
    sf::Sprite         pdaViewportSprite_;

    sf::Texture        pdaImageTexture_;
    sf::Sprite         pdaImageSprite_;

    bool               pdaHasImage_ = false;
    bool               pdaDragging_ = false;
    sf::Vector2f       pdaDragLast_{0.f, 0.f};
    sf::Vector2f       pdaPan_{0.f, 0.f};
    float              pdaZoom_ = 1.f;
    ///@}

    /** @name Input UI */
    ///@{
    sf::Text           inputLabel_;
    sf::RectangleShape inputBox_;
    sf::RectangleShape inputInner_;
    sf::Text           inputText_;
    sf::RectangleShape selectionRect_;
    sf::RectangleShape caretRect_;

    ///@}

    /** @name Buttons */
    ///@{
    struct Button {
        sf::RectangleShape box;
        sf::Text           label;
        bool               hovered = false;
        bool               pressed = false;

        bool contains(sf::Vector2f p) const { return box.getGlobalBounds().contains(p); }
    };

    Button evalButton_;
    Button cykButton_;
    Button astButton_;
    Button pdaButton_;
    Button pdaSimButton_;
    ///@}

    /** @name Result and Status */
    ///@{
    sf::Text           resultText_;
    sf::RectangleShape statusPanel_;
    sf::Text           statusText_;
    bool               statusGood_ = false;
    std::string        statusRaw_;
    ///@}

    /** @name Input State */
    ///@{
    bool        inputFocused_ = false;
    bool        selecting_    = false;

    std::string inputBuffer_;
    std::size_t caretIndex_  = 0;
    std::size_t selAnchor_   = 0;
    std::size_t selCaret_    = 0;
    float       textScrollX_ = 0.f;
    sf::Clock   caretBlinkClock_;
    bool        caretVisible_ = true;
    ///@}

    /** @name Grammar and Parsing */
    ///@{
    CFG                       cfg_;
    SLRParser                 parser_;
    expr::OperatorEnvironment env_;
    cfg::OperatorConfig       cykOps_;
    CFG                       cykCfg_;
    bool                      cykIsInCnf_      = false;
    bool                      cykLastAccepted_ = false;
    ///@}

    /**
     * @brief Pushdown automaton used for diagram generation and structure validation.
     */
    PDA pda_;

    /** @name Miscellaneous */
    ///@{
    std::ostringstream null_out;
    ///@}
};

#endif // MBTOG_EXPRESSIONAPP_H
