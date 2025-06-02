// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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
enum class Token : td::BYTE {Label=0, Variable, Parameter, EmbParam,//variables, params, can be any length
    Function, Group, AttribName, AttribEnumValue, Structure, Reserved, //defined by user, constraint on length: MaxFunctionOrReservedKeyLength
    ConstString, ConstInt, SpecialConst, ConstFloat, ConstImag, ConstNumericComplex, ConstColor, ConstLinePattern, ConstDotPattern, ConstAny, ConstTrue, ConstFalse, ConstLogic, //constants
    OpenParenthesis, CloseParenthesis, OpenBracket, CloseBracket, OpenCurlyBrace, CloseCurlyBrace,  //brackets
    Plus, Minus, Multiply, Divide, DivideInt, DivideRight, Power,
    MultiplyEqual, DivideEqual, PlusEqual, MinusEqual, PowerEqual, StreamLeft, StreamRight, PointerToLeft, PointerToRight,
    Assignment,    // =
    Prime, Second,  // '   ''
    BitwiseInvert, BitwiseOr, BitwiseAnd, BitwiseXOR,   //operations on bits
    LogicalNot, LogicalOr, LogicalAnd, LogicalXOR,      //logical operations
    LessThan, LessThanOrEqual, GreatherThan, GreatherThanOrEqual, IsEqual, IsDifferent,     //comparison operators
    Comma, Period, Colon, Semicolon,  //end of operators
    Distribution, Stat,
    Comment, CCommentBegin, CCommentEnd, EndOfLine, Error};

constexpr const char* const TokenNames[] = { "Label", "Var", "Param", "EmbParam",
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
    "Dist", "Stat",
    "//", "/*", "*/", "NL", "ERR"};


constexpr const char* const RealFunctionNames[] = {"abs","acos","asin","atg","cos","exp","sqrt","ln","log","sin","tg", "sqr", "atg2", "min", "max", "smin", "smax", "avg", "var", "dev", "diff", "sign",
    "sinh", "cosh", "tgh", "asnh", "acsh", "atgh",
    "int", "lim", "rnd", "disn", "disc"};

enum class RealFunction : td::BYTE {Abs, ACos, ASin, ATg, Cos, Exp, Sqrt, Lm, Log, Sin, Tg, Sqr, ATg2, Min, Max, SMin, SMax, Avg, Var, Dev, Diff, Sign, Sinh, Cosh, Tgh, ASinh, ACosh, Atgh, Int, Lim, Rnd, Disn, Disc, Last};

constexpr const char* const ComplexFunctionNames[] = {"conj", "cabs", "rtop", "ptor", "real", "imag"}; //additional complex functions

enum class Group : td::BYTE {Model=0, SubModel, Vars, Params, Distribs, Stats, ReInit, PreProc, NLEs, WLSEs, ECs,
    ODEs, PDEs, TFs, IterPostP, BasePostP, PostProc, Limits, Repeats, Plots, Last};

constexpr const char* const GroupNames[] = {"Model", "SubModel", "Vars", "Params", "Distribs", "Stats", "ReInit", "PreProc", "NLEs", "WLSEs", "ECs", "ODEs", "PDEs", "TFs", "IterPostP", "BasePostP", "PostProc", "Limits", "Repeats", "Plots"};

//all attribIDs are in this enum
enum class AttribID : td::BYTE {Domain=0, Type, Method, Eps, DeltaT, Out, Weight, Conj, Enabled, Signal, CopyParamsFromParent, Min, Max, Mean, Dev, Integer, K, P, N, Alpha, Beta, Lambda, ReInit, AlwaysOn, InitMain, MaxIter, //namingPos == 0
    Width, Name,  //common
    MinX, MaxX, MinY, MaxY, LineColorL, LineColorD, BackColor, Legend, Step, Pattern, XLabel, YLabel,//namingPos == 1
    Last};

inline bool isColor(AttribID attID)
{
    return (attID >= AttribID::LineColorL && attID <= AttribID::BackColor);
}

using TokenLabel = struct _tokLab
{
    const char* const lbl;
    Token token;
};

