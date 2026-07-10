// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Writer.h
    @brief XML/HTML document writer that serializes elements, attributes, and text to file or in-memory buffer. */
#pragma once
#include <cnt/StringBuilder.h>
#include <fstream>
#include <fo/FileOperations.h>
#include <cnt/Stack.h>
#include <mu/Utils.h>
#include <mu/EnumSerializerManager.h>
#include <td/BoolCh.h>
#include <mem/Buffer.h>
#include <td/Concepts.h>

namespace xml
{
	/// @brief Serializes XML, HTML, or XHTML documents to a file or an in-memory string builder.
	class Writer
	{
		//just for simulation of unexisting font formating
		/// @brief Placeholder type used when no font-format list is provided to writeStringWithChecking.
		class DummyFmt{	};

	public:
		/// @brief Supported document type declarations.
		typedef enum _DT {
			DOC_XML = 0, ///< Standard XML document
			DOC_HTML,    ///< HTML5 document
			DOC_XHTML,   ///< XHTML 1.0 Strict document
			DOC_NONE     ///< No document type declared
		} DocType;

    protected:
		mu::Utils _locConverter;                              ///< Default local converter instance when no external converter is supplied.
		mu::Utils* _pConverter;                               ///< Pointer to the active value-to-string converter.
		mu::EnumSerializerManager* _enumSerializers;          ///< Optional manager for serializing enum values by name.
		std::ofstream _f;                                     ///< Output file stream used when writing to disk.
		cnt::Stack<td::String, 16> _openNodes;                ///< Stack of open element names for proper closing.
        cnt::StringBuilder<td::String, 16*1024> _builder;    ///< In-memory string builder accumulating the document.
        mem::Buffer* _pBuffer = nullptr;                      ///< Optional external memory buffer for output.
        mem::Buffer::Writer _buffWriter;                      ///< Writer facade over the external memory buffer.
		DocType _docType = DOC_NONE;                          ///< Current document type set at startDocument time.
        td::WORD _nodeDeep;                                   ///< Current nesting depth used for indentation.
        td::BYTE _showEncoding;                               ///< Controls XML declaration emission: 0=none, 1=no encoding, 2=with encoding.
		td::BYTE _ignoreCR = 0;                               ///< When non-zero, carriage-return characters are stripped from text content.
        td::BYTE _inMemory;                                   ///< Non-zero when writing to an in-memory buffer instead of a file.
        td::BYTE _autoWhiteSpace;                             ///< Non-zero when newlines and indentation tabs are inserted automatically.
        td::BYTE _nodeStarted;                                ///< Non-zero while the opening tag of the current element has not yet been closed.
        //bool _attribsWritten;
        td::BYTE _nodeString;                                 ///< Non-zero when text content has been written inside the current element.
	protected:

		/// @brief Writes a null-terminated string to the current output destination.
		/// @param str Null-terminated character string to write.
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

		/// @brief Writes a string of known length to the current output destination.
		/// @param str Pointer to the character data to write.
		/// @param strLen Number of characters to write; nothing is written when zero or negative.
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

		/// @brief Writes a string of known length (size_t variant) to the current output destination.
		/// @param strLen Number of characters to write.
		/// @param str Pointer to the character data to write.
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

		/// @brief Writes a td::String to the current output destination.
		/// @param str Reference to the string object whose content is written.
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

		/// @brief Writes a newline character followed by tab characters matching the current nesting depth.
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
		/// @brief Dummy overload used when no font-format list is provided; always returns false.
		/// @param str Unused text pointer.
		/// @param fnt Unused DummyFmt reference.
		/// @param nCharsToSkip Unused skip counter.
		/// @return Always false.
		constexpr bool isFmt(const char*, const DummyFmt&, size_t&) const
		{
			return false;
		}

		/// @brief Checks whether the text at @p pTxt begins with one of the format marker strings.
		/// @tparam TFNTFMT Container type exposing @c size() and indexed access to format entries with @c fmtLen and @c pStrFmt members.
		/// @param pTxt Pointer to the current position in the text being scanned.
		/// @param fntFmts Collection of font-format descriptors to match against.
		/// @param nCharsToSkip Set to the length of the matched format string when a match is found.
		/// @return True if a format marker was found at @p pTxt, false otherwise.
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

