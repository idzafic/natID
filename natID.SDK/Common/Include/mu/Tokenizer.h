// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Tokenizer.h
    @brief Template-based lexical tokenizer supporting XML, C, and alphanumeric parsing modes. */
#pragma once
#include <td/BufferString.h>
#include <td/String.h>

namespace mu
{
	/// @brief Enumeration of available parser modes that control token recognition rules.
	enum class ParserType : td::BYTE {
		Xml,                ///< XML parser mode: recognizes XML-specific tokens and entities.
		C,                  ///< C-language parser mode: recognizes C-style tokens and operators.
		AlphaNumerical,     ///< Alphanumeric-only mode: treats colons and hyphens as identifier characters.
		AlphaNumericalUTF8  ///< Alphanumeric mode with UTF-8 multi-byte character support.
	};

	/// @brief Enumeration of exception types that the parser can raise.
	enum class ExceptionType : td::BYTE {
		EoF,                    ///< End of input stream reached unexpectedly.
		WrongToken,             ///< An unexpected or malformed token was encountered.
		EoLn,                   ///< Unexpected end of line encountered while parsing a string.
		NotAnXMLFile,           ///< Input does not appear to be a valid XML file.
		CannotOpenFile,         ///< The specified file could not be opened.
		WrongVersion,           ///< File version is not supported.
		UnsupportedEncoding,    ///< Character encoding is not supported.
		UnsupportedFileEncoding,///< File-level encoding declaration is not supported.
		WrongHeader,            ///< File header is malformed or missing.
		TextAfterEnd            ///< Text content was found after the closing root element.
	};

	/// @brief Exception type thrown by the tokenizer when a parse error occurs.
	class ParserException
	{
	public:
        td::String message; ///< Human-readable description of the parse error.
		int lineNo;          ///< 0-based line number where the error occurred; -1 if unknown.
		int charNumber;      ///< Position within the line where the error occurred; -1 if unknown.
		ExceptionType type;  ///< Classification of the parse error.

		/// @brief Default constructor; initializes to an EOF exception with no location info.
		ParserException()
			: lineNo(-1)
			, charNumber(-1)
			, type(ExceptionType::EoF)
		{
		}

		/// @brief Constructs an exception with full location and message information.
		/// @param lNo 0-based line number of the error.
		/// @param posInMessage Character position within the line.
		/// @param tp The exception type classification.
		/// @param pMessage Null-terminated error description string.
		ParserException(int lNo, int posInMessage, ExceptionType tp, const char* pMessage)
			: lineNo(lNo)
			, charNumber(posInMessage)
			, type(tp)
			, message(pMessage)
		{
		}

		/// @brief Writes a human-readable error description to the given output stream.
		/// @tparam OSTREAM Output stream type supporting operator<<.
		/// @param o The stream to write the error message to.
		/// @param fileName Name of the file being parsed, used in the error message.
		template<class OSTREAM>
		void show(OSTREAM& o, const char* fileName) const
		{
			if (lineNo >= 0)
				o << "Parser Error! " << fileName << ":" << lineNo+1<< ", " << message.c_str() << td::ENDL;
			else
				o << "Parser Error! " << fileName << ":" << message.c_str() << td::ENDL;
		}
	};

