
// Generated from UVLCppParser.g4 by ANTLR 4.13.1


#include "UVLCppParserListener.h"

#include "UVLCppParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct UVLCppParserStaticData final {
  UVLCppParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  UVLCppParserStaticData(const UVLCppParserStaticData&) = delete;
  UVLCppParserStaticData(UVLCppParserStaticData&&) = delete;
  UVLCppParserStaticData& operator=(const UVLCppParserStaticData&) = delete;
  UVLCppParserStaticData& operator=(UVLCppParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag uvlcppparserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
UVLCppParserStaticData *uvlcppparserParserStaticData = nullptr;

void uvlcppparserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (uvlcppparserParserStaticData != nullptr) {
    return;
  }
#else
  assert(uvlcppparserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<UVLCppParserStaticData>(
    std::vector<std::string>{
      "featureModel", "includes", "includeLine", "namespace", "imports", 
      "importLine", "features", "group", "groupSpec", "feature", "featureCardinality", 
      "attributes", "attribute", "valueAttribute", "key", "value", "vector", 
      "constraintAttribute", "constraintList", "constraints", "constraintLine", 
      "constraint", "equation", "expression", "additiveExpression", "multiplicativeExpression", 
      "primaryExpression", "aggregateFunction", "sumAggregateFunction", 
      "avgAggregateFunction", "stringAggregateFunction", "numericAggregateFunction", 
      "reference", "id", "featureType", "languageLevel", "majorLevel", "minorLevel"
    },
    std::vector<std::string>{
      "", "'('", "')'", "'['", "']'", "'{'", "'}'", "'/*'", "'*/'", "", 
      "'<INDENT>'", "'<DEDENT>'", "'include'", "'features'", "'imports'", 
      "'namespace'", "'as'", "'constraint'", "'constraints'", "'cardinality'", 
      "'String'", "'Boolean'", "'Integer'", "'Real'", "'len'", "'sum'", 
      "'avg'", "'floor'", "'ceil'", "'Type'", "'Arithmetic'", "'group-cardinality'", 
      "'feature-cardinality'", "'aggregate-function'", "'string-constraints'", 
      "'or'", "'alternative'", "'optional'", "'mandatory'", "", "'!'", "'&'", 
      "'|'", "'<=>'", "'=>'", "'=='", "'<'", "'<='", "'>'", "'>='", "'!='", 
      "'/'", "'*'", "'+'", "'-'", "", "", "", "','", "'.'"
    },
    std::vector<std::string>{
      "", "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACK", "CLOSE_BRACK", "OPEN_BRACE", 
      "CLOSE_BRACE", "OPEN_COMMENT", "CLOSE_COMMENT", "NEWLINE", "INDENT", 
      "DEDENT", "INCLUDE_KEY", "FEATURES_KEY", "IMPORTS_KEY", "NAMESPACE_KEY", 
      "AS_KEY", "CONSTRAINT_KEY", "CONSTRAINTS_KEY", "CARDINALITY_KEY", 
      "STRING_KEY", "BOOLEAN_KEY", "INTEGER_KEY", "REAL_KEY", "LEN_KEY", 
      "SUM_KEY", "AVG_KEY", "FLOOR_KEY", "CEIL_KEY", "TYPE_KEY", "ARITHMETIC_KEY", 
      "GROUP_CARDINALITY_KEY", "FEATURE_CARDINALITY_KEY", "AGGREGATE_KEY", 
      "STRING_CONSTRAINTS_KEY", "ORGROUP", "ALTERNATIVE", "OPTIONAL", "MANDATORY", 
      "CARDINALITY", "NOT", "AND", "OR", "EQUIVALENCE", "IMPLICATION", "EQUAL", 
      "LOWER", "LOWER_EQUALS", "GREATER", "GREATER_EQUALS", "NOT_EQUALS", 
      "DIV", "MUL", "ADD", "SUB", "FLOAT", "INTEGER", "BOOLEAN", "COMMA", 
      "DOT", "ID_NOT_STRICT", "ID_STRICT", "STRING", "SKIP_"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,63,433,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,1,0,3,0,78,8,0,1,0,3,0,81,8,0,1,0,3,0,84,8,0,1,
  	0,3,0,87,8,0,1,0,3,0,90,8,0,1,0,3,0,93,8,0,1,0,3,0,96,8,0,1,0,3,0,99,
  	8,0,1,0,3,0,102,8,0,1,0,1,0,1,1,1,1,1,1,1,1,5,1,110,8,1,10,1,12,1,113,
  	9,1,1,1,1,1,1,2,1,2,1,2,1,3,1,3,1,3,1,4,1,4,1,4,1,4,5,4,127,8,4,10,4,
  	12,4,130,9,4,1,4,1,4,1,5,1,5,1,5,3,5,137,8,5,1,5,1,5,1,6,1,6,1,6,1,6,
  	1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,157,8,7,1,8,1,8,1,
  	8,4,8,162,8,8,11,8,12,8,163,1,8,1,8,1,9,3,9,169,8,9,1,9,1,9,3,9,173,8,
  	9,1,9,3,9,176,8,9,1,9,1,9,1,9,4,9,181,8,9,11,9,12,9,182,1,9,1,9,3,9,187,
  	8,9,1,10,1,10,1,10,1,11,1,11,1,11,1,11,5,11,196,8,11,10,11,12,11,199,
  	9,11,3,11,201,8,11,1,11,1,11,1,12,1,12,3,12,207,8,12,1,13,1,13,3,13,211,
  	8,13,1,14,1,14,1,15,1,15,1,15,1,15,1,15,1,15,3,15,221,8,15,1,16,1,16,
  	1,16,1,16,5,16,227,8,16,10,16,12,16,230,9,16,3,16,232,8,16,1,16,1,16,
  	1,17,1,17,1,17,1,17,3,17,240,8,17,1,18,1,18,1,18,1,18,5,18,246,8,18,10,
  	18,12,18,249,9,18,3,18,251,8,18,1,18,1,18,1,19,1,19,1,19,1,19,5,19,259,
  	8,19,10,19,12,19,262,9,19,1,19,1,19,1,20,1,20,1,20,1,21,1,21,1,21,1,21,
  	1,21,1,21,1,21,1,21,1,21,3,21,278,8,21,1,21,1,21,1,21,1,21,1,21,1,21,
  	1,21,1,21,1,21,1,21,1,21,1,21,5,21,292,8,21,10,21,12,21,295,9,21,1,22,
  	1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,
  	1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,3,22,321,8,22,1,23,1,23,
  	1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,5,24,334,8,24,10,24,12,24,
  	337,9,24,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,5,25,348,8,25,10,
  	25,12,25,351,9,25,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,3,26,362,
  	8,26,1,27,1,27,1,27,1,27,3,27,368,8,27,1,28,1,28,1,28,1,28,1,28,3,28,
  	375,8,28,1,28,1,28,1,28,1,29,1,29,1,29,1,29,1,29,3,29,385,8,29,1,29,1,
  	29,1,29,1,30,1,30,1,30,1,30,1,30,1,31,1,31,1,31,1,31,1,31,1,31,1,31,1,
  	31,1,31,1,31,3,31,405,8,31,1,32,1,32,1,32,5,32,410,8,32,10,32,12,32,413,
  	9,32,1,32,1,32,1,33,1,33,1,34,1,34,1,35,1,35,1,35,1,35,3,35,425,8,35,
  	3,35,427,8,35,1,36,1,36,1,37,1,37,1,37,0,3,42,48,50,38,0,2,4,6,8,10,12,
  	14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,
  	60,62,64,66,68,70,72,74,0,4,1,0,60,61,1,0,20,23,2,0,21,21,29,30,1,0,31,
  	34,461,0,77,1,0,0,0,2,105,1,0,0,0,4,116,1,0,0,0,6,119,1,0,0,0,8,122,1,
  	0,0,0,10,133,1,0,0,0,12,140,1,0,0,0,14,156,1,0,0,0,16,158,1,0,0,0,18,
  	168,1,0,0,0,20,188,1,0,0,0,22,191,1,0,0,0,24,206,1,0,0,0,26,208,1,0,0,
  	0,28,212,1,0,0,0,30,220,1,0,0,0,32,222,1,0,0,0,34,239,1,0,0,0,36,241,
  	1,0,0,0,38,254,1,0,0,0,40,265,1,0,0,0,42,277,1,0,0,0,44,320,1,0,0,0,46,
  	322,1,0,0,0,48,324,1,0,0,0,50,338,1,0,0,0,52,361,1,0,0,0,54,367,1,0,0,
  	0,56,369,1,0,0,0,58,379,1,0,0,0,60,389,1,0,0,0,62,404,1,0,0,0,64,411,
  	1,0,0,0,66,416,1,0,0,0,68,418,1,0,0,0,70,420,1,0,0,0,72,428,1,0,0,0,74,
  	430,1,0,0,0,76,78,3,6,3,0,77,76,1,0,0,0,77,78,1,0,0,0,78,80,1,0,0,0,79,
  	81,5,9,0,0,80,79,1,0,0,0,80,81,1,0,0,0,81,83,1,0,0,0,82,84,3,2,1,0,83,
  	82,1,0,0,0,83,84,1,0,0,0,84,86,1,0,0,0,85,87,5,9,0,0,86,85,1,0,0,0,86,
  	87,1,0,0,0,87,89,1,0,0,0,88,90,3,8,4,0,89,88,1,0,0,0,89,90,1,0,0,0,90,
  	92,1,0,0,0,91,93,5,9,0,0,92,91,1,0,0,0,92,93,1,0,0,0,93,95,1,0,0,0,94,
  	96,3,12,6,0,95,94,1,0,0,0,95,96,1,0,0,0,96,98,1,0,0,0,97,99,5,9,0,0,98,
  	97,1,0,0,0,98,99,1,0,0,0,99,101,1,0,0,0,100,102,3,38,19,0,101,100,1,0,
  	0,0,101,102,1,0,0,0,102,103,1,0,0,0,103,104,5,0,0,1,104,1,1,0,0,0,105,
  	106,5,12,0,0,106,107,5,9,0,0,107,111,5,10,0,0,108,110,3,4,2,0,109,108,
  	1,0,0,0,110,113,1,0,0,0,111,109,1,0,0,0,111,112,1,0,0,0,112,114,1,0,0,
  	0,113,111,1,0,0,0,114,115,5,11,0,0,115,3,1,0,0,0,116,117,3,70,35,0,117,
  	118,5,9,0,0,118,5,1,0,0,0,119,120,5,15,0,0,120,121,3,64,32,0,121,7,1,
  	0,0,0,122,123,5,14,0,0,123,124,5,9,0,0,124,128,5,10,0,0,125,127,3,10,
  	5,0,126,125,1,0,0,0,127,130,1,0,0,0,128,126,1,0,0,0,128,129,1,0,0,0,129,
  	131,1,0,0,0,130,128,1,0,0,0,131,132,5,11,0,0,132,9,1,0,0,0,133,136,3,
  	64,32,0,134,135,5,16,0,0,135,137,3,64,32,0,136,134,1,0,0,0,136,137,1,
  	0,0,0,137,138,1,0,0,0,138,139,5,9,0,0,139,11,1,0,0,0,140,141,5,13,0,0,
  	141,142,5,9,0,0,142,143,5,10,0,0,143,144,3,18,9,0,144,145,5,11,0,0,145,
  	13,1,0,0,0,146,147,5,35,0,0,147,157,3,16,8,0,148,149,5,36,0,0,149,157,
  	3,16,8,0,150,151,5,37,0,0,151,157,3,16,8,0,152,153,5,38,0,0,153,157,3,
  	16,8,0,154,155,5,39,0,0,155,157,3,16,8,0,156,146,1,0,0,0,156,148,1,0,
  	0,0,156,150,1,0,0,0,156,152,1,0,0,0,156,154,1,0,0,0,157,15,1,0,0,0,158,
  	159,5,9,0,0,159,161,5,10,0,0,160,162,3,18,9,0,161,160,1,0,0,0,162,163,
  	1,0,0,0,163,161,1,0,0,0,163,164,1,0,0,0,164,165,1,0,0,0,165,166,5,11,
  	0,0,166,17,1,0,0,0,167,169,3,68,34,0,168,167,1,0,0,0,168,169,1,0,0,0,
  	169,170,1,0,0,0,170,172,3,64,32,0,171,173,3,20,10,0,172,171,1,0,0,0,172,
  	173,1,0,0,0,173,175,1,0,0,0,174,176,3,22,11,0,175,174,1,0,0,0,175,176,
  	1,0,0,0,176,177,1,0,0,0,177,186,5,9,0,0,178,180,5,10,0,0,179,181,3,14,
  	7,0,180,179,1,0,0,0,181,182,1,0,0,0,182,180,1,0,0,0,182,183,1,0,0,0,183,
  	184,1,0,0,0,184,185,5,11,0,0,185,187,1,0,0,0,186,178,1,0,0,0,186,187,
  	1,0,0,0,187,19,1,0,0,0,188,189,5,19,0,0,189,190,5,39,0,0,190,21,1,0,0,
  	0,191,200,5,5,0,0,192,197,3,24,12,0,193,194,5,58,0,0,194,196,3,24,12,
  	0,195,193,1,0,0,0,196,199,1,0,0,0,197,195,1,0,0,0,197,198,1,0,0,0,198,
  	201,1,0,0,0,199,197,1,0,0,0,200,192,1,0,0,0,200,201,1,0,0,0,201,202,1,
  	0,0,0,202,203,5,6,0,0,203,23,1,0,0,0,204,207,3,26,13,0,205,207,3,34,17,
  	0,206,204,1,0,0,0,206,205,1,0,0,0,207,25,1,0,0,0,208,210,3,28,14,0,209,
  	211,3,30,15,0,210,209,1,0,0,0,210,211,1,0,0,0,211,27,1,0,0,0,212,213,
  	3,66,33,0,213,29,1,0,0,0,214,221,5,57,0,0,215,221,5,55,0,0,216,221,5,
  	56,0,0,217,221,5,62,0,0,218,221,3,22,11,0,219,221,3,32,16,0,220,214,1,
  	0,0,0,220,215,1,0,0,0,220,216,1,0,0,0,220,217,1,0,0,0,220,218,1,0,0,0,
  	220,219,1,0,0,0,221,31,1,0,0,0,222,231,5,3,0,0,223,228,3,30,15,0,224,
  	225,5,58,0,0,225,227,3,30,15,0,226,224,1,0,0,0,227,230,1,0,0,0,228,226,
  	1,0,0,0,228,229,1,0,0,0,229,232,1,0,0,0,230,228,1,0,0,0,231,223,1,0,0,
  	0,231,232,1,0,0,0,232,233,1,0,0,0,233,234,5,4,0,0,234,33,1,0,0,0,235,
  	236,5,17,0,0,236,240,3,42,21,0,237,238,5,18,0,0,238,240,3,36,18,0,239,
  	235,1,0,0,0,239,237,1,0,0,0,240,35,1,0,0,0,241,250,5,3,0,0,242,247,3,
  	42,21,0,243,244,5,58,0,0,244,246,3,42,21,0,245,243,1,0,0,0,246,249,1,
  	0,0,0,247,245,1,0,0,0,247,248,1,0,0,0,248,251,1,0,0,0,249,247,1,0,0,0,
  	250,242,1,0,0,0,250,251,1,0,0,0,251,252,1,0,0,0,252,253,5,4,0,0,253,37,
  	1,0,0,0,254,255,5,18,0,0,255,256,5,9,0,0,256,260,5,10,0,0,257,259,3,40,
  	20,0,258,257,1,0,0,0,259,262,1,0,0,0,260,258,1,0,0,0,260,261,1,0,0,0,
  	261,263,1,0,0,0,262,260,1,0,0,0,263,264,5,11,0,0,264,39,1,0,0,0,265,266,
  	3,42,21,0,266,267,5,9,0,0,267,41,1,0,0,0,268,269,6,21,-1,0,269,278,3,
  	44,22,0,270,278,3,64,32,0,271,272,5,1,0,0,272,273,3,42,21,0,273,274,5,
  	2,0,0,274,278,1,0,0,0,275,276,5,40,0,0,276,278,3,42,21,5,277,268,1,0,
  	0,0,277,270,1,0,0,0,277,271,1,0,0,0,277,275,1,0,0,0,278,293,1,0,0,0,279,
  	280,10,4,0,0,280,281,5,41,0,0,281,292,3,42,21,5,282,283,10,3,0,0,283,
  	284,5,42,0,0,284,292,3,42,21,4,285,286,10,2,0,0,286,287,5,44,0,0,287,
  	292,3,42,21,3,288,289,10,1,0,0,289,290,5,43,0,0,290,292,3,42,21,2,291,
  	279,1,0,0,0,291,282,1,0,0,0,291,285,1,0,0,0,291,288,1,0,0,0,292,295,1,
  	0,0,0,293,291,1,0,0,0,293,294,1,0,0,0,294,43,1,0,0,0,295,293,1,0,0,0,
  	296,297,3,46,23,0,297,298,5,45,0,0,298,299,3,46,23,0,299,321,1,0,0,0,
  	300,301,3,46,23,0,301,302,5,46,0,0,302,303,3,46,23,0,303,321,1,0,0,0,
  	304,305,3,46,23,0,305,306,5,48,0,0,306,307,3,46,23,0,307,321,1,0,0,0,
  	308,309,3,46,23,0,309,310,5,47,0,0,310,311,3,46,23,0,311,321,1,0,0,0,
  	312,313,3,46,23,0,313,314,5,49,0,0,314,315,3,46,23,0,315,321,1,0,0,0,
  	316,317,3,46,23,0,317,318,5,50,0,0,318,319,3,46,23,0,319,321,1,0,0,0,
  	320,296,1,0,0,0,320,300,1,0,0,0,320,304,1,0,0,0,320,308,1,0,0,0,320,312,
  	1,0,0,0,320,316,1,0,0,0,321,45,1,0,0,0,322,323,3,48,24,0,323,47,1,0,0,
  	0,324,325,6,24,-1,0,325,326,3,50,25,0,326,335,1,0,0,0,327,328,10,3,0,
  	0,328,329,5,53,0,0,329,334,3,50,25,0,330,331,10,2,0,0,331,332,5,54,0,
  	0,332,334,3,50,25,0,333,327,1,0,0,0,333,330,1,0,0,0,334,337,1,0,0,0,335,
  	333,1,0,0,0,335,336,1,0,0,0,336,49,1,0,0,0,337,335,1,0,0,0,338,339,6,
  	25,-1,0,339,340,3,52,26,0,340,349,1,0,0,0,341,342,10,3,0,0,342,343,5,
  	52,0,0,343,348,3,52,26,0,344,345,10,2,0,0,345,346,5,51,0,0,346,348,3,
  	52,26,0,347,341,1,0,0,0,347,344,1,0,0,0,348,351,1,0,0,0,349,347,1,0,0,
  	0,349,350,1,0,0,0,350,51,1,0,0,0,351,349,1,0,0,0,352,362,5,55,0,0,353,
  	362,5,56,0,0,354,362,5,62,0,0,355,362,3,54,27,0,356,362,3,64,32,0,357,
  	358,5,1,0,0,358,359,3,46,23,0,359,360,5,2,0,0,360,362,1,0,0,0,361,352,
  	1,0,0,0,361,353,1,0,0,0,361,354,1,0,0,0,361,355,1,0,0,0,361,356,1,0,0,
  	0,361,357,1,0,0,0,362,53,1,0,0,0,363,368,3,56,28,0,364,368,3,58,29,0,
  	365,368,3,60,30,0,366,368,3,62,31,0,367,363,1,0,0,0,367,364,1,0,0,0,367,
  	365,1,0,0,0,367,366,1,0,0,0,368,55,1,0,0,0,369,370,5,25,0,0,370,374,5,
  	1,0,0,371,372,3,64,32,0,372,373,5,58,0,0,373,375,1,0,0,0,374,371,1,0,
  	0,0,374,375,1,0,0,0,375,376,1,0,0,0,376,377,3,64,32,0,377,378,5,2,0,0,
  	378,57,1,0,0,0,379,380,5,26,0,0,380,384,5,1,0,0,381,382,3,64,32,0,382,
  	383,5,58,0,0,383,385,1,0,0,0,384,381,1,0,0,0,384,385,1,0,0,0,385,386,
  	1,0,0,0,386,387,3,64,32,0,387,388,5,2,0,0,388,59,1,0,0,0,389,390,5,24,
  	0,0,390,391,5,1,0,0,391,392,3,64,32,0,392,393,5,2,0,0,393,61,1,0,0,0,
  	394,395,5,27,0,0,395,396,5,1,0,0,396,397,3,64,32,0,397,398,5,2,0,0,398,
  	405,1,0,0,0,399,400,5,28,0,0,400,401,5,1,0,0,401,402,3,64,32,0,402,403,
  	5,2,0,0,403,405,1,0,0,0,404,394,1,0,0,0,404,399,1,0,0,0,405,63,1,0,0,
  	0,406,407,3,66,33,0,407,408,5,59,0,0,408,410,1,0,0,0,409,406,1,0,0,0,
  	410,413,1,0,0,0,411,409,1,0,0,0,411,412,1,0,0,0,412,414,1,0,0,0,413,411,
  	1,0,0,0,414,415,3,66,33,0,415,65,1,0,0,0,416,417,7,0,0,0,417,67,1,0,0,
  	0,418,419,7,1,0,0,419,69,1,0,0,0,420,426,3,72,36,0,421,424,5,59,0,0,422,
  	425,3,74,37,0,423,425,5,52,0,0,424,422,1,0,0,0,424,423,1,0,0,0,425,427,
  	1,0,0,0,426,421,1,0,0,0,426,427,1,0,0,0,427,71,1,0,0,0,428,429,7,2,0,
  	0,429,73,1,0,0,0,430,431,7,3,0,0,431,75,1,0,0,0,46,77,80,83,86,89,92,
  	95,98,101,111,128,136,156,163,168,172,175,182,186,197,200,206,210,220,
  	228,231,239,247,250,260,277,291,293,320,333,335,347,349,361,367,374,384,
  	404,411,424,426
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  uvlcppparserParserStaticData = staticData.release();
}

}

UVLCppParser::UVLCppParser(TokenStream *input) : UVLCppParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

UVLCppParser::UVLCppParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  UVLCppParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *uvlcppparserParserStaticData->atn, uvlcppparserParserStaticData->decisionToDFA, uvlcppparserParserStaticData->sharedContextCache, options);
}

