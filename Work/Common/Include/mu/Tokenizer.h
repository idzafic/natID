// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/BufferString.h>
#include <td/String.h>

namespace mu
{
	enum class ParserType : td::BYTE {Xml, C, AlphaNumerical, AlphaNumericalUTF8};

	enum class ExceptionType : td::BYTE {EoF, WrongToken, EoLn, NotAnXMLFile, CannotOpenFile, WrongVersion, UnsupportedEncoding, UnsupportedFileEncoding, WrongHeader, TextAfterEnd};
		
	class ParserException
	{
	public:
        td::String message;
		int lineNo;
		int charNumber;			
		ExceptionType type;
		ParserException()
			: lineNo(-1)
			, charNumber(-1)
			, type(ExceptionType::EoF)
		{
		}

		ParserException(int lNo, int posInMessage, ExceptionType tp, const char* pMessage)
			: lineNo(lNo)
			, charNumber(posInMessage)
			, type(tp)
			, message(pMessage)
		{
		}
		template<class OSTREAM>
		void show(OSTREAM& o, const char* fileName) const
		{
			if (lineNo >= 0)
				o << "Parser Error! " << fileName << ":" << lineNo+1<< ", " << message.c_str() << td::ENDL;
			else
				o << "Parser Error! " << fileName << ":" << message.c_str() << td::ENDL;
		}
	};

	template<typename TCHAR, class TINBUFFER, class TOUTBUFFER, ParserType PARSER_TYPE, bool FROM_MEMORY>
	class Tokenizer
	{
	public:
		//                                           <      >       \                /      +        -       *       :         ;           ,       !         ?           &     |      #       =        '         "          0-9         99        99.9     a-z A-Z,_          (              )                 [                     ]                   {                     }
		enum class Token : td::BYTE {T_NOT_INITIALIZED = 0, T_LT,  T_GT,  T_BACK_SLASH,  T_SLASH, T_PLUS, T_MINUS, T_MULT, T_COLON, T_SEMICOLON, T_COMMA, T_EXCL, T_QUESTION, T_AND, T_OR, T_FENCE, T_EQUAL, T_SQUOTE, T_DQUOTE, T_NUMERIC, T_INTEGER,  T_FLOAT, T_ALPHANUM,  T_BRACKET_OPEN, T_BRACKET_CLOSE, T_RECT_BRACKET_OPEN, T_RECT_BRACKET_CLOSE, T_SPEC_BRACKET_OPEN, T_SPEC_BRACKET_CLOSE, T_NOT_ALLOWED};
		enum class NumberType : td::BYTE {NT_INT, NT_FLOAT} ;

	protected:
		TINBUFFER*		_pInput = nullptr;
		TOUTBUFFER		_outBuffer;
//		double _outDbl;
//		double outInt;
        int _nLines;
        Token _specialCharToken;
		td::BYTE _nonStrictMode = 0;

		typename TINBUFFER::const_iterator it;
		typename TINBUFFER::const_iterator itBegin;
		typename TINBUFFER::const_iterator itEnd;
	public:		

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

		void initialize(TINBUFFER* inputBuffer)
		{
			_pInput = inputBuffer;			
		}

		void initIterators()
		{
			itBegin = it = _pInput->currentPosition();			
			itEnd = _pInput->end();
		}
        
        void reset()
        {
            _pInput = nullptr;
            _outBuffer.erase();
            _nLines = 0;
        }

		inline void checkBuffer()
		{
			if (it == itEnd)
			{
				reload();
			}			
		}

		inline void moveNext()
		{
			++it;
			checkBuffer();
		}

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

		inline bool isNumeric(TCHAR ch) const
		{
			if (ch >= '0' && ch <= '9')
				return true;
			return false;
		}

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

		inline bool isWhiteSpace(TCHAR ch) const
		{
			if (ch == ' ' || ch == '\t')
				return true;
			return false;
		}

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
						UTF16 ch16 = (UTF16) atoi(tmp);
						UTF16 u16[2];
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
		
		inline void parseReservedSign()
		{
			if (PARSER_TYPE == ParserType::Xml)
				return parseReservedXMLSign();			
			else			
				return parseReservedCSign();
		}
	public:
		void moveToNewLine()
		{
		}

		void nonStrictMode(bool nonStrict = true)
		{
            if (nonStrict)
                _nonStrictMode = 1;
            else
                _nonStrictMode = 0;
		}

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
							hashNo <<= 5;									 // shift hashNo left by 5 bits
							hashNo ^= (highorder >> 27);					 // move the highorder 5 bits to the low-order
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