	/// @brief Low-level character-stream tokenizer parameterized by character type, buffers, parser mode, and source.
	/// @tparam TCHAR The character type (e.g. char or wchar_t).
	/// @tparam TINBUFFER Input buffer type providing iterator access to the character stream.
	/// @tparam TOUTBUFFER Output buffer type used to accumulate recognized token text.
	/// @tparam PARSER_TYPE The parsing mode (XML, C, AlphaNumerical, or AlphaNumericalUTF8).
	/// @tparam FROM_MEMORY True if input is read from memory; false if read from a file stream.
	template<typename TCHAR, class TINBUFFER, class TOUTBUFFER, ParserType PARSER_TYPE, bool FROM_MEMORY>
	class Tokenizer
	{
	public:
		//                                           <      >       \                /      +        -       *       :         ;           ,       !         ?           &     |      #       =        '         "          0-9         99        99.9     a-z A-Z,_          (              )                 [                     ]                   {                     }
		/// @brief Enumeration of all token types that the tokenizer can recognize.
		enum class Token : td::BYTE {
			T_NOT_INITIALIZED = 0, ///< Token has not been set yet.
			T_LT,                  ///< Less-than '<'.
			T_GT,                  ///< Greater-than '>'.
			T_BACK_SLASH,          ///< Back-slash '\\'.
			T_SLASH,               ///< Forward-slash '/'.
			T_PLUS,                ///< Plus '+'.
			T_MINUS,               ///< Minus '-'.
			T_MULT,                ///< Asterisk '*'.
			T_COLON,               ///< Colon ':'.
			T_SEMICOLON,           ///< Semicolon ';'.
			T_COMMA,               ///< Comma ','.
			T_EXCL,                ///< Exclamation mark '!'.
			T_QUESTION,            ///< Question mark '?'.
			T_AND,                 ///< Ampersand '&'.
			T_OR,                  ///< Pipe '|'.
			T_FENCE,               ///< Hash '#'.
			T_EQUAL,               ///< Equals '='.
			T_SQUOTE,              ///< Single quote '\''.
			T_DQUOTE,              ///< Double quote '"'.
			T_NUMERIC,             ///< Numeric token (integer or float).
			T_INTEGER,             ///< Pure integer numeric token.
			T_FLOAT,               ///< Floating-point numeric token.
			T_ALPHANUM,            ///< Alphanumeric identifier token.
			T_BRACKET_OPEN,        ///< Opening parenthesis '('.
			T_BRACKET_CLOSE,       ///< Closing parenthesis ')'.
			T_RECT_BRACKET_OPEN,   ///< Opening square bracket '['.
			T_RECT_BRACKET_CLOSE,  ///< Closing square bracket ']'.
			T_SPEC_BRACKET_OPEN,   ///< Opening curly brace '{'.
			T_SPEC_BRACKET_CLOSE,  ///< Closing curly brace '}'.
			T_NOT_ALLOWED          ///< Character is not allowed in the current parser context.
		};

		/// @brief Enumeration distinguishing integer from floating-point numeric tokens.
		enum class NumberType : td::BYTE {
			NT_INT,   ///< The number contains no decimal point (integer).
			NT_FLOAT  ///< The number contains a decimal point (float).
		};

	protected:
		TINBUFFER*   _pInput = nullptr;     ///< Pointer to the current input buffer.
		TOUTBUFFER   _outBuffer;            ///< Accumulation buffer for the current token's text.
//		double _outDbl;
//		double outInt;
        int _nLines;                        ///< Running count of newline characters seen so far.
        Token _specialCharToken;            ///< Token type used for the special escape-introducing character.
		td::BYTE _nonStrictMode = 0;        ///< Non-zero enables lenient parsing that skips unrecognized XML entities.

		typename TINBUFFER::const_iterator it;      ///< Iterator pointing to the current input character.
		typename TINBUFFER::const_iterator itBegin; ///< Iterator pointing to the first character of the current buffer.
		typename TINBUFFER::const_iterator itEnd;   ///< Iterator pointing one past the last character of the current buffer.
	public:

		/// @brief Default constructor; sets up the tokenizer state and selects the appropriate special character for the parser mode.
		Tokenizer()
			: _nLines(0)
			, _pInput(nullptr)
			, _outBuffer()
			, _specialCharToken(Token::T_AND)
//			, _outDbl(0)
//			, outInt(0)
		{
			if (PARSER_TYPE != mu::ParserType::Xml)
			{
				_specialCharToken = Token::T_BACK_SLASH;
			}
		}

	protected:

		/// @brief Sets the input buffer for subsequent tokenization.
		/// @param inputBuffer Pointer to the input buffer to use.
		void initialize(TINBUFFER* inputBuffer)
		{
			_pInput = inputBuffer;
		}

		/// @brief Initializes the iterator positions from the current input buffer.
		void initIterators()
		{
			itBegin = it = _pInput->currentPosition();
			itEnd = _pInput->end();
		}

        /// @brief Resets the tokenizer state, clearing the input pointer, output buffer, and line counter.
        void reset()
        {
            _pInput = nullptr;
            _outBuffer.erase();
            _nLines = 0;
        }

