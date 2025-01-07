// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/Tokenizer.h>
#include <td/BufferString.h>
#include <td/String.h>
#include <cnt/PushBackVector.h>
#include <mem/PointerDeleter.h>
#include <mu/Utils.h>

namespace mu
{
	template <class TSTR = td::String, class TINPBUFFER = StringWrapper8, mu::ParserType PARSER_TYPE=mu::ParserType::C >
	class SimpleParser : public Tokenizer<char, TINPBUFFER, FileString8, PARSER_TYPE, true>
	{
	protected:
		typedef Tokenizer<char, TINPBUFFER, FileString8, PARSER_TYPE, true> TBASE;
		typedef typename Tokenizer<char, TINPBUFFER, FileString8, PARSER_TYPE, true>::Token TBaseToken;

		typename TBASE::Token _currentToken = TBASE::Token::T_NOT_INITIALIZED;

		template<class TVECT>
		inline void putToken(typename TBASE::Token t, TVECT& tokens)
		{
			if (t == TBASE::Token::T_ALPHANUM)		
			{
				TBASE::template parseName<false, true>();
				TSTR& str = tokens.push_back();
				str.fromKnownString(TBASE::_outBuffer.c_str(), TBASE::_outBuffer.length());

			}
			else if (t == TBASE::Token::T_NUMERIC)
			{
				TBASE::parseNumber();
				TSTR& str = tokens.push_back();
				str.fromKnownString(TBASE::_outBuffer.c_str(), TBASE::_outBuffer.length());
			}							
		}
				
		inline void getVal(const char* pStr, td::LUINT8& val) const
		{
			val = mu::Utils::toLUINT8(pStr);
		}

		inline void getVal(const char* pStr, int& val) const
		{
			val = mu::Utils::toINT4(pStr);
		}		


	public:
		SimpleParser(){}		

		template<class TVECT> // =cnt::PushBackVector<td::String>>
		bool parseAlphanumFromFile(const char* fileName, TVECT& tokens)
		{
			std::ifstream f;
			f.open(fileName, std::ios_base::binary);
			if (!f.is_open())
				return false;

			//FileBuffer1k inputWrapper;
			TINPBUFFER inputWrapper;
			TBASE::pInput = &inputWrapper;
			TBASE::pInput->setFileAsInput(&f);
			
			TBASE::initIterators();
			try
			{
				while (TBASE::it != TBASE::itEnd)
				{
					typename TBASE::Token t = TBASE::template getNextToken<false>();
					putToken(t, tokens);
				}
			}
			catch (const mu::ParserException&)
			{
				if (TBASE::outBuffer.length() > 0)
				{
					if (TBASE::isAlpha(*TBASE::outBuffer.begin()) || TBASE::isNumeric(*TBASE::outBuffer.begin()))
					{
						TSTR& str = tokens.push_back();
						str.fromKnownString(TBASE::outBuffer.c_str(), TBASE::outBuffer.length());
						return true;
					}
					return false;
				}
			}
			TBASE::pInput = nullptr;
			return true;
		}
		
		template<class TVECT> // =cnt::PushBackVector<td::String>>
		void parseAlphanum(const char* pStr,  TVECT& tokens)
		{				
			//StringWrapper8 inputWrapper;
			TINPBUFFER inputWrapper;
			TBASE::_pInput = &inputWrapper; // new StringWrapper8;		

			TBASE::_pInput->setBufferAsInput(pStr);
			TBASE::initIterators();			
			try
			{					
				while (TBASE::it != TBASE::itEnd)
				{
					typename TBASE::Token t = TBASE::template getNextToken<false>();
					putToken(t, tokens);
				}								
			}
			catch(const mu::ParserException& )
			{
				if (TBASE::_outBuffer.length() > 0)
				{
					if (TBASE::isAlpha(*TBASE::_outBuffer.begin()) || TBASE::isNumeric(*TBASE::_outBuffer.begin()) )
					{
						TSTR& str = tokens.push_back();
						str.fromKnownString(TBASE::_outBuffer.c_str(), TBASE::_outBuffer.length());
					}
				}
			}
			//delete TBASE::pInput;
			TBASE::_pInput = nullptr;
		}