UVLCppParser::~UVLCppParser() {
  delete _interpreter;
}

const atn::ATN& UVLCppParser::getATN() const {
  return *uvlcppparserParserStaticData->atn;
}

std::string UVLCppParser::getGrammarFileName() const {
  return "UVLCppParser.g4";
}

const std::vector<std::string>& UVLCppParser::getRuleNames() const {
  return uvlcppparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& UVLCppParser::getVocabulary() const {
  return uvlcppparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView UVLCppParser::getSerializedATN() const {
  return uvlcppparserParserStaticData->serializedATN;
}


//----------------- FeatureModelContext ------------------------------------------------------------------

UVLCppParser::FeatureModelContext::FeatureModelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::FeatureModelContext::EOF() {
  return getToken(UVLCppParser::EOF, 0);
}

UVLCppParser::NamespaceContext* UVLCppParser::FeatureModelContext::namespace_() {
  return getRuleContext<UVLCppParser::NamespaceContext>(0);
}

std::vector<tree::TerminalNode *> UVLCppParser::FeatureModelContext::NEWLINE() {
  return getTokens(UVLCppParser::NEWLINE);
}

tree::TerminalNode* UVLCppParser::FeatureModelContext::NEWLINE(size_t i) {
  return getToken(UVLCppParser::NEWLINE, i);
}

UVLCppParser::IncludesContext* UVLCppParser::FeatureModelContext::includes() {
  return getRuleContext<UVLCppParser::IncludesContext>(0);
}

UVLCppParser::ImportsContext* UVLCppParser::FeatureModelContext::imports() {
  return getRuleContext<UVLCppParser::ImportsContext>(0);
}

UVLCppParser::FeaturesContext* UVLCppParser::FeatureModelContext::features() {
  return getRuleContext<UVLCppParser::FeaturesContext>(0);
}

UVLCppParser::ConstraintsContext* UVLCppParser::FeatureModelContext::constraints() {
  return getRuleContext<UVLCppParser::ConstraintsContext>(0);
}


size_t UVLCppParser::FeatureModelContext::getRuleIndex() const {
  return UVLCppParser::RuleFeatureModel;
}

void UVLCppParser::FeatureModelContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFeatureModel(this);
}

void UVLCppParser::FeatureModelContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFeatureModel(this);
}

UVLCppParser::FeatureModelContext* UVLCppParser::featureModel() {
  FeatureModelContext *_localctx = _tracker.createInstance<FeatureModelContext>(_ctx, getState());
  enterRule(_localctx, 0, UVLCppParser::RuleFeatureModel);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(77);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::NAMESPACE_KEY) {
      setState(76);
      namespace_();
    }
    setState(80);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      setState(79);
      match(UVLCppParser::NEWLINE);
      break;
    }

    default:
      break;
    }
    setState(83);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::INCLUDE_KEY) {
      setState(82);
      includes();
    }
    setState(86);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(85);
      match(UVLCppParser::NEWLINE);
      break;
    }

    default:
      break;
    }
    setState(89);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::IMPORTS_KEY) {
      setState(88);
      imports();
    }
    setState(92);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      setState(91);
      match(UVLCppParser::NEWLINE);
      break;
    }

    default:
      break;
    }
    setState(95);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::FEATURES_KEY) {
      setState(94);
      features();
    }
    setState(98);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::NEWLINE) {
      setState(97);
      match(UVLCppParser::NEWLINE);
    }
    setState(101);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::CONSTRAINTS_KEY) {
      setState(100);
      constraints();
    }
    setState(103);
    match(UVLCppParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncludesContext ------------------------------------------------------------------

UVLCppParser::IncludesContext::IncludesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::IncludesContext::INCLUDE_KEY() {
  return getToken(UVLCppParser::INCLUDE_KEY, 0);
}

tree::TerminalNode* UVLCppParser::IncludesContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}

tree::TerminalNode* UVLCppParser::IncludesContext::INDENT() {
  return getToken(UVLCppParser::INDENT, 0);
}

tree::TerminalNode* UVLCppParser::IncludesContext::DEDENT() {
  return getToken(UVLCppParser::DEDENT, 0);
}

std::vector<UVLCppParser::IncludeLineContext *> UVLCppParser::IncludesContext::includeLine() {
  return getRuleContexts<UVLCppParser::IncludeLineContext>();
}

UVLCppParser::IncludeLineContext* UVLCppParser::IncludesContext::includeLine(size_t i) {
  return getRuleContext<UVLCppParser::IncludeLineContext>(i);
}


size_t UVLCppParser::IncludesContext::getRuleIndex() const {
  return UVLCppParser::RuleIncludes;
}

void UVLCppParser::IncludesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIncludes(this);
}

void UVLCppParser::IncludesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIncludes(this);
}

