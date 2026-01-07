
// Generated from UVLCppParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  UVLCppParser : public antlr4::Parser {
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

  enum {
    RuleFeatureModel = 0, RuleIncludes = 1, RuleIncludeLine = 2, RuleNamespace = 3, 
    RuleImports = 4, RuleImportLine = 5, RuleFeatures = 6, RuleGroup = 7, 
    RuleGroupSpec = 8, RuleFeature = 9, RuleFeatureCardinality = 10, RuleAttributes = 11, 
    RuleAttribute = 12, RuleValueAttribute = 13, RuleKey = 14, RuleValue = 15, 
    RuleVector = 16, RuleConstraintAttribute = 17, RuleConstraintList = 18, 
    RuleConstraints = 19, RuleConstraintLine = 20, RuleConstraint = 21, 
    RuleEquation = 22, RuleExpression = 23, RuleAdditiveExpression = 24, 
    RuleMultiplicativeExpression = 25, RulePrimaryExpression = 26, RuleAggregateFunction = 27, 
    RuleSumAggregateFunction = 28, RuleAvgAggregateFunction = 29, RuleStringAggregateFunction = 30, 
    RuleNumericAggregateFunction = 31, RuleReference = 32, RuleId = 33, 
    RuleFeatureType = 34, RuleLanguageLevel = 35, RuleMajorLevel = 36, RuleMinorLevel = 37
  };

  explicit UVLCppParser(antlr4::TokenStream *input);

  UVLCppParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~UVLCppParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class FeatureModelContext;
  class IncludesContext;
  class IncludeLineContext;
  class NamespaceContext;
  class ImportsContext;
  class ImportLineContext;
  class FeaturesContext;
  class GroupContext;
  class GroupSpecContext;
  class FeatureContext;
  class FeatureCardinalityContext;
  class AttributesContext;
  class AttributeContext;
  class ValueAttributeContext;
  class KeyContext;
  class ValueContext;
  class VectorContext;
  class ConstraintAttributeContext;
  class ConstraintListContext;
  class ConstraintsContext;
  class ConstraintLineContext;
  class ConstraintContext;
  class EquationContext;
  class ExpressionContext;
  class AdditiveExpressionContext;
  class MultiplicativeExpressionContext;
  class PrimaryExpressionContext;
  class AggregateFunctionContext;
  class SumAggregateFunctionContext;
  class AvgAggregateFunctionContext;
  class StringAggregateFunctionContext;
  class NumericAggregateFunctionContext;
  class ReferenceContext;
  class IdContext;
  class FeatureTypeContext;
  class LanguageLevelContext;
  class MajorLevelContext;
  class MinorLevelContext; 

  class  FeatureModelContext : public antlr4::ParserRuleContext {
  public:
    FeatureModelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    NamespaceContext *namespace_();
    std::vector<antlr4::tree::TerminalNode *> NEWLINE();
    antlr4::tree::TerminalNode* NEWLINE(size_t i);
    IncludesContext *includes();
    ImportsContext *imports();
    FeaturesContext *features();
    ConstraintsContext *constraints();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FeatureModelContext* featureModel();

  class  IncludesContext : public antlr4::ParserRuleContext {
  public:
    IncludesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INCLUDE_KEY();
    antlr4::tree::TerminalNode *NEWLINE();
    antlr4::tree::TerminalNode *INDENT();
    antlr4::tree::TerminalNode *DEDENT();
    std::vector<IncludeLineContext *> includeLine();
    IncludeLineContext* includeLine(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IncludesContext* includes();

  class  IncludeLineContext : public antlr4::ParserRuleContext {
  public:
    IncludeLineContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LanguageLevelContext *languageLevel();
    antlr4::tree::TerminalNode *NEWLINE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IncludeLineContext* includeLine();

  class  NamespaceContext : public antlr4::ParserRuleContext {
  public:
    NamespaceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NAMESPACE_KEY();
    ReferenceContext *reference();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NamespaceContext* namespace_();

  class  ImportsContext : public antlr4::ParserRuleContext {
  public:
    ImportsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMPORTS_KEY();
    antlr4::tree::TerminalNode *NEWLINE();
    antlr4::tree::TerminalNode *INDENT();
    antlr4::tree::TerminalNode *DEDENT();
    std::vector<ImportLineContext *> importLine();
    ImportLineContext* importLine(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ImportsContext* imports();

  class  ImportLineContext : public antlr4::ParserRuleContext {
  public:
    UVLCppParser::ReferenceContext *ns = nullptr;
    UVLCppParser::ReferenceContext *alias = nullptr;
    ImportLineContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NEWLINE();
    std::vector<ReferenceContext *> reference();
    ReferenceContext* reference(size_t i);
    antlr4::tree::TerminalNode *AS_KEY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ImportLineContext* importLine();

  class  FeaturesContext : public antlr4::ParserRuleContext {
  public:
    FeaturesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FEATURES_KEY();
    antlr4::tree::TerminalNode *NEWLINE();
    antlr4::tree::TerminalNode *INDENT();
    FeatureContext *feature();
    antlr4::tree::TerminalNode *DEDENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FeaturesContext* features();

  class  GroupContext : public antlr4::ParserRuleContext {
  public:
    GroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    GroupContext() = default;
    void copyFrom(GroupContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  AlternativeGroupContext : public GroupContext {
  public:
    AlternativeGroupContext(GroupContext *ctx);

    antlr4::tree::TerminalNode *ALTERNATIVE();
    GroupSpecContext *groupSpec();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  OptionalGroupContext : public GroupContext {
  public:
    OptionalGroupContext(GroupContext *ctx);

    antlr4::tree::TerminalNode *OPTIONAL();
    GroupSpecContext *groupSpec();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  MandatoryGroupContext : public GroupContext {
  public:
    MandatoryGroupContext(GroupContext *ctx);

    antlr4::tree::TerminalNode *MANDATORY();
    GroupSpecContext *groupSpec();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CardinalityGroupContext : public GroupContext {
  public:
    CardinalityGroupContext(GroupContext *ctx);

    antlr4::tree::TerminalNode *CARDINALITY();
    GroupSpecContext *groupSpec();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  OrGroupContext : public GroupContext {
  public:
    OrGroupContext(GroupContext *ctx);

    antlr4::tree::TerminalNode *ORGROUP();
    GroupSpecContext *groupSpec();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  GroupContext* group();

  class  GroupSpecContext : public antlr4::ParserRuleContext {
  public:
    GroupSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NEWLINE();
    antlr4::tree::TerminalNode *INDENT();
    antlr4::tree::TerminalNode *DEDENT();
    std::vector<FeatureContext *> feature();
    FeatureContext* feature(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GroupSpecContext* groupSpec();

  class  FeatureContext : public antlr4::ParserRuleContext {
  public:
    FeatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReferenceContext *reference();
    antlr4::tree::TerminalNode *NEWLINE();
    FeatureTypeContext *featureType();
    FeatureCardinalityContext *featureCardinality();
    AttributesContext *attributes();
    antlr4::tree::TerminalNode *INDENT();
    antlr4::tree::TerminalNode *DEDENT();
    std::vector<GroupContext *> group();
    GroupContext* group(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FeatureContext* feature();

  class  FeatureCardinalityContext : public antlr4::ParserRuleContext {
  public:
    FeatureCardinalityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CARDINALITY_KEY();
    antlr4::tree::TerminalNode *CARDINALITY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FeatureCardinalityContext* featureCardinality();

  class  AttributesContext : public antlr4::ParserRuleContext {
  public:
    AttributesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACE();
    antlr4::tree::TerminalNode *CLOSE_BRACE();
    std::vector<AttributeContext *> attribute();
    AttributeContext* attribute(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AttributesContext* attributes();

  class  AttributeContext : public antlr4::ParserRuleContext {
  public:
    AttributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueAttributeContext *valueAttribute();
    ConstraintAttributeContext *constraintAttribute();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AttributeContext* attribute();

  class  ValueAttributeContext : public antlr4::ParserRuleContext {
  public:
    ValueAttributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    KeyContext *key();
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValueAttributeContext* valueAttribute();

  class  KeyContext : public antlr4::ParserRuleContext {
  public:
    KeyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  KeyContext* key();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BOOLEAN();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *STRING();
    AttributesContext *attributes();
    VectorContext *vector();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValueContext* value();

  class  VectorContext : public antlr4::ParserRuleContext {
  public:
    VectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACK();
    antlr4::tree::TerminalNode *CLOSE_BRACK();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  VectorContext* vector();

  class  ConstraintAttributeContext : public antlr4::ParserRuleContext {
  public:
    ConstraintAttributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ConstraintAttributeContext() = default;
    void copyFrom(ConstraintAttributeContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ListConstraintAttributeContext : public ConstraintAttributeContext {
  public:
    ListConstraintAttributeContext(ConstraintAttributeContext *ctx);

    antlr4::tree::TerminalNode *CONSTRAINTS_KEY();
    ConstraintListContext *constraintList();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SingleConstraintAttributeContext : public ConstraintAttributeContext {
  public:
    SingleConstraintAttributeContext(ConstraintAttributeContext *ctx);

    antlr4::tree::TerminalNode *CONSTRAINT_KEY();
    ConstraintContext *constraint();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  ConstraintAttributeContext* constraintAttribute();

  class  ConstraintListContext : public antlr4::ParserRuleContext {
  public:
    ConstraintListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPEN_BRACK();
    antlr4::tree::TerminalNode *CLOSE_BRACK();
    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstraintListContext* constraintList();

  class  ConstraintsContext : public antlr4::ParserRuleContext {
  public:
    ConstraintsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONSTRAINTS_KEY();
    antlr4::tree::TerminalNode *NEWLINE();
    antlr4::tree::TerminalNode *INDENT();
    antlr4::tree::TerminalNode *DEDENT();
    std::vector<ConstraintLineContext *> constraintLine();
    ConstraintLineContext* constraintLine(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstraintsContext* constraints();

  class  ConstraintLineContext : public antlr4::ParserRuleContext {
  public:
    ConstraintLineContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstraintContext *constraint();
    antlr4::tree::TerminalNode *NEWLINE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstraintLineContext* constraintLine();

  class  ConstraintContext : public antlr4::ParserRuleContext {
  public:
    ConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ConstraintContext() = default;
    void copyFrom(ConstraintContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  OrConstraintContext : public ConstraintContext {
  public:
    OrConstraintContext(ConstraintContext *ctx);

    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);
    antlr4::tree::TerminalNode *OR();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  EquationConstraintContext : public ConstraintContext {
  public:
    EquationConstraintContext(ConstraintContext *ctx);

    EquationContext *equation();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  LiteralConstraintContext : public ConstraintContext {
  public:
    LiteralConstraintContext(ConstraintContext *ctx);

    ReferenceContext *reference();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  ParenthesisConstraintContext : public ConstraintContext {
  public:
    ParenthesisConstraintContext(ConstraintContext *ctx);

    antlr4::tree::TerminalNode *OPEN_PAREN();
    ConstraintContext *constraint();
    antlr4::tree::TerminalNode *CLOSE_PAREN();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  NotConstraintContext : public ConstraintContext {
  public:
    NotConstraintContext(ConstraintContext *ctx);

    antlr4::tree::TerminalNode *NOT();
    ConstraintContext *constraint();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  AndConstraintContext : public ConstraintContext {
  public:
    AndConstraintContext(ConstraintContext *ctx);

    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);
    antlr4::tree::TerminalNode *AND();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  EquivalenceConstraintContext : public ConstraintContext {
  public:
    EquivalenceConstraintContext(ConstraintContext *ctx);

    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);
    antlr4::tree::TerminalNode *EQUIVALENCE();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  ImplicationConstraintContext : public ConstraintContext {
  public:
    ImplicationConstraintContext(ConstraintContext *ctx);

    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);
    antlr4::tree::TerminalNode *IMPLICATION();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  ConstraintContext* constraint();
  ConstraintContext* constraint(int precedence);
  class  EquationContext : public antlr4::ParserRuleContext {
  public:
    EquationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    EquationContext() = default;
    void copyFrom(EquationContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  EqualEquationContext : public EquationContext {
  public:
    EqualEquationContext(EquationContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *EQUAL();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  LowerEquationContext : public EquationContext {
  public:
    LowerEquationContext(EquationContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *LOWER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  LowerEqualsEquationContext : public EquationContext {
  public:
    LowerEqualsEquationContext(EquationContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *LOWER_EQUALS();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  GreaterEqualsEquationContext : public EquationContext {
  public:
    GreaterEqualsEquationContext(EquationContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *GREATER_EQUALS();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  GreaterEquationContext : public EquationContext {
  public:
    GreaterEquationContext(EquationContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *GREATER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  NotEqualsEquationContext : public EquationContext {
  public:
    NotEqualsEquationContext(EquationContext *ctx);

    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *NOT_EQUALS();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  EquationContext* equation();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AdditiveExpressionContext *additiveExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ExpressionContext* expression();

  class  AdditiveExpressionContext : public antlr4::ParserRuleContext {
  public:
    AdditiveExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AdditiveExpressionContext() = default;
    void copyFrom(AdditiveExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  MultiplicativeExprContext : public AdditiveExpressionContext {
  public:
    MultiplicativeExprContext(AdditiveExpressionContext *ctx);

    MultiplicativeExpressionContext *multiplicativeExpression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  AddExpressionContext : public AdditiveExpressionContext {
  public:
    AddExpressionContext(AdditiveExpressionContext *ctx);

    AdditiveExpressionContext *additiveExpression();
    antlr4::tree::TerminalNode *ADD();
    MultiplicativeExpressionContext *multiplicativeExpression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SubExpressionContext : public AdditiveExpressionContext {
  public:
    SubExpressionContext(AdditiveExpressionContext *ctx);

    AdditiveExpressionContext *additiveExpression();
    antlr4::tree::TerminalNode *SUB();
    MultiplicativeExpressionContext *multiplicativeExpression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  AdditiveExpressionContext* additiveExpression();
  AdditiveExpressionContext* additiveExpression(int precedence);
  class  MultiplicativeExpressionContext : public antlr4::ParserRuleContext {
  public:
    MultiplicativeExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    MultiplicativeExpressionContext() = default;
    void copyFrom(MultiplicativeExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  PrimaryExpressionExpressionContext : public MultiplicativeExpressionContext {
  public:
    PrimaryExpressionExpressionContext(MultiplicativeExpressionContext *ctx);

    PrimaryExpressionContext *primaryExpression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DivExpressionContext : public MultiplicativeExpressionContext {
  public:
    DivExpressionContext(MultiplicativeExpressionContext *ctx);

    MultiplicativeExpressionContext *multiplicativeExpression();
    antlr4::tree::TerminalNode *DIV();
    PrimaryExpressionContext *primaryExpression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  MulExpressionContext : public MultiplicativeExpressionContext {
  public:
    MulExpressionContext(MultiplicativeExpressionContext *ctx);

    MultiplicativeExpressionContext *multiplicativeExpression();
    antlr4::tree::TerminalNode *MUL();
    PrimaryExpressionContext *primaryExpression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  MultiplicativeExpressionContext* multiplicativeExpression();
  MultiplicativeExpressionContext* multiplicativeExpression(int precedence);
  class  PrimaryExpressionContext : public antlr4::ParserRuleContext {
  public:
    PrimaryExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PrimaryExpressionContext() = default;
    void copyFrom(PrimaryExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BracketExpressionContext : public PrimaryExpressionContext {
  public:
    BracketExpressionContext(PrimaryExpressionContext *ctx);

    antlr4::tree::TerminalNode *OPEN_PAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *CLOSE_PAREN();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  AggregateFunctionExpressionContext : public PrimaryExpressionContext {
  public:
    AggregateFunctionExpressionContext(PrimaryExpressionContext *ctx);

    AggregateFunctionContext *aggregateFunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  FloatLiteralExpressionContext : public PrimaryExpressionContext {
  public:
    FloatLiteralExpressionContext(PrimaryExpressionContext *ctx);

    antlr4::tree::TerminalNode *FLOAT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  StringLiteralExpressionContext : public PrimaryExpressionContext {
  public:
    StringLiteralExpressionContext(PrimaryExpressionContext *ctx);

    antlr4::tree::TerminalNode *STRING();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  IntegerLiteralExpressionContext : public PrimaryExpressionContext {
  public:
    IntegerLiteralExpressionContext(PrimaryExpressionContext *ctx);

    antlr4::tree::TerminalNode *INTEGER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  LiteralExpressionContext : public PrimaryExpressionContext {
  public:
    LiteralExpressionContext(PrimaryExpressionContext *ctx);

    ReferenceContext *reference();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  PrimaryExpressionContext* primaryExpression();

  class  AggregateFunctionContext : public antlr4::ParserRuleContext {
  public:
    AggregateFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AggregateFunctionContext() = default;
    void copyFrom(AggregateFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  NumericAggregateFunctionExpressionContext : public AggregateFunctionContext {
  public:
    NumericAggregateFunctionExpressionContext(AggregateFunctionContext *ctx);

    NumericAggregateFunctionContext *numericAggregateFunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  StringAggregateFunctionExpressionContext : public AggregateFunctionContext {
  public:
    StringAggregateFunctionExpressionContext(AggregateFunctionContext *ctx);

    StringAggregateFunctionContext *stringAggregateFunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  AvgAggregateFunctionExpressionContext : public AggregateFunctionContext {
  public:
    AvgAggregateFunctionExpressionContext(AggregateFunctionContext *ctx);

    AvgAggregateFunctionContext *avgAggregateFunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  SumAggregateFunctionExpressionContext : public AggregateFunctionContext {
  public:
    SumAggregateFunctionExpressionContext(AggregateFunctionContext *ctx);

    SumAggregateFunctionContext *sumAggregateFunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  AggregateFunctionContext* aggregateFunction();

  class  SumAggregateFunctionContext : public antlr4::ParserRuleContext {
  public:
    SumAggregateFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SUM_KEY();
    antlr4::tree::TerminalNode *OPEN_PAREN();
    std::vector<ReferenceContext *> reference();
    ReferenceContext* reference(size_t i);
    antlr4::tree::TerminalNode *CLOSE_PAREN();
    antlr4::tree::TerminalNode *COMMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SumAggregateFunctionContext* sumAggregateFunction();

  class  AvgAggregateFunctionContext : public antlr4::ParserRuleContext {
  public:
    AvgAggregateFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AVG_KEY();
    antlr4::tree::TerminalNode *OPEN_PAREN();
    std::vector<ReferenceContext *> reference();
    ReferenceContext* reference(size_t i);
    antlr4::tree::TerminalNode *CLOSE_PAREN();
    antlr4::tree::TerminalNode *COMMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AvgAggregateFunctionContext* avgAggregateFunction();

  class  StringAggregateFunctionContext : public antlr4::ParserRuleContext {
  public:
    StringAggregateFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    StringAggregateFunctionContext() = default;
    void copyFrom(StringAggregateFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  LengthAggregateFunctionContext : public StringAggregateFunctionContext {
  public:
    LengthAggregateFunctionContext(StringAggregateFunctionContext *ctx);

    antlr4::tree::TerminalNode *LEN_KEY();
    antlr4::tree::TerminalNode *OPEN_PAREN();
    ReferenceContext *reference();
    antlr4::tree::TerminalNode *CLOSE_PAREN();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  StringAggregateFunctionContext* stringAggregateFunction();

  class  NumericAggregateFunctionContext : public antlr4::ParserRuleContext {
  public:
    NumericAggregateFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    NumericAggregateFunctionContext() = default;
    void copyFrom(NumericAggregateFunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CeilAggregateFunctionContext : public NumericAggregateFunctionContext {
  public:
    CeilAggregateFunctionContext(NumericAggregateFunctionContext *ctx);

    antlr4::tree::TerminalNode *CEIL_KEY();
    antlr4::tree::TerminalNode *OPEN_PAREN();
    ReferenceContext *reference();
    antlr4::tree::TerminalNode *CLOSE_PAREN();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  FloorAggregateFunctionContext : public NumericAggregateFunctionContext {
  public:
    FloorAggregateFunctionContext(NumericAggregateFunctionContext *ctx);

    antlr4::tree::TerminalNode *FLOOR_KEY();
    antlr4::tree::TerminalNode *OPEN_PAREN();
    ReferenceContext *reference();
    antlr4::tree::TerminalNode *CLOSE_PAREN();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  NumericAggregateFunctionContext* numericAggregateFunction();

  class  ReferenceContext : public antlr4::ParserRuleContext {
  public:
    ReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ReferenceContext* reference();

  class  IdContext : public antlr4::ParserRuleContext {
  public:
    IdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID_STRICT();
    antlr4::tree::TerminalNode *ID_NOT_STRICT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IdContext* id();

  class  FeatureTypeContext : public antlr4::ParserRuleContext {
  public:
    FeatureTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING_KEY();
    antlr4::tree::TerminalNode *INTEGER_KEY();
    antlr4::tree::TerminalNode *BOOLEAN_KEY();
    antlr4::tree::TerminalNode *REAL_KEY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FeatureTypeContext* featureType();

  class  LanguageLevelContext : public antlr4::ParserRuleContext {
  public:
    LanguageLevelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MajorLevelContext *majorLevel();
    antlr4::tree::TerminalNode *DOT();
    MinorLevelContext *minorLevel();
    antlr4::tree::TerminalNode *MUL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LanguageLevelContext* languageLevel();

  class  MajorLevelContext : public antlr4::ParserRuleContext {
  public:
    MajorLevelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BOOLEAN_KEY();
    antlr4::tree::TerminalNode *ARITHMETIC_KEY();
    antlr4::tree::TerminalNode *TYPE_KEY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MajorLevelContext* majorLevel();

  class  MinorLevelContext : public antlr4::ParserRuleContext {
  public:
    MinorLevelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GROUP_CARDINALITY_KEY();
    antlr4::tree::TerminalNode *FEATURE_CARDINALITY_KEY();
    antlr4::tree::TerminalNode *AGGREGATE_KEY();
    antlr4::tree::TerminalNode *STRING_CONSTRAINTS_KEY();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MinorLevelContext* minorLevel();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool constraintSempred(ConstraintContext *_localctx, size_t predicateIndex);
  bool additiveExpressionSempred(AdditiveExpressionContext *_localctx, size_t predicateIndex);
  bool multiplicativeExpressionSempred(MultiplicativeExpressionContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

