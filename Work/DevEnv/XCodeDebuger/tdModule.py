import lldb
import lldb.formatters.Logger
        
#Napomena: koristiti print kao debug
#String part
def getString8(ptrBuffer):
    ptrVal = ptrBuffer.GetValueAsSigned(0)
    if ptrVal == 0:
        return "null"
    dataLen = ptrBuffer.GetPointeeData(0,4)
    error = lldb.SBError()
    refAndSize = dataLen.GetUnsignedInt32(error, 0)
    if refAndSize == 0:
        return "null_2"
    size = refAndSize & 0x0FFFFFFF
    refs = refAndSize >> 28
    if size == 0:
        "len={0}:{1},s8=''".format(size, refs)
    data = ptrBuffer.GetPointeeData(4, size)
    strVal = data.GetString(error,0)
    if error.Fail():
        error2 = lldb.SBError()
        rawData = data.ReadRawData(error2, 0, size)
        strVal = rawData.decode('utf8')
        if error2.Fail():
            return "len={0}:{1}:,<Error:td::String nije dobro kreiran>".format(size, refs)
    strLen = len(strVal)
    return "len={0}:{2}:{1},s8='{3}'".format(size, refs, strLen, strVal)
    
def getString8Summary(valobj,dict):
    ptrBuffer = valobj.GetChildAtIndex(0)
    return getString8(ptrBuffer)
    
def getString8SummaryExt(valobj,dict):
    ptrBuffer = valobj.GetChildAtIndex(0)
    return getString8(ptrBuffer)

def getString8SummaryOK(valobj,dict):
    ptrVal = valobj.GetChildAtIndex(0).GetValueAsUnsigned(0)
    if ptrVal == 0:
        return "null"
    ptrBuffer = valobj.GetChildAtIndex(0)
    dataLen = ptrBuffer.GetPointeeData(0,4)
    error = lldb.SBError()
    refAndSize = dataLen.GetUnsignedInt32(error, 0)
    if refAndSize == 0:
        return "null_2"
    size = refAndSize & 0x0FFFFFFF
    refs = refAndSize >> 28
    if size == 0:
        "len={0}:{1},s8=''".format(size, refs)
    data = ptrBuffer.GetPointeeData(4, size)
    strVal = data.GetString(error,0)
    if error.Fail():
        error2 = lldb.SBError()
        rawData = data.ReadRawData(error2, 0, size)
        strVal = rawData.decode('utf8')
        if error2.Fail():
            return "len={0}:{1}:,<Error:td::String nije dobro kreiran>".format(size, refs)
    strLen = len(strVal)
    return "len={0}:{2}:{1},s8='{3}'".format(size, refs, strLen, strVal)
    
def getString16Summary(valobj,dict):
    ptrVal = valobj.GetChildAtIndex(0).GetValueAsUnsigned(0)
    if ptrVal == 0:
        return "null"
    ptrBuffer = valobj.GetChildAtIndex(0)
    dataLen = ptrBuffer.GetPointeeData(0,4)
    error = lldb.SBError()
    refAndSize = dataLen.GetUnsignedInt32(error, 0)
    if refAndSize == 0:
        return "null"
    size = refAndSize & 0x0FFFFFFF
    refs = refAndSize >> 28
    if size == 0:
        "len={0}:{1},str=''".format(size, refs)
    data = ptrBuffer.GetPointeeData(4, size)
    rawData = data.ReadRawData(error, 0, size)
    strVal = rawData.decode('utf16')
    if error.Fail():
        return "len={0}:{1}:,<Error:td::String nije dobro kreiran>".format(size, refs)
    strLen = len(strVal)
    return "len={0}:{2}:{1},s8='{3}'".format(size, refs, strLen, strVal)

#Date, Time and DateTime part
def getDateString(tdDate):
    tdDay = 0x0000001F & tdDate
    tdMonth = ((0x000001E0 & tdDate) >> 5)
    tdYear = ((0x3FFFFE00 & tdDate) >> 9)
    strVal = "{0:04d}-{1:02d}-{2:02d}".format(tdYear, tdMonth, tdDay)
    return strVal;
    
