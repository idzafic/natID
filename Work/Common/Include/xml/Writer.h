// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cnt/StringBuilder.h>
#include <fstream>
#include <fo/FileOperations.h>
#include <cnt/Stack.h>
#include <mu/Utils.h>
#include <mu/EnumSerializerManager.h>
#include <td/BoolCh.h>
#include <mem/Buffer.h>

namespace xml
{	
	class Writer
	{		
		//just for simulation of unexisting font formating
		class DummyFmt{	};

	public:
		typedef enum _DT { DOC_XML = 0, DOC_HTML, DOC_XHTML, DOC_NONE} DocType;

    protected:
		mu::Utils _locConverter;
		mu::Utils* _pConverter;
		mu::EnumSerializerManager* _enumSerializers;
		std::ofstream _f;
		cnt::Stack<td::String, 16> _openNodes;
        cnt::StringBuilder<td::String, 16*1024> _builder;
        mem::Buffer* _pBuffer = nullptr;
        mem::Buffer::Writer _buffWriter;
		DocType _docType = DOC_NONE;
        td::WORD _nodeDeep;
        td::BYTE _showEncoding;
		td::BYTE _ignoreCR = 0;
        td::BYTE _inMemory;
        td::BYTE _autoWhiteSpace;
        td::BYTE _nodeStarted;
        //bool _attribsWritten;
        td::BYTE _nodeString;
	protected:		

		void writeString(const char* str)
		{
			if (_inMemory)
			{
                if (_pBuffer)
                {
                    auto strLen = strlen(str);
                    _buffWriter.append(str, strLen);
                }
                else
                    _builder.appendString(str);
				//_builder.appendString(str);
			}
			else
			{
				_f << str;				
			}			
		}

		inline void writeString(const char* str, int strLen)
		{
			if (strLen > 0)
			{
				if (_inMemory)
				{
                    if (_pBuffer)
                        _buffWriter.append(str, strLen);
                    else
                        _builder.appendString(str, strLen);
				}
				else
				{
					_f.write(str, (std::streamsize) strLen);
				}
			}			
		}

		inline void writeString(size_t strLen, const char* str)
		{
			if (strLen > 0)
			{
				if (_inMemory)
				{
                    if (_pBuffer)
                        _buffWriter.append(str, strLen);
                    else
                        _builder.appendString(strLen, str);
                    
					//_builder.appendString(strLen, str);
				}
				else
				{
					_f.write(str, (std::streamsize) strLen);
				}
			}
		}

		void writeString(td::String& str)
		{
			if (_inMemory)
			{
                if (_pBuffer)
                    _buffWriter.append(str);
                else
                    _builder.appendString(str);
			}
			else
			{
				_f << str.c_str();				
			}			
		}

		void writeNewLineAndTabs()
		{
			if (!_inMemory)
			{			
				_f << '\n';
				for (int i=0; i<_nodeDeep; ++i)
					_f << '\t';
			}
			else
			{
                if (_pBuffer)
                {
                    _buffWriter.append("\n", 1);
                    for (int i=0; i<_nodeDeep; ++i)
                        _buffWriter.append("\t", 1);
                }
                else
                {
                    _builder << "\n";
                    for (int i=0; i<_nodeDeep; ++i)
                        _builder << "\t";
                }
				
			}	
		}

		//just for dummy format (there is no format provided)
		constexpr bool isFmt(const char*, const DummyFmt&, size_t&) const
		{
			return false;
		}

		template <class TFNTFMT>
		bool isFmt(const char* pTxt, const TFNTFMT& fntFmts, size_t& nCharsToSkip) const
		{
			const size_t nFormats = fntFmts.size();
			for (size_t i = 0; i < nFormats; ++i)
			{
				nCharsToSkip = fntFmts[i].fmtLen;
				if (strncmp(pTxt, fntFmts[i].pStrFmt, nCharsToSkip) == 0)
				{
					return true;
				}
			}
			return false;
		}