UVLCppParser::IncludesContext* UVLCppParser::includes() {
  IncludesContext *_localctx = _tracker.createInstance<IncludesContext>(_ctx, getState());
  enterRule(_localctx, 2, UVLCppParser::RuleIncludes);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(105);
    match(UVLCppParser::INCLUDE_KEY);
    setState(106);
    match(UVLCppParser::NEWLINE);
    setState(107);
    match(UVLCppParser::INDENT);
    setState(111);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1612709888) != 0)) {
      setState(108);
      includeLine();
      setState(113);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(114);
    match(UVLCppParser::DEDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncludeLineContext ------------------------------------------------------------------

UVLCppParser::IncludeLineContext::IncludeLineContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::LanguageLevelContext* UVLCppParser::IncludeLineContext::languageLevel() {
  return getRuleContext<UVLCppParser::LanguageLevelContext>(0);
}

tree::TerminalNode* UVLCppParser::IncludeLineContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}


size_t UVLCppParser::IncludeLineContext::getRuleIndex() const {
  return UVLCppParser::RuleIncludeLine;
}

void UVLCppParser::IncludeLineContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIncludeLine(this);
}

void UVLCppParser::IncludeLineContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIncludeLine(this);
}

UVLCppParser::IncludeLineContext* UVLCppParser::includeLine() {
  IncludeLineContext *_localctx = _tracker.createInstance<IncludeLineContext>(_ctx, getState());
  enterRule(_localctx, 4, UVLCppParser::RuleIncludeLine);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(116);
    languageLevel();
    setState(117);
    match(UVLCppParser::NEWLINE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamespaceContext ------------------------------------------------------------------

UVLCppParser::NamespaceContext::NamespaceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::NamespaceContext::NAMESPACE_KEY() {
  return getToken(UVLCppParser::NAMESPACE_KEY, 0);
}

UVLCppParser::ReferenceContext* UVLCppParser::NamespaceContext::reference() {
  return getRuleContext<UVLCppParser::ReferenceContext>(0);
}


size_t UVLCppParser::NamespaceContext::getRuleIndex() const {
  return UVLCppParser::RuleNamespace;
}

void UVLCppParser::NamespaceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNamespace(this);
}

void UVLCppParser::NamespaceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNamespace(this);
}

UVLCppParser::NamespaceContext* UVLCppParser::namespace_() {
  NamespaceContext *_localctx = _tracker.createInstance<NamespaceContext>(_ctx, getState());
  enterRule(_localctx, 6, UVLCppParser::RuleNamespace);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(119);
    match(UVLCppParser::NAMESPACE_KEY);
    setState(120);
    reference();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ImportsContext ------------------------------------------------------------------

UVLCppParser::ImportsContext::ImportsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::ImportsContext::IMPORTS_KEY() {
  return getToken(UVLCppParser::IMPORTS_KEY, 0);
}

tree::TerminalNode* UVLCppParser::ImportsContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}

tree::TerminalNode* UVLCppParser::ImportsContext::INDENT() {
  return getToken(UVLCppParser::INDENT, 0);
}

tree::TerminalNode* UVLCppParser::ImportsContext::DEDENT() {
  return getToken(UVLCppParser::DEDENT, 0);
}

std::vector<UVLCppParser::ImportLineContext *> UVLCppParser::ImportsContext::importLine() {
  return getRuleContexts<UVLCppParser::ImportLineContext>();
}

UVLCppParser::ImportLineContext* UVLCppParser::ImportsContext::importLine(size_t i) {
  return getRuleContext<UVLCppParser::ImportLineContext>(i);
}


size_t UVLCppParser::ImportsContext::getRuleIndex() const {
  return UVLCppParser::RuleImports;
}

void UVLCppParser::ImportsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterImports(this);
}

void UVLCppParser::ImportsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitImports(this);
}

UVLCppParser::ImportsContext* UVLCppParser::imports() {
  ImportsContext *_localctx = _tracker.createInstance<ImportsContext>(_ctx, getState());
  enterRule(_localctx, 8, UVLCppParser::RuleImports);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(122);
    match(UVLCppParser::IMPORTS_KEY);
    setState(123);
    match(UVLCppParser::NEWLINE);
    setState(124);
    match(UVLCppParser::INDENT);
    setState(128);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == UVLCppParser::ID_NOT_STRICT

    || _la == UVLCppParser::ID_STRICT) {
      setState(125);
      importLine();
      setState(130);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(131);
    match(UVLCppParser::DEDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ImportLineContext ------------------------------------------------------------------

UVLCppParser::ImportLineContext::ImportLineContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::ImportLineContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}

std::vector<UVLCppParser::ReferenceContext *> UVLCppParser::ImportLineContext::reference() {
  return getRuleContexts<UVLCppParser::ReferenceContext>();
}

UVLCppParser::ReferenceContext* UVLCppParser::ImportLineContext::reference(size_t i) {
  return getRuleContext<UVLCppParser::ReferenceContext>(i);
}

tree::TerminalNode* UVLCppParser::ImportLineContext::AS_KEY() {
  return getToken(UVLCppParser::AS_KEY, 0);
}


size_t UVLCppParser::ImportLineContext::getRuleIndex() const {
  return UVLCppParser::RuleImportLine;
}

void UVLCppParser::ImportLineContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterImportLine(this);
}

void UVLCppParser::ImportLineContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitImportLine(this);
}

UVLCppParser::ImportLineContext* UVLCppParser::importLine() {
  ImportLineContext *_localctx = _tracker.createInstance<ImportLineContext>(_ctx, getState());
  enterRule(_localctx, 10, UVLCppParser::RuleImportLine);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(133);
    antlrcpp::downCast<ImportLineContext *>(_localctx)->ns = reference();
    setState(136);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::AS_KEY) {
      setState(134);
      match(UVLCppParser::AS_KEY);
      setState(135);
      antlrcpp::downCast<ImportLineContext *>(_localctx)->alias = reference();
    }
    setState(138);
    match(UVLCppParser::NEWLINE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FeaturesContext ------------------------------------------------------------------

UVLCppParser::FeaturesContext::FeaturesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::FeaturesContext::FEATURES_KEY() {
  return getToken(UVLCppParser::FEATURES_KEY, 0);
}

tree::TerminalNode* UVLCppParser::FeaturesContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}

tree::TerminalNode* UVLCppParser::FeaturesContext::INDENT() {
  return getToken(UVLCppParser::INDENT, 0);
}

UVLCppParser::FeatureContext* UVLCppParser::FeaturesContext::feature() {
  return getRuleContext<UVLCppParser::FeatureContext>(0);
}

tree::TerminalNode* UVLCppParser::FeaturesContext::DEDENT() {
  return getToken(UVLCppParser::DEDENT, 0);
}


size_t UVLCppParser::FeaturesContext::getRuleIndex() const {
  return UVLCppParser::RuleFeatures;
}

void UVLCppParser::FeaturesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFeatures(this);
}

void UVLCppParser::FeaturesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFeatures(this);
}

UVLCppParser::FeaturesContext* UVLCppParser::features() {
  FeaturesContext *_localctx = _tracker.createInstance<FeaturesContext>(_ctx, getState());
  enterRule(_localctx, 12, UVLCppParser::RuleFeatures);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(140);
    match(UVLCppParser::FEATURES_KEY);
    setState(141);
    match(UVLCppParser::NEWLINE);
    setState(142);
    match(UVLCppParser::INDENT);
    setState(143);
    feature();
    setState(144);
    match(UVLCppParser::DEDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupContext ------------------------------------------------------------------

UVLCppParser::GroupContext::GroupContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::GroupContext::getRuleIndex() const {
  return UVLCppParser::RuleGroup;
}

void UVLCppParser::GroupContext::copyFrom(GroupContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AlternativeGroupContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::AlternativeGroupContext::ALTERNATIVE() {
  return getToken(UVLCppParser::ALTERNATIVE, 0);
}

UVLCppParser::GroupSpecContext* UVLCppParser::AlternativeGroupContext::groupSpec() {
  return getRuleContext<UVLCppParser::GroupSpecContext>(0);
}

UVLCppParser::AlternativeGroupContext::AlternativeGroupContext(GroupContext *ctx) { copyFrom(ctx); }

void UVLCppParser::AlternativeGroupContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAlternativeGroup(this);
}
void UVLCppParser::AlternativeGroupContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAlternativeGroup(this);
}
//----------------- OptionalGroupContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::OptionalGroupContext::OPTIONAL() {
  return getToken(UVLCppParser::OPTIONAL, 0);
}

UVLCppParser::GroupSpecContext* UVLCppParser::OptionalGroupContext::groupSpec() {
  return getRuleContext<UVLCppParser::GroupSpecContext>(0);
}

UVLCppParser::OptionalGroupContext::OptionalGroupContext(GroupContext *ctx) { copyFrom(ctx); }

void UVLCppParser::OptionalGroupContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptionalGroup(this);
}
void UVLCppParser::OptionalGroupContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptionalGroup(this);
}
//----------------- MandatoryGroupContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::MandatoryGroupContext::MANDATORY() {
  return getToken(UVLCppParser::MANDATORY, 0);
}

UVLCppParser::GroupSpecContext* UVLCppParser::MandatoryGroupContext::groupSpec() {
  return getRuleContext<UVLCppParser::GroupSpecContext>(0);
}

UVLCppParser::MandatoryGroupContext::MandatoryGroupContext(GroupContext *ctx) { copyFrom(ctx); }

void UVLCppParser::MandatoryGroupContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMandatoryGroup(this);
}
void UVLCppParser::MandatoryGroupContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMandatoryGroup(this);
}
//----------------- CardinalityGroupContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::CardinalityGroupContext::CARDINALITY() {
  return getToken(UVLCppParser::CARDINALITY, 0);
}

UVLCppParser::GroupSpecContext* UVLCppParser::CardinalityGroupContext::groupSpec() {
  return getRuleContext<UVLCppParser::GroupSpecContext>(0);
}

UVLCppParser::CardinalityGroupContext::CardinalityGroupContext(GroupContext *ctx) { copyFrom(ctx); }

void UVLCppParser::CardinalityGroupContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCardinalityGroup(this);
}
void UVLCppParser::CardinalityGroupContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCardinalityGroup(this);
}
//----------------- OrGroupContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::OrGroupContext::ORGROUP() {
  return getToken(UVLCppParser::ORGROUP, 0);
}

UVLCppParser::GroupSpecContext* UVLCppParser::OrGroupContext::groupSpec() {
  return getRuleContext<UVLCppParser::GroupSpecContext>(0);
}

UVLCppParser::OrGroupContext::OrGroupContext(GroupContext *ctx) { copyFrom(ctx); }

void UVLCppParser::OrGroupContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrGroup(this);
}
void UVLCppParser::OrGroupContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrGroup(this);
}
UVLCppParser::GroupContext* UVLCppParser::group() {
  GroupContext *_localctx = _tracker.createInstance<GroupContext>(_ctx, getState());
  enterRule(_localctx, 14, UVLCppParser::RuleGroup);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(156);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case UVLCppParser::ORGROUP: {
        _localctx = _tracker.createInstance<UVLCppParser::OrGroupContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(146);
        match(UVLCppParser::ORGROUP);
        setState(147);
        groupSpec();
        break;
      }

      case UVLCppParser::ALTERNATIVE: {
        _localctx = _tracker.createInstance<UVLCppParser::AlternativeGroupContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(148);
        match(UVLCppParser::ALTERNATIVE);
        setState(149);
        groupSpec();
        break;
      }

      case UVLCppParser::OPTIONAL: {
        _localctx = _tracker.createInstance<UVLCppParser::OptionalGroupContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(150);
        match(UVLCppParser::OPTIONAL);
        setState(151);
        groupSpec();
        break;
      }

      case UVLCppParser::MANDATORY: {
        _localctx = _tracker.createInstance<UVLCppParser::MandatoryGroupContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(152);
        match(UVLCppParser::MANDATORY);
        setState(153);
        groupSpec();
        break;
      }

      case UVLCppParser::CARDINALITY: {
        _localctx = _tracker.createInstance<UVLCppParser::CardinalityGroupContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(154);
        match(UVLCppParser::CARDINALITY);
        setState(155);
        groupSpec();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupSpecContext ------------------------------------------------------------------

UVLCppParser::GroupSpecContext::GroupSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::GroupSpecContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}

tree::TerminalNode* UVLCppParser::GroupSpecContext::INDENT() {
  return getToken(UVLCppParser::INDENT, 0);
}

tree::TerminalNode* UVLCppParser::GroupSpecContext::DEDENT() {
  return getToken(UVLCppParser::DEDENT, 0);
}

std::vector<UVLCppParser::FeatureContext *> UVLCppParser::GroupSpecContext::feature() {
  return getRuleContexts<UVLCppParser::FeatureContext>();
}

UVLCppParser::FeatureContext* UVLCppParser::GroupSpecContext::feature(size_t i) {
  return getRuleContext<UVLCppParser::FeatureContext>(i);
}


size_t UVLCppParser::GroupSpecContext::getRuleIndex() const {
  return UVLCppParser::RuleGroupSpec;
}

void UVLCppParser::GroupSpecContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGroupSpec(this);
}

void UVLCppParser::GroupSpecContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGroupSpec(this);
}

UVLCppParser::GroupSpecContext* UVLCppParser::groupSpec() {
  GroupSpecContext *_localctx = _tracker.createInstance<GroupSpecContext>(_ctx, getState());
  enterRule(_localctx, 16, UVLCppParser::RuleGroupSpec);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(158);
    match(UVLCppParser::NEWLINE);
    setState(159);
    match(UVLCppParser::INDENT);
    setState(161); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(160);
      feature();
      setState(163); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3458764513836269568) != 0));
    setState(165);
    match(UVLCppParser::DEDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FeatureContext ------------------------------------------------------------------

UVLCppParser::FeatureContext::FeatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::ReferenceContext* UVLCppParser::FeatureContext::reference() {
  return getRuleContext<UVLCppParser::ReferenceContext>(0);
}

tree::TerminalNode* UVLCppParser::FeatureContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}

UVLCppParser::FeatureTypeContext* UVLCppParser::FeatureContext::featureType() {
  return getRuleContext<UVLCppParser::FeatureTypeContext>(0);
}

