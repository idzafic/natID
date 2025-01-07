from utils import DisplayFormat
from dumper import Children, SubItem, DumperBase
  
def extractUtf8String(d, ptr, size):
    if size > 32768:
        size = 32768
    if size > 0:
        result = bytearray(size)
    else:
        result = bytearray()
    
    iPos = 0
    iCh = 0
    toSkeep = 0
    nSpecialChs = 0
    iBytes = 0
    while True:
        b = d.extractSomething(ptr, 'B', 8)
        if b == 0:
            break
        if b < 32:
            nSpecialChs += 1
            #b=63
        if size > 0:
            result[iPos] = b
        else:
            result.append(b)
        iPos += 1
        iBytes += 1
        if toSkeep == 0:
            if b < 128:
                iCh += 1
            else:
                bTest = b >> 5
                if bTest == 6:
                    toSkeep = 1
                else:
                    bTest = b >> 4
                    if bTest == 14:
                        toSkeep = 2
                    else:
                        toSkeep = 3
        else:
            toSkeep -= 1
            if toSkeep <= 0:
                iCh += 1
                toSkeep = 0
        if size > 0:
            if iPos >= size:
                break
        ptr += 1
    #strToRet = "-----"
    return iBytes, nSpecialChs, iCh, result.decode('utf8', errors='replace')

def extractUtf16String(d, ptr, size):
    if size > 32768:
        size = 32768
    if size == 0:
        size = 4096
    if size > 0:
        result = bytearray(size)
    else:
        result = bytearray()
    iPos = 0
    iCh = 0
    nSpecialChs = 0
    iBytes = 0
    while True:
        b1 = d.extractSomething(ptr, 'B', 8)
        b2 = d.extractSomething(ptr+1, 'B', 8)
        #b1 = d.extractByte(ptr)
        #b2 = d.extractByte(ptr+1)
        if b1 == 0 and b2 == 0:
            break
        if size > 0:
            result[iPos] = b1
            result[iPos + 1] = b2
        else:
            result.append(b1)
            result.append(b2)
        ch16 = b1
        ch16 = (ch16 << 8) | b2
        iBytes += 2
        if ch16 < 32:
            nSpecialChs += 1
        iPos += 2
        iCh += 1
        if size > 0:
            if iPos >= size:
                break
        ptr += 2
    return iBytes, nSpecialChs, iCh, result.decode('utf16', errors='replace')

def getTdString(d, ptr, enc, lenBytes):
    if ptr == 0:
        if enc == 0:
            return "l=0:0:0, s8=nullptr"
        elif enc == 1:
            return "l=0:0:0, s16=nullptr"
        else:
            return "l=0:0:0, s32=nullptr"
    else:
        if lenBytes > 0:
            refAndSize = d.extractUInt(ptr)
            size = refAndSize & 0x0FFFFFFF
            refs = refAndSize >> 28
            ptrStr = ptr + lenBytes             
            if enc == 0:                
                iBytes, nSpecialChs, iCh, strData = extractUtf8String(d, ptrStr, size)
                return "len={0}:{1}:{2},s8={3}".format(size, refs, iCh, strData)
            elif enc == 1:
                iBytes, nSpecialChs, iCh, strData = extractUtf16String(d, ptrStr, size)
                return "len={0}:{1}:{2},s8={3}".format(size, refs, iCh, strData)
            else:
                return "UNDEF:UTF-32 encoding with enc={} is not supported".format(argName)
        else:
            size = 0
            ptrStr = ptr
            iBytes, nSpecialChs, iCh, strData = extractUtf8String(d, ptrStr, size)
            return "len={0}:{1}:{2},s8={3}".format(size, refs, iCh, strData)

def qdump__td__StringBase(d, value):
    tempArg = value.type.templateArgument(0)
    lenBytes = value.type.templateArgument(2)
    argName = tempArg.name
    enc = 0
    if argName == "char":
        enc = 0
    elif argName == "short" or argName == "wchar_t":
        enc = 1
    else:
        enc = 2
    ptr = value["buffer"].pointer()
    strFormatted = getTdString(d, ptr, enc, lenBytes)
    d.putValue("{0}".format(strFormatted))
    