		template <class TFNTFMT>
		void writeStringWithChecking(const char* str, const TFNTFMT& fntFmts)
		{
			if (str == 0)
				return;
			//_outBuffer.clean();
			const char* pV = str;
			const char* pBuff = pV;
			size_t nChsInBuff = 0;
			while (*pV != '\0')
			{
				if (nChsInBuff == 0)
				{
					pBuff = pV;
				}

				if (*pV == '&')
				{
					writeString(nChsInBuff, pBuff);
					nChsInBuff = 0;
					writeString(5, "&amp;");
					//_outBuffer.append(5, "&amp;");
				}
				else if (*pV == '>')
				{
					writeString(nChsInBuff, pBuff);
					nChsInBuff = 0;
					writeString(4, "&gt;");
					//_outBuffer.append(4, "&gt;");
				}
				else if (*pV == '<')
				{
					size_t nToSkip = 0;
					if (isFmt(pV, fntFmts, nToSkip))
					{
						//_outBuffer.append((int)nToSkip, pV);
						pV += nToSkip;
						nChsInBuff += nToSkip;
						continue;
					}
					else
					{
						writeString(nChsInBuff, pBuff);
						nChsInBuff = 0;
						writeString(4, "&lt;");
						//_outBuffer.append(4, "&lt;");
					}

				}
				else if (*pV == '"')
				{
					writeString(nChsInBuff, pBuff);
					nChsInBuff = 0;
					writeString(6, "&quot;");
					//_outBuffer.append(6, "&quot;");
				}
				else if (*pV == '\'')
				{
					writeString(nChsInBuff, pBuff);
					nChsInBuff = 0;
					writeString(6, "&apos;");
					//_outBuffer.append(6, "&apos;");
				}
				else
				{
					if (_ignoreCR && (*pV == '\r'))
					{
						writeString(nChsInBuff, pBuff);
						nChsInBuff = 0;
						++pV;
						continue;
					}
					++nChsInBuff;
					//_outBuffer += *pV;
				}

				++pV;
			}
			writeString(nChsInBuff, pBuff);
			//writeString(_outBuffer.c_str());
		}

		void writeStringWithChecking(const char* str)
		{
			DummyFmt dummyFmt;
			writeStringWithChecking(str, dummyFmt);
		}

	public:
		explicit Writer(bool autoWhiteSpace = false, td::BYTE showEncoding = 1)
			: _inMemory(1)
			, _showEncoding(showEncoding)
			, _autoWhiteSpace(autoWhiteSpace ? 1 : 0)
			, _nodeDeep(0)
			, _nodeStarted(0)
			//, _attribsWritten(0)
			, _nodeString(0)
			, _pConverter(&_locConverter)
			//, _exernConverter(false)
			, _enumSerializers(nullptr)
            , _buffWriter(nullptr)
			
		{	
		}
        
        Writer(mem::Buffer* pBuffer, bool autoWhiteSpace = false, td::BYTE showEncoding = 1)
            : _inMemory(1)
            , _showEncoding(showEncoding)
            , _autoWhiteSpace(autoWhiteSpace ? 1 : 0)
            , _nodeDeep(0)
            , _nodeStarted(0)
            //, _attribsWritten(0)
            , _nodeString(0)
            , _pConverter(&_locConverter)
            //, _exernConverter(false)
            , _enumSerializers(nullptr)
            , _pBuffer(pBuffer)
            , _buffWriter(pBuffer)
        {
        }
        
        Writer(mem::Buffer::Writer& buffWriter, bool autoWhiteSpace = false, td::BYTE showEncoding = 1)
            : _inMemory(1)
            , _showEncoding(showEncoding)
            , _autoWhiteSpace(autoWhiteSpace ? 1 : 0)
            , _nodeDeep(0)
            , _nodeStarted(0)
            //, _attribsWritten(0)
            , _nodeString(0)
            , _pConverter(&_locConverter)
            //, _exernConverter(false)
            , _enumSerializers(nullptr)
            , _pBuffer(buffWriter.getBuffer())
            , _buffWriter(buffWriter)
        {
        }

		Writer(const td::String& fileName, mu::Utils* pConverter, bool autoWhiteSpace = true, td::BYTE showEncoding = 2)
			: _inMemory(0)
			, _showEncoding(showEncoding)
			, _autoWhiteSpace(autoWhiteSpace ? 1 : 0)
			, _nodeDeep(0)
			, _nodeStarted(0)
			//, _attribsWritten(0)
			, _nodeString(0)
			, _pConverter(pConverter)			
			//, _exernConverter(true)
			, _enumSerializers(nullptr)
            , _buffWriter(nullptr)
		{	
			if (pConverter == nullptr)
			{
				_pConverter = &_locConverter;
				//_exernConverter = false;
			}			

			fo::openFile(_f, fileName);

			//unsigned char ch[3] = { (unsigned char)0xEF, (unsigned char)0xBB, (unsigned char)0xBF };

			//fo::write(_f, (const char*) ch, 3);
		}

