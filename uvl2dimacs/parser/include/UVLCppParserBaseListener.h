
// Generated from UVLCppParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "UVLCppParserListener.h"


/**
 * This class provides an empty implementation of UVLCppParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  UVLCppParserBaseListener : public UVLCppParserListener {
public:

  virtual void enterFeatureModel(UVLCppParser::FeatureModelContext * /*ctx*/) override { }
  virtual void exitFeatureModel(UVLCppParser::FeatureModelContext * /*ctx*/) override { }

  virtual void enterIncludes(UVLCppParser::IncludesContext * /*ctx*/) override { }
  virtual void exitIncludes(UVLCppParser::IncludesContext * /*ctx*/) override { }

  virtual void enterIncludeLine(UVLCppParser::IncludeLineContext * /*ctx*/) override { }
  virtual void exitIncludeLine(UVLCppParser::IncludeLineContext * /*ctx*/) override { }

  virtual void enterNamespace(UVLCppParser::NamespaceContext * /*ctx*/) override { }
  virtual void exitNamespace(UVLCppParser::NamespaceContext * /*ctx*/) override { }

  virtual void enterImports(UVLCppParser::ImportsContext * /*ctx*/) override { }
  virtual void exitImports(UVLCppParser::ImportsContext * /*ctx*/) override { }

  virtual void enterImportLine(UVLCppParser::ImportLineContext * /*ctx*/) override { }
  virtual void exitImportLine(UVLCppParser::ImportLineContext * /*ctx*/) override { }

  virtual void enterFeatures(UVLCppParser::FeaturesContext * /*ctx*/) override { }
  virtual void exitFeatures(UVLCppParser::FeaturesContext * /*ctx*/) override { }

  virtual void enterOrGroup(UVLCppParser::OrGroupContext * /*ctx*/) override { }
  virtual void exitOrGroup(UVLCppParser::OrGroupContext * /*ctx*/) override { }

  virtual void enterAlternativeGroup(UVLCppParser::AlternativeGroupContext * /*ctx*/) override { }
  virtual void exitAlternativeGroup(UVLCppParser::AlternativeGroupContext * /*ctx*/) override { }

  virtual void enterOptionalGroup(UVLCppParser::OptionalGroupContext * /*ctx*/) override { }
  virtual void exitOptionalGroup(UVLCppParser::OptionalGroupContext * /*ctx*/) override { }

  virtual void enterMandatoryGroup(UVLCppParser::MandatoryGroupContext * /*ctx*/) override { }
  virtual void exitMandatoryGroup(UVLCppParser::MandatoryGroupContext * /*ctx*/) override { }

  virtual void enterCardinalityGroup(UVLCppParser::CardinalityGroupContext * /*ctx*/) override { }
  virtual void exitCardinalityGroup(UVLCppParser::CardinalityGroupContext * /*ctx*/) override { }

  virtual void enterGroupSpec(UVLCppParser::GroupSpecContext * /*ctx*/) override { }
  virtual void exitGroupSpec(UVLCppParser::GroupSpecContext * /*ctx*/) override { }

  virtual void enterFeature(UVLCppParser::FeatureContext * /*ctx*/) override { }
  virtual void exitFeature(UVLCppParser::FeatureContext * /*ctx*/) override { }

  virtual void enterFeatureCardinality(UVLCppParser::FeatureCardinalityContext * /*ctx*/) override { }
  virtual void exitFeatureCardinality(UVLCppParser::FeatureCardinalityContext * /*ctx*/) override { }

  virtual void enterAttributes(UVLCppParser::AttributesContext * /*ctx*/) override { }
  virtual void exitAttributes(UVLCppParser::AttributesContext * /*ctx*/) override { }

  virtual void enterAttribute(UVLCppParser::AttributeContext * /*ctx*/) override { }
  virtual void exitAttribute(UVLCppParser::AttributeContext * /*ctx*/) override { }

  virtual void enterValueAttribute(UVLCppParser::ValueAttributeContext * /*ctx*/) override { }
  virtual void exitValueAttribute(UVLCppParser::ValueAttributeContext * /*ctx*/) override { }

  virtual void enterKey(UVLCppParser::KeyContext * /*ctx*/) override { }
  virtual void exitKey(UVLCppParser::KeyContext * /*ctx*/) override { }

  virtual void enterValue(UVLCppParser::ValueContext * /*ctx*/) override { }
  virtual void exitValue(UVLCppParser::ValueContext * /*ctx*/) override { }

  virtual void enterVector(UVLCppParser::VectorContext * /*ctx*/) override { }
  virtual void exitVector(UVLCppParser::VectorContext * /*ctx*/) override { }

  virtual void enterSingleConstraintAttribute(UVLCppParser::SingleConstraintAttributeContext * /*ctx*/) override { }
  virtual void exitSingleConstraintAttribute(UVLCppParser::SingleConstraintAttributeContext * /*ctx*/) override { }

  virtual void enterListConstraintAttribute(UVLCppParser::ListConstraintAttributeContext * /*ctx*/) override { }
  virtual void exitListConstraintAttribute(UVLCppParser::ListConstraintAttributeContext * /*ctx*/) override { }

  virtual void enterConstraintList(UVLCppParser::ConstraintListContext * /*ctx*/) override { }
  virtual void exitConstraintList(UVLCppParser::ConstraintListContext * /*ctx*/) override { }

  virtual void enterConstraints(UVLCppParser::ConstraintsContext * /*ctx*/) override { }
  virtual void exitConstraints(UVLCppParser::ConstraintsContext * /*ctx*/) override { }

  virtual void enterConstraintLine(UVLCppParser::ConstraintLineContext * /*ctx*/) override { }
  virtual void exitConstraintLine(UVLCppParser::ConstraintLineContext * /*ctx*/) override { }

  virtual void enterOrConstraint(UVLCppParser::OrConstraintContext * /*ctx*/) override { }
  virtual void exitOrConstraint(UVLCppParser::OrConstraintContext * /*ctx*/) override { }

  virtual void enterEquationConstraint(UVLCppParser::EquationConstraintContext * /*ctx*/) override { }
  virtual void exitEquationConstraint(UVLCppParser::EquationConstraintContext * /*ctx*/) override { }

  virtual void enterLiteralConstraint(UVLCppParser::LiteralConstraintContext * /*ctx*/) override { }
  virtual void exitLiteralConstraint(UVLCppParser::LiteralConstraintContext * /*ctx*/) override { }

  virtual void enterParenthesisConstraint(UVLCppParser::ParenthesisConstraintContext * /*ctx*/) override { }
  virtual void exitParenthesisConstraint(UVLCppParser::ParenthesisConstraintContext * /*ctx*/) override { }

  virtual void enterNotConstraint(UVLCppParser::NotConstraintContext * /*ctx*/) override { }
  virtual void exitNotConstraint(UVLCppParser::NotConstraintContext * /*ctx*/) override { }

  virtual void enterAndConstraint(UVLCppParser::AndConstraintContext * /*ctx*/) override { }
  virtual void exitAndConstraint(UVLCppParser::AndConstraintContext * /*ctx*/) override { }

  virtual void enterEquivalenceConstraint(UVLCppParser::EquivalenceConstraintContext * /*ctx*/) override { }
  virtual void exitEquivalenceConstraint(UVLCppParser::EquivalenceConstraintContext * /*ctx*/) override { }

  virtual void enterImplicationConstraint(UVLCppParser::ImplicationConstraintContext * /*ctx*/) override { }
  virtual void exitImplicationConstraint(UVLCppParser::ImplicationConstraintContext * /*ctx*/) override { }

  virtual void enterEqualEquation(UVLCppParser::EqualEquationContext * /*ctx*/) override { }
  virtual void exitEqualEquation(UVLCppParser::EqualEquationContext * /*ctx*/) override { }

  virtual void enterLowerEquation(UVLCppParser::LowerEquationContext * /*ctx*/) override { }
  virtual void exitLowerEquation(UVLCppParser::LowerEquationContext * /*ctx*/) override { }

  virtual void enterGreaterEquation(UVLCppParser::GreaterEquationContext * /*ctx*/) override { }
  virtual void exitGreaterEquation(UVLCppParser::GreaterEquationContext * /*ctx*/) override { }

  virtual void enterLowerEqualsEquation(UVLCppParser::LowerEqualsEquationContext * /*ctx*/) override { }
  virtual void exitLowerEqualsEquation(UVLCppParser::LowerEqualsEquationContext * /*ctx*/) override { }

  virtual void enterGreaterEqualsEquation(UVLCppParser::GreaterEqualsEquationContext * /*ctx*/) override { }
  virtual void exitGreaterEqualsEquation(UVLCppParser::GreaterEqualsEquationContext * /*ctx*/) override { }

  virtual void enterNotEqualsEquation(UVLCppParser::NotEqualsEquationContext * /*ctx*/) override { }
  virtual void exitNotEqualsEquation(UVLCppParser::NotEqualsEquationContext * /*ctx*/) override { }

  virtual void enterExpression(UVLCppParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(UVLCppParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterMultiplicativeExpr(UVLCppParser::MultiplicativeExprContext * /*ctx*/) override { }
  virtual void exitMultiplicativeExpr(UVLCppParser::MultiplicativeExprContext * /*ctx*/) override { }

  virtual void enterAddExpression(UVLCppParser::AddExpressionContext * /*ctx*/) override { }
  virtual void exitAddExpression(UVLCppParser::AddExpressionContext * /*ctx*/) override { }

  virtual void enterSubExpression(UVLCppParser::SubExpressionContext * /*ctx*/) override { }
  virtual void exitSubExpression(UVLCppParser::SubExpressionContext * /*ctx*/) override { }

  virtual void enterPrimaryExpressionExpression(UVLCppParser::PrimaryExpressionExpressionContext * /*ctx*/) override { }
  virtual void exitPrimaryExpressionExpression(UVLCppParser::PrimaryExpressionExpressionContext * /*ctx*/) override { }

  virtual void enterDivExpression(UVLCppParser::DivExpressionContext * /*ctx*/) override { }
  virtual void exitDivExpression(UVLCppParser::DivExpressionContext * /*ctx*/) override { }

  virtual void enterMulExpression(UVLCppParser::MulExpressionContext * /*ctx*/) override { }
  virtual void exitMulExpression(UVLCppParser::MulExpressionContext * /*ctx*/) override { }

  virtual void enterFloatLiteralExpression(UVLCppParser::FloatLiteralExpressionContext * /*ctx*/) override { }
  virtual void exitFloatLiteralExpression(UVLCppParser::FloatLiteralExpressionContext * /*ctx*/) override { }

  virtual void enterIntegerLiteralExpression(UVLCppParser::IntegerLiteralExpressionContext * /*ctx*/) override { }
  virtual void exitIntegerLiteralExpression(UVLCppParser::IntegerLiteralExpressionContext * /*ctx*/) override { }

  virtual void enterStringLiteralExpression(UVLCppParser::StringLiteralExpressionContext * /*ctx*/) override { }
  virtual void exitStringLiteralExpression(UVLCppParser::StringLiteralExpressionContext * /*ctx*/) override { }

  virtual void enterAggregateFunctionExpression(UVLCppParser::AggregateFunctionExpressionContext * /*ctx*/) override { }
  virtual void exitAggregateFunctionExpression(UVLCppParser::AggregateFunctionExpressionContext * /*ctx*/) override { }

  virtual void enterLiteralExpression(UVLCppParser::LiteralExpressionContext * /*ctx*/) override { }
  virtual void exitLiteralExpression(UVLCppParser::LiteralExpressionContext * /*ctx*/) override { }

  virtual void enterBracketExpression(UVLCppParser::BracketExpressionContext * /*ctx*/) override { }
  virtual void exitBracketExpression(UVLCppParser::BracketExpressionContext * /*ctx*/) override { }

  virtual void enterSumAggregateFunctionExpression(UVLCppParser::SumAggregateFunctionExpressionContext * /*ctx*/) override { }
  virtual void exitSumAggregateFunctionExpression(UVLCppParser::SumAggregateFunctionExpressionContext * /*ctx*/) override { }

  virtual void enterAvgAggregateFunctionExpression(UVLCppParser::AvgAggregateFunctionExpressionContext * /*ctx*/) override { }
  virtual void exitAvgAggregateFunctionExpression(UVLCppParser::AvgAggregateFunctionExpressionContext * /*ctx*/) override { }

  virtual void enterStringAggregateFunctionExpression(UVLCppParser::StringAggregateFunctionExpressionContext * /*ctx*/) override { }
  virtual void exitStringAggregateFunctionExpression(UVLCppParser::StringAggregateFunctionExpressionContext * /*ctx*/) override { }

  virtual void enterNumericAggregateFunctionExpression(UVLCppParser::NumericAggregateFunctionExpressionContext * /*ctx*/) override { }
  virtual void exitNumericAggregateFunctionExpression(UVLCppParser::NumericAggregateFunctionExpressionContext * /*ctx*/) override { }

  virtual void enterSumAggregateFunction(UVLCppParser::SumAggregateFunctionContext * /*ctx*/) override { }
  virtual void exitSumAggregateFunction(UVLCppParser::SumAggregateFunctionContext * /*ctx*/) override { }

  virtual void enterAvgAggregateFunction(UVLCppParser::AvgAggregateFunctionContext * /*ctx*/) override { }
  virtual void exitAvgAggregateFunction(UVLCppParser::AvgAggregateFunctionContext * /*ctx*/) override { }

  virtual void enterLengthAggregateFunction(UVLCppParser::LengthAggregateFunctionContext * /*ctx*/) override { }
  virtual void exitLengthAggregateFunction(UVLCppParser::LengthAggregateFunctionContext * /*ctx*/) override { }

  virtual void enterFloorAggregateFunction(UVLCppParser::FloorAggregateFunctionContext * /*ctx*/) override { }
  virtual void exitFloorAggregateFunction(UVLCppParser::FloorAggregateFunctionContext * /*ctx*/) override { }

  virtual void enterCeilAggregateFunction(UVLCppParser::CeilAggregateFunctionContext * /*ctx*/) override { }
  virtual void exitCeilAggregateFunction(UVLCppParser::CeilAggregateFunctionContext * /*ctx*/) override { }

  virtual void enterReference(UVLCppParser::ReferenceContext * /*ctx*/) override { }
  virtual void exitReference(UVLCppParser::ReferenceContext * /*ctx*/) override { }

  virtual void enterId(UVLCppParser::IdContext * /*ctx*/) override { }
  virtual void exitId(UVLCppParser::IdContext * /*ctx*/) override { }

  virtual void enterFeatureType(UVLCppParser::FeatureTypeContext * /*ctx*/) override { }
  virtual void exitFeatureType(UVLCppParser::FeatureTypeContext * /*ctx*/) override { }

  virtual void enterLanguageLevel(UVLCppParser::LanguageLevelContext * /*ctx*/) override { }
  virtual void exitLanguageLevel(UVLCppParser::LanguageLevelContext * /*ctx*/) override { }

  virtual void enterMajorLevel(UVLCppParser::MajorLevelContext * /*ctx*/) override { }
  virtual void exitMajorLevel(UVLCppParser::MajorLevelContext * /*ctx*/) override { }

  virtual void enterMinorLevel(UVLCppParser::MinorLevelContext * /*ctx*/) override { }
  virtual void exitMinorLevel(UVLCppParser::MinorLevelContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