UVLCppParser::FeatureCardinalityContext* UVLCppParser::FeatureContext::featureCardinality() {
  return getRuleContext<UVLCppParser::FeatureCardinalityContext>(0);
}

UVLCppParser::AttributesContext* UVLCppParser::FeatureContext::attributes() {
  return getRuleContext<UVLCppParser::AttributesContext>(0);
}

tree::TerminalNode* UVLCppParser::FeatureContext::INDENT() {
  return getToken(UVLCppParser::INDENT, 0);
}

tree::TerminalNode* UVLCppParser::FeatureContext::DEDENT() {
  return getToken(UVLCppParser::DEDENT, 0);
}

std::vector<UVLCppParser::GroupContext *> UVLCppParser::FeatureContext::group() {
  return getRuleContexts<UVLCppParser::GroupContext>();
}

UVLCppParser::GroupContext* UVLCppParser::FeatureContext::group(size_t i) {
  return getRuleContext<UVLCppParser::GroupContext>(i);
}


size_t UVLCppParser::FeatureContext::getRuleIndex() const {
  return UVLCppParser::RuleFeature;
}

void UVLCppParser::FeatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFeature(this);
}

void UVLCppParser::FeatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFeature(this);
}

UVLCppParser::FeatureContext* UVLCppParser::feature() {
  FeatureContext *_localctx = _tracker.createInstance<FeatureContext>(_ctx, getState());
  enterRule(_localctx, 18, UVLCppParser::RuleFeature);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(168);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 15728640) != 0)) {
      setState(167);
      featureType();
    }
    setState(170);
    reference();
    setState(172);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::CARDINALITY_KEY) {
      setState(171);
      featureCardinality();
    }
    setState(175);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::OPEN_BRACE) {
      setState(174);
      attributes();
    }
    setState(177);
    match(UVLCppParser::NEWLINE);
    setState(186);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::INDENT) {
      setState(178);
      match(UVLCppParser::INDENT);
      setState(180); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(179);
        group();
        setState(182); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 1065151889408) != 0));
      setState(184);
      match(UVLCppParser::DEDENT);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FeatureCardinalityContext ------------------------------------------------------------------

UVLCppParser::FeatureCardinalityContext::FeatureCardinalityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::FeatureCardinalityContext::CARDINALITY_KEY() {
  return getToken(UVLCppParser::CARDINALITY_KEY, 0);
}

tree::TerminalNode* UVLCppParser::FeatureCardinalityContext::CARDINALITY() {
  return getToken(UVLCppParser::CARDINALITY, 0);
}


size_t UVLCppParser::FeatureCardinalityContext::getRuleIndex() const {
  return UVLCppParser::RuleFeatureCardinality;
}

void UVLCppParser::FeatureCardinalityContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFeatureCardinality(this);
}

void UVLCppParser::FeatureCardinalityContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFeatureCardinality(this);
}

UVLCppParser::FeatureCardinalityContext* UVLCppParser::featureCardinality() {
  FeatureCardinalityContext *_localctx = _tracker.createInstance<FeatureCardinalityContext>(_ctx, getState());
  enterRule(_localctx, 20, UVLCppParser::RuleFeatureCardinality);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(188);
    match(UVLCppParser::CARDINALITY_KEY);
    setState(189);
    match(UVLCppParser::CARDINALITY);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributesContext ------------------------------------------------------------------

UVLCppParser::AttributesContext::AttributesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::AttributesContext::OPEN_BRACE() {
  return getToken(UVLCppParser::OPEN_BRACE, 0);
}

tree::TerminalNode* UVLCppParser::AttributesContext::CLOSE_BRACE() {
  return getToken(UVLCppParser::CLOSE_BRACE, 0);
}

std::vector<UVLCppParser::AttributeContext *> UVLCppParser::AttributesContext::attribute() {
  return getRuleContexts<UVLCppParser::AttributeContext>();
}

UVLCppParser::AttributeContext* UVLCppParser::AttributesContext::attribute(size_t i) {
  return getRuleContext<UVLCppParser::AttributeContext>(i);
}

std::vector<tree::TerminalNode *> UVLCppParser::AttributesContext::COMMA() {
  return getTokens(UVLCppParser::COMMA);
}

tree::TerminalNode* UVLCppParser::AttributesContext::COMMA(size_t i) {
  return getToken(UVLCppParser::COMMA, i);
}


size_t UVLCppParser::AttributesContext::getRuleIndex() const {
  return UVLCppParser::RuleAttributes;
}

void UVLCppParser::AttributesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAttributes(this);
}

void UVLCppParser::AttributesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAttributes(this);
}

UVLCppParser::AttributesContext* UVLCppParser::attributes() {
  AttributesContext *_localctx = _tracker.createInstance<AttributesContext>(_ctx, getState());
  enterRule(_localctx, 22, UVLCppParser::RuleAttributes);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(191);
    match(UVLCppParser::OPEN_BRACE);
    setState(200);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3458764513820934144) != 0)) {
      setState(192);
      attribute();
      setState(197);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == UVLCppParser::COMMA) {
        setState(193);
        match(UVLCppParser::COMMA);
        setState(194);
        attribute();
        setState(199);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(202);
    match(UVLCppParser::CLOSE_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributeContext ------------------------------------------------------------------

UVLCppParser::AttributeContext::AttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::ValueAttributeContext* UVLCppParser::AttributeContext::valueAttribute() {
  return getRuleContext<UVLCppParser::ValueAttributeContext>(0);
}

UVLCppParser::ConstraintAttributeContext* UVLCppParser::AttributeContext::constraintAttribute() {
  return getRuleContext<UVLCppParser::ConstraintAttributeContext>(0);
}


size_t UVLCppParser::AttributeContext::getRuleIndex() const {
  return UVLCppParser::RuleAttribute;
}

void UVLCppParser::AttributeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAttribute(this);
}

void UVLCppParser::AttributeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAttribute(this);
}

UVLCppParser::AttributeContext* UVLCppParser::attribute() {
  AttributeContext *_localctx = _tracker.createInstance<AttributeContext>(_ctx, getState());
  enterRule(_localctx, 24, UVLCppParser::RuleAttribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(206);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case UVLCppParser::ID_NOT_STRICT:
      case UVLCppParser::ID_STRICT: {
        enterOuterAlt(_localctx, 1);
        setState(204);
        valueAttribute();
        break;
      }

      case UVLCppParser::CONSTRAINT_KEY:
      case UVLCppParser::CONSTRAINTS_KEY: {
        enterOuterAlt(_localctx, 2);
        setState(205);
        constraintAttribute();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueAttributeContext ------------------------------------------------------------------

UVLCppParser::ValueAttributeContext::ValueAttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::KeyContext* UVLCppParser::ValueAttributeContext::key() {
  return getRuleContext<UVLCppParser::KeyContext>(0);
}

UVLCppParser::ValueContext* UVLCppParser::ValueAttributeContext::value() {
  return getRuleContext<UVLCppParser::ValueContext>(0);
}


size_t UVLCppParser::ValueAttributeContext::getRuleIndex() const {
  return UVLCppParser::RuleValueAttribute;
}

void UVLCppParser::ValueAttributeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValueAttribute(this);
}

void UVLCppParser::ValueAttributeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValueAttribute(this);
}

UVLCppParser::ValueAttributeContext* UVLCppParser::valueAttribute() {
  ValueAttributeContext *_localctx = _tracker.createInstance<ValueAttributeContext>(_ctx, getState());
  enterRule(_localctx, 26, UVLCppParser::RuleValueAttribute);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(208);
    key();
    setState(210);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4863887597560135720) != 0)) {
      setState(209);
      value();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeyContext ------------------------------------------------------------------

UVLCppParser::KeyContext::KeyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::IdContext* UVLCppParser::KeyContext::id() {
  return getRuleContext<UVLCppParser::IdContext>(0);
}


size_t UVLCppParser::KeyContext::getRuleIndex() const {
  return UVLCppParser::RuleKey;
}

void UVLCppParser::KeyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterKey(this);
}

void UVLCppParser::KeyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitKey(this);
}

UVLCppParser::KeyContext* UVLCppParser::key() {
  KeyContext *_localctx = _tracker.createInstance<KeyContext>(_ctx, getState());
  enterRule(_localctx, 28, UVLCppParser::RuleKey);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(212);
    id();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

UVLCppParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::ValueContext::BOOLEAN() {
  return getToken(UVLCppParser::BOOLEAN, 0);
}

tree::TerminalNode* UVLCppParser::ValueContext::FLOAT() {
  return getToken(UVLCppParser::FLOAT, 0);
}

tree::TerminalNode* UVLCppParser::ValueContext::INTEGER() {
  return getToken(UVLCppParser::INTEGER, 0);
}

tree::TerminalNode* UVLCppParser::ValueContext::STRING() {
  return getToken(UVLCppParser::STRING, 0);
}

UVLCppParser::AttributesContext* UVLCppParser::ValueContext::attributes() {
  return getRuleContext<UVLCppParser::AttributesContext>(0);
}

UVLCppParser::VectorContext* UVLCppParser::ValueContext::vector() {
  return getRuleContext<UVLCppParser::VectorContext>(0);
}


size_t UVLCppParser::ValueContext::getRuleIndex() const {
  return UVLCppParser::RuleValue;
}

void UVLCppParser::ValueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValue(this);
}

void UVLCppParser::ValueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValue(this);
}

UVLCppParser::ValueContext* UVLCppParser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 30, UVLCppParser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(220);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case UVLCppParser::BOOLEAN: {
        enterOuterAlt(_localctx, 1);
        setState(214);
        match(UVLCppParser::BOOLEAN);
        break;
      }

      case UVLCppParser::FLOAT: {
        enterOuterAlt(_localctx, 2);
        setState(215);
        match(UVLCppParser::FLOAT);
        break;
      }

      case UVLCppParser::INTEGER: {
        enterOuterAlt(_localctx, 3);
        setState(216);
        match(UVLCppParser::INTEGER);
        break;
      }

      case UVLCppParser::STRING: {
        enterOuterAlt(_localctx, 4);
        setState(217);
        match(UVLCppParser::STRING);
        break;
      }

      case UVLCppParser::OPEN_BRACE: {
        enterOuterAlt(_localctx, 5);
        setState(218);
        attributes();
        break;
      }

      case UVLCppParser::OPEN_BRACK: {
        enterOuterAlt(_localctx, 6);
        setState(219);
        vector();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VectorContext ------------------------------------------------------------------

UVLCppParser::VectorContext::VectorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::VectorContext::OPEN_BRACK() {
  return getToken(UVLCppParser::OPEN_BRACK, 0);
}

tree::TerminalNode* UVLCppParser::VectorContext::CLOSE_BRACK() {
  return getToken(UVLCppParser::CLOSE_BRACK, 0);
}

std::vector<UVLCppParser::ValueContext *> UVLCppParser::VectorContext::value() {
  return getRuleContexts<UVLCppParser::ValueContext>();
}

UVLCppParser::ValueContext* UVLCppParser::VectorContext::value(size_t i) {
  return getRuleContext<UVLCppParser::ValueContext>(i);
}

std::vector<tree::TerminalNode *> UVLCppParser::VectorContext::COMMA() {
  return getTokens(UVLCppParser::COMMA);
}

tree::TerminalNode* UVLCppParser::VectorContext::COMMA(size_t i) {
  return getToken(UVLCppParser::COMMA, i);
}


size_t UVLCppParser::VectorContext::getRuleIndex() const {
  return UVLCppParser::RuleVector;
}

void UVLCppParser::VectorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVector(this);
}

void UVLCppParser::VectorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVector(this);
}

UVLCppParser::VectorContext* UVLCppParser::vector() {
  VectorContext *_localctx = _tracker.createInstance<VectorContext>(_ctx, getState());
  enterRule(_localctx, 32, UVLCppParser::RuleVector);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(222);
    match(UVLCppParser::OPEN_BRACK);
    setState(231);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4863887597560135720) != 0)) {
      setState(223);
      value();
      setState(228);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == UVLCppParser::COMMA) {
        setState(224);
        match(UVLCppParser::COMMA);
        setState(225);
        value();
        setState(230);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(233);
    match(UVLCppParser::CLOSE_BRACK);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintAttributeContext ------------------------------------------------------------------

UVLCppParser::ConstraintAttributeContext::ConstraintAttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::ConstraintAttributeContext::getRuleIndex() const {
  return UVLCppParser::RuleConstraintAttribute;
}

void UVLCppParser::ConstraintAttributeContext::copyFrom(ConstraintAttributeContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ListConstraintAttributeContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::ListConstraintAttributeContext::CONSTRAINTS_KEY() {
  return getToken(UVLCppParser::CONSTRAINTS_KEY, 0);
}

UVLCppParser::ConstraintListContext* UVLCppParser::ListConstraintAttributeContext::constraintList() {
  return getRuleContext<UVLCppParser::ConstraintListContext>(0);
}

UVLCppParser::ListConstraintAttributeContext::ListConstraintAttributeContext(ConstraintAttributeContext *ctx) { copyFrom(ctx); }

void UVLCppParser::ListConstraintAttributeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterListConstraintAttribute(this);
}
void UVLCppParser::ListConstraintAttributeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitListConstraintAttribute(this);
}
//----------------- SingleConstraintAttributeContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::SingleConstraintAttributeContext::CONSTRAINT_KEY() {
  return getToken(UVLCppParser::CONSTRAINT_KEY, 0);
}

UVLCppParser::ConstraintContext* UVLCppParser::SingleConstraintAttributeContext::constraint() {
  return getRuleContext<UVLCppParser::ConstraintContext>(0);
}

UVLCppParser::SingleConstraintAttributeContext::SingleConstraintAttributeContext(ConstraintAttributeContext *ctx) { copyFrom(ctx); }