		/// @brief Writes a string to the output, escaping XML special characters and optionally preserving allowed markup sequences.
		/// @tparam TFNTFMT Container of font-format descriptors whose markup sequences must pass through unescaped.
		/// @param str Null-terminated source string; nothing is written if null.
		/// @param fntFmts Collection of format markers that are allowed verbatim in the output.
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

		/// @brief Writes a string with XML special-character escaping, using no allowed-markup overrides.
		/// @param str Null-terminated string to escape and write.
		void writeStringWithChecking(const char* str)
		{
			DummyFmt dummyFmt;
			writeStringWithChecking(str, dummyFmt);
		}

	public:
		/// @brief Constructs an in-memory writer using the internal string builder.
		/// @param autoWhiteSpace When true, newlines and indentation are inserted automatically between elements.
		/// @param showEncoding Controls the XML declaration: 1 emits a declaration without encoding attribute, 2 includes the encoding attribute.
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

        /// @brief Constructs an in-memory writer that appends output to an external memory buffer.
        /// @param pBuffer Pointer to the target memory buffer; may be null.
        /// @param autoWhiteSpace When true, newlines and indentation are inserted automatically.
        /// @param showEncoding Controls the XML declaration emission level.
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

        /// @brief Constructs an in-memory writer sharing an existing buffer writer.
        /// @param buffWriter Existing buffer writer whose underlying buffer is reused.
        /// @param autoWhiteSpace When true, newlines and indentation are inserted automatically.
        /// @param showEncoding Controls the XML declaration emission level.
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

		/// @brief Constructs a file-based writer using a td::String path and an optional external converter.
		/// @param fileName Path to the output file.
		/// @param pConverter Pointer to an external converter; when null, the local converter is used.
		/// @param autoWhiteSpace When true, newlines and indentation are inserted automatically.
		/// @param showEncoding Controls the XML declaration emission level.
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

		/// @brief Constructs a file-based writer using a td::String path and the local converter.
		/// @param fileName Path to the output file.
		/// @param autoWhiteSpace When true, newlines and indentation are inserted automatically.
		/// @param showEncoding Controls the XML declaration emission level.
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

		/// @brief Constructs a file-based writer using a C-string path and an optional external converter.
		/// @param fileName Null-terminated path to the output file.
		/// @param pConverter Pointer to an external converter; when null, the local converter is used.
		/// @param autoWhiteSpace When true, newlines and indentation are inserted automatically.
		/// @param showEncoding Controls the XML declaration emission level.
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

		/// @brief Constructs a file-based writer using a C-string path and the local converter.
		/// @param fileName Null-terminated path to the output file.
		/// @param autoWhiteSpace When true, newlines and indentation are inserted automatically.
		/// @param showEncoding Controls the XML declaration emission level.
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

		/// @brief Destructor; closes the document and the underlying file stream if open.
		~Writer()
		{
			close();
		}

        /// @brief Sets whether carriage-return characters should be stripped when writing text content.
        /// @param ignore When true, CR characters are ignored during text output.
        void ignoreCR(bool ignore)
        {
            _ignoreCR = (ignore ? 1 : 0);
        }

		/// @brief Opens the writer for file output using a td::String path.
		/// @param fileName Path to the output file.
		/// @param pConverter Optional converter; falls back to the local converter when null.
		/// @param autoWhiteSpace When true, automatic indentation is enabled.
		/// @param showEncoding Controls the XML declaration emission level.
		/// @return True if the file was opened successfully.
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

		/// @brief Opens the writer for file output using a C-string path.
		/// @param fileName Null-terminated path to the output file.
		/// @param pConverter Optional converter; falls back to the local converter when null.
		/// @param autoWhiteSpace When true, automatic indentation is enabled.
		/// @param showEncoding Controls the XML declaration emission level.
		/// @return True if the file was opened successfully.
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

		/// @brief Switches the writer to in-memory mode backed by the internal string builder.
		/// @param autoWhiteSpace When true, automatic indentation is enabled.
		/// @param showEncoding Controls the XML declaration emission level.
		/// @param pConverter Optional converter; falls back to the local converter when null.
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