def getTimeString(tdTime):
    h = tdTime // (60 * 60 * 10000)
    m = tdTime % (60 * 60 * 10000)
    m = m // (60 * 10000)
    s = tdTime % (60 * 10000)
    s = s // 10000
    fr = tdTime % 10000
    strVal = "{0:02d}:{1:02d}:{2:02d}.{3:04d}".format(h, m, s, fr)
    return strVal;
    
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
    return strVal;
    
def getDateSummary(valobj,dict):
    tdDate = valobj.GetChildAtIndex(0).GetValueAsUnsigned(0)
    return getDateString(tdDate)
    
def getTimeSummary(valobj,dict):
    tdTime = valobj.GetChildAtIndex(0).GetValueAsUnsigned(0)
    return getTimeString(tdTime)

def getDateTimeSummary(valobj,dict):
    tdDate = valobj.GetChildMemberWithName("_date").GetValueAsUnsigned(0)
    tdTime = valobj.GetChildMemberWithName("_time").GetValueAsUnsigned(0)
    return getDateTimeString(tdDate, tdTime)
    
#Decimal part
def extractDecimal(val, s):
    absVal = abs(val)
    intPart = absVal // s
    decPart = absVal % s
    if val < 0:
        intPart = -intPart
    return intPart, decPart;

def getDecimalString(val, dType):
    strVal = ""
    if dType == 0:
        strVal = "{0}"
        strVal = strVal.format(val);
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
    return strVal;
        
    
def putAsDecimal(d, val, dType):
    d.putNumChild(0)
    d.putValue(getDecimalString(val, dType))
    
def getDecimal0Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 0)
    
def getDecimal1Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 1)
    
def getDecimal2Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 2)
    
def getDecimal3Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 3)

def getDecimal4Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 4)
    
#SmallDecimal part
def getSDecimal0Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 0)
    
def getSDecimal1Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 1)
    
def getSDecimal2Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 2)
    
def getSDecimal3Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 3)

def getSDecimal4Summary(valobj,dict):
    val = valobj.GetChildAtIndex(0).GetValueAsSigned(0)
    return getDecimalString(val, 4)
   
#Variant type
def getTdDataType(tdDataType):
    dtStr = ['TD_NONE', 'boolean', 'byte', 'word', 'int2', 'uint4', 'int4', 'lint8', 'luint8', 'real4', 'real8', 'string8', 'string16', 'string32', 'enumerator', 'date', 'time', 'dateTime', 'decimal0', 'decimal1', 'decimal2', 'decimal3', 'decimal4', 'ch', 'nch', 'chFix', 'nchFix', 'ch7', 'dbDate', 'dbTime', 'dbDateTime', 'dbDecimal', 'binary', 'guid', 'color', 'colorID', 'linePattern', 'smallDecimal0', 'smallDecimal1', 'smallDecimal2', 'smallDecimal3', 'smallDecimal4', 'rect', 'point', 'variant', 'ptrBoolean', 'ptrByte', 'ptrWord', 'ptrInt2', 'ptrUint4', 'ptrInt4', 'ptrLint8', 'ptrLuint8', 'ptrReal4', 'ptrReal8', 'ptrString8', 'ptrString16', 'ptrString32', 'ptrEnumerator', 'ptrDate', 'ptrTime', 'ptrDateTime', 'ptrDecimal0', 'ptrDecimal1', 'ptrDecimal2', 'ptrDecimal3', 'ptrDecimal4', 'ptrCh', 'ptrNch', 'ptrChFix', 'ptrSmallChFix', 'ptrNchFix', 'ptrDbDate', 'ptrDbTime', 'ptrDbDateTime', 'ptrDbDecimal', 'ptrBinary', 'ptrGuid', 'ptrColor', 'ptrSmalldecimal0', 'ptrSmalldecimal1', 'ptrSmalldecimal2', 'ptrSmalldecimal3', 'ptrSmalldecimal4', 'ptrRect', 'ptrPoint', 'ptrVariant', 'ptrVoid', 'TD_LAST']
    lenOfTypes = len(dtStr);
    if lenOfTypes <= tdDataType:
        return 'getTdDataType_OutOfRangeFor:{0}'.format(tdDataType)
    return dtStr[tdDataType]
    
def getVariantName(tdDataType):
    dtStr = ['TD_NONE', '_bVal', '_byteVal', '_wordVal', '_i2Val', '_u4Val', '_i4Val', '_lu8Val', '_li8Val', '_r4Val', '_r8Val']
    lenOfNames = len(dtStr);
    if lenOfNames <= tdDataType:
        return 'getVariantName_OutOfRangeFor:{0}'.format(tdDataType)
    return dtStr[tdDataType]
    