		/// @brief Reloads the input buffer if the current iterator has reached the end.
		inline void checkBuffer()
		{
			if (it == itEnd)
			{
				reload();
			}
		}

		/// @brief Advances the iterator by one character and checks for end-of-buffer.
		inline void moveNext()
		{
			++it;
			checkBuffer();
		}

		/// @brief Requests the next chunk of data from the input buffer; throws ParserException on end of input.
		inline void reload()
		{
            assert(_pInput);
            if (_pInput->load())
            {
                it = _pInput->begin();
                itEnd = _pInput->end();
            }
            else
            {
                throw ParserException (_nLines, -1, ExceptionType::EoF, "End of input detected");
            }
		}

		/// @brief Returns true if the character is a valid identifier start or continuation character.
		/// @param ch The character to test.
		/// @return true if the character is alphabetic or underscore (with mode-specific extensions).
		inline bool isAlpha(TCHAR ch) const
		{
			if ((ch >= 'A' && ch <= 'Z' ) ||
				(ch >= 'a' && ch <= 'z')  ||
				(ch == '_'))
				return true;
			if (PARSER_TYPE == ParserType::AlphaNumerical)
			{
				if (ch == ':')
					return true;
			}

			if (PARSER_TYPE == ParserType::AlphaNumericalUTF8)
			{
				if (ch >= 127 || ch < 0)
					return true;
			}

			return false;
		}

		/// @brief Returns true if the character is an ASCII decimal digit.
		/// @param ch The character to test.
		/// @return true if ch is '0' through '9'.
		inline bool isNumeric(TCHAR ch) const
		{
			if (ch >= '0' && ch <= '9')
				return true;
			return false;
		}

		/// @brief Returns true if the character can appear inside an identifier (alpha, digit, or mode-specific punctuation).
		/// @param ch The character to test.
		/// @return true if ch is a valid alphanumeric identifier character.
		inline bool isAlhaNumeric(TCHAR ch)
		{
			if (isAlpha(ch) || isNumeric(ch))
				return true;
			if ((PARSER_TYPE == ParserType::Xml) || (PARSER_TYPE == ParserType::AlphaNumerical))
			{
				if ( (ch == ':') || (ch == '-') || (ch == '.') )
					return true;
			}
			return false;
		}

		/// @brief Returns true if the character is a horizontal whitespace character (space or tab).
		/// @param ch The character to test.
		/// @return true if ch is ' ' or '\\t'.
		inline bool isWhiteSpace(TCHAR ch) const
		{
			if (ch == ' ' || ch == '\t')
				return true;
			return false;
		}

		/// @brief Returns true if the character is a newline character; also increments the line counter.
		/// @param ch The character to test.
		/// @return true if ch is '\\n' or '\\r'.
		inline bool isNewLine(TCHAR ch)
		{
			if (ch == '\n')
			{
				++_nLines;
				return true;
			}
			if (ch == '\r')
				return true;

			return false;
		}

		/// @brief Skips whitespace and optionally accumulates skipped characters into the output buffer.
		/// @tparam storeSpaces If true, skipped whitespace characters are written to _outBuffer.
		template <bool storeSpaces>
		void skipWhiteSpaces()
		{
			_outBuffer.erase();

			do
			{
				while (it != itEnd)
				{
					TCHAR ch = *it;
					if (isWhiteSpace(ch) || isNewLine(ch))
					{
						if (storeSpaces)
							_outBuffer += ch;
						++it;
					}
					else
						return;
				}
				checkBuffer();
			} while (true);
		}



		//&lt;
		//The less-than character (<) starts element markup (the first character of a start-tag or an end-tag).

		//	&amp;
		//The ampersand character (&) starts entity markup (the first character of a character entity reference).

		//	&gt;
		//The greater-than character (>) ends a start-tag or an end-tag.

		//	&quot;
		//The double-quote character (") can be symbolised with this character entity reference when you need to embed a double-quote inside a string which is already double-quoted.

		//	&apos;
		//The apostrophe or single-quote character (') can be symbolised with this character entity reference when you need to embed a single-quote or apostrophe inside a string which is already single-quoted.