constexpr TokenLabel AtttribNames[] = {{"domain", Token::AttribEnumValue}, {"type",Token::AttribEnumValue},
    {"method",Token::AttribEnumValue}, {"eps",Token::ConstFloat}, {"dT",Token::ConstFloat}, {"out",Token::ConstLogic},
    {"w",Token::ConstFloat}, {"conj",Token::ConstLogic}, {"enabled",Token::ConstLogic}, {"signal",Token::AttribEnumValue},
    {"copyPars",Token::ConstInt},{"min",Token::ConstFloat},{"max",Token::ConstFloat},{"mean",Token::ConstFloat},
    {"dev",Token::ConstFloat}, {"integer",Token::ConstLogic}, {"k",Token::ConstFloat},
    {"p",Token::ConstFloat}, {"n",Token::ConstFloat}, {"alpha",Token::ConstFloat}, {"beta",Token::ConstFloat},
    {"lambda",Token::ConstFloat}, {"reInit",Token::ConstLogic}, {"alwaysOn",Token::ConstLogic},
    {"initMain",Token::ConstLogic}, {"maxIter",Token::ConstInt},
    {"width",Token::ConstFloat}, {"name",Token::ConstString}, //common part (below is for naming==1
    {"minX",Token::ConstFloat}, {"maxX",Token::ConstFloat}, {"minY",Token::ConstFloat}, {"maxY",Token::ConstFloat},
    {"colorL",Token::ConstColor}, {"colorD",Token::ConstColor}, {"backColor",Token::AttribEnumValue},
    {"legend",Token::ConstLogic},
    {"step",Token::ConstFloat}, {"pattern",Token::ConstLinePattern},
    {"xLabel",Token::ConstString}, {"yLabel",Token::ConstString}};

using AttribDesc = struct _AttDesc
{
    AttribID attribID;
    Token attribValueType;
};

enum class ReservedID : td::BYTE {Main, Repeat, LaplaceOperator, Group, //naming = 0
    If, Else, End, //Common begins with If
    LinePlot, BarPlot, PiePlot, HLine, VLine, HBand, VBand, Cond, XAxis, YAxis, Last}; //naming = 1 (starts with LinePlot)


inline bool isScopeIncreaser(ReservedID reservedID)
{
    return ( (reservedID == ReservedID::If) || (reservedID == ReservedID::Group) ||
            (reservedID == ReservedID::LinePlot) || (reservedID == ReservedID::BarPlot) || (reservedID == ReservedID::PiePlot));
}

using ReservedDesc = struct _RDS
{
    ReservedID reservedID;
    td::BYTE scopeIncrease;
    td::BYTE scopeDescrease;
};

using ReservedScopeDesc = struct _RSDS
{
    td::BYTE scopeIncrease;
    td::BYTE scopeDescrease;
};

using GroupDesc = struct _GDS
{
    Group groupID;
};

using ReservedLblDesc = struct _RLBLDS
{
    const char* const lbl;
    ReservedScopeDesc reservedDesc;
};

//see ReservedID above
constexpr ReservedLblDesc ReservedNames[] = { {"@main", {0, 0}}, {"repeat", {0, 0}}, {"s", {0, 0}}, {"group", {0, 0}},
    {"if", {1, 0}}, {"else", {0, 0}}, {"end", {0, 1}}, //Common parw starts with if
    {"linePlot", {1, 0}}, {"barPlot", {1, 0}}, {"piePlot", {1, 0}}, {"hLine@", {0, 0}}, {"vLine@", {0, 0}},
    {"hBand@", {0, 0}}, {"vBand@", {0, 0}},
    {"@cond", {0, 0}}, {"@x", {0, 0}}, {"@y", {0, 0}}};

inline const char* const getTokenName(sc::Token token)
{
    size_t tokPos = size_t(token);
    if (token > Token::Error)
        return "NA";
    return TokenNames[tokPos];
}

inline const char* const getAttribName(sc::AttribID attribID)
{
    size_t tokPos = size_t(attribID);
    if (attribID >= AttribID::Last)
        return "NA";
    return AtttribNames[tokPos].lbl;
}

inline const char* const getReserveName(sc::ReservedID reservedID)
{
    size_t tokPos = size_t(reservedID);
    if (reservedID >= ReservedID::Last)
        return "NA";
    return ReservedNames[tokPos].lbl;
}