void UVLCppParser::SingleConstraintAttributeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSingleConstraintAttribute(this);
}
void UVLCppParser::SingleConstraintAttributeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSingleConstraintAttribute(this);
}
UVLCppParser::ConstraintAttributeContext* UVLCppParser::constraintAttribute() {
  ConstraintAttributeContext *_localctx = _tracker.createInstance<ConstraintAttributeContext>(_ctx, getState());
  enterRule(_localctx, 34, UVLCppParser::RuleConstraintAttribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(239);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case UVLCppParser::CONSTRAINT_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::SingleConstraintAttributeContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(235);
        match(UVLCppParser::CONSTRAINT_KEY);
        setState(236);
        constraint(0);
        break;
      }

      case UVLCppParser::CONSTRAINTS_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::ListConstraintAttributeContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(237);
        match(UVLCppParser::CONSTRAINTS_KEY);
        setState(238);
        constraintList();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintListContext ------------------------------------------------------------------

UVLCppParser::ConstraintListContext::ConstraintListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::ConstraintListContext::OPEN_BRACK() {
  return getToken(UVLCppParser::OPEN_BRACK, 0);
}

tree::TerminalNode* UVLCppParser::ConstraintListContext::CLOSE_BRACK() {
  return getToken(UVLCppParser::CLOSE_BRACK, 0);
}

std::vector<UVLCppParser::ConstraintContext *> UVLCppParser::ConstraintListContext::constraint() {
  return getRuleContexts<UVLCppParser::ConstraintContext>();
}

UVLCppParser::ConstraintContext* UVLCppParser::ConstraintListContext::constraint(size_t i) {
  return getRuleContext<UVLCppParser::ConstraintContext>(i);
}

std::vector<tree::TerminalNode *> UVLCppParser::ConstraintListContext::COMMA() {
  return getTokens(UVLCppParser::COMMA);
}

tree::TerminalNode* UVLCppParser::ConstraintListContext::COMMA(size_t i) {
  return getToken(UVLCppParser::COMMA, i);
}


size_t UVLCppParser::ConstraintListContext::getRuleIndex() const {
  return UVLCppParser::RuleConstraintList;
}

void UVLCppParser::ConstraintListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstraintList(this);
}

void UVLCppParser::ConstraintListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstraintList(this);
}

UVLCppParser::ConstraintListContext* UVLCppParser::constraintList() {
  ConstraintListContext *_localctx = _tracker.createInstance<ConstraintListContext>(_ctx, getState());
  enterRule(_localctx, 36, UVLCppParser::RuleConstraintList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(241);
    match(UVLCppParser::OPEN_BRACK);
    setState(250);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8178538023336542210) != 0)) {
      setState(242);
      constraint(0);
      setState(247);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == UVLCppParser::COMMA) {
        setState(243);
        match(UVLCppParser::COMMA);
        setState(244);
        constraint(0);
        setState(249);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(252);
    match(UVLCppParser::CLOSE_BRACK);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintsContext ------------------------------------------------------------------

UVLCppParser::ConstraintsContext::ConstraintsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::ConstraintsContext::CONSTRAINTS_KEY() {
  return getToken(UVLCppParser::CONSTRAINTS_KEY, 0);
}

tree::TerminalNode* UVLCppParser::ConstraintsContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}

tree::TerminalNode* UVLCppParser::ConstraintsContext::INDENT() {
  return getToken(UVLCppParser::INDENT, 0);
}

tree::TerminalNode* UVLCppParser::ConstraintsContext::DEDENT() {
  return getToken(UVLCppParser::DEDENT, 0);
}

std::vector<UVLCppParser::ConstraintLineContext *> UVLCppParser::ConstraintsContext::constraintLine() {
  return getRuleContexts<UVLCppParser::ConstraintLineContext>();
}

UVLCppParser::ConstraintLineContext* UVLCppParser::ConstraintsContext::constraintLine(size_t i) {
  return getRuleContext<UVLCppParser::ConstraintLineContext>(i);
}


size_t UVLCppParser::ConstraintsContext::getRuleIndex() const {
  return UVLCppParser::RuleConstraints;
}

void UVLCppParser::ConstraintsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstraints(this);
}

void UVLCppParser::ConstraintsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstraints(this);
}

UVLCppParser::ConstraintsContext* UVLCppParser::constraints() {
  ConstraintsContext *_localctx = _tracker.createInstance<ConstraintsContext>(_ctx, getState());
  enterRule(_localctx, 38, UVLCppParser::RuleConstraints);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(254);
    match(UVLCppParser::CONSTRAINTS_KEY);
    setState(255);
    match(UVLCppParser::NEWLINE);
    setState(256);
    match(UVLCppParser::INDENT);
    setState(260);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8178538023336542210) != 0)) {
      setState(257);
      constraintLine();
      setState(262);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(263);
    match(UVLCppParser::DEDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintLineContext ------------------------------------------------------------------

UVLCppParser::ConstraintLineContext::ConstraintLineContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::ConstraintContext* UVLCppParser::ConstraintLineContext::constraint() {
  return getRuleContext<UVLCppParser::ConstraintContext>(0);
}

tree::TerminalNode* UVLCppParser::ConstraintLineContext::NEWLINE() {
  return getToken(UVLCppParser::NEWLINE, 0);
}


size_t UVLCppParser::ConstraintLineContext::getRuleIndex() const {
  return UVLCppParser::RuleConstraintLine;
}

void UVLCppParser::ConstraintLineContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstraintLine(this);
}

void UVLCppParser::ConstraintLineContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstraintLine(this);
}

UVLCppParser::ConstraintLineContext* UVLCppParser::constraintLine() {
  ConstraintLineContext *_localctx = _tracker.createInstance<ConstraintLineContext>(_ctx, getState());
  enterRule(_localctx, 40, UVLCppParser::RuleConstraintLine);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(265);
    constraint(0);
    setState(266);
    match(UVLCppParser::NEWLINE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintContext ------------------------------------------------------------------

UVLCppParser::ConstraintContext::ConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::ConstraintContext::getRuleIndex() const {
  return UVLCppParser::RuleConstraint;
}

void UVLCppParser::ConstraintContext::copyFrom(ConstraintContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- OrConstraintContext ------------------------------------------------------------------

std::vector<UVLCppParser::ConstraintContext *> UVLCppParser::OrConstraintContext::constraint() {
  return getRuleContexts<UVLCppParser::ConstraintContext>();
}

UVLCppParser::ConstraintContext* UVLCppParser::OrConstraintContext::constraint(size_t i) {
  return getRuleContext<UVLCppParser::ConstraintContext>(i);
}

tree::TerminalNode* UVLCppParser::OrConstraintContext::OR() {
  return getToken(UVLCppParser::OR, 0);
}

UVLCppParser::OrConstraintContext::OrConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::OrConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrConstraint(this);
}
void UVLCppParser::OrConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrConstraint(this);
}
//----------------- EquationConstraintContext ------------------------------------------------------------------

UVLCppParser::EquationContext* UVLCppParser::EquationConstraintContext::equation() {
  return getRuleContext<UVLCppParser::EquationContext>(0);
}

UVLCppParser::EquationConstraintContext::EquationConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::EquationConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEquationConstraint(this);
}
void UVLCppParser::EquationConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEquationConstraint(this);
}
//----------------- LiteralConstraintContext ------------------------------------------------------------------

UVLCppParser::ReferenceContext* UVLCppParser::LiteralConstraintContext::reference() {
  return getRuleContext<UVLCppParser::ReferenceContext>(0);
}

UVLCppParser::LiteralConstraintContext::LiteralConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::LiteralConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteralConstraint(this);
}
void UVLCppParser::LiteralConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteralConstraint(this);
}
//----------------- ParenthesisConstraintContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::ParenthesisConstraintContext::OPEN_PAREN() {
  return getToken(UVLCppParser::OPEN_PAREN, 0);
}

UVLCppParser::ConstraintContext* UVLCppParser::ParenthesisConstraintContext::constraint() {
  return getRuleContext<UVLCppParser::ConstraintContext>(0);
}

tree::TerminalNode* UVLCppParser::ParenthesisConstraintContext::CLOSE_PAREN() {
  return getToken(UVLCppParser::CLOSE_PAREN, 0);
}

UVLCppParser::ParenthesisConstraintContext::ParenthesisConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::ParenthesisConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParenthesisConstraint(this);
}
void UVLCppParser::ParenthesisConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParenthesisConstraint(this);
}
//----------------- NotConstraintContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::NotConstraintContext::NOT() {
  return getToken(UVLCppParser::NOT, 0);
}

UVLCppParser::ConstraintContext* UVLCppParser::NotConstraintContext::constraint() {
  return getRuleContext<UVLCppParser::ConstraintContext>(0);
}

UVLCppParser::NotConstraintContext::NotConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::NotConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNotConstraint(this);
}
void UVLCppParser::NotConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNotConstraint(this);
}
//----------------- AndConstraintContext ------------------------------------------------------------------

std::vector<UVLCppParser::ConstraintContext *> UVLCppParser::AndConstraintContext::constraint() {
  return getRuleContexts<UVLCppParser::ConstraintContext>();
}

UVLCppParser::ConstraintContext* UVLCppParser::AndConstraintContext::constraint(size_t i) {
  return getRuleContext<UVLCppParser::ConstraintContext>(i);
}

tree::TerminalNode* UVLCppParser::AndConstraintContext::AND() {
  return getToken(UVLCppParser::AND, 0);
}

UVLCppParser::AndConstraintContext::AndConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::AndConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAndConstraint(this);
}
void UVLCppParser::AndConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAndConstraint(this);
}
//----------------- EquivalenceConstraintContext ------------------------------------------------------------------

std::vector<UVLCppParser::ConstraintContext *> UVLCppParser::EquivalenceConstraintContext::constraint() {
  return getRuleContexts<UVLCppParser::ConstraintContext>();
}

UVLCppParser::ConstraintContext* UVLCppParser::EquivalenceConstraintContext::constraint(size_t i) {
  return getRuleContext<UVLCppParser::ConstraintContext>(i);
}

tree::TerminalNode* UVLCppParser::EquivalenceConstraintContext::EQUIVALENCE() {
  return getToken(UVLCppParser::EQUIVALENCE, 0);
}

UVLCppParser::EquivalenceConstraintContext::EquivalenceConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::EquivalenceConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEquivalenceConstraint(this);
}
void UVLCppParser::EquivalenceConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEquivalenceConstraint(this);
}
//----------------- ImplicationConstraintContext ------------------------------------------------------------------

std::vector<UVLCppParser::ConstraintContext *> UVLCppParser::ImplicationConstraintContext::constraint() {
  return getRuleContexts<UVLCppParser::ConstraintContext>();
}

UVLCppParser::ConstraintContext* UVLCppParser::ImplicationConstraintContext::constraint(size_t i) {
  return getRuleContext<UVLCppParser::ConstraintContext>(i);
}

tree::TerminalNode* UVLCppParser::ImplicationConstraintContext::IMPLICATION() {
  return getToken(UVLCppParser::IMPLICATION, 0);
}

UVLCppParser::ImplicationConstraintContext::ImplicationConstraintContext(ConstraintContext *ctx) { copyFrom(ctx); }

void UVLCppParser::ImplicationConstraintContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterImplicationConstraint(this);
}
void UVLCppParser::ImplicationConstraintContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitImplicationConstraint(this);
}

UVLCppParser::ConstraintContext* UVLCppParser::constraint() {
   return constraint(0);
}

UVLCppParser::ConstraintContext* UVLCppParser::constraint(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  UVLCppParser::ConstraintContext *_localctx = _tracker.createInstance<ConstraintContext>(_ctx, parentState);
  UVLCppParser::ConstraintContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 42;
  enterRecursionRule(_localctx, 42, UVLCppParser::RuleConstraint, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(277);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<EquationConstraintContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(269);
      equation();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<LiteralConstraintContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(270);
      reference();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ParenthesisConstraintContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(271);
      match(UVLCppParser::OPEN_PAREN);
      setState(272);
      constraint(0);
      setState(273);
      match(UVLCppParser::CLOSE_PAREN);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<NotConstraintContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(275);
      match(UVLCppParser::NOT);
      setState(276);
      constraint(5);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(293);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(291);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<AndConstraintContext>(_tracker.createInstance<ConstraintContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleConstraint);
          setState(279);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(280);
          match(UVLCppParser::AND);
          setState(281);
          constraint(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<OrConstraintContext>(_tracker.createInstance<ConstraintContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleConstraint);
          setState(282);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(283);
          match(UVLCppParser::OR);
          setState(284);
          constraint(4);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ImplicationConstraintContext>(_tracker.createInstance<ConstraintContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleConstraint);
          setState(285);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(286);
          match(UVLCppParser::IMPLICATION);
          setState(287);
          constraint(3);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<EquivalenceConstraintContext>(_tracker.createInstance<ConstraintContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleConstraint);
          setState(288);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(289);
          match(UVLCppParser::EQUIVALENCE);
          setState(290);
          constraint(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(295);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- EquationContext ------------------------------------------------------------------

UVLCppParser::EquationContext::EquationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::EquationContext::getRuleIndex() const {
  return UVLCppParser::RuleEquation;
}

void UVLCppParser::EquationContext::copyFrom(EquationContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- EqualEquationContext ------------------------------------------------------------------

std::vector<UVLCppParser::ExpressionContext *> UVLCppParser::EqualEquationContext::expression() {
  return getRuleContexts<UVLCppParser::ExpressionContext>();
}

UVLCppParser::ExpressionContext* UVLCppParser::EqualEquationContext::expression(size_t i) {
  return getRuleContext<UVLCppParser::ExpressionContext>(i);
}

tree::TerminalNode* UVLCppParser::EqualEquationContext::EQUAL() {
  return getToken(UVLCppParser::EQUAL, 0);
}

UVLCppParser::EqualEquationContext::EqualEquationContext(EquationContext *ctx) { copyFrom(ctx); }

void UVLCppParser::EqualEquationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqualEquation(this);
}
void UVLCppParser::EqualEquationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqualEquation(this);
}
//----------------- LowerEquationContext ------------------------------------------------------------------

std::vector<UVLCppParser::ExpressionContext *> UVLCppParser::LowerEquationContext::expression() {
  return getRuleContexts<UVLCppParser::ExpressionContext>();
}

UVLCppParser::ExpressionContext* UVLCppParser::LowerEquationContext::expression(size_t i) {
  return getRuleContext<UVLCppParser::ExpressionContext>(i);
}

tree::TerminalNode* UVLCppParser::LowerEquationContext::LOWER() {
  return getToken(UVLCppParser::LOWER, 0);
}

UVLCppParser::LowerEquationContext::LowerEquationContext(EquationContext *ctx) { copyFrom(ctx); }

void UVLCppParser::LowerEquationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLowerEquation(this);
}
void UVLCppParser::LowerEquationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLowerEquation(this);
}
//----------------- LowerEqualsEquationContext ------------------------------------------------------------------

std::vector<UVLCppParser::ExpressionContext *> UVLCppParser::LowerEqualsEquationContext::expression() {
  return getRuleContexts<UVLCppParser::ExpressionContext>();
}

UVLCppParser::ExpressionContext* UVLCppParser::LowerEqualsEquationContext::expression(size_t i) {
  return getRuleContext<UVLCppParser::ExpressionContext>(i);
}

tree::TerminalNode* UVLCppParser::LowerEqualsEquationContext::LOWER_EQUALS() {
  return getToken(UVLCppParser::LOWER_EQUALS, 0);
}

UVLCppParser::LowerEqualsEquationContext::LowerEqualsEquationContext(EquationContext *ctx) { copyFrom(ctx); }

void UVLCppParser::LowerEqualsEquationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLowerEqualsEquation(this);
}
void UVLCppParser::LowerEqualsEquationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLowerEqualsEquation(this);
}
//----------------- GreaterEqualsEquationContext ------------------------------------------------------------------