def getVariantDataType(flags):
    tdDataType = flags & 0x000000FF
    dbDataType = (flags >> 8) & 0x000000FF
    dbLen = flags >> 32
    strTdDataType = getTdDataType(tdDataType)
    return strTdDataType, tdDataType, dbDataType, dbLen;

def getVariantString(valobj, tdDataType):
    pVarBase = valobj.GetChildAtIndex(0)
    pUnion = pVarBase.GetChildAtIndex(0);
    
    if tdDataType == 0:
        return "UNDEF";
    elif tdDataType == 1:    #boolean
        intVal = pUnion.GetChildMemberWithName("_bVal").GetValueAsSigned(0)
        if intVal == ord('Y'):
            return "bVal=true";
        else:
            return "bVal=false";
    elif tdDataType > 1 and tdDataType <= 8:
        if tdDataType % 2 == 1:
            intVal = pUnion.GetChildMemberWithName(getVariantName(tdDataType)).GetValueAsSigned(0)
        else:
            intVal = pUnion.GetChildMemberWithName(getVariantName(tdDataType)).GetValueAsUnsigned(0)
        return "{0}".format(intVal);
    elif tdDataType == 9:
        dataVal = pUnion.GetChildMemberWithName(getVariantName(tdDataType)).GetData()
        error = lldb.SBError()
        floatVal = dataVal.GetFloat(error, 0);
        return "{0:f}".format(floatVal)
    elif tdDataType == 10:
        dataVal = pUnion.GetChildMemberWithName(getVariantName(tdDataType)).GetData()
        error = lldb.SBError()
        dblVal = dataVal.GetDouble(error, 0);
        return "{0}".format(dblVal)
    elif tdDataType == 11:
        ptrBuffer = pUnion.GetChildMemberWithName("_strVal")
        return getString8(ptrBuffer)
    elif tdDataType == 15:
        dateVal = pUnion.GetChildMemberWithName("_dateVal").GetValueAsUnsigned(0)
        return getDateString(dateVal)
    elif tdDataType == 16:
        timeVal = pUnion.GetChildMemberWithName("_timeVal").GetValueAsUnsigned(0)
        return getTimeString(timeVal)
    elif tdDataType == 17:
        dtVal = pUnion.GetChildMemberWithName("_dtVal").GetValueAsUnsigned(0)
        dateVal = dtVal >> 32
        timeVal = dtVal & 0x00000000FFFFFFFF
        return getDateTimeString(dateVal, timeVal);
    elif tdDataType >= 18 and tdDataType <= 22:
        decVal = pUnion.GetChildMemberWithName("_decVal").GetValueAsSigned(0)
        decType = tdDataType - 18
        return getDecimalString(decVal, decType)
    elif tdDataType >= 35 and tdDataType <= 39:
        decVal = pUnion.GetChildMemberWithName("_smallDecVal").GetValueAsSigned(0)
        decType = tdDataType - 35
        return getDecimalString(decVal, decType)
    return "NOT FINISHED-tdModule.py(ln275)"

def getVariantSummary(valobj,dict):
    #logger = lldb.formatters.Logger.Logger()
    flags = valobj.GetChildMemberWithName("_flags").GetValueAsSigned(0)
    strTdDataType, tdDataType, dbDataType, dbLen = getVariantDataType(flags)
    #return "dt{0}, dt{1}, db{2}, dblen{3}".format(strTdDataType, tdDataType, dbDataType, dbLen)
    if tdDataType == 11:
        lbl = ""
        if dbDataType > 0:
            strDBDataType = getTdDataType(dbDataType)
            lbl = "vt={0}:{1}:{2}".format(strTdDataType, strDBDataType, dbLen)
        else:
            lbl = "vt={0}".format(strTdDataType)
    else:
        lbl = "vt={0}".format(strTdDataType)
    strVal = getVariantString(valobj, tdDataType)
    return "{0}; {1}".format(lbl, strVal)
    
def getGuiSizeSummary(valobj,dict):
    #logger = lldb.formatters.Logger.Logger()
    width = valobj.GetChildMemberWithName("width").GetValueAsSigned()
    height = valobj.GetChildMemberWithName("height").GetValueAsSigned()
    return "width={0}, height={1}".format(width, height)
        
class SafeFullVectorChildrenProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.update()
        #koristiti print kao debug
        #print('init SafeFullVectorChildrenProvider')
        # Initialize the Python object using valobj as the variable to provide synthetic children.
    def num_children(self):
        return self.size
        # Return the number of children you want your object to have.
    def get_child_index(self,name):
        try:
            return int(name.lstrip('[').rstrip(']'))
        except:
            return -1;
        # Return the index of the synthetic child whose name is given as argument.
    def get_child_at_index(self,index):
        if index < 0 or index >= self.num_children():
            return None;
        offset = index * self.type_size
        return self.data.CreateChildAtOffset('['+str(index)+']', offset, self.value_type)
        # Return a new LLDB SBValue object representing the child at the index given as argument.
    #def update(self):
       
    def has_children(self):
        return True
        # Return True if this object might have children and False if this object can be guaranteed to not have children.
    #def get_value(self):
        # Return an SBValue to be presented as the value of the synthetic value under consideration.
        
         # Update the internal state of this Python object whenever the state of the variables in LLDB changes.
    def update(self):
        self.data = self.valobj.GetChildMemberWithName('_data')
        self.size = self.valobj.GetChildMemberWithName("_size").GetValueAsUnsigned(0)
        #self.type_size = self.data_type.GetByteSize()
        the_type = self.valobj.GetType()
        if the_type.IsReferenceType():
            the_type = the_type.GetDereferencedType()
        self.value_type = the_type.GetTemplateArgumentType(0)
        self.type_size = self.value_type.GetByteSize()
        
def __lldb_init_module(debugger,dict):
    debugger.HandleCommand('type summary add -F tdModule.getString8Summary "td::String"')
    debugger.HandleCommand('type summary add -F tdModule.getString8SummaryExt "td::StringExt"')
    debugger.HandleCommand('type summary add -F tdModule.getString16Summary "td::StringUTF16"')
    debugger.HandleCommand('type summary add -F tdModule.getDateSummary "td::Date"')
    debugger.HandleCommand('type summary add -F tdModule.getTimeSummary "td::Time"')
    debugger.HandleCommand('type summary add -F tdModule.getDateTimeSummary "td::DateTime"')
    debugger.HandleCommand('type summary add -F tdModule.getDecimal0Summary "td::Decimal0"')
    debugger.HandleCommand('type summary add -F tdModule.getDecimal1Summary "td::Decimal1"')
    debugger.HandleCommand('type summary add -F tdModule.getDecimal2Summary "td::Decimal2"')
    debugger.HandleCommand('type summary add -F tdModule.getDecimal3Summary "td::Decimal3"')
    debugger.HandleCommand('type summary add -F tdModule.getDecimal4Summary "td::Decimal4"')
    debugger.HandleCommand('type summary add -F tdModule.getSDecimal0Summary "td::SmallDecimal0"')
    debugger.HandleCommand('type summary add -F tdModule.getSDecimal1Summary "td::SmallDecimal1"')
    debugger.HandleCommand('type summary add -F tdModule.getSDecimal2Summary "td::SmallDecimal2"')
    debugger.HandleCommand('type summary add -F tdModule.getSDecimal3Summary "td::SmallDecimal3"')
    debugger.HandleCommand('type summary add -F tdModule.getSDecimal4Summary "td::SmallDecimal4"')
    debugger.HandleCommand('type summary add -F tdModule.getVariantSummary "td::Variant"')
    debugger.HandleCommand('type summary add -F tdModule.getGuiSizeSummary "gui::Size"')
    #debugger.HandleCommand('type summary add -l tdModule.getSafeFullVectorSummary -x "^cnt::SafeFullVector<.+,.+>$"')
    debugger.HandleCommand('type synthetic add -l tdModule.SafeFullVectorChildrenProvider -x "^cnt::SafeFullVector<.+,.+>$"')
    debugger.HandleCommand('type summary add --expand -x "^cnt::SafeFullVector<.+,.+>$" --summary-string "${svar%#} items')
 #   debugger.HandleCommand('type synthetic add -w oliora -l extra_formatters.LlvmSmallVectorSynthProvider -x "^llvm::SmallVector<.+,.+>$"')