def getDateString(tdDate):
    tdDay = 0x0000001F & tdDate
    tdMonth = ((0x000001E0 & tdDate) >> 5)
    tdYear = ((0x3FFFFE00 & tdDate) >> 9)
    strVal = "{0:04d}-{1:02d}-{2:02d}".format(tdYear, tdMonth, tdDay)
    return strVal
    
def getTimeString(tdTime):
    h = tdTime // (60 * 60 * 10000)
    m = tdTime % (60 * 60 * 10000)
    m = m // (60 * 10000)
    s = tdTime % (60 * 10000)
    s = s // 10000
    fr = tdTime % 10000
    strVal = "{0:02d}:{1:02d}:{2:02d}.{3:04d}".format(h, m, s, fr)
    return strVal
    
def getDateTimeString(tdDate, tdTime):
    day = 0x0000001F & tdDate
    mnth = ((0x000001E0 & tdDate) >> 5)
    y = ((0x3FFFFE00 & tdDate) >> 9)
    h = tdTime // (60 * 60 * 10000)
    m = tdTime % (60 * 60 * 10000)
    m = m // (60 * 10000)
    s = tdTime % (60 * 10000)
    s = s // 10000
    fr = tdTime % 10000    
    locUtc = "loc"
    isUtc = (0x40000000 & tdDate) >> 28
    if isUtc > 0:
        locUtc = "utc"    
    strVal = "{0:04d}-{1:02d}-{2:02d} {3:02d}:{4:02d}:{5:02d}.{6:04d} {7}".format(y, mnth, day, h, m, s, fr, locUtc)
    return strVal
    
def qdump__td__Date(d, value):
    d.putNumChild(0)
    tdDate = value["_date"].integer()
    d.putValue(getDateString(tdDate))
    
def qdump__td__Time(d, value):
    d.putNumChild(0)
    tdTime = value["_time"].integer()
    d.putValue(getTimeString(tdTime))
    
def qdump__td__DateTime(d, value):
    d.putNumChild(0)
    tdDate = value["_date"].integer()
    tdTime = value["_time"].integer()
    d.putValue(getDateTimeString(tdDate, tdTime))

def extractDecimal(val, s):
    absVal = abs(val)
    intPart = absVal // s
    decPart = absVal % s
    if val < 0:
        intPart = -intPart
    return intPart, decPart

def getDecimalString(val, dType):
    strVal = ""
    if dType == 0:
        strVal = "{0}"
        strVal = strVal.format(val)
    elif dType == 1:
        intPart, decPart = extractDecimal(val, 10)
        strVal = "{0}.{1}"
        strVal = strVal.format(intPart, decPart)
    elif dType == 2:
        intPart, decPart = extractDecimal(val, 100)
        strVal = "{0}.{1:02d}"
        strVal = strVal.format(intPart, decPart)
    elif dType == 3:
        intPart, decPart = extractDecimal(val, 1000)
        strVal = "{0}.{1:03d}"
        strVal = strVal.format(intPart, decPart)
    elif dType == 4:
        intPart, decPart = extractDecimal(val, 10000)
        strVal = "{0}.{1:04d}"
        strVal = strVal.format(intPart, decPart)
    else:
        strVal = "{0}.UNDEF"
        strVal = strVal.format(val)
    return strVal
        
    
def putAsDecimal(d, val, dType):
    d.putNumChild(0)
    d.putValue(getDecimalString(val, dType))

def qdump__td__Decimal(d, value):
    val = value["_value"].integer()
    nDec = int(value.type.templateArgument(1))
    putAsDecimal(d, val, nDec)
   