		/// @brief Resets the writer state so a new document can be written without reopening.
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

		/// @brief Returns whether the writer is ready to accept output.
		/// @return True when writing to memory or when the underlying file stream is open.
		bool isOk() const
		{
			if (_inMemory)
				return true;

			return _f.is_open();
		}

		/// @brief Sets the enum serializer manager used to convert enum values to their string names.
		/// @param pEnumSerialMgr Pointer to the enum serializer manager; may be null to disable enum serialization.
		void setEnumSerializerManager(mu::EnumSerializerManager* pEnumSerialMgr)
		{
			_enumSerializers = pEnumSerialMgr;
		}
	protected:
		/// @brief Writes a compile-time-sized C-string literal to the current output destination.
		/// @tparam size Compile-time array size of the string literal (includes the null terminator).
		/// @param str Reference to the character array to write.
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
		/// @brief Writes the XML declaration and sets the document type to XML.
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

        /// @brief Writes the XML declaration and the Apple Property List DOCTYPE preamble.
        void startPlistDocument()
        {
            _showEncoding = 2;
            startDocument();
            //<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
            putCStr("\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">");
        }

		/// @brief Writes the HTML or XHTML DOCTYPE declaration and sets the document type accordingly.
		/// @param docType The HTML variant to declare; defaults to DOC_HTML.
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

		/// @brief Opens a new element with the given tag name, handling pending tag closure and indentation.
		/// @param nodeName Null-terminated name of the element to open.
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

        /// @brief Opens a new element using a td::String element name.
        /// @param nodeName Name of the element to open.
        void startNode(const td::String& nodeName)
        {
            startElement(nodeName.c_str());
        }

        /// @brief Opens a new element using a C-string element name.
        /// @param nodeName Null-terminated name of the element to open.
        void startNode(const char* nodeName)
        {
            startElement(nodeName);
        }

		/// @brief Opens a namespace-qualified element (prefix:localName).
		/// @param pStrNameSpace Null-terminated namespace prefix string.
		/// @param nodeName Null-terminated local name of the element.
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


		/// @brief Opens a self-closing HTML meta element, handling both HTML and XHTML modes.
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

		/// @brief Writes a pre-formatted attribute using the converter's string representation of @p val.
		/// @tparam ATTTYPE Type of the value to serialize as an attribute.
		/// @param val Value to convert and write as an unquoted attribute token.
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

        /// @brief Writes a StringExt-typed attribute, optionally escaping the value.
        /// @param attribName Attribute name as a StringExt.
        /// @param val Attribute value as a StringExt.
        /// @param checkValue When true, the value is XML-escaped before writing.
        void attribute(const td::StringExt& attribName, const td::StringExt& val, bool checkValue = false)
        {
            attributeString(attribName.c_str(), val, checkValue);
        }

		/// @brief Writes a boolean attribute using its BoolCh text representation.
		/// @param attribName Null-terminated attribute name.
		/// @param val Boolean value wrapped in a BoolCh.
		void attribute(const char* attribName, const td::BoolCh& val)
		{
			attribute(attribName, val());
		}

		/// @brief Writes a td::Variant attribute, handling each data type appropriately.
		/// @param attribName Null-terminated attribute name.
		/// @param val Variant value to serialize.
		/// @param writeTD_NONE When true, TD_NONE typed variants are still written using the converter.
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

		/// @brief Writes a scalar numeric attribute (copy-by-value variant).
		/// @tparam ATTTYPE Arithmetic type of the attribute value.
		/// @param attribName Null-terminated attribute name.
		/// @param val Attribute value to convert and write.
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

		/// @brief Writes a td::String attribute, optionally XML-escaping the value.
		/// @param attribName Null-terminated attribute name.
		/// @param attribValue String attribute value.
		/// @param checkValue When true, special XML characters in the value are escaped.
		void attribute(const char* attribName, const td::String& attribValue, bool checkValue=true)
		{
            attributeString(attribName, attribValue.c_str(), checkValue);
		}

		/// @brief Writes a boolean attribute using the converter's "true"/"false" representation.
		/// @param attribName Null-terminated attribute name.
		/// @param val Boolean value.
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

        /// @brief Writes a char attribute, writing the character directly if it is printable.
        /// @param attribName Null-terminated attribute name.
        /// @param chVal Character value to write; non-printable characters produce an empty value.
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
                writeString(tmp);
            }
            putCStr("\"");
        }