		Writer(const td::String& fileName, bool autoWhiteSpace = true, td::BYTE showEncoding = 2)
			: _inMemory(0)
			, _showEncoding(showEncoding)
			, _autoWhiteSpace(autoWhiteSpace ? 1 : 0)
			, _nodeDeep(0)
			, _nodeStarted(0)
			//, _attribsWritten(0)
			, _nodeString(0)
			, _pConverter(&_locConverter)			
			, _enumSerializers(nullptr)
            , _buffWriter(nullptr)
		{		
			fo::openFile(_f, fileName);
		}

		Writer(const char* fileName, mu::Utils* pConverter, bool autoWhiteSpace = true, td::BYTE showEncoding = 2)
			: _inMemory(0)
			, _showEncoding(showEncoding)
			, _autoWhiteSpace(autoWhiteSpace ? 1 : 0)
			, _nodeDeep(0)
			, _nodeStarted(0)
			//, _attribsWritten(0)
			, _nodeString(0)
			, _pConverter(pConverter)
			//, _exernConverter(true)
			, _enumSerializers(nullptr)
			, _buffWriter(nullptr)
		{
			if (pConverter == nullptr)
			{
				_pConverter = &_locConverter;
				//_exernConverter = false;
			}

			fo::openFile(_f, fileName);

			//unsigned char ch[3] = { (unsigned char)0xEF, (unsigned char)0xBB, (unsigned char)0xBF };

			//fo::write(_f, (const char*) ch, 3);
		}

		Writer(const char* fileName, bool autoWhiteSpace = true, td::BYTE showEncoding = 2)
			: _inMemory(0)
			, _showEncoding(showEncoding)
			, _autoWhiteSpace(autoWhiteSpace ? 1 : 0)
			, _nodeDeep(0)
			, _nodeStarted(0)
			//, _attribsWritten(0)
			, _nodeString(0)
			, _pConverter(&_locConverter)
			, _enumSerializers(nullptr)
			, _buffWriter(nullptr)
		{
			fo::openFile(_f, fileName);
		}

		//Writer()
		//	: _inMemory(false)
		//	, _showEncoding(true)
		//	, _autoWhiteSpace(true)
		//	, _nodeDeep(0)
		//	, _nodeStarted(false)
		//	, _attribsWritten(false)
		//	, _nodeString(false)
		//	, _pConverter(&_locConverter)
		//	, enumSerializers(0)
		//{
		//}

		~Writer()
		{				
			close();
		}

        void ignoreCR(bool ignore)
        {
            _ignoreCR = (ignore ? 1 : 0);
        }
        
		bool open(const td::String& fileName, mu::Utils* pConverter = nullptr, bool autoWhiteSpace = true, td::BYTE showEncoding = 2)
		{
			_inMemory = 0;
			_showEncoding = showEncoding;
			_autoWhiteSpace = (autoWhiteSpace ? 1 : 0);
			
			if (pConverter)
				_pConverter = pConverter;
			else
				_pConverter = &_locConverter;

			return fo::openFile(_f, fileName);
		}

		bool open(const char* fileName, mu::Utils* pConverter = nullptr, bool autoWhiteSpace = true, td::BYTE showEncoding = 2)
		{
			_inMemory = 0;
			_showEncoding = showEncoding;
			_autoWhiteSpace = (autoWhiteSpace ? 1 : 0);

			if (pConverter)
				_pConverter = pConverter;
			else
				_pConverter = &_locConverter;

			return fo::openFile(_f, fileName);
		}

		void open(bool autoWhiteSpace = false, td::BYTE showEncoding = 1, mu::Utils* pConverter = nullptr)
		{
			_inMemory = 1;
			_showEncoding = showEncoding;
            _autoWhiteSpace = (autoWhiteSpace ? 1 : 0);
			if (pConverter)
				_pConverter = pConverter;
			else
				_pConverter = &_locConverter;
		}