def getTdDataType(tdDataType):
    dtStr = ['TD_NONE', 'boolean', 'byte', 'word', 'int2', 'uint4', 'int4', 'lint8', 'luint8', 'real4', 'real8', 'string8', 'string16', 'string32', 'enumerator', 'date', 'time', 'dateTime', 'decimal0', 'decimal1', 'decimal2', 'decimal3', 'decimal4', 'ch', 'nch', 'chFix', 'nchFix', 'ch7', 'dbDate', 'dbTime', 'dbDateTime', 'dbDecimal', 'binary', 'guid', 'color', 'smallDecimal0', 'smallDecimal1', 'smallDecimal2', 'smallDecimal3', 'smallDecimal4', 'rect', 'point', 'variant', 'ptrBoolean', 'ptrByte', 'ptrWord', 'ptrInt2', 'ptrUint4', 'ptrInt4', 'ptrLint8', 'ptrLuint8', 'ptrReal4', 'ptrReal8', 'ptrString8', 'ptrString16', 'ptrString32', 'ptrEnumerator', 'ptrDate', 'ptrTime', 'ptrDateTime', 'ptrDecimal0', 'ptrDecimal1', 'ptrDecimal2', 'ptrDecimal3', 'ptrDecimal4', 'ptrCh', 'ptrNch', 'ptrChFix', 'ptrSmallChFix', 'ptrNchFix', 'ptrDbDate', 'ptrDbTime', 'ptrDbDateTime', 'ptrDbDecimal', 'ptrBinary', 'ptrGuid', 'ptrColor', 'ptrSmalldecimal0', 'ptrSmalldecimal1', 'ptrSmalldecimal2', 'ptrSmalldecimal3', 'ptrSmalldecimal4', 'ptrRect', 'ptrPoint', 'ptrVariant', 'ptrVoid', 'TD_LAST']
    return dtStr[tdDataType]
    
def getVariantDataType(flags):
    tdDataType = flags & 0x000000FF
    dbDataType = (flags >> 8) & 0x000000FF
    dbLen = flags >> 32
    strTdDataType = getTdDataType(tdDataType)
    return strTdDataType, tdDataType, dbDataType, dbLen
    
def getVariantValueString(d, tdDataType, value):
    intVal = value["_li8Val"].integer()
    if tdDataType == 0:
        return "UNDEF"
    elif tdDataType == 1:    #boolean
        intVal = value["_bVal"].integer()
        if intVal == ord('Y'):
            return "true"
        else:
            return "false"
    elif tdDataType > 1 and tdDataType <= 8:
        strVal ="{0}"
        return strVal.format(intVal)
    elif tdDataType == 9:
        fVal = value["_r4Val"].floatingPoint()
        strVal="{0:f}"
        return strVal.format(fVal)
    elif tdDataType == 10:
        fVal = value["_r8Val"].floatingPoint()
        strVal="{0}"
        return strVal.format(fVal)
    elif tdDataType == 11:
        ptr = value["_strVal"].pointer()
        size, refs, strData = getTdString(d, ptr)
        strVal = "l={0}:{1}, s8={2}"
        return strVal.format(size, refs, strData)
    elif tdDataType == 15:
        dateVal = value["_dateVal"].integer()
        return getDateString(dateVal)
    elif tdDataType == 16:
        timeVal = value["_timeVal"].integer()
        return getTimeString(timeVal)
    elif tdDataType == 17:
        dtVal = value["_dtVal"].integer()
        dateVal = dtVal >> 32
        timeVal = dtVal & 0x00000000FFFFFFFF
        return getDateTimeString(dateVal, timeVal)
    elif tdDataType >= 18 and tdDataType <= 22:
        decVal = value["_decVal"].integer()
        decType = tdDataType - 18
        return getDecimalString(decVal, decType)
    return "NOT FINISHED"
        
def qdump__td__Variant(d, value):
    flags = value["_flags"].integer()
    strTdDataType, tdDataType, dbDataType, dbLen = getVariantDataType(flags)
    if tdDataType == 11:
        lbl = ""
        if dbDataType > 0:
            strDBDataType = getTdDataType(dbDataType)
            lbl = "vt={0}:{1}:{2}".format(strTdDataType, strDBDataType, dbLen)
        else:
            lbl = "vt={0}".format(strTdDataType)
        ptr = value["_strVal"].pointer()
        enc = 0
        lenBytes = 4
        #utf8 is supported
        strFormatted = getTdString(d, ptr, enc, lenBytes)
        d.putValue("vt=string8, {0}".format(strFormatted))
        return
    d.putNumChild(0)
    strVarValue = getVariantValueString(d, tdDataType, value)
    if dbDataType > 0:
        strDBDataType = getTdDataType(dbDataType)
        d.putValue("vt={0}:{1}:{2}, val={3}".format(strTdDataType, strDBDataType, dbLen,  strVarValue))
    else:
        d.putValue("vt={0}, val={1}".format(strTdDataType, strVarValue))