		//cnt::PushBackVector<int>
		template<class TVECT>  // =cnt::PushBackVector<int> >
		bool parseNumbers(const char* pStr,  TVECT& tokens)
		{				
			//StringWrapper8 inputWrapper;
			TINPBUFFER inputWrapper;
			TBASE::pInput = &inputWrapper; // new StringWrapper8;
			TBASE::pInput->setBufferAsInput(pStr);
			TBASE::initIterators();			
			try
			{					
				while (TBASE::it != TBASE::itEnd)
				{
					typename TBASE::Token t = TBASE::template getNextToken<false>();
					if (t == TBASE::T_NUMERIC)
					{
						TBASE::parseNumber();
						typename TVECT::T_DATA val;						
						getVal(TBASE::outBuffer.c_str(), val);
						tokens.push_back(val);
					}
					else
					{
						if (t == TBASE::T_ALPHANUM)
						{
							//delete TBASE::pInput;
							TBASE::pInput = nullptr;
							return false; //TBASE::moveNext();
						}
							
					}
						
				}								
			}
			catch(const mu::ParserException& )
			{
				if (TBASE::outBuffer.length() > 0)
				{
					if (TBASE::isNumeric(*TBASE::outBuffer.begin()) )
					{
						tokens.push_back( (atoi (TBASE::outBuffer.c_str()) )); 						
					}
				}
			}
			//delete TBASE::pInput;
			TBASE::pInput = nullptr;
			return true;
		}

		void initBufferWrapper(TINPBUFFER* inputWrapper)
		{
			TBASE::pInput = inputWrapper;
		}

	protected:
		template<class TCALLBACK>
		void passToken(typename TBASE::Token t, TCALLBACK& callBack)
		{
			_currentToken = t;
			if (t == TBASE::T_ALPHANUM)
			{
				auto hashVal = TBASE::template parseName<true, true>();				
				//auto len = TBASE::outBuffer.length();
				//if (len > 0)
				{
					callBack.handleAlphaNum(TBASE::outBuffer.c_str(), TBASE::outBuffer.length(), hashVal);
				}				
			}
			else if (t == TBASE::T_NUMERIC)
			{
				TBASE::parseNumber();
				
				//if (len > 0)
				{
					callBack.handleNum(TBASE::outBuffer.c_str(), TBASE::outBuffer.length());
				}
			}
			else
			{
				callBack.handleToken(t, TBASE::outBuffer.c_str(), TBASE::outBuffer.length());
			}
		}
		
	public:
		template<class TCALLBACK>
		void parseWithCallback(const TSTR& str, TCALLBACK& callBack)
		{
			assert(TBASE::pInput);

			//StringWrapper8 inputWrapper;
			//TINPBUFFER inputWrapper;
			//TBASE::pInput = &inputWrapper; // new StringWrapper8;		

			TBASE::pInput->setBufferAsInput(str.c_str());
			TBASE::initIterators();
			try
			{
				callBack.handleInit();
				while (TBASE::it != TBASE::itEnd)
				{
					_currentToken = TBASE::T_NOT_INITIALIZED;
					TBASE::template skipWhiteSpaces<true>();
					auto whiteSpaceLen = TBASE::outBuffer.length();
					if (whiteSpaceLen > 0)
					{
						callBack.handleWhiteSpace(TBASE::outBuffer.c_str(), whiteSpaceLen, false);
					}

					typename TBASE::Token t = TBASE::template getNextTokenWithStoredTokenString<false>();
					passToken(t, callBack);
					//callback.handleToken(t, TBASE::outBuffer.c_str(), TBASE::outBuffer.length());
				}
			}
			catch (const mu::ParserException&)
			{
				auto buffLen = TBASE::outBuffer.length();
				if (buffLen > 0)
				{
					auto pStr = TBASE::outBuffer.c_str();
					if (_currentToken != TBASE::T_NOT_INITIALIZED)
					{
						if (_currentToken == TBASE::T_ALPHANUM)
						{							
							callBack.handleAlphaNum(pStr, buffLen, 0);
						}
						else if (_currentToken == TBASE::T_NUMERIC)
						{
							callBack.handleNum(pStr, buffLen);
						}
						else
						{
							if (TBASE::outBuffer.isWhiteSpaceOnly())
							{
								callBack.handleWhiteSpace(pStr, buffLen, true);
							}
						}
					}
					else
					{
						if (TBASE::outBuffer.isWhiteSpaceOnly())
						{
							callBack.handleWhiteSpace(pStr, buffLen, true);							
						}
						else
						{
							_currentToken = TBASE::isAnyToken(pStr[0]);
							if (_currentToken == TBASE::T_ALPHANUM)
							{
								callBack.handleAlphaNum(pStr, buffLen, 0);
							}
							else if (_currentToken == TBASE::T_NUMERIC)
							{
								callBack.handleNum(pStr, buffLen);
							}
							else
							{		
								passToken(_currentToken, callBack);
							}
						}					   
					}
				}
			}
			callBack.handleFinish();
			//delete TBASE::pInput;
			//TBASE::pInput = nullptr;
		}
	};

	typedef mu::SimpleParser<td::String, StringWrapper8, mu::ParserType::AlphaNumerical> AlphaParser;
	typedef mu::SimpleParser<td::String, StringWrapper8, mu::ParserType::C> AlphaNumParser;
}