// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Token.h
    @brief Defines token types, enumerations, and helper structures for the symbolic compiler (sc) subsystem. */
#pragma once
#include <td/Types.h>
#include <td/Range.h>
#include <td/String.h>
#include <td/StrKey.h>
#include <td/ColorID.h>
#include <td/LinePattern.h>
#include <td/DotPattern.h>

namespace sc
{

const size_t cMaxFunctionOrReservedKeyLength = td::StrKey::Length;

//NOTE: If you edit Token, you need also to edit TokenNames below
//NOTE: Also you need to adjust mapping from sc:Token to SLIB_NAMESPACE::Token (Token.h in symbDblSolver/inc/Token.h)
//      and in RPNParser.cpp: constexpr const Token::Type sTT[] -> line 1275
/// @brief Enumeration of all token types recognized by the symbolic compiler.
enum class Token : td::BYTE {Label=0, ///< Named label identifier
    Variable, ///< User-defined variable
    Parameter, ///< User-defined parameter
    Const, ///< Named constant (converted from expression)
    EmbParam, ///< Embedded parameter
    Function, ///< User-defined function identifier
    Group, ///< Group construct
    AttribName, ///< Attribute name token
    AttribEnumValue, ///< Attribute enumeration value
    Structure, ///< Structure identifier
    Reserved, ///< Reserved keyword (max length: MaxFunctionOrReservedKeyLength)
    ConstString, ///< String literal constant
    ConstInt, ///< Integer literal constant
    SpecialConst, ///< Special named constant (e.g. pi, e)
    ConstFloat, ///< Floating-point literal constant
    ConstImag, ///< Imaginary number literal constant
    ConstNumericComplex, ///< Complex numeric constant
    ConstColor, ///< Color constant
    ConstLinePattern, ///< Line pattern constant
    ConstDotPattern, ///< Dot pattern constant
    ConstAny, ///< Generic constant placeholder
    ConstTrue, ///< Boolean true constant
    ConstFalse, ///< Boolean false constant
    ConstLogic, ///< Logical constant
    OpenParenthesis, ///< Opening parenthesis '('
    CloseParenthesis, ///< Closing parenthesis ')'
    OpenBracket, ///< Opening square bracket '['
    CloseBracket, ///< Closing square bracket ']'
    OpenCurlyBrace, ///< Opening curly brace '{'
    CloseCurlyBrace, ///< Closing curly brace '}'
    Plus, ///< Addition operator '+'
    Minus, ///< Subtraction operator '-'
    Multiply, ///< Multiplication operator '*'
    Divide, ///< Division operator '/'
    DivideInt, ///< Integer division operator '%'
    DivideRight, ///< Right division operator '\'
    Power, ///< Power/exponentiation operator '^'
    MultiplyEqual, ///< Multiply-assign operator '*='
    DivideEqual, ///< Divide-assign operator '/='
    PlusEqual, ///< Add-assign operator '+='
    MinusEqual, ///< Subtract-assign operator '-='
    PowerEqual, ///< Power-assign operator '^='
    StreamLeft, ///< Left stream operator '<<'
    StreamRight, ///< Right stream operator '>>'
    PointerToLeft, ///< Pointer-to-left operator '<-'
    PointerToRight, ///< Pointer-to-right operator '->'
    Assignment, ///< Assignment operator '='
    Prime, ///< Prime operator '\''
    Second, ///< Double-prime operator '\'\''
    BitwiseInvert, ///< Bitwise invert operator '~'
    BitwiseOr, ///< Bitwise OR operator '|'
    BitwiseAnd, ///< Bitwise AND operator '&'
    BitwiseXOR, ///< Bitwise XOR operator '#'
    LogicalNot, ///< Logical NOT operator '!'
    LogicalOr, ///< Logical OR operator '||'
    LogicalAnd, ///< Logical AND operator '&&'
    LogicalXOR, ///< Logical XOR operator '##'
    LessThan, ///< Less-than comparison '<'
    LessThanOrEqual, ///< Less-than-or-equal comparison '<='
    GreatherThan, ///< Greater-than comparison '>'
    GreatherThanOrEqual, ///< Greater-than-or-equal comparison '>='
    IsEqual, ///< Equality comparison '=='
    IsDifferent, ///< Inequality comparison '!='
    Comma, ///< Comma separator ','
    Period, ///< Period/dot '.'
    Colon, ///< Colon ':'
    Semicolon, ///< Semicolon ';'
    Distribution, ///< Statistical distribution token
    Stat, ///< Statistical expression token
    DataSet, ///< Data set token
    Sampler, ///< Sampler token
    Comment, ///< Single-line comment '//'
    CCommentBegin, ///< C-style comment begin '/*'
    CCommentEnd, ///< C-style comment end '*/'
    EndOfLine, ///< End-of-line token
    Error ///< Error token (invalid/unrecognized input)
};

constexpr const char* const TokenNames[] = { "Label", "Var", "Param", "Const", "EmbParam",
    "Fn", "Group", "AttName", "AttEnValue", "Structure", "Reserved",
    "CStr", "CInt", "CSpecial", "CReal", "CImag", "CCmplx", "CClr", "CLinePattern", "CDotPattern", "CConstAny", "true", "false", "ConstLogic", //constants
    "(", ")", "[", "]", "{", "}",  //brackets
    "+", "-", "*", "/", "%", "\\", "^",
    "*=", "/=", "+=", "-=", "^=", "<<", ">>", "<-", "->",
    "=",     // =
    "\'", "\'\'",  // '   ''
    "~","|", "&", "#",   //operations on bits
    "!", "||", "&&", "##",      //logical operations
    "<", "<=", ">", ">=", "==", "!=",     //comparison operators
    ",", ".", ":", ";",  //end of operators
    "Dist", "Stat", "DataSet", "Sampler",
    "//", "/*", "*/", "NL", "ERR"};

static_assert( static_cast<size_t>(Token::Error)+1 == std::size(TokenNames),
    "TokenNames must match Token enum"
);

constexpr const char* const RealFunctionNames[] = {"abs","acos","asin","atg","cos","exp","sqrt","ln","log","sin","tg", "sqr", "atg2", "min", "max", "smin", "smax", "avg", "var", "dev", "diff", "sign",
    "sinh", "cosh", "tgh", "asnh", "acsh", "atgh",
    "int", "lim", "rnd", "disn", "disc", "round", "sample"};

/// @brief Enumeration of built-in real-valued mathematical functions.
enum class RealFunction : td::BYTE {Abs=0, ///< Absolute value
    ACos, ///< Arc cosine
    ASin, ///< Arc sine
    ATg, ///< Arc tangent
    Cos, ///< Cosine
    Exp, ///< Exponential
    Sqrt, ///< Square root
    Lm, ///< Natural logarithm
    Log, ///< Base-10 logarithm
    Sin, ///< Sine
    Tg, ///< Tangent
    Sqr, ///< Square
    ATg2, ///< Two-argument arc tangent
    Min, ///< Minimum value
    Max, ///< Maximum value
    SMin, ///< Smooth minimum
    SMax, ///< Smooth maximum
    Avg, ///< Average
    Var, ///< Variance
    Dev, ///< Standard deviation
    Diff, ///< Difference/derivative
    Sign, ///< Sign function
    Sinh, ///< Hyperbolic sine
    Cosh, ///< Hyperbolic cosine
    Tgh, ///< Hyperbolic tangent
    ASinh, ///< Inverse hyperbolic sine
    ACosh, ///< Inverse hyperbolic cosine
    Atgh, ///< Inverse hyperbolic tangent
    Int, ///< Integer part
    Lim, ///< Limit function
    Rnd, ///< Random number
    Disn, ///< Normal distribution sample
    Disc, ///< Continuous distribution sample
    Round, ///< Round to nearest integer
    Sample, ///< Generic sample function
    Last ///< Sentinel value for array bounds checking
};

static_assert( static_cast<size_t>(RealFunction::Last) == std::size(RealFunctionNames),
    "RealFunctionNames must match RealFunction enum"
);

constexpr const char* const ComplexFunctionNames[] = {"conj", "cabs", "rtop", "ptor", "real", "imag"}; //additional complex functions

/// @brief Enumeration of model group types that organize equations and declarations.
enum class Group : td::BYTE {Model=0, ///< Top-level model group
    SubModel, ///< Sub-model group
    Vars, ///< Variables group
    Params, ///< Parameters group
    Consts, ///< Constants group
    DataSets, ///< Data sets group
    Samplers, ///< Samplers group
    Distribs, ///< Distributions group
    Stats, ///< Statistics group
    ReInit, ///< Re-initialization group
    PreProc, ///< Pre-processing group
    NLEs, ///< Non-linear equations group
    WLSEs, ///< Weighted least squares equations group
    ECs, ///< Equality constraints group
    ODEs, ///< Ordinary differential equations group
    PDEs, ///< Partial differential equations group
    TFs, ///< Transfer functions group
    IterPostP, ///< Iterative post-processing group
    BasePostP, ///< Base post-processing group
    PostProc, ///< General post-processing group
    Limits, ///< Limits group
    Repeats, ///< Repeats group
    Plots, ///< Plots group
    Last ///< Sentinel value for array bounds checking
};

constexpr const char* const GroupNames[] = {"Model", "SubModel", "Vars", "Params", "Consts", "DataSets", "Samplers", "Distribs", "Stats", "ReInit", "PreProc", "NLEs", "WLSEs", "ECs", "ODEs", "PDEs", "TFs", "IterPostP", "BasePostP", "PostProc", "Limits", "Repeats", "Plots"};

static_assert( static_cast<size_t>(Group::Last) == std::size(GroupNames),
    "GroupNames must match Group enum"
);

//all attribIDs are in this enum
/// @brief Enumeration of all attribute identifiers used in model definitions.
enum class AttribID : td::BYTE {Domain=0, ///< Domain attribute (real/complex)
    Type, ///< Type attribute
    Method, ///< Numerical method attribute
    Eps, ///< Tolerance/epsilon attribute
    DeltaT, ///< Time step attribute
    Conn, ///< Connection/file path attribute
    Data, ///< Data query attribute
    DS, ///< Data set reference attribute
    Param, ///< Parameter reference attribute
    OutVals, ///< Output values attribute
    Out, ///< Output flag attribute
    Weight, ///< Weight attribute
    Conj, ///< Conjugate flag attribute
    Enabled, ///< Enabled flag attribute
    Signal, ///< Signal type attribute
    CopyParamsFromParent, ///< Copy parameters from parent model attribute
    Min, ///< Minimum value attribute
    Max, ///< Maximum value attribute
    Mean, ///< Mean value attribute
    Dev, ///< Standard deviation attribute
    Integer, ///< Integer flag attribute
    K, ///< Gain coefficient attribute
    P, ///< Probability attribute
    N, ///< Count attribute
    Alpha, ///< Alpha distribution parameter
    Beta, ///< Beta distribution parameter
    Lambda, ///< Lambda distribution parameter
    ReInit, ///< Re-initialization flag attribute
    AlwaysOn, ///< Always-on flag attribute
    InitMain, ///< Initialize main model attribute
    Simplify, ///< Simplification flag attribute
    Pivot, ///< Pivot strategy attribute
    MaxIter, ///< Maximum iterations attribute
    Width, ///< Width attribute (common)
    Name, ///< Name attribute (common)
    MinX, ///< Minimum X axis value
    MaxX, ///< Maximum X axis value
    MinY, ///< Minimum Y axis value
    MaxY, ///< Maximum Y axis value
    LineColorL, ///< Line color (light theme)
    LineColorD, ///< Line color (dark theme)
    BackColor, ///< Background color
    Legend, ///< Legend visibility flag
    Step, ///< Step size attribute
    Pattern, ///< Line pattern attribute
    XLabel, ///< X axis label
    YLabel, ///< Y axis label
    Anchor, ///< Anchor position attribute
    NCols, ///< Number of columns attribute
    AnchorX, ///< X anchor coordinate
    AnchorY, ///< Y anchor coordinate
    Last ///< Sentinel value for array bounds checking
};

//conn=":data/acc5.txt" data="SELECT time, xAcc as ax, yAcc as ay"]
//
//Samplers:
//    accell [ds=dsAccell param="time" outVals=

/// @brief Checks whether the given attribute ID represents a color attribute.
/// @param attID The attribute ID to check.
/// @return True if the attribute is a color attribute, false otherwise.
inline bool isColor(AttribID attID)
{
    return (attID >= AttribID::LineColorL && attID <= AttribID::BackColor);
}

/// @brief Associates a string label with its corresponding token type.
using TokenLabel = struct _tokLab
{
    const char* const lbl; ///< String label text
    Token token; ///< Corresponding token type
};

constexpr TokenLabel AtttribNames[] = {{"domain", Token::AttribEnumValue}, {"type",Token::AttribEnumValue},
    {"method",Token::AttribEnumValue}, {"eps",Token::ConstFloat}, {"dT",Token::ConstFloat}, {"conn",Token::ConstString}, {"data",Token::ConstString}, {"ds",Token::ConstString}, {"param",Token::ConstString}, {"outVals",Token::ConstString}, {"out",Token::ConstLogic},
    {"w",Token::ConstFloat}, {"conj",Token::ConstLogic}, {"enabled",Token::ConstLogic}, {"signal",Token::AttribEnumValue},
    {"copyPars",Token::ConstInt},{"min",Token::ConstFloat},{"max",Token::ConstFloat},{"mean",Token::ConstFloat},
    {"dev",Token::ConstFloat}, {"integer",Token::ConstLogic}, {"k",Token::ConstFloat},
    {"p",Token::ConstFloat}, {"n",Token::ConstFloat}, {"alpha",Token::ConstFloat}, {"beta",Token::ConstFloat},
    {"lambda",Token::ConstFloat}, {"reInit",Token::ConstLogic}, {"alwaysOn",Token::ConstLogic},
    {"initMain",Token::ConstLogic}, {"simplify",Token::ConstLogic}, {"pivot",Token::AttribEnumValue}, {"maxIter",Token::ConstInt},
    {"width",Token::ConstFloat}, {"name",Token::ConstString}, //common part (below is for naming==1
    {"minX",Token::ConstFloat}, {"maxX",Token::ConstFloat}, {"minY",Token::ConstFloat}, {"maxY",Token::ConstFloat},
    {"colorL",Token::ConstColor}, {"colorD",Token::ConstColor}, {"backColor",Token::AttribEnumValue},
    {"legend",Token::ConstLogic},
    {"step",Token::ConstFloat}, {"pattern",Token::ConstLinePattern},
    {"xLabel",Token::ConstString}, {"yLabel",Token::ConstString},
    {"anchor",Token::AttribEnumValue}, {"nCols",Token::ConstInt},
    {"anchorX",Token::ConstString}, {"anchorY",Token::ConstString}};

static_assert( static_cast<size_t>(AttribID::Last) == std::size(AtttribNames),
    "AtttribNames must match AttribID enum"
);

/// @brief Pairs an attribute ID with its expected value token type.
using AttribDesc = struct _AttDesc
{
    AttribID attribID; ///< The attribute identifier
    Token attribValueType; ///< The token type of the attribute value
};

/// @brief Enumeration of reserved keyword identifiers in the symbolic compiler language.
enum class ReservedID : td::BYTE {Main, ///< Main model entry point '@main'
    Repeat, ///< Repeat construct
    LaplaceOperator, ///< Laplace operator 's'
    Group, ///< Group keyword
    If, ///< If conditional
    Else, ///< Else branch
    Switch, ///< Switch statement
    Case, ///< Case clause
    Default, ///< Default clause
    End, ///< End of block
    LinePlot, ///< Line plot declaration
    BarPlot, ///< Bar plot declaration
    PiePlot, ///< Pie plot declaration
    HLine, ///< Horizontal line plot element
    VLine, ///< Vertical line plot element
    HBand, ///< Horizontal band plot element
    VBand, ///< Vertical band plot element
    Cond, ///< Condition expression '@cond'
    XAxis, ///< X axis reference '@x'
    YAxis, ///< Y axis reference '@y'
    Last ///< Sentinel value for array bounds checking
};


/// @brief Checks whether the given reserved keyword increases scope depth.
/// @param reservedID The reserved keyword to check.
/// @return True if the keyword opens a new scope, false otherwise.
inline bool isScopeIncreaser(ReservedID reservedID)
{
    return ( (reservedID == ReservedID::Group) ||
            (reservedID == ReservedID::If) || (reservedID == ReservedID::Switch) ||
            (reservedID == ReservedID::LinePlot) || (reservedID == ReservedID::BarPlot) || (reservedID == ReservedID::PiePlot));
}

/// @brief Checks whether the given reserved keyword decreases scope depth.
/// @param reservedID The reserved keyword to check.
/// @return True if the keyword closes or partially closes a scope, false otherwise.
inline bool isScopeDecreaser(ReservedID reservedID)
{
    return ((reservedID == ReservedID::Else) || (reservedID == ReservedID::Group) );
}

/// @brief Describes a reserved keyword with its ID and scope change effect.
using ReservedDesc = struct _RDS
{
    ReservedID reservedID; ///< The reserved keyword identifier
    td::BYTE scopeIncrease; ///< Scope depth increase caused by this keyword
    td::BYTE scopeDescrease; ///< Scope depth decrease caused by this keyword
};

/// @brief Describes the scope change effect of a reserved keyword without its ID.
using ReservedScopeDesc = struct _RSDS
{
    td::BYTE scopeIncrease; ///< Scope depth increase amount
    td::BYTE scopeDescrease; ///< Scope depth decrease amount
};

/// @brief Describes a group construct with its group type ID.
using GroupDesc = struct _GDS
{
    Group groupID; ///< The group type identifier
};

/// @brief Associates a reserved keyword string label with its scope change descriptor.
using ReservedLblDesc = struct _RLBLDS
{
    const char* const lbl; ///< String representation of the reserved keyword
    ReservedScopeDesc reservedDesc; ///< Scope change descriptor
};

//see ReservedID above
constexpr ReservedLblDesc ReservedNames[] = { {"@main", {0, 0}}, {"repeat", {0, 0}}, {"s", {0, 0}}, {"group", {0, 0}},
    {"if", {1, 0}}, {"else", {0, 0}}, {"switch", {1, 0}}, {"case", {0, 0}}, {"default", {0, 0}}, {"end", {0, 1}}, //Common parw starts with if
    {"linePlot", {1, 0}}, {"barPlot", {1, 0}}, {"piePlot", {1, 0}}, {"hLine@", {0, 0}}, {"vLine@", {0, 0}},
    {"hBand@", {0, 0}}, {"vBand@", {0, 0}},
    {"@cond", {0, 0}}, {"@x", {0, 0}}, {"@y", {0, 0}}};

static_assert( static_cast<size_t>(ReservedID::Last) == std::size(ReservedNames),
    "ReservedNames must match ReservedID enum"
);

/// @brief Returns the string name for a given token type.
/// @param token The token whose name is requested.
/// @return A pointer to the constant string name, or "NA" if the token is out of range.
inline const char* const getTokenName(sc::Token token)
{
    size_t tokPos = size_t(token);
    if (token > Token::Error)
        return "NA";
    return TokenNames[tokPos];
}

/// @brief Returns the string label for a given attribute ID.
/// @param attribID The attribute ID whose label is requested.
/// @return A pointer to the constant string label, or "NA" if the ID is out of range.
inline const char* const getAttribName(sc::AttribID attribID)
{
    size_t tokPos = size_t(attribID);
    if (attribID >= AttribID::Last)
        return "NA";
    return AtttribNames[tokPos].lbl;
}

/// @brief Returns the string label for a given reserved keyword ID.
/// @param reservedID The reserved keyword ID whose label is requested.
/// @return A pointer to the constant string label, or "NA" if the ID is out of range.
inline const char* const getReserveName(sc::ReservedID reservedID)
{
    size_t tokPos = size_t(reservedID);
    if (reservedID >= ReservedID::Last)
        return "NA";
    return ReservedNames[tokPos].lbl;
}

//Label types - for coloring
/// @brief Enumeration of model structural types affecting grouping behavior.
enum class Type : td::BYTE {ModelWithGroups=0, ///< Model that contains explicit groups
    NoModelWithGroups, ///< Non-model structure with groups
    NoModelNoGroups ///< Flat structure without model or groups
};
/// @brief Enumeration of mathematical domains supported by the compiler.
enum class Domain : td::BYTE {Real = 0, ///< Real number domain
    Complex ///< Complex number domain
};
/// @brief Enumeration indicating whether user-defined labels exist.
enum class UserLabels : td::BYTE {None=0, ///< No user labels
    Extern, ///< External user labels
    Intern ///< Internal user labels
};
/// @brief Enumeration of user-defined symbol categories.
enum class UserLabel : td::BYTE {Variable=0, ///< Variable symbol
    Param, ///< Parameter symbol
    Const, ///< Constant symbol
    Distrib, ///< Distribution symbol
    Stat, ///< Statistical symbol
    DataSet, ///< Data set symbol
    Sampler, ///< Sampler symbol
    Last ///< Sentinel value for array bounds checking
};
/// @brief Enumeration of function set categories.
enum class FunctionSet : td::BYTE {Real=0, ///< Real-valued function set
    Complex, ///< Complex-valued function set
    NA ///< Not applicable / unspecified
};
/// @brief Enumeration of color groups used to syntax-highlight model tokens.
enum class ColorGroup : td::BYTE {Variable=0, ///< Variable token color group
    Param, ///< Parameter token color group
    Const, ///< Constant token color group
    DataSet, ///< Data set token color group
    NumericConstant, ///< Numeric constant color group
    LogicalConstant, ///< Logical constant color group
    StringConstant, ///< String constant color group
    ColorConstant, ///< Color constant color group
    Function, ///< Function token color group
    Group, ///< Group keyword color group
    Operator, ///< Operator token color group
    AttribName, ///< Attribute name color group
    AttribValue, ///< Attribute value color group
    Comment, ///< Comment color group
    UnknownLabel, ///< Unknown identifier color group
    DoubleDefinition, ///< Duplicate definition color group
    NotInModel, ///< Symbol not in model color group
    Reserved, ///< Reserved keyword color group
    Other ///< Miscellaneous color group
};

/// @brief Enumeration of supplementary group types for extended model sections.
enum class SupplementaryGroup : td::BYTE {ReInit=0, ///< Re-initialization section
    PreProc, ///< Pre-processing section
    IterPostProc, ///< Iterative post-processing section
    BasePostProc, ///< Base post-processing section
    PostProc, ///< General post-processing section
    LimitGroup, ///< Limit constraints section
    Repeat, ///< Repeat section
    Last ///< Sentinel value for array bounds checking
};

//    enum class Model : td::BYTE {NA=0, Main, SubModel};

//enum class Group : td::BYTE {Other=0, Model, Vars, Params, WLS};

/// @brief Enumeration of token location contexts within a model structure.
enum class Location : td::BYTE {BeforeModel=0, ///< Before any model declaration
    OnModel, ///< On the model declaration line
    Model, ///< Inside the main model
    ModelVars, ///< Inside the main model variables section
    ModelParams, ///< Inside the main model parameters section
    ModelConsts, ///< Inside the main model constants section
    ModelDistribs, ///< Inside the main model distributions section
    ModelStats, ///< Inside the main model statistics section
    ModelDataSets, ///< Inside the main model data sets section
    ModelSamplers, ///< Inside the main model samplers section
    ModelBody, ///< Inside the main model body
    SubModel, ///< Inside a sub-model declaration
    SubModelVars, ///< Inside a sub-model variables section
    SubModelParams, ///< Inside a sub-model parameters section
    SubModelConsts, ///< Inside a sub-model constants section
    SubModelDistribs, ///< Inside a sub-model distributions section
    SubModelStats, ///< Inside a sub-model statistics section
    SubModelDataSets, ///< Inside a sub-model data sets section
    SubModelSamplers, ///< Inside a sub-model samplers section
    SubModelBody, ///< Inside a sub-model body
    NotInModel ///< Outside any model context
};

using Range = td::Range<td::UINT4>;

/// @brief Enumeration of mathematical model types.
enum class ModelType : unsigned char {NL=0, ///< Non-linear model
    WLS, ///< Weighted least squares model
    ODE, ///< Ordinary differential equation model
    DAE, ///< Differential algebraic equation model
    NA ///< Not applicable / unspecified
};

/// @brief Enumeration of expression types within a model body.
enum class ExpressionType : td::BYTE {Error=0, ///< Error / invalid expression
    Normal, ///< Normal equation expression
    Sampler, ///< Sampler expression
    If, ///< If conditional expression
    Else, ///< Else branch expression
    Switch, ///< Switch statement expression
    Case, ///< Case clause expression
    End, ///< End-of-block expression
    Grouper, ///< Group definition expression
    Repeater ///< Repeat construct expression
};

/// @brief Enumeration indicating whether a model is a main model or a sub-model.
enum class ModelLevel : td::BYTE {MainModel=0, ///< Top-level main model
    SubModel ///< Nested sub-model
};

/// @brief Enumeration of symbol input/output roles.
enum class InputOuputType : unsigned char {Internal = 0, ///< Internal symbol (not exposed)
    Input, ///< Input symbol
    Output ///< Output symbol
};

/// @brief Enumeration of symbol variable types.
enum class SymbolType : unsigned char {Parameter = 0, ///< Parameter symbol
    NormalVariable, ///< Normal state variable
    ArbitraryVariable, ///< Arbitrary (free) variable
    StateVariable, ///< Differential state variable
    ArbitraryStateVariable ///< Arbitrary differential state variable
};

/// @brief Enumeration of numeric data types for symbols.
enum class NumericType : unsigned char {Bool=0, ///< Boolean type
    Int, ///< Integer type
    Real, ///< Real (double) type
    Cmplx ///< Complex type
};

/// @brief Aggregates symbol type, numeric type, and input/output role information.
using SymbolInfo = struct _SI
{
    SymbolType symbolType; ///< The type of the symbol (parameter, variable, etc.)
    NumericType numericType; ///< The numeric type of the symbol
    InputOuputType inputOutputType; ///< Whether the symbol is internal, input, or output
};

/// @brief Associates a string label with a numeric type identifier.
using NumericTypeLabel = struct _ntLab
{
    const char* lbl; ///< String name of the numeric type
    NumericType numType; ///< Corresponding numeric type enumeration value
};

/// @brief Enumeration of special pre-defined mathematical constants.
enum class SpecialConstant : td::INT2 {Pi = 0, ///< Mathematical constant pi
    EulerNumber, ///< Euler's number e
    SolutionOK ///< Sentinel value indicating a successful solution
};

/// @brief Associates a name string with a special constant value.
using SpecialConstantInfo = struct _SpecConstInf
{
    const char* name; ///< String name of the special constant
    SpecialConstant value; ///< The special constant enumeration value
};

/// @brief Associates an alternative name string with a real function.
using AlternativeRealFnNames = struct _ARFn
{
    const char* name; ///< Alternative string name for the function
    RealFunction realFn; ///< The corresponding real function enumeration value
};

/// @brief Enumeration indicating whether a symbol originates from the current model or a parent model.
enum class SymbolOrigin : td::BYTE {ThisModel=0, ///< Symbol defined in the current model
    ParentModel ///< Symbol inherited from the parent model
};

/// @brief Represents a single tokenized value with its associated metadata.
using TokVal = struct _TokV
{
    union
    {
        td::ColorID colorID; ///< Color constant value
        td::LinePattern linePattern; ///< Line pattern constant value
        td::INT4 i4Val; ///< 32-bit integer constant value
        td::LINT8 i8Val; ///< 64-bit integer constant value
        double r8Val; ///< Double-precision floating-point constant value
        const td::StringExt* pStr; ///< Pointer to string constant value
    };

    td::INT4 index; ///< Index into the symbol table or constant list
    td::UINT2 colNo; ///< Column number of this token in the source text
    sc::Token token; ///< The token type of this value
    union
    {
        td::BYTE structMemberPos = 0; //for explicit structure memebers (for Variables and Params)
        AttribID attribID;  //filled for attribute values only
        SymbolOrigin symbolOrigin;
    };

    /// @brief Attempts to extract a double value from this token.
    /// @param val Reference to the output double variable.
    /// @return True if the token holds a numeric value and extraction succeeded, false otherwise.
    bool getValue(double& val) const
    {
        switch(token)
        {
            case sc::Token::ConstInt:
                val = i4Val;
                return true;
                break;
            case sc::Token::ConstFloat:
            case sc::Token::ConstImag:
                val = r8Val;
                return true;
                break;
            case sc::Token::ConstTrue:
                val = 1;
                return true;
                break;
            case sc::Token::ConstFalse:
                val = 0;
                return true;
                break;
            case sc::Token::SpecialConst:
                val = r8Val;
                return true;
                break;

            default:
                return false;
        }
        return false;
    }

    /// @brief Attempts to extract a complex value from this token.
    /// @param val Reference to the output complex variable.
    /// @return True if the token holds a numeric value and extraction succeeded, false otherwise.
    bool getValue(td::cmplx& val) const
    {
        switch(token)
        {
            case sc::Token::ConstInt:
                val.real(i4Val);
                return true;
                break;
            case sc::Token::ConstFloat:
                val.real(r8Val);
                return true;
                break;
            case sc::Token::ConstImag:
                val.imag(r8Val);
                return true;
                break;
            case sc::Token::ConstTrue:
                val = 1;
                return true;
                break;
            case sc::Token::ConstFalse:
                val = 0;
                return true;
                break;
            case sc::Token::SpecialConst:
                val.real(r8Val);
                return true;
                break;
            default:
                return false;
        }
        return false;
    }
};

/// @brief Holds element count and position tracking data for a model group.
using CountersAndPointers = struct _CP
{
    td::UINT4 nElems; ///< Number of elements in the group
    td::UINT4 positionInList; ///< Starting position in the lines-info list
    td::UINT4 nExpressions; ///< Number of expressions within the group
};

/// @brief Checks whether the given token is any kind of constant.
/// @param tok The token to test.
/// @return True if the token is a constant (string through logical), false otherwise.
inline bool isConstant(sc::Token tok)
{
    return ((tok >= Token::ConstString) && (tok <= Token::ConstLogic));
}

/// @brief Checks whether the given token is a real numeric constant.
/// @param tok The token to test.
/// @return True if the token is an integer or floating-point constant, false otherwise.
inline bool isRealConstant(sc::Token tok)
{
    return ((tok >= Token::ConstInt) && (tok <= Token::ConstFloat));
}

/// @brief Checks whether the given token is a numeric constant (integer, float, or imaginary).
/// @param tok The token to test.
/// @return True if the token is a numeric constant, false otherwise.
inline bool isNumericConstant(sc::Token tok)
{
    return ((tok >= Token::ConstInt) && (tok <= Token::ConstImag));
}

/// @brief Checks whether the given token is a logical constant (true, false, or logic).
/// @param tok The token to test.
/// @return True if the token is a logical constant, false otherwise.
inline bool isLogicalConstant(sc::Token tok)
{
    return ((tok >= Token::ConstTrue) && (tok <= Token::ConstLogic));
}

/// @brief Checks whether the given token is an operator.
/// @param tok The token to test.
/// @return True if the token is an operator, false otherwise.
inline bool isOperator(sc::Token tok)
{
    return ((tok >= Token::Plus) && (tok <= Token::IsDifferent));
}

/// @brief Checks whether the given token is an assignment operator.
/// @param tok The token to test.
/// @return True if the token is an assignment or compound-assignment operator, false otherwise.
inline bool isAssignment(sc::Token tok)
{
    return ((tok >= Token::MultiplyEqual) && (tok <= Token::Assignment));
}

/// @brief Checks whether the given token is any kind of opening bracket.
/// @param tok The token to test.
/// @return True if the token is '(', '[', or '{', false otherwise.
inline bool isAnyOpenBracket(sc::Token tok)
{
    return (tok == sc::Token::OpenParenthesis) || (tok == sc::Token::OpenBracket) || (tok == sc::Token::OpenCurlyBrace);
}

/// @brief Checks whether the given group ID represents a base equation group (NLEs through TFs).
/// @param groupID The group ID to check.
/// @return True if the group is a base solver group, false otherwise.
inline bool isBase(sc::Group groupID)
{
    if (groupID >= sc::Group::NLEs && groupID <= sc::Group::TFs)
        return true;
    return false;
}

} //namspace sc