std::vector<UVLCppParser::ExpressionContext *> UVLCppParser::GreaterEqualsEquationContext::expression() {
  return getRuleContexts<UVLCppParser::ExpressionContext>();
}

UVLCppParser::ExpressionContext* UVLCppParser::GreaterEqualsEquationContext::expression(size_t i) {
  return getRuleContext<UVLCppParser::ExpressionContext>(i);
}

tree::TerminalNode* UVLCppParser::GreaterEqualsEquationContext::GREATER_EQUALS() {
  return getToken(UVLCppParser::GREATER_EQUALS, 0);
}

UVLCppParser::GreaterEqualsEquationContext::GreaterEqualsEquationContext(EquationContext *ctx) { copyFrom(ctx); }

void UVLCppParser::GreaterEqualsEquationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGreaterEqualsEquation(this);
}
void UVLCppParser::GreaterEqualsEquationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGreaterEqualsEquation(this);
}
//----------------- GreaterEquationContext ------------------------------------------------------------------

std::vector<UVLCppParser::ExpressionContext *> UVLCppParser::GreaterEquationContext::expression() {
  return getRuleContexts<UVLCppParser::ExpressionContext>();
}

UVLCppParser::ExpressionContext* UVLCppParser::GreaterEquationContext::expression(size_t i) {
  return getRuleContext<UVLCppParser::ExpressionContext>(i);
}

tree::TerminalNode* UVLCppParser::GreaterEquationContext::GREATER() {
  return getToken(UVLCppParser::GREATER, 0);
}

UVLCppParser::GreaterEquationContext::GreaterEquationContext(EquationContext *ctx) { copyFrom(ctx); }

void UVLCppParser::GreaterEquationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGreaterEquation(this);
}
void UVLCppParser::GreaterEquationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGreaterEquation(this);
}
//----------------- NotEqualsEquationContext ------------------------------------------------------------------

std::vector<UVLCppParser::ExpressionContext *> UVLCppParser::NotEqualsEquationContext::expression() {
  return getRuleContexts<UVLCppParser::ExpressionContext>();
}

UVLCppParser::ExpressionContext* UVLCppParser::NotEqualsEquationContext::expression(size_t i) {
  return getRuleContext<UVLCppParser::ExpressionContext>(i);
}

tree::TerminalNode* UVLCppParser::NotEqualsEquationContext::NOT_EQUALS() {
  return getToken(UVLCppParser::NOT_EQUALS, 0);
}

UVLCppParser::NotEqualsEquationContext::NotEqualsEquationContext(EquationContext *ctx) { copyFrom(ctx); }

void UVLCppParser::NotEqualsEquationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNotEqualsEquation(this);
}
void UVLCppParser::NotEqualsEquationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNotEqualsEquation(this);
}
UVLCppParser::EquationContext* UVLCppParser::equation() {
  EquationContext *_localctx = _tracker.createInstance<EquationContext>(_ctx, getState());
  enterRule(_localctx, 44, UVLCppParser::RuleEquation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(320);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<UVLCppParser::EqualEquationContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(296);
      expression();
      setState(297);
      match(UVLCppParser::EQUAL);
      setState(298);
      expression();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<UVLCppParser::LowerEquationContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(300);
      expression();
      setState(301);
      match(UVLCppParser::LOWER);
      setState(302);
      expression();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<UVLCppParser::GreaterEquationContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(304);
      expression();
      setState(305);
      match(UVLCppParser::GREATER);
      setState(306);
      expression();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<UVLCppParser::LowerEqualsEquationContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(308);
      expression();
      setState(309);
      match(UVLCppParser::LOWER_EQUALS);
      setState(310);
      expression();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UVLCppParser::GreaterEqualsEquationContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(312);
      expression();
      setState(313);
      match(UVLCppParser::GREATER_EQUALS);
      setState(314);
      expression();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<UVLCppParser::NotEqualsEquationContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(316);
      expression();
      setState(317);
      match(UVLCppParser::NOT_EQUALS);
      setState(318);
      expression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

UVLCppParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::AdditiveExpressionContext* UVLCppParser::ExpressionContext::additiveExpression() {
  return getRuleContext<UVLCppParser::AdditiveExpressionContext>(0);
}


size_t UVLCppParser::ExpressionContext::getRuleIndex() const {
  return UVLCppParser::RuleExpression;
}

void UVLCppParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void UVLCppParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}

UVLCppParser::ExpressionContext* UVLCppParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 46, UVLCppParser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(322);
    additiveExpression(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveExpressionContext ------------------------------------------------------------------

UVLCppParser::AdditiveExpressionContext::AdditiveExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::AdditiveExpressionContext::getRuleIndex() const {
  return UVLCppParser::RuleAdditiveExpression;
}

void UVLCppParser::AdditiveExpressionContext::copyFrom(AdditiveExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MultiplicativeExprContext ------------------------------------------------------------------

UVLCppParser::MultiplicativeExpressionContext* UVLCppParser::MultiplicativeExprContext::multiplicativeExpression() {
  return getRuleContext<UVLCppParser::MultiplicativeExpressionContext>(0);
}

UVLCppParser::MultiplicativeExprContext::MultiplicativeExprContext(AdditiveExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::MultiplicativeExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultiplicativeExpr(this);
}
void UVLCppParser::MultiplicativeExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultiplicativeExpr(this);
}
//----------------- AddExpressionContext ------------------------------------------------------------------

UVLCppParser::AdditiveExpressionContext* UVLCppParser::AddExpressionContext::additiveExpression() {
  return getRuleContext<UVLCppParser::AdditiveExpressionContext>(0);
}

tree::TerminalNode* UVLCppParser::AddExpressionContext::ADD() {
  return getToken(UVLCppParser::ADD, 0);
}

UVLCppParser::MultiplicativeExpressionContext* UVLCppParser::AddExpressionContext::multiplicativeExpression() {
  return getRuleContext<UVLCppParser::MultiplicativeExpressionContext>(0);
}

UVLCppParser::AddExpressionContext::AddExpressionContext(AdditiveExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::AddExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAddExpression(this);
}
void UVLCppParser::AddExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAddExpression(this);
}
//----------------- SubExpressionContext ------------------------------------------------------------------

UVLCppParser::AdditiveExpressionContext* UVLCppParser::SubExpressionContext::additiveExpression() {
  return getRuleContext<UVLCppParser::AdditiveExpressionContext>(0);
}

tree::TerminalNode* UVLCppParser::SubExpressionContext::SUB() {
  return getToken(UVLCppParser::SUB, 0);
}

UVLCppParser::MultiplicativeExpressionContext* UVLCppParser::SubExpressionContext::multiplicativeExpression() {
  return getRuleContext<UVLCppParser::MultiplicativeExpressionContext>(0);
}

UVLCppParser::SubExpressionContext::SubExpressionContext(AdditiveExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::SubExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSubExpression(this);
}
void UVLCppParser::SubExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSubExpression(this);
}

UVLCppParser::AdditiveExpressionContext* UVLCppParser::additiveExpression() {
   return additiveExpression(0);
}

UVLCppParser::AdditiveExpressionContext* UVLCppParser::additiveExpression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  UVLCppParser::AdditiveExpressionContext *_localctx = _tracker.createInstance<AdditiveExpressionContext>(_ctx, parentState);
  UVLCppParser::AdditiveExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 48;
  enterRecursionRule(_localctx, 48, UVLCppParser::RuleAdditiveExpression, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<MultiplicativeExprContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(325);
    multiplicativeExpression(0);
    _ctx->stop = _input->LT(-1);
    setState(335);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(333);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<AddExpressionContext>(_tracker.createInstance<AdditiveExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleAdditiveExpression);
          setState(327);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(328);
          match(UVLCppParser::ADD);
          setState(329);
          multiplicativeExpression(0);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<SubExpressionContext>(_tracker.createInstance<AdditiveExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleAdditiveExpression);
          setState(330);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(331);
          match(UVLCppParser::SUB);
          setState(332);
          multiplicativeExpression(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(337);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- MultiplicativeExpressionContext ------------------------------------------------------------------

UVLCppParser::MultiplicativeExpressionContext::MultiplicativeExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::MultiplicativeExpressionContext::getRuleIndex() const {
  return UVLCppParser::RuleMultiplicativeExpression;
}

void UVLCppParser::MultiplicativeExpressionContext::copyFrom(MultiplicativeExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- PrimaryExpressionExpressionContext ------------------------------------------------------------------

UVLCppParser::PrimaryExpressionContext* UVLCppParser::PrimaryExpressionExpressionContext::primaryExpression() {
  return getRuleContext<UVLCppParser::PrimaryExpressionContext>(0);
}

UVLCppParser::PrimaryExpressionExpressionContext::PrimaryExpressionExpressionContext(MultiplicativeExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::PrimaryExpressionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimaryExpressionExpression(this);
}
void UVLCppParser::PrimaryExpressionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimaryExpressionExpression(this);
}
//----------------- DivExpressionContext ------------------------------------------------------------------

UVLCppParser::MultiplicativeExpressionContext* UVLCppParser::DivExpressionContext::multiplicativeExpression() {
  return getRuleContext<UVLCppParser::MultiplicativeExpressionContext>(0);
}

tree::TerminalNode* UVLCppParser::DivExpressionContext::DIV() {
  return getToken(UVLCppParser::DIV, 0);
}

UVLCppParser::PrimaryExpressionContext* UVLCppParser::DivExpressionContext::primaryExpression() {
  return getRuleContext<UVLCppParser::PrimaryExpressionContext>(0);
}

UVLCppParser::DivExpressionContext::DivExpressionContext(MultiplicativeExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::DivExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDivExpression(this);
}
void UVLCppParser::DivExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDivExpression(this);
}
//----------------- MulExpressionContext ------------------------------------------------------------------

UVLCppParser::MultiplicativeExpressionContext* UVLCppParser::MulExpressionContext::multiplicativeExpression() {
  return getRuleContext<UVLCppParser::MultiplicativeExpressionContext>(0);
}

tree::TerminalNode* UVLCppParser::MulExpressionContext::MUL() {
  return getToken(UVLCppParser::MUL, 0);
}

UVLCppParser::PrimaryExpressionContext* UVLCppParser::MulExpressionContext::primaryExpression() {
  return getRuleContext<UVLCppParser::PrimaryExpressionContext>(0);
}

UVLCppParser::MulExpressionContext::MulExpressionContext(MultiplicativeExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::MulExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMulExpression(this);
}
void UVLCppParser::MulExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMulExpression(this);
}

UVLCppParser::MultiplicativeExpressionContext* UVLCppParser::multiplicativeExpression() {
   return multiplicativeExpression(0);
}

UVLCppParser::MultiplicativeExpressionContext* UVLCppParser::multiplicativeExpression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  UVLCppParser::MultiplicativeExpressionContext *_localctx = _tracker.createInstance<MultiplicativeExpressionContext>(_ctx, parentState);
  UVLCppParser::MultiplicativeExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 50;
  enterRecursionRule(_localctx, 50, UVLCppParser::RuleMultiplicativeExpression, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<PrimaryExpressionExpressionContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(339);
    primaryExpression();
    _ctx->stop = _input->LT(-1);
    setState(349);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(347);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MulExpressionContext>(_tracker.createInstance<MultiplicativeExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMultiplicativeExpression);
          setState(341);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(342);
          match(UVLCppParser::MUL);
          setState(343);
          primaryExpression();
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<DivExpressionContext>(_tracker.createInstance<MultiplicativeExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMultiplicativeExpression);
          setState(344);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(345);
          match(UVLCppParser::DIV);
          setState(346);
          primaryExpression();
          break;
        }

        default:
          break;
        } 
      }
      setState(351);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PrimaryExpressionContext ------------------------------------------------------------------