        /// @brief Writes a numeric (non-boolean) attribute value using the converter.
        /// @tparam TVAL Numeric type satisfying td::conc::NumericNotBool.
        /// @param attribName Null-terminated attribute name.
        /// @param val Numeric value to write.
        template <td::conc::NumericNotBool TVAL>
        void attribute(const char* attribName, TVAL val)
        {
            if (!_nodeStarted)
                throw td::String("Cannot add attrib! There is no node started!");


            putCStr(" ");
            writeString(attribName);
            putCStr("=\"");
            assert(_pConverter != nullptr);
            writeString(_pConverter->c_str(val));
            putCStr("\"");
        }

        /// @brief Writes a non-numeric attribute value with XML escaping applied to the string representation.
        /// @tparam ATTTYPE Non-numeric type satisfying td::conc::NonNumeric.
        /// @param attribName Null-terminated attribute name.
        /// @param val Value to convert and write with XML escaping.
        template <td::conc::NonNumeric ATTTYPE>
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

		/// @brief Convenience wrapper that writes a boolean attribute; delegates to attribute(const char*, bool).
		/// @param attribName Null-terminated attribute name.
		/// @param val Boolean value.
		void attributeBool(const char* attribName, bool val)
		{
			attribute(attribName, val);
		}

		/// @brief Writes an enum attribute using a registered enum serializer to produce the name string.
		/// @tparam TENUM Enumeration type to serialize.
		/// @param enumPos Index of the enum serializer in the manager.
		/// @param attribName Null-terminated attribute name.
		/// @param val Enum value to write as its registered name.
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

		/// @brief Writes a GDI ARGB color attribute, optionally converting from OpenGL RGBA byte order.
		/// @tparam convertToGDIARGB_FromGLARGB When true, the value is byte-swapped from GL RGBA to GDI ARGB before writing.
		/// @param attribName Null-terminated attribute name.
		/// @param val Color value in either GL RGBA or native GDI ARGB format.
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

		/// @brief Returns the active value-to-string converter.
		/// @return Pointer to the current mu::Utils converter; never null.
		mu::Utils* getConverter()
		{
			assert(_pConverter != 0);
			return _pConverter;
		}

		/// @brief Writes a string attribute, optionally XML-escaping the C-string value.
		/// @param attribName Null-terminated attribute name.
		/// @param attribValue Null-terminated attribute value.
		/// @param checkValue When true, special XML characters are escaped in the value.
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

		/// @brief Writes a namespace-qualified string attribute.
		/// @param pNameSpace Null-terminated namespace prefix.
		/// @param attribName Null-terminated local attribute name.
		/// @param attribValue Null-terminated attribute value.
		/// @param checkValue When true, special XML characters are escaped in the value.
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

		/// @brief Writes a td::String attribute, optionally XML-escaping the value.
		/// @param attribName Null-terminated attribute name.
		/// @param attribValue Attribute value as a td::String.
		/// @param checkValue When true, special XML characters are escaped.
		void attributeString(const char* attribName, const td::String& attribValue, bool checkValue = false)
		{
			attributeString(attribName, attribValue.c_str(), checkValue);
		}

        /// @brief Writes a td::StringExt attribute, optionally XML-escaping the value.
        /// @param attribName Null-terminated attribute name.
        /// @param attribValue Attribute value as a td::StringExt.
        /// @param checkValue When true, special XML characters are escaped.
        void attributeString(const char* attribName, const td::StringExt& attribValue, bool checkValue = false)
        {
            attributeString(attribName, attribValue.c_str(), checkValue);
        }