//Label types - for coloring
enum class Type : td::BYTE {ModelWithGroups=0, NoModelWithGroups, NoModelNoGroups};
enum class Domain : td::BYTE {Real = 0, Complex};
enum class UserLabels : td::BYTE {None=0, Extern, Intern};
enum class UserLabel : td::BYTE {Variable=0, Param, Distrib, Stat, Last}; //this object
enum class FunctionSet : td::BYTE {Real=0, Complex, NA}; //function groups
enum class ColorGroup : td::BYTE {Variable=0, Param, NumericConstant, LogicalConstant, StringConstant, ColorConstant, Function, Group, Operator, AttribName, AttribValue, Comment, UnknownLabel, DoubleDefinition, NotInModel, Reserved, Other}; //color groups

enum class SupplementaryGroup : td::BYTE {ReInit=0, PreProc, IterPostProc, BasePostProc, PostProc, LimitGroup, Repeat, Last};

//    enum class Model : td::BYTE {NA=0, Main, SubModel};

//enum class Group : td::BYTE {Other=0, Model, Vars, Params, WLS};

enum class Location : td::BYTE {BeforeModel=0, OnModel, Model, ModelVars, ModelDistribs, ModelStats, ModelParams, ModelBody, SubModel, SubModelVars, SubModelDistribs, SubModelStats, SubModelParams, SubModelBody, NotInModel};

using Range = td::Range<td::UINT4>;

enum class ModelType : unsigned char {NL=0, WLS, ODE, DAE, NA};

enum class ExpressionType : td::BYTE {Error=0, Normal, If, Else, End, Grouper, Repeater};

enum class ModelLevel : td::BYTE {MainModel=0, SubModel};

enum class InputOuputType : unsigned char {Internal = 0, Input, Output};

enum class SymbolType : unsigned char {Parameter = 0, NormalVariable, ArbitraryVariable, StateVariable, ArbitraryStateVariable};

enum class NumericType : unsigned char {Bool=0, Int, Real, Cmplx};

using SymbolInfo = struct _SI
{
    SymbolType symbolType;
    NumericType numericType;
    InputOuputType inputOutputType;
};

using NumericTypeLabel = struct _ntLab
{
    const char* lbl;
    NumericType numType;
};

enum class SpecialConstant : td::INT2 {Pi = 0, EulerNumber, SolutionOK};

using SpecialConstantInfo = struct _SpecConstInf
{
    const char* name;
    SpecialConstant value;
};

using AlternativeRealFnNames = struct _ARFn
{
    const char* name;
    RealFunction realFn;
};

enum class SymbolOrigin : td::BYTE {ThisModel=0, ParentModel};

using TokVal = struct _TokV
{
    union
    {
        td::ColorID colorID;
        td::LinePattern linePattern;
        td::INT4 i4Val;
        td::LINT8 i8Val;
        double r8Val;
        const td::StringExt* pStr;
    };
    
    td::INT4 index;
    td::UINT2 colNo;
    sc::Token token;
    union
    {
        td::BYTE structMemberPos = 0; //for explicit structure memebers (for Variables and Params)
        AttribID attribID;  //filled for attribute values only
        SymbolOrigin symbolOrigin;
    };
    
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

using CountersAndPointers = struct _CP
{
    td::UINT4 nElems;
    td::UINT4 positionInList;
    td::UINT4 nExpressions;
};

inline bool isConstant(sc::Token tok)
{
    return ((tok >= Token::ConstString) && (tok <= Token::ConstLogic));
}

inline bool isRealConstant(sc::Token tok)
{
    return ((tok >= Token::ConstInt) && (tok <= Token::ConstFloat));
}

inline bool isNumericConstant(sc::Token tok)
{
    return ((tok >= Token::ConstInt) && (tok <= Token::ConstImag));
}

inline bool isLogicalConstant(sc::Token tok)
{
    return ((tok >= Token::ConstTrue) && (tok <= Token::ConstLogic));
}

inline bool isOperator(sc::Token tok)
{
    return ((tok >= Token::Plus) && (tok <= Token::IsDifferent));
}

inline bool isAssignment(sc::Token tok)
{
    return ((tok >= Token::MultiplyEqual) && (tok <= Token::Assignment));
}

inline bool isAnyOpenBracket(sc::Token tok)
{
    return (tok == sc::Token::OpenParenthesis) || (tok == sc::Token::OpenBracket) || (tok == sc::Token::OpenCurlyBrace);
}

} //namspace sc
