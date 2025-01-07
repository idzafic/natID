// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#define RPT_COL_POS_NOTOK  (-1)

//Generic IDs
// #Pos
#define RPT_INT_AUTO_NUMBER  (-10)
// #Page
#define RPT_TXT_PAGE_NUMBER  (-12)
#define RPT_INT_PAGE_NUMBER  (-13)
#define RPT_INT_TOTAL_NUMBER_OF_PAGES  (-14)
// #Now
#define RPT_DT_NOW  (-15)
#define RPT_GENERIC_LAST  (-16)

//DOCUMENT IDs
// #DocType
#define RPT_TXT_DOCUMENT_TYPE  (-1000)
// #DocID
#define RPT_TXT_DOCUMENT_ID  (-1001)
// #DocDT"			
#define RPT_DT_DOCUMENT_DATETIME (-1002)
// #DocPayDays
#define RPT_INT_DOCUMENT_PAYMENT_DAYS (-1003)
// #DocExtraIR
#define RPT_INT_DOCUMENT_EXTRA_INTEREST_RATE_AFTER_DAYS (-1004)
// #DocPayDT
#define RPT_DT_DOCUMENT_PAYMENT_DEADLINE_DATETIME (-1005)
// #DocState
#define RPT_INT_DOCUMENT_STATE (-1006)  //0-initial, 1-review, 2-final
// #DocFromDT
#define RPT_DT_DOCUMENT_FROM_DATETIME (-1007)
// #DocToDT"	
#define RPT_DT_DOCUMENT_TO_DATETIME (-1008)
// #DocMoneyUnit
#define RPT_TXT_DOCUMENT_MONEY_UNIT (-1009)
// #DocMoneySymb
#define RPT_TXT_DOCUMENT_MONEY_SYMBOL (-1010)
// #DocReclamDays
#define RPT_INT_DOCUMENT_RECLAMATION_DAYS (-1011)
// #DocCustDocNo
#define RPT_TXT_DOCUMENT_CUST_DOC_NO (-1012)
// #DocCustDocDT
#define RPT_DT_DOCUMENT_CUST_DOC_DT (-1013)
// #DocIntDocNo
#define RPT_DT_DOCUMENT_INT_DOC_NO (-1014)
// #DocIntDocDT
#define RPT_DT_DOCUMENT_INT_DOC_DT (-1015)
// #DocRevision
#define RPT_TXT_DOCUMENT_REVISION (-1016)
// #DocRevisionDT
#define RPT_DATE_DOCUMENT_REVISION (-1017)
// #DocFormID
#define RPT_TXT_DOCUMENT_FORM_ID (-1018)
#define RPT_DOCUMENT_LAST (-1019)


//COMPANY IDs
// #Company1
#define RPT_TXT_COMPANY1  (-2001)
// #Company2
#define RPT_TXT_COMPANY2  (-2002)
// #Company3
#define RPT_TXT_COMPANY3  (-2003)
// #Company4
#define RPT_TXT_COMPANY4  (-2004)
// #Company5
#define RPT_TXT_COMPANY5  (-2005)
// #Company6
#define RPT_TXT_COMPANY6  (-2006)
// #Company7
#define RPT_TXT_COMPANY7  (-2007)
// #Company8
#define RPT_TXT_COMPANY8  (-2008)
// #Company9
#define RPT_TXT_COMPANY9  (-2009)
// #Company10
#define RPT_TXT_COMPANY10 (-2010)
#define RPT_COMPANY_LAST (-2011)

//OBJECT IDs
// #ObjType
#define RPT_TXT_OBJ_TYPE  (-3000)
// #ObjID
#define RPT_TXT_OBJ_ID  (-3001)
// #ObjName
#define RPT_TXT_OBJ_NAME  (-3002)
// #ObjPlace
#define RPT_TXT_OBJ_PLACE  (-3003)
// #ObjAddress
#define RPT_TXT_OBJ_ADDRESS  (-3004)
// #ObjPhone
#define RPT_TXT_OBJ_PHONE  (-3005)
#define RPT_OBJ_LAST  (-3006)


