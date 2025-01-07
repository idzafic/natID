import gdb
import re
import struct



def extractUtf8String(ptr, size):
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
        b = ptr.dereference().cast(gdb.lookup_type('char'))
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

def extractUtf16String(ptr, size):
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
        b1 = ptr.dereference().cast(gdb.lookup_type('char'))
        b2 = (ptr+1).dereference().cast(gdb.lookup_type('char'))
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


def getTdStringParams(val):
    match = re.search(r"StringBase<(.+)>", str(val.type.strip_typedefs()))
    if match:
        parameters = match.group(1).split(',')
        params = [param.strip() for param in parameters]
        params[0] = str(gdb.lookup_type(params[0]).strip_typedefs())
        params[2] = int(params[2])
        return params[0], params[1], params[2]


class tdString:
    def __init__(self, buffer, t_char, encoding, sizeBytes):
        self.t_char = t_char
        self.encoding = encoding
        self.sizeBytes = sizeBytes
        self.buffer = buffer
    

    def to_string(self):
        if self.buffer == 0:
            self.size = 0
            self.refs = 0
            self.iCh = 0
            if self.t_char == "char":
                return "s8=nullptr"
            elif self.t_char == "short":
                return "s16=nullptr"
            else:
                return "s32=nullptr"
        else:
            if self.sizeBytes > 0: 
                refAndSize = self.buffer.dereference().cast(gdb.lookup_type('unsigned int')) 
                size = refAndSize & 0x0FFFFFFF
                self.size = size
                refs = refAndSize >> 28 
                self.refs = refs
                ptrStr = self.buffer + self.sizeBytes
                if self.t_char == "char":
                    self.iBytes, self.nSpecialChs, self.iCh, self.strData = extractUtf8String(ptrStr, size)
                    return self.strData
                elif self.t_char == "short":
                    self.iBytes, self.nSpecialChs, self.iCh, self.strData = extractUtf16String(ptrStr, size)
                    return self.strData
                else:
                    return "UNDEF:UTF-32 encoding with enc={} is not supported".format(self.encoding)
            else:
                self.size = 0
                ptrStr = self.buffer
                self.iBytes, self.nSpecialChs, self.iCh, self.strData = extractUtf8String(ptrStr, self.size)
                return self.strData

    def children(self):
        yield "size", self.size
        yield "refs", self.refs
        yield "chars", self.iCh

    def display_hint(self):
        return 'string'



def getStringHelper(val):
    if str(val.type.strip_typedefs()).startswith("td::StringBase<"):
        t_char, encoding, sizeBytes  = getTdStringParams(val)
        return tdString(val['buffer'], t_char, encoding, sizeBytes)
    return None

gdb.pretty_printers.append(getStringHelper)


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

class tdDate:
    def __init__(self, value):
        self.tdDate = int(value["_date"])

    def to_string(self):
        return str(getDateString(self.tdDate))

        
class tdTime:
    def __init__(self, value):
        self.tdTime = int(value["_time"])
    
    def to_string(self):
        return str(getTimeString(self.tdTime))

class tdDateTime:

    def __init__(self, value):
        self.tdDate = int(value["_date"])
        self.tdTime = int(value["_time"])

    def to_string(self):
        return str(getDateTimeString(self.tdDate, self.tdTime))



    
def getDateTimeHelper(val):
    if str(val.type) == "td::Date":
        return tdDate(val)
    if str(val.type) == "td::Time":
        return tdTime(val)
    if str(val.type) == "td::DateTime":
        return tdDateTime(val)
    

gdb.pretty_printers.append(getDateTimeHelper)


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


class tdDecimal:

    def __init__(self, value):
        self.val = int(value["_value"])

        match = re.search(r"Decimal<(.+)>", str(value.type.strip_typedefs()))
        if match:
            parameters = match.group(1).split(',')
            self.params = parameters = [param.strip() for param in parameters]
            self.params[0] = str(gdb.lookup_type(self.params[0]).strip_typedefs())
            self.params[1] = int(self.params[1])

    def to_string(self):
        return str(getDecimalString(self.val, self.params[1]))
        
            

def getDecimalHelper(val):
    if str(val.type.strip_typedefs()).startswith("td::Decimal<"):
        return tdDecimal(val)
    

gdb.pretty_printers.append(getDecimalHelper)