UVLCppParser::PrimaryExpressionContext::PrimaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::PrimaryExpressionContext::getRuleIndex() const {
  return UVLCppParser::RulePrimaryExpression;
}

void UVLCppParser::PrimaryExpressionContext::copyFrom(PrimaryExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- BracketExpressionContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::BracketExpressionContext::OPEN_PAREN() {
  return getToken(UVLCppParser::OPEN_PAREN, 0);
}

UVLCppParser::ExpressionContext* UVLCppParser::BracketExpressionContext::expression() {
  return getRuleContext<UVLCppParser::ExpressionContext>(0);
}

tree::TerminalNode* UVLCppParser::BracketExpressionContext::CLOSE_PAREN() {
  return getToken(UVLCppParser::CLOSE_PAREN, 0);
}

UVLCppParser::BracketExpressionContext::BracketExpressionContext(PrimaryExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::BracketExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBracketExpression(this);
}
void UVLCppParser::BracketExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBracketExpression(this);
}
//----------------- AggregateFunctionExpressionContext ------------------------------------------------------------------

UVLCppParser::AggregateFunctionContext* UVLCppParser::AggregateFunctionExpressionContext::aggregateFunction() {
  return getRuleContext<UVLCppParser::AggregateFunctionContext>(0);
}

UVLCppParser::AggregateFunctionExpressionContext::AggregateFunctionExpressionContext(PrimaryExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::AggregateFunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAggregateFunctionExpression(this);
}
void UVLCppParser::AggregateFunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAggregateFunctionExpression(this);
}
//----------------- FloatLiteralExpressionContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::FloatLiteralExpressionContext::FLOAT() {
  return getToken(UVLCppParser::FLOAT, 0);
}

UVLCppParser::FloatLiteralExpressionContext::FloatLiteralExpressionContext(PrimaryExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::FloatLiteralExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFloatLiteralExpression(this);
}
void UVLCppParser::FloatLiteralExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFloatLiteralExpression(this);
}
//----------------- StringLiteralExpressionContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::StringLiteralExpressionContext::STRING() {
  return getToken(UVLCppParser::STRING, 0);
}

UVLCppParser::StringLiteralExpressionContext::StringLiteralExpressionContext(PrimaryExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::StringLiteralExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStringLiteralExpression(this);
}
void UVLCppParser::StringLiteralExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStringLiteralExpression(this);
}
//----------------- IntegerLiteralExpressionContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::IntegerLiteralExpressionContext::INTEGER() {
  return getToken(UVLCppParser::INTEGER, 0);
}

UVLCppParser::IntegerLiteralExpressionContext::IntegerLiteralExpressionContext(PrimaryExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::IntegerLiteralExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIntegerLiteralExpression(this);
}
void UVLCppParser::IntegerLiteralExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIntegerLiteralExpression(this);
}
//----------------- LiteralExpressionContext ------------------------------------------------------------------

UVLCppParser::ReferenceContext* UVLCppParser::LiteralExpressionContext::reference() {
  return getRuleContext<UVLCppParser::ReferenceContext>(0);
}

UVLCppParser::LiteralExpressionContext::LiteralExpressionContext(PrimaryExpressionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::LiteralExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteralExpression(this);
}
void UVLCppParser::LiteralExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteralExpression(this);
}
UVLCppParser::PrimaryExpressionContext* UVLCppParser::primaryExpression() {
  PrimaryExpressionContext *_localctx = _tracker.createInstance<PrimaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 52, UVLCppParser::RulePrimaryExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(361);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case UVLCppParser::FLOAT: {
        _localctx = _tracker.createInstance<UVLCppParser::FloatLiteralExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(352);
        match(UVLCppParser::FLOAT);
        break;
      }

      case UVLCppParser::INTEGER: {
        _localctx = _tracker.createInstance<UVLCppParser::IntegerLiteralExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(353);
        match(UVLCppParser::INTEGER);
        break;
      }

      case UVLCppParser::STRING: {
        _localctx = _tracker.createInstance<UVLCppParser::StringLiteralExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(354);
        match(UVLCppParser::STRING);
        break;
      }

      case UVLCppParser::LEN_KEY:
      case UVLCppParser::SUM_KEY:
      case UVLCppParser::AVG_KEY:
      case UVLCppParser::FLOOR_KEY:
      case UVLCppParser::CEIL_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::AggregateFunctionExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(355);
        aggregateFunction();
        break;
      }

      case UVLCppParser::ID_NOT_STRICT:
      case UVLCppParser::ID_STRICT: {
        _localctx = _tracker.createInstance<UVLCppParser::LiteralExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(356);
        reference();
        break;
      }

      case UVLCppParser::OPEN_PAREN: {
        _localctx = _tracker.createInstance<UVLCppParser::BracketExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(357);
        match(UVLCppParser::OPEN_PAREN);
        setState(358);
        expression();
        setState(359);
        match(UVLCppParser::CLOSE_PAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AggregateFunctionContext ------------------------------------------------------------------

UVLCppParser::AggregateFunctionContext::AggregateFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::AggregateFunctionContext::getRuleIndex() const {
  return UVLCppParser::RuleAggregateFunction;
}

void UVLCppParser::AggregateFunctionContext::copyFrom(AggregateFunctionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- NumericAggregateFunctionExpressionContext ------------------------------------------------------------------

UVLCppParser::NumericAggregateFunctionContext* UVLCppParser::NumericAggregateFunctionExpressionContext::numericAggregateFunction() {
  return getRuleContext<UVLCppParser::NumericAggregateFunctionContext>(0);
}

UVLCppParser::NumericAggregateFunctionExpressionContext::NumericAggregateFunctionExpressionContext(AggregateFunctionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::NumericAggregateFunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumericAggregateFunctionExpression(this);
}
void UVLCppParser::NumericAggregateFunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumericAggregateFunctionExpression(this);
}
//----------------- StringAggregateFunctionExpressionContext ------------------------------------------------------------------

UVLCppParser::StringAggregateFunctionContext* UVLCppParser::StringAggregateFunctionExpressionContext::stringAggregateFunction() {
  return getRuleContext<UVLCppParser::StringAggregateFunctionContext>(0);
}

UVLCppParser::StringAggregateFunctionExpressionContext::StringAggregateFunctionExpressionContext(AggregateFunctionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::StringAggregateFunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStringAggregateFunctionExpression(this);
}
void UVLCppParser::StringAggregateFunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStringAggregateFunctionExpression(this);
}
//----------------- AvgAggregateFunctionExpressionContext ------------------------------------------------------------------

UVLCppParser::AvgAggregateFunctionContext* UVLCppParser::AvgAggregateFunctionExpressionContext::avgAggregateFunction() {
  return getRuleContext<UVLCppParser::AvgAggregateFunctionContext>(0);
}

UVLCppParser::AvgAggregateFunctionExpressionContext::AvgAggregateFunctionExpressionContext(AggregateFunctionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::AvgAggregateFunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAvgAggregateFunctionExpression(this);
}
void UVLCppParser::AvgAggregateFunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAvgAggregateFunctionExpression(this);
}
//----------------- SumAggregateFunctionExpressionContext ------------------------------------------------------------------

UVLCppParser::SumAggregateFunctionContext* UVLCppParser::SumAggregateFunctionExpressionContext::sumAggregateFunction() {
  return getRuleContext<UVLCppParser::SumAggregateFunctionContext>(0);
}

UVLCppParser::SumAggregateFunctionExpressionContext::SumAggregateFunctionExpressionContext(AggregateFunctionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::SumAggregateFunctionExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSumAggregateFunctionExpression(this);
}
void UVLCppParser::SumAggregateFunctionExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSumAggregateFunctionExpression(this);
}
UVLCppParser::AggregateFunctionContext* UVLCppParser::aggregateFunction() {
  AggregateFunctionContext *_localctx = _tracker.createInstance<AggregateFunctionContext>(_ctx, getState());
  enterRule(_localctx, 54, UVLCppParser::RuleAggregateFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(367);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case UVLCppParser::SUM_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::SumAggregateFunctionExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(363);
        sumAggregateFunction();
        break;
      }

      case UVLCppParser::AVG_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::AvgAggregateFunctionExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(364);
        avgAggregateFunction();
        break;
      }

      case UVLCppParser::LEN_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::StringAggregateFunctionExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(365);
        stringAggregateFunction();
        break;
      }

      case UVLCppParser::FLOOR_KEY:
      case UVLCppParser::CEIL_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::NumericAggregateFunctionExpressionContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(366);
        numericAggregateFunction();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SumAggregateFunctionContext ------------------------------------------------------------------

UVLCppParser::SumAggregateFunctionContext::SumAggregateFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::SumAggregateFunctionContext::SUM_KEY() {
  return getToken(UVLCppParser::SUM_KEY, 0);
}

tree::TerminalNode* UVLCppParser::SumAggregateFunctionContext::OPEN_PAREN() {
  return getToken(UVLCppParser::OPEN_PAREN, 0);
}

std::vector<UVLCppParser::ReferenceContext *> UVLCppParser::SumAggregateFunctionContext::reference() {
  return getRuleContexts<UVLCppParser::ReferenceContext>();
}

UVLCppParser::ReferenceContext* UVLCppParser::SumAggregateFunctionContext::reference(size_t i) {
  return getRuleContext<UVLCppParser::ReferenceContext>(i);
}

tree::TerminalNode* UVLCppParser::SumAggregateFunctionContext::CLOSE_PAREN() {
  return getToken(UVLCppParser::CLOSE_PAREN, 0);
}

tree::TerminalNode* UVLCppParser::SumAggregateFunctionContext::COMMA() {
  return getToken(UVLCppParser::COMMA, 0);
}


size_t UVLCppParser::SumAggregateFunctionContext::getRuleIndex() const {
  return UVLCppParser::RuleSumAggregateFunction;
}

void UVLCppParser::SumAggregateFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSumAggregateFunction(this);
}

void UVLCppParser::SumAggregateFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSumAggregateFunction(this);
}

UVLCppParser::SumAggregateFunctionContext* UVLCppParser::sumAggregateFunction() {
  SumAggregateFunctionContext *_localctx = _tracker.createInstance<SumAggregateFunctionContext>(_ctx, getState());
  enterRule(_localctx, 56, UVLCppParser::RuleSumAggregateFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(369);
    match(UVLCppParser::SUM_KEY);
    setState(370);
    match(UVLCppParser::OPEN_PAREN);
    setState(374);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
    case 1: {
      setState(371);
      reference();
      setState(372);
      match(UVLCppParser::COMMA);
      break;
    }

    default:
      break;
    }
    setState(376);
    reference();
    setState(377);
    match(UVLCppParser::CLOSE_PAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AvgAggregateFunctionContext ------------------------------------------------------------------

UVLCppParser::AvgAggregateFunctionContext::AvgAggregateFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::AvgAggregateFunctionContext::AVG_KEY() {
  return getToken(UVLCppParser::AVG_KEY, 0);
}

tree::TerminalNode* UVLCppParser::AvgAggregateFunctionContext::OPEN_PAREN() {
  return getToken(UVLCppParser::OPEN_PAREN, 0);
}

std::vector<UVLCppParser::ReferenceContext *> UVLCppParser::AvgAggregateFunctionContext::reference() {
  return getRuleContexts<UVLCppParser::ReferenceContext>();
}

UVLCppParser::ReferenceContext* UVLCppParser::AvgAggregateFunctionContext::reference(size_t i) {
  return getRuleContext<UVLCppParser::ReferenceContext>(i);
}

tree::TerminalNode* UVLCppParser::AvgAggregateFunctionContext::CLOSE_PAREN() {
  return getToken(UVLCppParser::CLOSE_PAREN, 0);
}

tree::TerminalNode* UVLCppParser::AvgAggregateFunctionContext::COMMA() {
  return getToken(UVLCppParser::COMMA, 0);
}


size_t UVLCppParser::AvgAggregateFunctionContext::getRuleIndex() const {
  return UVLCppParser::RuleAvgAggregateFunction;
}

void UVLCppParser::AvgAggregateFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAvgAggregateFunction(this);
}

void UVLCppParser::AvgAggregateFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAvgAggregateFunction(this);
}