		/// @brief Returns the Token type for a character that introduces a special/escape sequence, or T_NOT_INITIALIZED if none.
		/// @param ch The character to classify.
		/// @return The matching special Token value, or Token::T_NOT_INITIALIZED.
		inline Token isSpecial(TCHAR ch)
		{
			if (PARSER_TYPE == ParserType::Xml)
			{
				switch (ch)
				{
				//case '#':
				//	return T_FENCE;
				case '\'':
					return Token::T_SQUOTE;
				case '"':
					return Token::T_DQUOTE;
				case '&':
					return Token::T_AND;
				case '>':
					return Token::T_GT;
				case '<':
					return Token::T_LT;
				}
			}
			else
			{
				switch (ch)
				{
				case '\'':
					return Token::T_SQUOTE;
				case '\"':
					return Token::T_DQUOTE;
				case '\\':
					return Token::T_BACK_SLASH;
				}
			}
			return Token::T_NOT_INITIALIZED;
		}

		/// @brief Returns the Token type for a non-alphanumeric character in the current parser mode, or T_NOT_INITIALIZED.
		/// @param ch The character to classify.
		/// @return The matching non-alphanumeric Token value, or Token::T_NOT_INITIALIZED.
		inline Token isNonAlphaNumToken(TCHAR ch) const
		{
			if (PARSER_TYPE == ParserType::Xml)
			{
				switch (ch)
				{
				case '=':
					return Token::T_EQUAL;
				case '\'':
					return Token::T_SQUOTE;
				case '"':
					return Token::T_DQUOTE;
				case '<':
					return Token::T_LT;
				case '>':
					return Token::T_GT;
				case '/':
					return Token::T_SLASH;
				case '?':
					return Token::T_QUESTION;
				case '!':
					return Token::T_EXCL;
				case '&':
					return Token::T_AND;
				case '#':
					return Token::T_FENCE;
				case '*':
					return Token::T_MULT;
				case '-':
					return Token::T_MINUS;
				case '+':
					return Token::T_PLUS;
				case '(':
					return Token::T_BRACKET_OPEN;
				case ')':
					return Token::T_BRACKET_CLOSE;
				case '[':
					return Token::T_RECT_BRACKET_OPEN;
				case ']':
					return Token::T_RECT_BRACKET_CLOSE;
				case '{':
					return Token::T_SPEC_BRACKET_OPEN;
				case '}':
					return Token::T_SPEC_BRACKET_CLOSE;
				case '\\':
					return Token::T_BACK_SLASH;
				}
				return Token::T_NOT_INITIALIZED;
			}
			else if (PARSER_TYPE == ParserType::C)
			{
				switch (ch)
				{
				case '=':
					return Token::T_EQUAL;
				case '-':
					return Token::T_MINUS;
				case ':':
					return Token::T_COLON;
				case ';':
					return Token::T_SEMICOLON;
				}
			}
			else if (PARSER_TYPE == ParserType::AlphaNumericalUTF8)
			{
				switch (ch)
				{
				case '=':
					return Token::T_EQUAL;
				case '-':
					return Token::T_MINUS;
				case ':':
					return Token::T_COLON;
				case ';':
					return Token::T_SEMICOLON;
				case '>':
					return Token::T_GT;
				case ',':
					return Token::T_COMMA;
				}
			}
			//
			return Token::T_NOT_INITIALIZED;
		}

		/// @brief Classifies any character into its Token type (alphanumeric, numeric, or symbol).
		/// @param ch The character to classify.
		/// @return The most specific Token type for the given character.
		inline Token isAnyToken(TCHAR ch) const
		{
			if (PARSER_TYPE == ParserType::Xml)
			{
				if (isAlpha(ch))
					return Token::T_ALPHANUM;
				return isNonAlphaNumToken(ch);
			}
			else
			{
				if (isAlpha(ch))
					return Token::T_ALPHANUM;
				if (isNumeric(ch))
					return Token::T_NUMERIC;

				return isNonAlphaNumToken(ch);
			}

		}