def getTdDataType(tdDataType):
    dtStr = ['TD_NONE', 'boolean', 'byte', 'word', 'int2', 'uint4', 'int4', 'lint8', 'luint8', 'real4', 'real8', 'string8', 'string16', 'string32', 'enumerator', 'date', 'time', 'dateTime', 'decimal0', 'decimal1', 'decimal2', 'decimal3', 'decimal4', 'ch', 'nch', 'chFix', 'nchFix', 'ch7', 'dbDate', 'dbTime', 'dbDateTime', 'dbDecimal', 'binary', 'guid', 'color', 'smallDecimal0', 'smallDecimal1', 'smallDecimal2', 'smallDecimal3', 'smallDecimal4', 'rect', 'point', 'variant', 'ptrBoolean', 'ptrByte', 'ptrWord', 'ptrInt2', 'ptrUint4', 'ptrInt4', 'ptrLint8', 'ptrLuint8', 'ptrReal4', 'ptrReal8', 'ptrString8', 'ptrString16', 'ptrString32', 'ptrEnumerator', 'ptrDate', 'ptrTime', 'ptrDateTime', 'ptrDecimal0', 'ptrDecimal1', 'ptrDecimal2', 'ptrDecimal3', 'ptrDecimal4', 'ptrCh', 'ptrNch', 'ptrChFix', 'ptrSmallChFix', 'ptrNchFix', 'ptrDbDate', 'ptrDbTime', 'ptrDbDateTime', 'ptrDbDecimal', 'ptrBinary', 'ptrGuid', 'ptrColor', 'ptrSmalldecimal0', 'ptrSmalldecimal1', 'ptrSmalldecimal2', 'ptrSmalldecimal3', 'ptrSmalldecimal4', 'ptrRect', 'ptrPoint', 'ptrVariant', 'ptrVoid', 'TD_LAST']
    return dtStr[tdDataType]

def getVariantDataType(flags):
    tdDataType = flags & 0x000000FF
    dbDataType = (flags >> 8) & 0x000000FF
    dbLen = flags >> 32
    strTdDataType = getTdDataType(tdDataType)
    return strTdDataType, tdDataType, dbDataType, dbLen

def getVariantValueString(tdDataType, value):
    intVal = int(value["_li8Val"])
    if tdDataType == 0:
        return "UNDEF"
    elif tdDataType == 1:    #boolean
        intVal = int(value["_bVal"])
        if intVal == ord('Y'):
            return "true"
        else:
            return "false"
    elif tdDataType > 1 and tdDataType <= 8:
        strVal ="{0}"
        return strVal.format(intVal)
    elif tdDataType == 9:
        fVal = float(value["_r4Val"])
        strVal="{0:f}"
        return strVal.format(fVal)
    elif tdDataType == 10:
        fVal = float(value["_r8Val"])
        strVal="{0}"
        return strVal.format(fVal)
    elif tdDataType == 11:
        ptr = value["_strVal"]
        return tdString(ptr).to_string()
    elif tdDataType == 15:
        dateVal = int(value["_dateVal"])
        return getDateString(dateVal)
    elif tdDataType == 16:
        timeVal = int(value["_timeVal"])
        return getTimeString(timeVal)
    elif tdDataType == 17:
        dtVal = int(value["_dtVal"])
        dateVal = dtVal >> 32
        timeVal = dtVal & 0x00000000FFFFFFFF
        return getDateTimeString(dateVal, timeVal)
    elif tdDataType >= 18 and tdDataType <= 22:
        decVal = int(value["_decVal"])
        decType = tdDataType - 18
        return getDecimalString(decVal, decType)
    return "NOT FINISHED"


class VariantType:

    def __init__(self, TdType, DbType = None, DbLen = None):
        self.TdType = TdType
        self.DbType = DbType
        self.DbLen = DbLen

    def to_string(self):
        return str(self.TdType)
    
    def children(self):
        
        yield "DB_Type", str("self.DbType")
        yield "DB_length", "dklk"

    def display_hint(self):
        return 'string'


def make_char_pointer(string):
    original_bytes = string.encode("UTF-8")
    original = gdb.Value(original_bytes + b"\0", gdb.lookup_type("char").array(len(original_bytes)))
    adjusted = original.cast(gdb.lookup_type("char").pointer())
    return adjusted

class tdVariant:

    def __init__(self, value):
        self.returnStr = "error"
        flags = int(value["_flags"])
        strTdDataType, tdDataType, dbDataType, dbLen = getVariantDataType(flags)
        if tdDataType == 11:
            if dbDataType > 0:
                strDBDataType = getTdDataType(dbDataType)
                self.tip = VariantType(strTdDataType, strDBDataType, dbLen)
            else:
                self.tip = VariantType(strTdDataType)
            ptr = value["_strVal"]
            lenBytes = 4
            #utf8 is supported
            self.string = tdString(ptr, "char", "UTF8", lenBytes)
            self.returnStr = self.string.to_string()
            return None
        
        
        strVarValue = getVariantValueString(tdDataType, value)
        if dbDataType > 0:
            strDBDataType = getTdDataType(dbDataType)
            self.tip = VariantType(strTdDataType, strDBDataType, dbLen)
            self.returnStr =   str(strVarValue)
        else:
            self.tip = VariantType(strTdDataType)
            self.returnStr = str(strVarValue)
        

    def to_string(self):
        return self.returnStr
    
    def children(self):
        yield "TD_type", make_char_pointer(self.tip.TdType)
        if self.tip.DbType is not None:
            yield "DB_type", make_char_pointer(self.tip.DbType)
        if self.tip.DbLen is not None:
            yield "DB_len", self.tip.DbLen



    def display_hint(self):
        return 'string'
    


def getVariantHelper(val):
    if str(val.type.strip_typedefs()) == "td::Variant":
        return tdVariant(val)
    

gdb.pretty_printers.append(getVariantHelper)



class dumby:
    def __init__(self, val):
        self.str = str(val.type.strip_typedefs())

    def to_string(self):
        return self.str