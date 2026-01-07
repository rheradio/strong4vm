
// Generated from UVLCppParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "UVLCppParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by UVLCppParser.
 */
class  UVLCppParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterFeatureModel(UVLCppParser::FeatureModelContext *ctx) = 0;
  virtual void exitFeatureModel(UVLCppParser::FeatureModelContext *ctx) = 0;

  virtual void enterIncludes(UVLCppParser::IncludesContext *ctx) = 0;
  virtual void exitIncludes(UVLCppParser::IncludesContext *ctx) = 0;

  virtual void enterIncludeLine(UVLCppParser::IncludeLineContext *ctx) = 0;
  virtual void exitIncludeLine(UVLCppParser::IncludeLineContext *ctx) = 0;

  virtual void enterNamespace(UVLCppParser::NamespaceContext *ctx) = 0;
  virtual void exitNamespace(UVLCppParser::NamespaceContext *ctx) = 0;

  virtual void enterImports(UVLCppParser::ImportsContext *ctx) = 0;
  virtual void exitImports(UVLCppParser::ImportsContext *ctx) = 0;

  virtual void enterImportLine(UVLCppParser::ImportLineContext *ctx) = 0;
  virtual void exitImportLine(UVLCppParser::ImportLineContext *ctx) = 0;

  virtual void enterFeatures(UVLCppParser::FeaturesContext *ctx) = 0;
  virtual void exitFeatures(UVLCppParser::FeaturesContext *ctx) = 0;

  virtual void enterOrGroup(UVLCppParser::OrGroupContext *ctx) = 0;
  virtual void exitOrGroup(UVLCppParser::OrGroupContext *ctx) = 0;

  virtual void enterAlternativeGroup(UVLCppParser::AlternativeGroupContext *ctx) = 0;
  virtual void exitAlternativeGroup(UVLCppParser::AlternativeGroupContext *ctx) = 0;

  virtual void enterOptionalGroup(UVLCppParser::OptionalGroupContext *ctx) = 0;
  virtual void exitOptionalGroup(UVLCppParser::OptionalGroupContext *ctx) = 0;

  virtual void enterMandatoryGroup(UVLCppParser::MandatoryGroupContext *ctx) = 0;
  virtual void exitMandatoryGroup(UVLCppParser::MandatoryGroupContext *ctx) = 0;

  virtual void enterCardinalityGroup(UVLCppParser::CardinalityGroupContext *ctx) = 0;
  virtual void exitCardinalityGroup(UVLCppParser::CardinalityGroupContext *ctx) = 0;

  virtual void enterGroupSpec(UVLCppParser::GroupSpecContext *ctx) = 0;
  virtual void exitGroupSpec(UVLCppParser::GroupSpecContext *ctx) = 0;

  virtual void enterFeature(UVLCppParser::FeatureContext *ctx) = 0;
  virtual void exitFeature(UVLCppParser::FeatureContext *ctx) = 0;

  virtual void enterFeatureCardinality(UVLCppParser::FeatureCardinalityContext *ctx) = 0;
  virtual void exitFeatureCardinality(UVLCppParser::FeatureCardinalityContext *ctx) = 0;

  virtual void enterAttributes(UVLCppParser::AttributesContext *ctx) = 0;
  virtual void exitAttributes(UVLCppParser::AttributesContext *ctx) = 0;

  virtual void enterAttribute(UVLCppParser::AttributeContext *ctx) = 0;
  virtual void exitAttribute(UVLCppParser::AttributeContext *ctx) = 0;

  virtual void enterValueAttribute(UVLCppParser::ValueAttributeContext *ctx) = 0;
  virtual void exitValueAttribute(UVLCppParser::ValueAttributeContext *ctx) = 0;

  virtual void enterKey(UVLCppParser::KeyContext *ctx) = 0;
  virtual void exitKey(UVLCppParser::KeyContext *ctx) = 0;

  virtual void enterValue(UVLCppParser::ValueContext *ctx) = 0;
  virtual void exitValue(UVLCppParser::ValueContext *ctx) = 0;

  virtual void enterVector(UVLCppParser::VectorContext *ctx) = 0;
  virtual void exitVector(UVLCppParser::VectorContext *ctx) = 0;

  virtual void enterSingleConstraintAttribute(UVLCppParser::SingleConstraintAttributeContext *ctx) = 0;
  virtual void exitSingleConstraintAttribute(UVLCppParser::SingleConstraintAttributeContext *ctx) = 0;

  virtual void enterListConstraintAttribute(UVLCppParser::ListConstraintAttributeContext *ctx) = 0;
  virtual void exitListConstraintAttribute(UVLCppParser::ListConstraintAttributeContext *ctx) = 0;

  virtual void enterConstraintList(UVLCppParser::ConstraintListContext *ctx) = 0;
  virtual void exitConstraintList(UVLCppParser::ConstraintListContext *ctx) = 0;

  virtual void enterConstraints(UVLCppParser::ConstraintsContext *ctx) = 0;
  virtual void exitConstraints(UVLCppParser::ConstraintsContext *ctx) = 0;

  virtual void enterConstraintLine(UVLCppParser::ConstraintLineContext *ctx) = 0;
  virtual void exitConstraintLine(UVLCppParser::ConstraintLineContext *ctx) = 0;

  virtual void enterOrConstraint(UVLCppParser::OrConstraintContext *ctx) = 0;
  virtual void exitOrConstraint(UVLCppParser::OrConstraintContext *ctx) = 0;

  virtual void enterEquationConstraint(UVLCppParser::EquationConstraintContext *ctx) = 0;
  virtual void exitEquationConstraint(UVLCppParser::EquationConstraintContext *ctx) = 0;

  virtual void enterLiteralConstraint(UVLCppParser::LiteralConstraintContext *ctx) = 0;
  virtual void exitLiteralConstraint(UVLCppParser::LiteralConstraintContext *ctx) = 0;

  virtual void enterParenthesisConstraint(UVLCppParser::ParenthesisConstraintContext *ctx) = 0;
  virtual void exitParenthesisConstraint(UVLCppParser::ParenthesisConstraintContext *ctx) = 0;

  virtual void enterNotConstraint(UVLCppParser::NotConstraintContext *ctx) = 0;
  virtual void exitNotConstraint(UVLCppParser::NotConstraintContext *ctx) = 0;

  virtual void enterAndConstraint(UVLCppParser::AndConstraintContext *ctx) = 0;
  virtual void exitAndConstraint(UVLCppParser::AndConstraintContext *ctx) = 0;

  virtual void enterEquivalenceConstraint(UVLCppParser::EquivalenceConstraintContext *ctx) = 0;
  virtual void exitEquivalenceConstraint(UVLCppParser::EquivalenceConstraintContext *ctx) = 0;

  virtual void enterImplicationConstraint(UVLCppParser::ImplicationConstraintContext *ctx) = 0;
  virtual void exitImplicationConstraint(UVLCppParser::ImplicationConstraintContext *ctx) = 0;

  virtual void enterEqualEquation(UVLCppParser::EqualEquationContext *ctx) = 0;
  virtual void exitEqualEquation(UVLCppParser::EqualEquationContext *ctx) = 0;

  virtual void enterLowerEquation(UVLCppParser::LowerEquationContext *ctx) = 0;
  virtual void exitLowerEquation(UVLCppParser::LowerEquationContext *ctx) = 0;

  virtual void enterGreaterEquation(UVLCppParser::GreaterEquationContext *ctx) = 0;
  virtual void exitGreaterEquation(UVLCppParser::GreaterEquationContext *ctx) = 0;

  virtual void enterLowerEqualsEquation(UVLCppParser::LowerEqualsEquationContext *ctx) = 0;
  virtual void exitLowerEqualsEquation(UVLCppParser::LowerEqualsEquationContext *ctx) = 0;

  virtual void enterGreaterEqualsEquation(UVLCppParser::GreaterEqualsEquationContext *ctx) = 0;
  virtual void exitGreaterEqualsEquation(UVLCppParser::GreaterEqualsEquationContext *ctx) = 0;

  virtual void enterNotEqualsEquation(UVLCppParser::NotEqualsEquationContext *ctx) = 0;
  virtual void exitNotEqualsEquation(UVLCppParser::NotEqualsEquationContext *ctx) = 0;

  virtual void enterExpression(UVLCppParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(UVLCppParser::ExpressionContext *ctx) = 0;

  virtual void enterMultiplicativeExpr(UVLCppParser::MultiplicativeExprContext *ctx) = 0;
  virtual void exitMultiplicativeExpr(UVLCppParser::MultiplicativeExprContext *ctx) = 0;

  virtual void enterAddExpression(UVLCppParser::AddExpressionContext *ctx) = 0;
  virtual void exitAddExpression(UVLCppParser::AddExpressionContext *ctx) = 0;

  virtual void enterSubExpression(UVLCppParser::SubExpressionContext *ctx) = 0;
  virtual void exitSubExpression(UVLCppParser::SubExpressionContext *ctx) = 0;

  virtual void enterPrimaryExpressionExpression(UVLCppParser::PrimaryExpressionExpressionContext *ctx) = 0;
  virtual void exitPrimaryExpressionExpression(UVLCppParser::PrimaryExpressionExpressionContext *ctx) = 0;

  virtual void enterDivExpression(UVLCppParser::DivExpressionContext *ctx) = 0;
  virtual void exitDivExpression(UVLCppParser::DivExpressionContext *ctx) = 0;

  virtual void enterMulExpression(UVLCppParser::MulExpressionContext *ctx) = 0;
  virtual void exitMulExpression(UVLCppParser::MulExpressionContext *ctx) = 0;

  virtual void enterFloatLiteralExpression(UVLCppParser::FloatLiteralExpressionContext *ctx) = 0;
  virtual void exitFloatLiteralExpression(UVLCppParser::FloatLiteralExpressionContext *ctx) = 0;

  virtual void enterIntegerLiteralExpression(UVLCppParser::IntegerLiteralExpressionContext *ctx) = 0;
  virtual void exitIntegerLiteralExpression(UVLCppParser::IntegerLiteralExpressionContext *ctx) = 0;

  virtual void enterStringLiteralExpression(UVLCppParser::StringLiteralExpressionContext *ctx) = 0;
  virtual void exitStringLiteralExpression(UVLCppParser::StringLiteralExpressionContext *ctx) = 0;

  virtual void enterAggregateFunctionExpression(UVLCppParser::AggregateFunctionExpressionContext *ctx) = 0;
  virtual void exitAggregateFunctionExpression(UVLCppParser::AggregateFunctionExpressionContext *ctx) = 0;

  virtual void enterLiteralExpression(UVLCppParser::LiteralExpressionContext *ctx) = 0;
  virtual void exitLiteralExpression(UVLCppParser::LiteralExpressionContext *ctx) = 0;

  virtual void enterBracketExpression(UVLCppParser::BracketExpressionContext *ctx) = 0;
  virtual void exitBracketExpression(UVLCppParser::BracketExpressionContext *ctx) = 0;

  virtual void enterSumAggregateFunctionExpression(UVLCppParser::SumAggregateFunctionExpressionContext *ctx) = 0;
  virtual void exitSumAggregateFunctionExpression(UVLCppParser::SumAggregateFunctionExpressionContext *ctx) = 0;

  virtual void enterAvgAggregateFunctionExpression(UVLCppParser::AvgAggregateFunctionExpressionContext *ctx) = 0;
  virtual void exitAvgAggregateFunctionExpression(UVLCppParser::AvgAggregateFunctionExpressionContext *ctx) = 0;

  virtual void enterStringAggregateFunctionExpression(UVLCppParser::StringAggregateFunctionExpressionContext *ctx) = 0;
  virtual void exitStringAggregateFunctionExpression(UVLCppParser::StringAggregateFunctionExpressionContext *ctx) = 0;

  virtual void enterNumericAggregateFunctionExpression(UVLCppParser::NumericAggregateFunctionExpressionContext *ctx) = 0;
  virtual void exitNumericAggregateFunctionExpression(UVLCppParser::NumericAggregateFunctionExpressionContext *ctx) = 0;

  virtual void enterSumAggregateFunction(UVLCppParser::SumAggregateFunctionContext *ctx) = 0;
  virtual void exitSumAggregateFunction(UVLCppParser::SumAggregateFunctionContext *ctx) = 0;

  virtual void enterAvgAggregateFunction(UVLCppParser::AvgAggregateFunctionContext *ctx) = 0;
  virtual void exitAvgAggregateFunction(UVLCppParser::AvgAggregateFunctionContext *ctx) = 0;

  virtual void enterLengthAggregateFunction(UVLCppParser::LengthAggregateFunctionContext *ctx) = 0;
  virtual void exitLengthAggregateFunction(UVLCppParser::LengthAggregateFunctionContext *ctx) = 0;

  virtual void enterFloorAggregateFunction(UVLCppParser::FloorAggregateFunctionContext *ctx) = 0;
  virtual void exitFloorAggregateFunction(UVLCppParser::FloorAggregateFunctionContext *ctx) = 0;

  virtual void enterCeilAggregateFunction(UVLCppParser::CeilAggregateFunctionContext *ctx) = 0;
  virtual void exitCeilAggregateFunction(UVLCppParser::CeilAggregateFunctionContext *ctx) = 0;

  virtual void enterReference(UVLCppParser::ReferenceContext *ctx) = 0;
  virtual void exitReference(UVLCppParser::ReferenceContext *ctx) = 0;

  virtual void enterId(UVLCppParser::IdContext *ctx) = 0;
  virtual void exitId(UVLCppParser::IdContext *ctx) = 0;

  virtual void enterFeatureType(UVLCppParser::FeatureTypeContext *ctx) = 0;
  virtual void exitFeatureType(UVLCppParser::FeatureTypeContext *ctx) = 0;

  virtual void enterLanguageLevel(UVLCppParser::LanguageLevelContext *ctx) = 0;
  virtual void exitLanguageLevel(UVLCppParser::LanguageLevelContext *ctx) = 0;

  virtual void enterMajorLevel(UVLCppParser::MajorLevelContext *ctx) = 0;
  virtual void exitMajorLevel(UVLCppParser::MajorLevelContext *ctx) = 0;

  virtual void enterMinorLevel(UVLCppParser::MinorLevelContext *ctx) = 0;
  virtual void exitMinorLevel(UVLCppParser::MinorLevelContext *ctx) = 0;


};

