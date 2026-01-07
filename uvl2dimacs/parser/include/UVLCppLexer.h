
#include <deque>
#include <stack>
#include <algorithm>


// Generated from UVLCppLexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  UVLCppLexer : public antlr4::Lexer {
public:
  enum {
    OPEN_PAREN = 1, CLOSE_PAREN = 2, OPEN_BRACK = 3, CLOSE_BRACK = 4, OPEN_BRACE = 5, 
    CLOSE_BRACE = 6, OPEN_COMMENT = 7, CLOSE_COMMENT = 8, NEWLINE = 9, INDENT = 10, 
    DEDENT = 11, INCLUDE_KEY = 12, FEATURES_KEY = 13, IMPORTS_KEY = 14, 
    NAMESPACE_KEY = 15, AS_KEY = 16, CONSTRAINT_KEY = 17, CONSTRAINTS_KEY = 18, 
    CARDINALITY_KEY = 19, STRING_KEY = 20, BOOLEAN_KEY = 21, INTEGER_KEY = 22, 
    REAL_KEY = 23, LEN_KEY = 24, SUM_KEY = 25, AVG_KEY = 26, FLOOR_KEY = 27, 
    CEIL_KEY = 28, TYPE_KEY = 29, ARITHMETIC_KEY = 30, GROUP_CARDINALITY_KEY = 31, 
    FEATURE_CARDINALITY_KEY = 32, AGGREGATE_KEY = 33, STRING_CONSTRAINTS_KEY = 34, 
    ORGROUP = 35, ALTERNATIVE = 36, OPTIONAL = 37, MANDATORY = 38, CARDINALITY = 39, 
    NOT = 40, AND = 41, OR = 42, EQUIVALENCE = 43, IMPLICATION = 44, EQUAL = 45, 
    LOWER = 46, LOWER_EQUALS = 47, GREATER = 48, GREATER_EQUALS = 49, NOT_EQUALS = 50, 
    DIV = 51, MUL = 52, ADD = 53, SUB = 54, FLOAT = 55, INTEGER = 56, BOOLEAN = 57, 
    COMMA = 58, DOT = 59, ID_NOT_STRICT = 60, ID_STRICT = 61, STRING = 62, 
    SKIP_ = 63
  };

  explicit UVLCppLexer(antlr4::CharStream *input);

  ~UVLCppLexer() override;


  private:
    // A queue where extra tokens are pushed on (see the NEWLINE lexer rule).
    std::deque<std::unique_ptr<antlr4::Token>> tokens;
    // The stack that keeps track of the indentation level.
    std::stack<int> indents;
    // The amount of opened braces, brackets and parenthesis.
    int opened = 0;
    // The most recently produced token.
    antlr4::Token* lastToken = nullptr;

  public:
    void emit(std::unique_ptr<antlr4::Token> token) override {
      antlr4::Lexer::setToken(std::move(token));
    }

    void emitToken(std::unique_ptr<antlr4::Token> token) {
      tokens.push_back(std::move(token));
    }

    std::unique_ptr<antlr4::Token> nextToken() override {
      // Check if the end-of-file is ahead and there are still some DEDENTS expected.
      if (_input->LA(1) == antlr4::IntStream::EOF && !indents.empty()) {
        // Remove any trailing EOF tokens from our buffer.
        for (auto it = tokens.begin(); it != tokens.end(); ) {
          if ((*it)->getType() == antlr4::Token::EOF) {
            it = tokens.erase(it);
          } else {
            ++it;
          }
        }
        // First emit an extra line break that serves as the end of the statement.
        emitToken(commonToken(NEWLINE, "\n"));
        // Now emit as much DEDENT tokens as needed.
        while (!indents.empty()) {
          emitToken(createDedent());
          indents.pop();
        }
        // Put the EOF back on the token stream.
        emitToken(commonToken(antlr4::Token::EOF, "<EOF>"));
      }

      // Check queue first before lexing next token
      if (!tokens.empty()) {
        std::unique_ptr<antlr4::Token> result = std::move(tokens.front());
        tokens.pop_front();
        if (result->getChannel() == antlr4::Token::DEFAULT_CHANNEL) {
          lastToken = result.get();
        }
        return result;
      }

      // No queued tokens, get next from lexer
      std::unique_ptr<antlr4::Token> next = antlr4::Lexer::nextToken();
      if (next->getChannel() == antlr4::Token::DEFAULT_CHANNEL) {
        // Keep track of the last token on the default channel.
        lastToken = next.get();
      }

      // Check queue again in case the lexer action added tokens
      if (!tokens.empty()) {
        // Push this token back for later, return queued token first
        tokens.push_back(std::move(next));
        std::unique_ptr<antlr4::Token> result = std::move(tokens.front());
        tokens.pop_front();
        if (result->getChannel() == antlr4::Token::DEFAULT_CHANNEL) {
          lastToken = result.get();
        }
        return result;
      }

      return next;
    }

  private:
    std::unique_ptr<antlr4::CommonToken> createDedent() {
      auto dedent = commonToken(DEDENT, "");
      if (lastToken) {
        dedent->setLine(lastToken->getLine());
      }
      return dedent;
    }

    std::unique_ptr<antlr4::CommonToken> commonToken(int type, const std::string& text) {
      auto token = std::make_unique<antlr4::CommonToken>(type, text);
      token->setLine(getLine());
      token->setCharPositionInLine(getCharPositionInLine());
      return token;
    }

    // Calculates the indentation of the provided spaces, taking the
    // following rules into account:
    //
    // "Tabs are replaced (from left to right) by one to eight spaces
    //  such that the total number of characters up to and including
    //  the replacement is a multiple of eight [...]"
    //
    //  -- https://docs.python.org/3.1/reference/lexical_analysis.html#indentation
    static int getIndentationCount(const std::string& spaces) {
      int count = 0;
      for (char ch : spaces) {
        switch (ch) {
          case '\t':
            count += 8 - (count % 8);
            break;
          default:
            // A normal space char.
            count++;
        }
      }
      return count;
    }

    bool atStartOfInput() {
      return getCharPositionInLine() == 0 && getLine() == 1;
    }


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  void action(antlr4::RuleContext *context, size_t ruleIndex, size_t actionIndex) override;

  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.
  void OPEN_PARENAction(antlr4::RuleContext *context, size_t actionIndex);
  void CLOSE_PARENAction(antlr4::RuleContext *context, size_t actionIndex);
  void OPEN_BRACKAction(antlr4::RuleContext *context, size_t actionIndex);
  void CLOSE_BRACKAction(antlr4::RuleContext *context, size_t actionIndex);
  void OPEN_BRACEAction(antlr4::RuleContext *context, size_t actionIndex);
  void CLOSE_BRACEAction(antlr4::RuleContext *context, size_t actionIndex);
  void OPEN_COMMENTAction(antlr4::RuleContext *context, size_t actionIndex);
  void CLOSE_COMMENTAction(antlr4::RuleContext *context, size_t actionIndex);
  void NEWLINEAction(antlr4::RuleContext *context, size_t actionIndex);

  // Individual semantic predicate functions triggered by sempred() above.
  bool NEWLINESempred(antlr4::RuleContext *_localctx, size_t predicateIndex);

};