//Signature IDs

// #Sign1
#define RPT_TXT_SINATURE1  (-4000)
// #Sign2
#define RPT_TXT_SINATURE2  (-4001)
// #Sign3
#define RPT_TXT_SINATURE3  (-4002)
// #Sign4
#define RPT_TXT_SINATURE4  (-4003)
// #Sign5
#define RPT_TXT_SINATURE5  (-4004)
// #Sign6
#define RPT_TXT_SINATURE6  (-4005)
// #Sign7
#define RPT_TXT_SINATURE7  (-4006)
// #Sign8
#define RPT_TXT_SINATURE8  (-4007)
// #Sign9
#define RPT_TXT_SINATURE9  (-4008)

#define RPT_SIGNATURE_LAST  (-4009)

#define RPT_FORMULA_VARIABLE_START (-10000)

namespace rpt
{
inline bool isGenericID(int id)
{
    return ((id <= RPT_INT_AUTO_NUMBER) && (id > RPT_GENERIC_LAST));
}

inline bool isDocumentID(int id)
{
    return ((id <= RPT_TXT_DOCUMENT_TYPE) && (id > RPT_DOCUMENT_LAST));
}

inline bool isCompanyID(int id)
{
    return ((id <= RPT_TXT_COMPANY1) && (id > RPT_COMPANY_LAST));
}

inline bool isObjectID(int id)
{
    return ((id <= RPT_TXT_OBJ_TYPE) && (id > RPT_OBJ_LAST));
}

inline bool isSignatureID(int id)
{
    return ((id <= RPT_TXT_SINATURE1) && (id > RPT_SIGNATURE_LAST));
}

template <typename TSTR>
inline int getAutoDocTxtID(const TSTR& str)
{
    if (str.length() <= 1)
        return -1;
    char ch = str.getAt(0);
    if (ch != '#')
        return -1;
    
    if (str.compareConstStr("DocType", 1))
        return RPT_TXT_DOCUMENT_TYPE;
    else if (str.compareConstStr("DocID", 1))
        return RPT_TXT_DOCUMENT_ID;
    else if (str.compareConstStr("DocDT", 1))
        return RPT_DT_DOCUMENT_DATETIME;
    else if (str.compareConstStr("DocFromDT", 1))
        return RPT_DT_DOCUMENT_FROM_DATETIME;
    else if (str.compareConstStr("DocToDT", 1))
        return RPT_DT_DOCUMENT_TO_DATETIME;
    else if (str.compareConstStr("DocPayDays", 1))
        return RPT_INT_DOCUMENT_PAYMENT_DAYS;
    else if (str.compareConstStr("DocExtraIR", 1))
        return RPT_INT_DOCUMENT_EXTRA_INTEREST_RATE_AFTER_DAYS;
    else if (str.compareConstStr("DocPayDT", 1))
        return RPT_DT_DOCUMENT_PAYMENT_DEADLINE_DATETIME;
    else if (str.compareConstStr("DocState", 1))
        return RPT_INT_DOCUMENT_STATE;
    else if (str.compareConstStr("DocMoneyUnit", 1))
        return RPT_TXT_DOCUMENT_MONEY_UNIT;
    else if (str.compareConstStr("DocMoneySymb", 1))
        return RPT_TXT_DOCUMENT_MONEY_SYMBOL;
    else if (str.compareConstStr("DocReclamDays", 1))
        return RPT_INT_DOCUMENT_RECLAMATION_DAYS;
    else if (str.compareConstStr("DocCustDocNo", 1))
        return RPT_TXT_DOCUMENT_CUST_DOC_NO;
    else if (str.compareConstStr("DocCustDocDT", 1))
        return RPT_DT_DOCUMENT_CUST_DOC_DT;
    else if (str.compareConstStr("DocIntDocNo", 1))
        return RPT_DT_DOCUMENT_INT_DOC_NO;
    else if (str.compareConstStr("DocIntDocDT", 1))
        return RPT_DT_DOCUMENT_INT_DOC_DT;
    else if (str.compareConstStr("DocFormID", 1))
        return RPT_TXT_DOCUMENT_FORM_ID;
    else if (str.compareConstStr("DocFormRevision", 1))
        return RPT_TXT_DOCUMENT_REVISION;
    else if (str.compareConstStr("DocFormRevisionDate", 1))
        return RPT_DATE_DOCUMENT_REVISION;
    return -1;
}

template <typename TSTR>
inline int getAutoObjTxtID(const TSTR& str)
{
    if (str.length() <= 1)
        return -1;
    char ch = str.getAt(0);
    if (ch != '#')
        return -1;
    
    if (str.compareConstStr("ObjType", 1))
        return RPT_TXT_OBJ_TYPE;
    else if (str.compareConstStr("ObjID", 1))
        return RPT_TXT_OBJ_ID;
    else if (str.compareConstStr("ObjName", 1))
        return RPT_TXT_OBJ_NAME;
    else if (str.compareConstStr("ObjPlace", 1))
        return RPT_TXT_OBJ_PLACE;
    else if (str.compareConstStr("ObjAddress", 1))
        return RPT_TXT_OBJ_ADDRESS;
    else if (str.compareConstStr("ObjPhone", 1))
        return RPT_TXT_OBJ_PHONE;
    return -1;
}

template <typename TSTR>
inline int getAutoCompanyTxtID(const TSTR& str)
{
    if (str.length() < 9)
        return -1;
    char ch = str.getAt(0);
    if (ch != '#')
        return -1;
    
    if (str.compareConstStr("Company1", 1))
        return RPT_TXT_COMPANY1;
    else if (str.compareConstStr("Company2", 1))
        return RPT_TXT_COMPANY2;
    else if (str.compareConstStr("Company3", 1))
        return RPT_TXT_COMPANY3;
    else if (str.compareConstStr("Company4", 1))
        return RPT_TXT_COMPANY4;
    else if (str.compareConstStr("Company5", 1))
        return RPT_TXT_COMPANY5;
    else if (str.compareConstStr("Company6", 1))
        return RPT_TXT_COMPANY6;
    else if (str.compareConstStr("Company7", 1))
        return RPT_TXT_COMPANY7;
    else if (str.compareConstStr("Company8", 1))
        return RPT_TXT_COMPANY8;
    else if (str.compareConstStr("Company9", 1))
        return RPT_TXT_COMPANY9;
    else if (str.compareConstStr("Company10", 1))
        return RPT_TXT_COMPANY10;
    return -1;
}

template <typename TSTR>
inline int getSignatureTxtID(const TSTR& str)
{
    // #Sign9
    auto nLen = str.length();
    if (nLen != 6)
        return -1;
    char ch = str.getAt(0);
    if (ch != '#')
        return -1;
    
    if (str.compareConstStr("Sign1", 1))
        return RPT_TXT_SINATURE1;
    else if (str.compareConstStr("Sign2", 1))
        return RPT_TXT_SINATURE2;
    else if (str.compareConstStr("Sign3", 1))
        return RPT_TXT_SINATURE3;
    else if (str.compareConstStr("Sign4", 1))
        return RPT_TXT_SINATURE4;
    else if (str.compareConstStr("Sign5", 1))
        return RPT_TXT_SINATURE5;
    else if (str.compareConstStr("Sign6", 1))
        return RPT_TXT_SINATURE6;
    else if (str.compareConstStr("Sign7", 1))
        return RPT_TXT_SINATURE7;
    else if (str.compareConstStr("Sign8", 1))
        return RPT_TXT_SINATURE8;
    else if (str.compareConstStr("Sign9", 1))
        return RPT_TXT_SINATURE9;
    return -1;
}
}