		void reset()
		{
			_nodeDeep = 0;
			_nodeStarted = 0;
			//_attribsWritten = 0;
			_nodeString = 0;
			_openNodes.reset();
			_builder.reset();
		}


		//void setConverter(Utils* pConverter)
		//{
		//	_pConverter = pConverter;
		//}

		bool isOk() const
		{
			if (_inMemory)
				return true;

			return _f.is_open();
		}

		void setEnumSerializerManager(mu::EnumSerializerManager* pEnumSerialMgr)
		{
			_enumSerializers = pEnumSerialMgr;
		}
	protected:
		template <size_t size>
		inline void putCStr(const char(&str)[size])
		{
			if (_inMemory)
            {
                if (_pBuffer)
                {
                    if (size > 0)
                        _buffWriter.append(&str[0], size-1);
                }
                else
                    _builder.appendCString(str);
            }
			else
				_f << str;
		}
	public:
		void startDocument()
		{			
			_docType = DOC_XML;
			if (_showEncoding != 0)
			{
				if (_showEncoding == 1)									
					putCStr("<?xml version=\"1.0\"?>");				
				else
					putCStr("<?xml version=\"1.0\" encoding=\"utf-8\"?>");				
			}			
		}
        
        void startPlistDocument()
        {
            _showEncoding = 2;
            startDocument();
            //<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
            putCStr("\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">");
        }

		void startHTMLDocument(DocType docType = DOC_HTML)
		{
			_docType = docType;
			if (_showEncoding != 0)
			{
				if (docType == DOC_HTML)
					putCStr("<!DOCTYPE html>");
				else if (docType == DOC_XHTML)
					putCStr("!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\"");
				else
				{
					assert(false);
				}
			}
		}

		void startElement(const char* nodeName)
		{
			if (_nodeStarted)
			{
				putCStr(">");
			}

			if (!_nodeString && _autoWhiteSpace)
			{				
				writeNewLineAndTabs();
				++_nodeDeep;
			}
			else
				++_nodeDeep;
			
			_nodeString = 0;

			putCStr("<");
			writeString(nodeName);
			_nodeStarted = 1;
			td::String str(nodeName);
			_openNodes.push(str);
		}

        void startNode(const td::String& nodeName)
        {
            startElement(nodeName.c_str());
        }
        
        void startNode(const char* nodeName)
        {
            startElement(nodeName);
        }
        
		void startElementNS(const char* pStrNameSpace, const char* nodeName)
		{
			if (_nodeStarted)
			{
				putCStr(">");
			}

			if (!_nodeString && _autoWhiteSpace)
			{
				writeNewLineAndTabs();
				++_nodeDeep;
			}
			else
				++_nodeDeep;

			_nodeString = 0;

			putCStr("<");
			writeString(pStrNameSpace);
			putCStr(":");
			writeString(nodeName);
			_nodeStarted = 1;
			td::String str;
			str.format("%s:%s", pStrNameSpace, nodeName);
			_openNodes.push(str);
		}


		void startMetaElement()
		{
			if (_nodeStarted)
			{
				putCStr(">");
			}

			if (!_nodeString && _autoWhiteSpace)
			{
				writeNewLineAndTabs();			
			}		
			

			_nodeString = 0;

			putCStr("<");
			putCStr("meta");
			_nodeStarted = 1;
			if (_docType != DOC_HTML)
			{
				td::String str("meta");
				++_nodeDeep;
				_openNodes.push(str);
			}
		}

		template <typename ATTTYPE>
		void attribute(ATTTYPE& val)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");
			assert(_pConverter != nullptr);
			writeString(" ");
			writeString(_pConverter->c_str(val));
		}

		//void attribute(const char* attribName, const td::String& val)
		//{
		//	if (!_nodeStarted)
		//		throw td::String("Cannot add attrib! There is no node started!");
		//	assert(_pConverter != 0);
		//	writeString(" ");
		//	writeString(attribName);
		//	writeString("=\"");			
		//	writeString(val.c_str());
		//	writeString("\"");
		//}

        void attribute(const td::StringExt& attribName, const td::StringExt& val, bool checkValue = false)
        {
            attributeString(attribName.c_str(), val, checkValue);
        }
        
		void attribute(const char* attribName, const td::BoolCh& val)
		{
			attribute(attribName, val());
		}