		/// @brief Parses a C-style escape sequence after the leading backslash and appends the result to _outBuffer.
		void parseReservedCSign()
		{
			checkBuffer();
			switch(*it)
			{
			case '\'':
				_outBuffer += '\'';
				break;
			case '\0':
				_outBuffer += '\0';
				break;
			case '\n':
				_outBuffer += '\n';
				break;
			case '\r':
				_outBuffer += '\r';
				break;
			case '\t':
				_outBuffer += '\t';
				break;
			case '\\':
				_outBuffer += '\\';
				break;
			case '\"':
				_outBuffer += '\"';
				break;
			default:
				throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in quoted string");
			}
			moveNext();
		}

		/// @brief Parses a numeric or hexadecimal Unicode character reference (e.g. &#1234;) and appends the UTF-8 result.
		void parseUnicodeCharacter()
		{
			++it;
			if (it == itEnd)
			{
				throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Wrong unicode formating!");
				return;
			}


			TCHAR tmp[16];
			TCHAR ch = td::toLower(*it);
			int iPos = 0;
			if (td::isNumeric(ch))
			{
				//decimal input
				do
				{
					ch = *it;
					if (ch == ';')
					{
						tmp[iPos] = 0;
						mu::UTF16 ch16 = (mu::UTF16) atoi(tmp);
						mu::UTF16 u16[2];
						u16[0] = ch16;
						u16[1] = 0;
						TCHAR pStr[5];
						UTF16* u16Start = &u16[0];
						TCHAR* u8Start = &pStr[0];
						int nLen = mu::UTFConverter::measureUTF16toUTF8(u16Start, u16Start + 1);
						if (nLen > 0)
						{
							mu::UTFConverter::convertUTF16toUTF8(u16Start, u16Start + 1, (mu::UTF8*)u8Start, (mu::UTF8*)(u8Start + 4));
							//pStr[nLen] = 0;
							_outBuffer.append(nLen, u8Start);
						}
						++it;
						return;
					}
					tmp[iPos++] = ch;
					if (iPos > 4)
					{
						throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Wrong unicode formating!");
						return;
					}
					++it;
				} while (it != itEnd);
			}
			else if (*it == 'x')
			{
				//hexadecimal input -> TODO
				assert(false);
			}
			throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Wrong unicode formating!");
		}