		/// @brief Writes a namespace-qualified td::String attribute.
		/// @param pNameSpace Null-terminated namespace prefix.
		/// @param attribName Null-terminated local attribute name.
		/// @param attribValue Attribute value as a td::String.
		/// @param checkValue When true, special XML characters are escaped.
		void attributeStringNS(const char* pNameSpace, const char* attribName, const td::String& attribValue, bool checkValue = false)
		{
			attributeStringNS(pNameSpace, attribName, attribValue.c_str(), checkValue);
		}

		/// @brief Writes XML-escaped text content into the current element from a C-string.
		/// @param nodeStr Null-terminated text to write as element content.
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

		/// @brief Writes XML-escaped text content into the current element from a td::String.
		/// @param nodeString String value to write as element content.
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

		/// @brief Writes XML-escaped text content with allowed inline markup sequences from a C-string.
		/// @tparam TFNTFMT Container of font-format descriptors that pass through unescaped.
		/// @param nodeStr Null-terminated text to write.
		/// @param fntFmts Collection of allowed markup sequences.
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

		/// @brief Writes XML-escaped text content with allowed inline markup sequences from a td::String.
		/// @tparam TFNTFMT Container of font-format descriptors that pass through unescaped.
		/// @param nodeString String value to write.
		/// @param fntFmts Collection of allowed markup sequences.
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

		/// @brief Writes an XML comment node (<!-- ... -->) at the current position.
		/// @param comment Null-terminated text to place inside the comment.
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

		/// @brief Writes an XML comment starting on a new line with optional leading tabs.
		/// @param comment Null-terminated text to place inside the comment.
		/// @param nTabsAfterNewLine Number of tab characters to insert after the newline before the comment.
		void commentInNewLine(const char* comment, size_t nTabsAfterNewLine = 0)
		{
			if (_nodeStarted)
			{
				putCStr(">");
				_nodeStarted = 0;
			}

            if (nTabsAfterNewLine == 0)
                putCStr("\n<!--");
            else
            {
                putCStr("\n");
                for (size_t i=0; i<nTabsAfterNewLine; ++i)
                    putCStr("\t");
                putCStr("<!--");
            }
			writeString(comment);
			putCStr("-->");
		}

		/// @brief Writes a CDATA section wrapping the given data.
		/// @param cdata Null-terminated data to wrap in a CDATA section.
		void CDATA(const char* cdata)
		{
			putCStr("CDATA[[");
			writeString(cdata);
			putCStr("]]>");
		}

		/// @brief Closes the current element, writing either a self-closing tag or a full end tag.
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

        /// @brief Closes the current element; alias for endElement().
        void endNode()
        {
            endElement();
        }

		/// @brief Closes an HTML meta element, using self-closing syntax for XHTML and plain closing for HTML.
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

		/// @brief Closes all remaining open elements to finalize the document.
		void endDocument()
		{
			while(!_openNodes.isEmpty())
			{
				endElement();
			}
		}

		/// @brief Flushes the underlying file stream to disk.
		void flush()
		{
			_f.flush();
		}

		/// @brief Closes the document, flushes pending elements, and closes the file stream if open.
		void close()
		{
			endDocument();
			if (!_inMemory)
			{
				if (_f.is_open())
					_f.close();
			}
		}

		/// @brief Retrieves the accumulated in-memory document content as a td::String.
		/// @param str Receives the serialized document text.
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

        /// @brief Returns the accumulated in-memory document content as a td::String by value.
        /// @return Serialized document text.
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

		/// @brief Returns a const reference to the internal string builder.
		/// @return Const reference to the StringBuilder accumulating the document.
		const cnt::StringBuilder<td::String, 16 * 1024>& getBuilder() const
		{
			assert(_inMemory);
			return _builder;
		}

		/// @brief Returns a mutable reference to the internal string builder.
		/// @return Mutable reference to the StringBuilder accumulating the document.
		cnt::StringBuilder<td::String, 16 * 1024>& getBuilder()
		{
			assert(_inMemory);
			return _builder;
		}

        /// @brief Serializes a node object into this writer by calling its toWriter method.
        /// @tparam XmlNode Type that exposes a toWriter(Writer&) method.
        /// @param node Node object to serialize.
        template <class XmlNode>
        void addNode(XmlNode& node)
        {
            node.toWriter(*this);
        }
	};
}