UVLCppParser::AvgAggregateFunctionContext* UVLCppParser::avgAggregateFunction() {
  AvgAggregateFunctionContext *_localctx = _tracker.createInstance<AvgAggregateFunctionContext>(_ctx, getState());
  enterRule(_localctx, 58, UVLCppParser::RuleAvgAggregateFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(379);
    match(UVLCppParser::AVG_KEY);
    setState(380);
    match(UVLCppParser::OPEN_PAREN);
    setState(384);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
    case 1: {
      setState(381);
      reference();
      setState(382);
      match(UVLCppParser::COMMA);
      break;
    }

    default:
      break;
    }
    setState(386);
    reference();
    setState(387);
    match(UVLCppParser::CLOSE_PAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringAggregateFunctionContext ------------------------------------------------------------------

UVLCppParser::StringAggregateFunctionContext::StringAggregateFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::StringAggregateFunctionContext::getRuleIndex() const {
  return UVLCppParser::RuleStringAggregateFunction;
}

void UVLCppParser::StringAggregateFunctionContext::copyFrom(StringAggregateFunctionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LengthAggregateFunctionContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::LengthAggregateFunctionContext::LEN_KEY() {
  return getToken(UVLCppParser::LEN_KEY, 0);
}

tree::TerminalNode* UVLCppParser::LengthAggregateFunctionContext::OPEN_PAREN() {
  return getToken(UVLCppParser::OPEN_PAREN, 0);
}

UVLCppParser::ReferenceContext* UVLCppParser::LengthAggregateFunctionContext::reference() {
  return getRuleContext<UVLCppParser::ReferenceContext>(0);
}

tree::TerminalNode* UVLCppParser::LengthAggregateFunctionContext::CLOSE_PAREN() {
  return getToken(UVLCppParser::CLOSE_PAREN, 0);
}

UVLCppParser::LengthAggregateFunctionContext::LengthAggregateFunctionContext(StringAggregateFunctionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::LengthAggregateFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLengthAggregateFunction(this);
}
void UVLCppParser::LengthAggregateFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLengthAggregateFunction(this);
}
UVLCppParser::StringAggregateFunctionContext* UVLCppParser::stringAggregateFunction() {
  StringAggregateFunctionContext *_localctx = _tracker.createInstance<StringAggregateFunctionContext>(_ctx, getState());
  enterRule(_localctx, 60, UVLCppParser::RuleStringAggregateFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    _localctx = _tracker.createInstance<UVLCppParser::LengthAggregateFunctionContext>(_localctx);
    enterOuterAlt(_localctx, 1);
    setState(389);
    match(UVLCppParser::LEN_KEY);
    setState(390);
    match(UVLCppParser::OPEN_PAREN);
    setState(391);
    reference();
    setState(392);
    match(UVLCppParser::CLOSE_PAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumericAggregateFunctionContext ------------------------------------------------------------------

UVLCppParser::NumericAggregateFunctionContext::NumericAggregateFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t UVLCppParser::NumericAggregateFunctionContext::getRuleIndex() const {
  return UVLCppParser::RuleNumericAggregateFunction;
}

void UVLCppParser::NumericAggregateFunctionContext::copyFrom(NumericAggregateFunctionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CeilAggregateFunctionContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::CeilAggregateFunctionContext::CEIL_KEY() {
  return getToken(UVLCppParser::CEIL_KEY, 0);
}

tree::TerminalNode* UVLCppParser::CeilAggregateFunctionContext::OPEN_PAREN() {
  return getToken(UVLCppParser::OPEN_PAREN, 0);
}

UVLCppParser::ReferenceContext* UVLCppParser::CeilAggregateFunctionContext::reference() {
  return getRuleContext<UVLCppParser::ReferenceContext>(0);
}

tree::TerminalNode* UVLCppParser::CeilAggregateFunctionContext::CLOSE_PAREN() {
  return getToken(UVLCppParser::CLOSE_PAREN, 0);
}

UVLCppParser::CeilAggregateFunctionContext::CeilAggregateFunctionContext(NumericAggregateFunctionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::CeilAggregateFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCeilAggregateFunction(this);
}
void UVLCppParser::CeilAggregateFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCeilAggregateFunction(this);
}
//----------------- FloorAggregateFunctionContext ------------------------------------------------------------------

tree::TerminalNode* UVLCppParser::FloorAggregateFunctionContext::FLOOR_KEY() {
  return getToken(UVLCppParser::FLOOR_KEY, 0);
}

tree::TerminalNode* UVLCppParser::FloorAggregateFunctionContext::OPEN_PAREN() {
  return getToken(UVLCppParser::OPEN_PAREN, 0);
}

UVLCppParser::ReferenceContext* UVLCppParser::FloorAggregateFunctionContext::reference() {
  return getRuleContext<UVLCppParser::ReferenceContext>(0);
}

tree::TerminalNode* UVLCppParser::FloorAggregateFunctionContext::CLOSE_PAREN() {
  return getToken(UVLCppParser::CLOSE_PAREN, 0);
}

UVLCppParser::FloorAggregateFunctionContext::FloorAggregateFunctionContext(NumericAggregateFunctionContext *ctx) { copyFrom(ctx); }

void UVLCppParser::FloorAggregateFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFloorAggregateFunction(this);
}
void UVLCppParser::FloorAggregateFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFloorAggregateFunction(this);
}
UVLCppParser::NumericAggregateFunctionContext* UVLCppParser::numericAggregateFunction() {
  NumericAggregateFunctionContext *_localctx = _tracker.createInstance<NumericAggregateFunctionContext>(_ctx, getState());
  enterRule(_localctx, 62, UVLCppParser::RuleNumericAggregateFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(404);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case UVLCppParser::FLOOR_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::FloorAggregateFunctionContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(394);
        match(UVLCppParser::FLOOR_KEY);
        setState(395);
        match(UVLCppParser::OPEN_PAREN);
        setState(396);
        reference();
        setState(397);
        match(UVLCppParser::CLOSE_PAREN);
        break;
      }

      case UVLCppParser::CEIL_KEY: {
        _localctx = _tracker.createInstance<UVLCppParser::CeilAggregateFunctionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(399);
        match(UVLCppParser::CEIL_KEY);
        setState(400);
        match(UVLCppParser::OPEN_PAREN);
        setState(401);
        reference();
        setState(402);
        match(UVLCppParser::CLOSE_PAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReferenceContext ------------------------------------------------------------------

UVLCppParser::ReferenceContext::ReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<UVLCppParser::IdContext *> UVLCppParser::ReferenceContext::id() {
  return getRuleContexts<UVLCppParser::IdContext>();
}

UVLCppParser::IdContext* UVLCppParser::ReferenceContext::id(size_t i) {
  return getRuleContext<UVLCppParser::IdContext>(i);
}

std::vector<tree::TerminalNode *> UVLCppParser::ReferenceContext::DOT() {
  return getTokens(UVLCppParser::DOT);
}

tree::TerminalNode* UVLCppParser::ReferenceContext::DOT(size_t i) {
  return getToken(UVLCppParser::DOT, i);
}


size_t UVLCppParser::ReferenceContext::getRuleIndex() const {
  return UVLCppParser::RuleReference;
}

void UVLCppParser::ReferenceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReference(this);
}

void UVLCppParser::ReferenceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReference(this);
}

UVLCppParser::ReferenceContext* UVLCppParser::reference() {
  ReferenceContext *_localctx = _tracker.createInstance<ReferenceContext>(_ctx, getState());
  enterRule(_localctx, 64, UVLCppParser::RuleReference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(411);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(406);
        id();
        setState(407);
        match(UVLCppParser::DOT); 
      }
      setState(413);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx);
    }
    setState(414);
    id();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdContext ------------------------------------------------------------------

UVLCppParser::IdContext::IdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::IdContext::ID_STRICT() {
  return getToken(UVLCppParser::ID_STRICT, 0);
}

tree::TerminalNode* UVLCppParser::IdContext::ID_NOT_STRICT() {
  return getToken(UVLCppParser::ID_NOT_STRICT, 0);
}


size_t UVLCppParser::IdContext::getRuleIndex() const {
  return UVLCppParser::RuleId;
}

void UVLCppParser::IdContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterId(this);
}

void UVLCppParser::IdContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitId(this);
}

UVLCppParser::IdContext* UVLCppParser::id() {
  IdContext *_localctx = _tracker.createInstance<IdContext>(_ctx, getState());
  enterRule(_localctx, 66, UVLCppParser::RuleId);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(416);
    _la = _input->LA(1);
    if (!(_la == UVLCppParser::ID_NOT_STRICT

    || _la == UVLCppParser::ID_STRICT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FeatureTypeContext ------------------------------------------------------------------

UVLCppParser::FeatureTypeContext::FeatureTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::FeatureTypeContext::STRING_KEY() {
  return getToken(UVLCppParser::STRING_KEY, 0);
}

tree::TerminalNode* UVLCppParser::FeatureTypeContext::INTEGER_KEY() {
  return getToken(UVLCppParser::INTEGER_KEY, 0);
}

tree::TerminalNode* UVLCppParser::FeatureTypeContext::BOOLEAN_KEY() {
  return getToken(UVLCppParser::BOOLEAN_KEY, 0);
}

tree::TerminalNode* UVLCppParser::FeatureTypeContext::REAL_KEY() {
  return getToken(UVLCppParser::REAL_KEY, 0);
}


size_t UVLCppParser::FeatureTypeContext::getRuleIndex() const {
  return UVLCppParser::RuleFeatureType;
}

void UVLCppParser::FeatureTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFeatureType(this);
}

void UVLCppParser::FeatureTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFeatureType(this);
}

UVLCppParser::FeatureTypeContext* UVLCppParser::featureType() {
  FeatureTypeContext *_localctx = _tracker.createInstance<FeatureTypeContext>(_ctx, getState());
  enterRule(_localctx, 68, UVLCppParser::RuleFeatureType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(418);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 15728640) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LanguageLevelContext ------------------------------------------------------------------

UVLCppParser::LanguageLevelContext::LanguageLevelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

UVLCppParser::MajorLevelContext* UVLCppParser::LanguageLevelContext::majorLevel() {
  return getRuleContext<UVLCppParser::MajorLevelContext>(0);
}

tree::TerminalNode* UVLCppParser::LanguageLevelContext::DOT() {
  return getToken(UVLCppParser::DOT, 0);
}

UVLCppParser::MinorLevelContext* UVLCppParser::LanguageLevelContext::minorLevel() {
  return getRuleContext<UVLCppParser::MinorLevelContext>(0);
}

tree::TerminalNode* UVLCppParser::LanguageLevelContext::MUL() {
  return getToken(UVLCppParser::MUL, 0);
}


size_t UVLCppParser::LanguageLevelContext::getRuleIndex() const {
  return UVLCppParser::RuleLanguageLevel;
}

void UVLCppParser::LanguageLevelContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLanguageLevel(this);
}

void UVLCppParser::LanguageLevelContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLanguageLevel(this);
}

UVLCppParser::LanguageLevelContext* UVLCppParser::languageLevel() {
  LanguageLevelContext *_localctx = _tracker.createInstance<LanguageLevelContext>(_ctx, getState());
  enterRule(_localctx, 70, UVLCppParser::RuleLanguageLevel);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(420);
    majorLevel();
    setState(426);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == UVLCppParser::DOT) {
      setState(421);
      match(UVLCppParser::DOT);
      setState(424);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case UVLCppParser::GROUP_CARDINALITY_KEY:
        case UVLCppParser::FEATURE_CARDINALITY_KEY:
        case UVLCppParser::AGGREGATE_KEY:
        case UVLCppParser::STRING_CONSTRAINTS_KEY: {
          setState(422);
          minorLevel();
          break;
        }

        case UVLCppParser::MUL: {
          setState(423);
          match(UVLCppParser::MUL);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MajorLevelContext ------------------------------------------------------------------

UVLCppParser::MajorLevelContext::MajorLevelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::MajorLevelContext::BOOLEAN_KEY() {
  return getToken(UVLCppParser::BOOLEAN_KEY, 0);
}

tree::TerminalNode* UVLCppParser::MajorLevelContext::ARITHMETIC_KEY() {
  return getToken(UVLCppParser::ARITHMETIC_KEY, 0);
}

tree::TerminalNode* UVLCppParser::MajorLevelContext::TYPE_KEY() {
  return getToken(UVLCppParser::TYPE_KEY, 0);
}


size_t UVLCppParser::MajorLevelContext::getRuleIndex() const {
  return UVLCppParser::RuleMajorLevel;
}

void UVLCppParser::MajorLevelContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMajorLevel(this);
}

void UVLCppParser::MajorLevelContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMajorLevel(this);
}

UVLCppParser::MajorLevelContext* UVLCppParser::majorLevel() {
  MajorLevelContext *_localctx = _tracker.createInstance<MajorLevelContext>(_ctx, getState());
  enterRule(_localctx, 72, UVLCppParser::RuleMajorLevel);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(428);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1612709888) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MinorLevelContext ------------------------------------------------------------------

UVLCppParser::MinorLevelContext::MinorLevelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* UVLCppParser::MinorLevelContext::GROUP_CARDINALITY_KEY() {
  return getToken(UVLCppParser::GROUP_CARDINALITY_KEY, 0);
}

tree::TerminalNode* UVLCppParser::MinorLevelContext::FEATURE_CARDINALITY_KEY() {
  return getToken(UVLCppParser::FEATURE_CARDINALITY_KEY, 0);
}

tree::TerminalNode* UVLCppParser::MinorLevelContext::AGGREGATE_KEY() {
  return getToken(UVLCppParser::AGGREGATE_KEY, 0);
}

tree::TerminalNode* UVLCppParser::MinorLevelContext::STRING_CONSTRAINTS_KEY() {
  return getToken(UVLCppParser::STRING_CONSTRAINTS_KEY, 0);
}


size_t UVLCppParser::MinorLevelContext::getRuleIndex() const {
  return UVLCppParser::RuleMinorLevel;
}

void UVLCppParser::MinorLevelContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMinorLevel(this);
}

void UVLCppParser::MinorLevelContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<UVLCppParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMinorLevel(this);
}

UVLCppParser::MinorLevelContext* UVLCppParser::minorLevel() {
  MinorLevelContext *_localctx = _tracker.createInstance<MinorLevelContext>(_ctx, getState());
  enterRule(_localctx, 74, UVLCppParser::RuleMinorLevel);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(430);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 32212254720) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool UVLCppParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 21: return constraintSempred(antlrcpp::downCast<ConstraintContext *>(context), predicateIndex);
    case 24: return additiveExpressionSempred(antlrcpp::downCast<AdditiveExpressionContext *>(context), predicateIndex);
    case 25: return multiplicativeExpressionSempred(antlrcpp::downCast<MultiplicativeExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool UVLCppParser::constraintSempred(ConstraintContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 4);
    case 1: return precpred(_ctx, 3);
    case 2: return precpred(_ctx, 2);
    case 3: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool UVLCppParser::additiveExpressionSempred(AdditiveExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 4: return precpred(_ctx, 3);
    case 5: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

bool UVLCppParser::multiplicativeExpressionSempred(MultiplicativeExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 6: return precpred(_ctx, 3);
    case 7: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

void UVLCppParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  uvlcppparserParserInitialize();
#else
  ::antlr4::internal::call_once(uvlcppparserParserOnceFlag, uvlcppparserParserInitialize);
#endif
}
