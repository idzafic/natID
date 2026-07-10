// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SimpleParser.h
    @brief Lightweight tokenizer-based parser for alphanumeric and numeric token extraction. */
#pragma once
#include <mu/Tokenizer.h>
#include <td/BufferString.h>
#include <td/String.h>
#include <cnt/PushBackVector.h>
#include <mem/PointerDeleter.h>
#include <mu/Utils.h>

namespace mu
{
	/// @brief Simple tokenizer-based parser that extracts alphanumeric and numeric tokens from strings or files.
	/// @tparam TSTR        String type used to store extracted tokens (defaults to td::String).
	/// @tparam TINPBUFFER  Input buffer wrapper type (defaults to StringWrapper8).
	/// @tparam PARSER_TYPE Tokenizer parsing mode (defaults to mu::ParserType::C).
	template <class TSTR = td::String, class TINPBUFFER = StringWrapper8, mu::ParserType PARSER_TYPE=mu::ParserType::C >
	class SimpleParser : public Tokenizer<char, TINPBUFFER, FileString8, PARSER_TYPE, true>
	{
	protected:
		typedef Tokenizer<char, TINPBUFFER, FileString8, PARSER_TYPE, true> TBASE;
		typedef typename Tokenizer<char, TINPBUFFER, FileString8, PARSER_TYPE, true>::Token TBaseToken;

		typename TBASE::Token _currentToken = TBASE::Token::T_NOT_INITIALIZED; ///< Most recently processed token type.

		/// @brief Pushes a single parsed token into the output container.
		/// @tparam TVECT Container type whose elements are TSTR instances.
		/// @param t      Token type just scanned.
		/// @param tokens Output container receiving the new string token.
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

		/// @brief Reads a LUINT8 value from a C-string.
		/// @param pStr Null-terminated string containing the numeric value.
		/// @param val  Output parameter receiving the parsed value.
		inline void getVal(const char* pStr, td::LUINT8& val) const
		{
			val = mu::Utils::toLUINT8(pStr);
		}

		/// @brief Reads an int value from a C-string.
		/// @param pStr Null-terminated string containing the numeric value.
		/// @param val  Output parameter receiving the parsed value.
		inline void getVal(const char* pStr, int& val) const
		{
			val = mu::Utils::toINT4(pStr);
		}


	public:
		/// @brief Default constructor.
		SimpleParser(){}

		/// @brief Reads a file and extracts all alphanumeric and numeric tokens into the provided container.
		/// @tparam TVECT Container type whose elements are TSTR instances.
		/// @param fileName Path to the file to parse.
		/// @param tokens   Output container receiving the extracted tokens.
		/// @return true on success, false if the file could not be opened or parsing failed.
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

		/// @brief Parses a C-string and extracts all alphanumeric and numeric tokens into the provided container.
		/// @tparam TVECT Container type whose elements are TSTR instances.
		/// @param pStr   Null-terminated input string to parse.
		/// @param tokens Output container receiving the extracted tokens.
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
		/// @brief Parses a C-string and extracts numeric tokens, converting each to the container's element type.
		/// @tparam TVECT Container type whose elements are numeric values.
		/// @param pStr   Null-terminated input string to parse.
		/// @param tokens Output container receiving the parsed numeric values.
		/// @return true if only numeric tokens were encountered, false if an alphabetic token was found.
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

		/// @brief Sets the input buffer wrapper used for subsequent parsing operations.
		/// @param inputWrapper Pointer to a pre-constructed TINPBUFFER to use as input.
		void initBufferWrapper(TINPBUFFER* inputWrapper)
		{
			TBASE::pInput = inputWrapper;
		}

	protected:
		/// @brief Dispatches a single token to the appropriate callback handler method.
		/// @tparam TCALLBACK Callback type exposing handleAlphaNum, handleNum, and handleToken methods.
		/// @param t        Token type to dispatch.
		/// @param callBack Callback object receiving the parsed token.
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
		/// @brief Parses the entire content of str and delivers each token to callBack.
		/// @tparam TCALLBACK Callback type exposing handleInit, handleFinish, handleWhiteSpace,
		///                   handleAlphaNum, handleNum, and handleToken methods.
		/// @param str      Input string to parse.
		/// @param callBack Callback object receiving each parsed token event.
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

	typedef mu::SimpleParser<td::String, StringWrapper8, mu::ParserType::AlphaNumerical> AlphaParser; ///< Parser variant that recognises only alphabetic tokens.
	typedef mu::SimpleParser<td::String, StringWrapper8, mu::ParserType::C> AlphaNumParser; ///< Parser variant that recognises both alphabetic and numeric tokens in C style.
}