		template <typename ATTTYPE>
		void attribute(const char* attribName, const ATTTYPE& val)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");

			
			putCStr(" ");
			writeString(attribName);
			putCStr("=\"");
			assert(_pConverter != nullptr);
			writeStringWithChecking(_pConverter->c_str(val));
			putCStr("\"");
		}
		
		void attribute(const char* attribName, const td::Variant& val, bool writeTD_NONE = true)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");

			putCStr(" ");
			writeString(attribName);
			putCStr("=\"");
			assert(_pConverter != nullptr);
			td::DataType dt = val.getType();
			if (dt == td::TD_NONE)
			{ 
				if (writeTD_NONE)
					writeString(_pConverter->c_str(val));
			}
			else if (dt == td::string8)
			{
				writeStringWithChecking(_pConverter->c_str(val));
			}	
			else
				writeString(_pConverter->c_str(val));
			
			putCStr("\"");
		}

		template <typename ATTTYPE>
		void attributeC(const char* attribName, ATTTYPE val)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");
			
			putCStr(" ");
			writeString(attribName);
			putCStr("=\"");
			assert(_pConverter != 0);
			writeString(_pConverter->c_str(val));
			putCStr("\"");
		}
		
		void attribute(const char* attribName, const td::String& attribValue)
		{
			attributeString(attribName, attribValue.c_str(), true);
		}

		void attribute(const char* attribName, bool val)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");
			assert(_pConverter != 0);
			putCStr(" ");
			writeString(attribName);
			putCStr("=\"");
			writeString(_pConverter->c_str(val));
			putCStr("\"");
		}
        
        void attribute(const char* attribName, char chVal)
        {
            if (!_nodeStarted)
                throw td::String("Cannot add attrib! There is no node started!");
            assert(_pConverter != 0);
            putCStr(" ");
            writeString(attribName);
            putCStr("=\"");
            if (chVal >= ' ')
            {
                char tmp[2];
                tmp[0] = chVal;
                tmp[1]=0;
                writeStringWithChecking(tmp);
            }
            putCStr("\"");
        }
        
		void attributeBool(const char* attribName, bool val)
		{
			attribute(attribName, val);
		}

		template <typename TENUM>
		void attributeEnum(int enumPos, const char* attribName, TENUM val)
		{
			assert(_enumSerializers != 0);
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");			
			putCStr(" ");
			writeString(attribName);
			putCStr("=\"");

			mu::EnumSerializer* pEnumSer = _enumSerializers->getEnum<TENUM>(enumPos);

			writeString(pEnumSer->getName(val).c_str());
			putCStr("\"");
		}		

		
		template <bool convertToGDIARGB_FromGLARGB>
		void attributeGDIARGB(const char* attribName, td::UINT4 val)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");
			assert(_pConverter != 0);
			if (convertToGDIARGB_FromGLARGB)
				val = td::GLRGBAToNativeGDI(val);

			putCStr(" ");
			writeString(attribName);
			putCStr("=\"");
			writeString(_pConverter->decToHex(val));
			putCStr("\"");
		}

		mu::Utils* getConverter()
		{
			assert(_pConverter != 0);
			return _pConverter;
		}

		void attributeString(const char* attribName, const char* attribValue, bool checkValue = false)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");
			putCStr(" ");
			writeString(attribName);
			putCStr("=\"");
			if (checkValue)
			{
				writeStringWithChecking(attribValue);
			}
			else
			{
				writeString(attribValue);
			}
			putCStr("\"");
		}

		void attributeStringNS(const char* pNameSpace, const char* attribName, const char* attribValue, bool checkValue = false)
		{
			if (!_nodeStarted)
				throw td::String("Cannot add attrib! There is no node started!");
			putCStr(" ");
			writeString(pNameSpace);
			putCStr(":");
			writeString(attribName);
			putCStr("=\"");
			if (checkValue)
			{
				writeStringWithChecking(attribValue);
			}
			else
			{
				writeString(attribValue);
			}
			putCStr("\"");
		}

		void attributeString(const char* attribName, const td::String& attribValue, bool checkValue = false)
		{
			attributeString(attribName, attribValue.c_str(), checkValue);
		}
        
        void attributeString(const char* attribName, const td::StringExt& attribValue, bool checkValue = false)
        {
            attributeString(attribName, attribValue.c_str(), checkValue);
        }

		void attributeStringNS(const char* pNameSpace, const char* attribName, const td::String& attribValue, bool checkValue = false)
		{
			attributeStringNS(pNameSpace, attribName, attribValue.c_str(), checkValue);
		}

		void nodeString(const char* nodeStr)
		{
			//if (_autoWhiteSpace)
			//	return;

			if (_nodeStarted)
			{
				putCStr(">");
				_nodeStarted = 0;
			}
			//writeString(nodeString);
			writeStringWithChecking(nodeStr);
			_nodeString = 1;
		}
		
		void nodeString(const td::String& nodeString)
		{

			if (_nodeStarted)
			{
				putCStr(">");
				_nodeStarted = 0;
			}
			writeStringWithChecking(nodeString.c_str());
			_nodeString = 1;
		}

		template <class TFNTFMT>
		void nodeString(const char* nodeStr, const TFNTFMT& fntFmts)
		{
			//if (_autoWhiteSpace)
			//	return;

			if (_nodeStarted)
			{
				putCStr(">");
				_nodeStarted = 0;
			}
			//writeString(nodeString);
			writeStringWithChecking(nodeStr, fntFmts);
			_nodeString = 1;
		}

		template <class TFNTFMT>
		void nodeString(const td::String& nodeString, const TFNTFMT& fntFmts)
		{
			if (_nodeStarted)
			{
				putCStr(">");
				_nodeStarted = 0;
			}
			writeStringWithChecking(nodeString.c_str(), fntFmts);
			_nodeString = 1;
		}

		void comment(const char* comment)
		{
            if (_nodeStarted)
            {
                putCStr(">");
                _nodeStarted = 0;
            }
            
			putCStr("<!--");
			writeString(comment);
			putCStr("-->");
		}

		void commentInNewLine(const char* comment)
		{
			if (_nodeStarted)
			{
				putCStr(">");
				_nodeStarted = 0;
			}

			putCStr("\n<!--");
			writeString(comment);
			putCStr("-->");
		}

		void CDATA(const char* cdata)
		{
			putCStr("CDATA[[");
			writeString(cdata);
			putCStr("]]>");
		}

		void endElement()
		{
			if (_nodeStarted)
			{
				putCStr("/>");
				_nodeStarted = 0;
				--_nodeDeep;
				_openNodes.pop();
				return;
			}								
			--_nodeDeep;
			td::String& str = _openNodes.top();
			if (!_nodeString && _autoWhiteSpace)
			{
				writeNewLineAndTabs();					
			}
			putCStr("</");
			writeString(str);
			putCStr(">");
			_openNodes.pop();	
			_nodeString = 0;
		}
        
        void endNode()
        {
            endElement();
        }

		void endMetaElement()
		{			
			if (_nodeStarted)
			{
				if (_docType == DOC_HTML)				
					putCStr(">");				
				else
				{
					putCStr("/>");					
					--_nodeDeep;
					_openNodes.pop();
				}				
				_nodeStarted = 0;
				return;
			}
			assert(false);
		}

		void endDocument()
		{
			while(!_openNodes.isEmpty())
			{
				endElement();
			}
		}

		void flush()
		{
			_f.flush();
		}

		void close()
		{
			endDocument();
			if (!_inMemory)
			{
				if (_f.is_open())
					_f.close();
			}
		}

		void getString(td::String& str) const
		{
			assert(_inMemory);
            if (_inMemory)
            {
                if (_pBuffer)
                    str = _pBuffer->toString();
                else
                    _builder.getString(str);
            }
		}

        td::String toString() const
        {
            assert(_inMemory);
            td::String str;
            if (_inMemory)
            {
                if (_pBuffer)
                    str = _pBuffer->toString();
                else
                    _builder.getString(str);
            }
            return str;
        }
        
		const cnt::StringBuilder<td::String, 16 * 1024>& getBuilder() const
		{
			assert(_inMemory);
			return _builder;
		}

		cnt::StringBuilder<td::String, 16 * 1024>& getBuilder()
		{
			assert(_inMemory);
			return _builder;
		}
        
        template <class XmlNode>
        void addNode(XmlNode& node)
        {
            node.toWriter(*this);
        }
	};
}