		/// @brief Parses an XML character entity reference (e.g. &amp;, &lt;) and appends the decoded character to _outBuffer.
		void parseReservedXMLSign()
		{
			//&nbsp; -->and='&amp;' manje='&lt;' vece='&gt;' apos='&apos;' navod='&quot;' nbsp='&nbsp;'
			int pos = 0;
			TCHAR destCh = 0;
			do
			{
				while (it != itEnd)
				{
					TCHAR ch = *it;
					if (pos == 0)
					{
						switch (ch)
						{
						case 'l':
							destCh = '<';
							break;
						case 'g':
							destCh = '>';
							break;
						case 'q':
							destCh = '"';
							break;
						case 'a':
							destCh = '&'; //could be &amp; or &apos;
							break;
						case '#':
							parseUnicodeCharacter();
							return;
						case 'n':
							destCh = (TCHAR)160; //&nbsp;
							break;
						default:
							if (_nonStrictMode != 0)
							{
								_outBuffer += '&';
								_outBuffer += ch;
								++it;
								return;
							}
							else
							{
								throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							}

						}
					}
					else if (pos == 1)
					{
						switch (destCh)
						{
						case '<':
						case '>':
							if(ch != 't')
							{
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							}
							break;
						case '"':
							if(ch != 'u')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							break;
						case '&':
							if (ch == 'p')
								destCh = '\''; //could be &amp; or &apos;
							else if (ch != 'm')
								throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							break;
						case (TCHAR)160:
							if (ch != 'b')
								throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string nbsp");
							break;
						default:
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
						}
					}
					else if (pos == 2)
					{
						switch (destCh)
						{
						case '<':
						case '>':
							if(ch != ';')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");

							++it;
							_outBuffer += destCh;
							return;
							break;
						case '"':
							if(ch != 'o')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");

							break;
						case '&':
							if(ch != 'p')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");

							break;
						case '\'':
							if(ch != 'o')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							break;
						case (TCHAR)160:
							if (ch != 's')
								throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string nbsp");
							break;
						default:
							{
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							}
						}
					}
					else if (pos == 3)
					{
						switch (destCh)
						{
						case '"':
							if(ch != 't')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							break;
						case '&':
							if(ch != ';')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							++it;
							_outBuffer += destCh;
							return;
							break;
						case '\'':
							if(ch != 's')
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
							break;
						case (TCHAR)160:
							if (ch != 'p')
								throw ParserException(_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string nbsp");
							break;
						default:
								throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
						}
					}
					else if (pos == 4)
					{
						if(ch != ';')
							throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");

						++it;
						_outBuffer += destCh;
						//if (destCh != (TCHAR)160)
						//{
							//outBuffer += destCh;
						//}
						//else
						//{
						//	outBuffer += 'N';
						//	outBuffer += 'B';
						//	outBuffer += 'S';
						//	outBuffer += 'P';
						//}
						return;
					}
					else
						throw ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing special character in string");
					++it;
					++pos;
				}
				checkBuffer();
			} while (true);
		}

		/// @brief Dispatches to the appropriate reserved-sign parser for the current parser mode (XML or C).
		inline void parseReservedSign()
		{
			if (PARSER_TYPE == ParserType::Xml)
				return parseReservedXMLSign();
			else
				return parseReservedCSign();
		}
	public:
		/// @brief No-op placeholder for moving to a new line; reserved for future use.
		void moveToNewLine()
		{
		}

		/// @brief Enables or disables non-strict (lenient) parsing mode.
		/// @param nonStrict Pass true to enable lenient mode; false to restore strict mode.
		void nonStrictMode(bool nonStrict = true)
		{
            if (nonStrict)
                _nonStrictMode = 1;
            else
                _nonStrictMode = 0;
		}

		/// @brief Parses a quoted string, stopping at the given end-token, and stores the result in _outBuffer.
		/// @param endToken The quote token (T_SQUOTE or T_DQUOTE) that terminates the string.
		void parseQuotedString(Token endToken)
		{
			_outBuffer.erase();
			do
			{
				while (it != itEnd)
				{
					TCHAR ch = *it;
					Token t = isSpecial(ch);
					if (t != Token::T_NOT_INITIALIZED)
					{
						if ( t == endToken)
						{
							moveNext(); //after quoted string must be a token
							return;
						}
						else if (t == _specialCharToken)
						{
							++it;
							parseReservedSign();
							continue;
						}
						else if ( (t == Token::T_SQUOTE) && ( endToken != Token::T_SQUOTE) )
						{
							_outBuffer += ch;
							++it;
							continue;
						}
						else if ( (t == Token::T_DQUOTE) && ( endToken != Token::T_DQUOTE) )
						{
							_outBuffer += ch;
							++it;
							continue;
						}
						else
							ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing string");
					}

					if (ch == '\n')
					{
						if (PARSER_TYPE == ParserType::Xml)
							++_nLines;
						else
							ParserException (_nLines, -1, ExceptionType::EoLn, "Unexpected end of line while parsing string");
					}
					_outBuffer += ch;
					++it;
				}
				checkBuffer();
			} while(true);
		}

		/// @brief Reads XML text content, stopping at the '<' character, and appends it to _outBuffer.
		void parseXMLText()
		{
			//no need to erase outbuffer
			do
			{
				while (it != itEnd)
				{
					TCHAR ch = *it;
					Token t = isSpecial(ch);
					if (t != Token::T_NOT_INITIALIZED)
					{
						if (t == Token::T_LT)
						{
							//moveNext();
							return;
						}
						else if (t == _specialCharToken)
						{
							++it;
							parseReservedSign();
							continue;
						}
						else if ( (t == Token::T_SQUOTE) || ( t == Token::T_DQUOTE) )
						{
							_outBuffer += ch;
							++it;
							continue;
						}
						else
							ParserException (_nLines, -1, ExceptionType::WrongToken, "Unexpected character while parsing text");
					}

					if (ch == '\n')
						++_nLines;

					_outBuffer += ch;
					++it;
				}
				checkBuffer();
			} while(true);
		}

		/// @brief Parses a numeric literal (integer or float) from the input and stores it in _outBuffer.
		/// @return NumberType::NT_INT if no decimal point was found; NumberType::NT_FLOAT otherwise.
		NumberType parseNumber()
		{
			NumberType toRet = NumberType::NT_INT;
			_outBuffer.erase();
			do
			{
				while (it != itEnd)
				{
					TCHAR ch = *it;
					if (isNumeric(ch))
					{
						_outBuffer += ch;
						++it;
					}
					else if ( (toRet == NumberType::NT_INT) && (ch == '.'))
					{
						_outBuffer += ch;
						++it;
						toRet = NumberType::NT_FLOAT;
					}
					else
						return toRet;
				}
				checkBuffer();
			} while(true);
		}

		/// @brief Parses an alphanumeric identifier and optionally computes its hash; stores result in _outBuffer.
		/// @tparam calculateHash If true, a rolling hash of the identifier characters is computed and returned.
		/// @tparam erasePrevious If true, _outBuffer is cleared before parsing begins.
		/// @return The computed hash value, or 0 if calculateHash is false.
		template <bool calculateHash, bool erasePrevious>
		unsigned int parseName()
		{
			unsigned int hashNo = 0;

			if (erasePrevious)
				_outBuffer.erase();

			do
			{
				while (it != itEnd)
				{
					TCHAR ch = *it;
					if (isAlhaNumeric(ch))
					{
						_outBuffer += ch;
						++it;
						if (calculateHash)
						{
							unsigned int highorder = hashNo & 0xf8000000;    // extract high-order 5 bits from hashNo
							hashNo <<= 5;                                    // shift hashNo left by 5 bits
							hashNo ^= (highorder >> 27);                     // move the highorder 5 bits to the low-order
							                                                 // end and XOR into hashNo
							hashNo ^= (unsigned int) ch;
						}
					}
					else
						return hashNo;
				}
				checkBuffer();
			} while(true);
			return hashNo;
		}

		/// @brief Skips a comment block of known length by matching the terminating sequence.
		/// @tparam storeComment If true, skipped comment text is stored in _outBuffer.
		/// @param nLen Length of the comment-end sequence.
		/// @param commentEnd Pointer to the character sequence that terminates the comment.
		template <bool storeComment>
		void skipComments(int nLen, const TCHAR* commentEnd)
		{
			int pos = 0;
			if (storeComment)
				_outBuffer.erase();
			do
			{
				while (it != itEnd)
				{
					if (pos == nLen)
					{
						return;
					}
					TCHAR ch = *it;
					if (ch == commentEnd[pos])
					{
						++pos;
						++it;
						continue;
					}
					else
					{
						if (storeComment)
						{
							for (int i=0; i<pos; ++i)
								_outBuffer += commentEnd[i];
						}
						pos = 0;
					}

					if (storeComment)
						_outBuffer += ch;

					if (ch == '\n')
						++_nLines;

					++it;
				}
				checkBuffer();
			} while(true);
		}


		/// @brief Skips leading whitespace and returns the next token type; advances the iterator past the token.
		/// @tparam storeSpaces If true, skipped whitespace is stored in _outBuffer before the token.
		/// @return The Token type of the next non-whitespace token.
		template <bool storeSpaces>
		Token getNextToken()
		{
			skipWhiteSpaces<storeSpaces>();

			Token toRet = isAnyToken(*it);

			if ((toRet != Token::T_ALPHANUM) && (toRet != Token::T_NUMERIC))
			{
				if (toRet == Token::T_NOT_INITIALIZED)
				{
					//put this one in buffer
					_outBuffer += *it;
					moveNext();
				}
				else
					moveNext();
			}


			return toRet;
		}

		/// @brief Like getNextToken but also stores the token character itself in _outBuffer before advancing.
		/// @tparam storeSpaces If true, skipped whitespace is stored before the token character.
		/// @return The Token type of the next non-whitespace token.
		template <bool storeSpaces>
		Token getNextTokenWithStoredTokenString()
		{
			skipWhiteSpaces<storeSpaces>();

			Token toRet = isAnyToken(*it);

			if ((toRet != Token::T_ALPHANUM) && (toRet != Token::T_NUMERIC))
			{
				_outBuffer += *it;
				moveNext();
			}
			return toRet;
		}
	};
}
