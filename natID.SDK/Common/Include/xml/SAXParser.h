// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SAXParser.h
    @brief Template-based SAX-style XML parser that processes XML documents via callback methods. */
#pragma once
#include <cnt/List.h>
#include <td/String.h>
#include <fo/FileOperations.h>
#include <cnt/SafeFullVector.h>
#include <map>
#include <istream>
#include <sstream>
#include <mu/Tokenizer.h>
#include <cnt/Stack.h>
#include <cnt/HashList.h>
#include <mu/Utils.h>
#include <cnt/priv/HashBucket.h>
#include <mu/IAppSettings.h>
#include <mu/ILogger.h>
#include <mem/BufferReader.h>

//(bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf) ||
//(bom[0] == 0xff && bom[1] == 0xfe) ||
//(bom[0] == 0xfe && bom[1] == 0xff) ||
//(bom[0] == 0 && bom[1] == 0 && bom[2] == 0xfe && bom[3] == 0xff))

//bomDict={ # bytepattern : name
//(0x00, 0x00, 0xFE, 0xFF) : "utf_32_be",

//(0xFF, 0xFE, 0x00, 0x00) : "utf_32_le",
//(0xFF, 0xFE, None, None) : "utf_16_le",

//(0xFE, 0xFF, None, None) : "utf_16_be",
//(0xEF, 0xBB, 0xBF, None) : "utf_8",
//}

namespace xml
{
	/// @brief Template SAX XML parser that invokes TXML_CONSUMER callbacks during parsing.
	/// @tparam TXML_CONSUMER  Consumer/handler class that receives parser callbacks (CRTP).
	/// @tparam TBUFF          Buffer type used for input data reading.
	/// @tparam NODE_AND_ATTRIB_HASH_SIZE  Hash table size for node and attribute name lookup.
	/// @tparam FROM_MEMORY    True if parsing from a memory buffer, false if parsing from a file.
	/// @tparam SKIP_COMMENTS  True to discard XML comments (default), false to deliver them.
	template <class TXML_CONSUMER, class TBUFF, unsigned int NODE_AND_ATTRIB_HASH_SIZE, bool FROM_MEMORY, bool SKIP_COMMENTS = true>
	class SAXParser : public mu::Tokenizer<char, TBUFF, FileString8, mu::ParserType::Xml, FROM_MEMORY>
	{
	public:
        /// @brief XML file encoding detected from the byte-order mark.
        enum class FileEncoding : td::BYTE {
            UTF_32_BE, ///< UTF-32 big-endian encoding
            UTF_32_LE, ///< UTF-32 little-endian encoding
            UTF_16_BE, ///< UTF-16 big-endian encoding
            UTF_16_LE, ///< UTF-16 little-endian encoding
            UTF_8,     ///< UTF-8 encoding
            None       ///< No BOM detected; assume UTF-8 or ASCII
        };
		/// @brief Classification of an XML node based on its content.
        enum class NodeType : td::BYTE {
            Dummy=0,                    ///< Self-closing element with no attributes
            WithAttribs,                ///< Element that has attributes
            WithoutAttribs,             ///< Element without attributes
            NODE_WITH_ATTRIBS_AND_TEXT, ///< Element with both attributes and text content
            NODE_WO_ATTRIBS_AND_TEXT    ///< Element with text content but no attributes
        };
		typedef cnt::HashEntry<td::StringExt> tHashEntry; ///< Hash entry type for node/attribute names.
		typedef mu::Tokenizer<char, TBUFF, FileString8, mu::ParserType::Xml, FROM_MEMORY> TBASE; ///< Convenience alias for the base tokenizer type.
    private:
        cnt::Stack< const tHashEntry *, 16> _processingNodes; ///< Stack of currently open node hash entries.

	protected:
		bool _callOnNodeTextForWhitespace = false; ///< If true, fire onNodeText even for whitespace-only text.
		mu::ParserException _lastException; ///< Stores the most recent parser exception for error reporting.
		const tHashEntry* _pLastNode = nullptr; ///< Hash entry of the most recently opened node.
		const tHashEntry* _pLastAttrib = nullptr; ///< Hash entry of the most recently encountered attribute.
		cnt::HashList< cnt::HashBucket< tHashEntry>, td::StringExt, NODE_AND_ATTRIB_HASH_SIZE>  _nodeAndAttribHash; ///< Combined hash table for node and attribute name lookups.
		std::ifstream _inputFile; ///< File stream used when parsing from a file.
		int _nNodesOnStack = 0; ///< Number of currently open nodes on the processing stack.
        FileEncoding _fileEncoding; ///< Detected encoding of the input file.
		td::BYTE _rootDetected = 0; ///< Non-zero once the root element opening tag has been seen.
		//bool nodeJustOpened;
		td::BYTE _endTextDetected = 0; ///< Non-zero when trailing content after the root closing tag is detected.

		td::BYTE _makeFullCallOnDummyNode = 1; ///< When non-zero, emit onOpenNode/onCloseNode callbacks for self-closing elements.

	protected:
		/// @brief Returns a pointer to the CRTP-derived consumer object.
		/// @return Pointer to the TXML_CONSUMER instance.
		inline TXML_CONSUMER* consumer()
		{
			return static_cast<TXML_CONSUMER*>(this);
		}

		/// @brief Looks up a node hash entry by its null-terminated name string.
		/// @param pStr  Null-terminated node name to look up.
		/// @return Pointer to the matching hash entry, or nullptr if not found.
		inline const tHashEntry* getNodeHashEntry(const char* pStr) const
		{
			unsigned int hash = mu::Utils::calcHashNo(pStr);
			return _nodeAndAttribHash.find(hash, pStr);
		}

		/// @brief Looks up a node hash entry by its pre-computed hash number.
		/// @param hashNo  Hash number to look up.
		/// @return Pointer to the matching hash entry, or nullptr if not found.
		inline const tHashEntry* getNodeHashEntry(td::UINT4 hashNo) const
		{
			return _nodeAndAttribHash.find(hashNo);
		}

		/// @brief Looks up an attribute hash entry by its null-terminated name string.
		/// @param pStr  Null-terminated attribute name to look up.
		/// @return Pointer to the matching hash entry, or nullptr if not found.
		inline const tHashEntry* getAttribHashEntry(const char* pStr) const
		{
			unsigned int hash = mu::Utils::calcHashNo(pStr);
			return _nodeAndAttribHash.find(hash, pStr);
		}

		/// @brief Reads the byte-order mark from the input and sets _fileEncoding accordingly.
		inline void detectFileEncoding()
		{
			unsigned char ch = (unsigned char) *TBASE::it;
			if (ch == 0x00)
			{
				//UTF_32_BE
				//(0x00, 0x00, 0xFE, 0xFF) : "utf_32_be",
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0x00)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0xFE)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0xFF)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");
				++TBASE::it;
				_fileEncoding = FileEncoding::UTF_32_BE;
			}
			else if(ch == 0xFF)
			{
				//UTF_32_LE or UTF_16_LE
				//(0xFF, 0xFE, 0x00, 0x00) : "utf_32_le",
				//(0xFF, 0xFE, None, None) : "utf_16_le",
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0xFE)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0x00)
				{
					_fileEncoding = FileEncoding::UTF_16_LE;
					return;
				}

				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0x00)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");
				++TBASE::it;
				_fileEncoding = FileEncoding::UTF_32_LE;

			}
			else if (ch == 0xEF)
			{
				//UTF_8
				//(0xEF, 0xBB, 0xBF
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0xBB)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0xBF)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");

				++TBASE::it;
				_fileEncoding = FileEncoding::UTF_8;
			}
			else if (ch == 0xFE)
			{
				//(0xFE, 0xFF, None, None) : "utf_16_be",
				++TBASE::it;
				ch = (unsigned char) *TBASE::it;
				if (ch != 0xFF)
					throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");

				++TBASE::it;
				_fileEncoding = FileEncoding::UTF_8;
			}
			else
			{
				_fileEncoding = FileEncoding::None;
			}
		}

		/// @brief Parses the XML declaration header (<?xml version="1.0" ...?>).
		inline void parseHeader()
		{
			//detect file encoding
			if (!FROM_MEMORY)
				TBASE::reload();
			if (TBASE::_pInput->size() < 10)
			{
				throw mu::ParserException(-1,-1, mu::ExceptionType::NotAnXMLFile, "Not an XML file");
			}
			detectFileEncoding();

			if (_fileEncoding != FileEncoding::UTF_8 && _fileEncoding != FileEncoding::None)
				throw mu::ParserException(-1,-1, mu::ExceptionType::UnsupportedFileEncoding, "Unsupported file encoding");

			typename TBASE::Token t = TBASE::template getNextToken<false>();
			if (t != TBASE::Token::T_LT)
				throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong header file");

			int pos = 0;

			do
			{
				while (TBASE::it != TBASE::itEnd)
				{
					switch(pos)
					{
					case 0:
						//<?xml
						if (*TBASE::it != '?')
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header");

						TBASE::moveNext();

						if (*TBASE::it != 'x')
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header");

						TBASE::template parseName<false, true>();

						if ( strcmp(TBASE::_outBuffer.c_str(), "xml") !=0)
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header");
						++pos;
						continue;
						break;

					case 1:
						//version
						t = TBASE::template getNextToken<false>();
						if (t != TBASE::Token::T_ALPHANUM)
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "XML version expected");
						TBASE::template parseName<false, true>();
						if (strcmp(TBASE::_outBuffer.c_str(), "version") != 0 )
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "XML version expected");
						++pos;
						continue;
						break;
					case 2:
						//=
						t = TBASE::template getNextToken<false>();
						if (t != TBASE::Token::T_EQUAL)
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header: Expected '=' after 'version'");
						++pos;
						continue;
					case 3:
						//"1.0"
						t = TBASE::template getNextToken<false>();
						if ( (t != TBASE::Token::T_SQUOTE) && (t != TBASE::Token::T_DQUOTE) )
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header: Expected version number");
						TBASE::parseQuotedString(t);
						if ( strcmp(TBASE::_outBuffer.c_str(), "1.0") != 0 )
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Unsupported XML version");
						++pos;
						continue;
					case 4:
						//either ?> or encoding
						t = TBASE::template getNextToken<false>();
						if (t == TBASE::Token::T_QUESTION)
						{
							// ?>
							if (*TBASE::it != '>')
								throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header: Expected xml header closing sign '>'");
							TBASE::moveNext();
							return;
						}
						else if (t == TBASE::Token::T_ALPHANUM)
						{
							//encoding
							TBASE::template parseName<false, true>();
							if ( strcmp(TBASE::_outBuffer.c_str(), "encoding") != 0 )
								throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Unsupported XML version");
							++pos;
							continue;
						}
						throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header");
					case 5:
						//=
						t = TBASE::template getNextToken<false>();
						if (t != TBASE::Token::T_EQUAL)
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header: Expected '=' after 'version'");
						++pos;
						continue;
					case 6:
						//"UTF-8"
						//the only supported encoding is UTF-8
						t = TBASE::template getNextToken<false>();
						if ( (t != TBASE::Token::T_SQUOTE) && (t != TBASE::Token::T_DQUOTE) )
							throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header: Expected version number");

						TBASE::parseQuotedString(t);

						TBASE::_outBuffer.toUpper();

						if (strcmp(TBASE::_outBuffer.c_str(), "UTF-8") != 0)
						{
							if (strcmp(TBASE::_outBuffer.c_str(), "UTF8") != 0)
								throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Unsupported XML encoding");
						}

						++pos;
						continue;
					case 7:
						//either ?> or encoding
						t = TBASE::template getNextToken<false>();
						if (t == TBASE::Token::T_QUESTION)
						{
							// ?>
							if (*TBASE::it != '>')
								throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header: Expected xml header closing sign '>'");
							TBASE::moveNext();
							return;
						}
						throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header: Expected xml header closing sign '?'");
						return;
					default:
						throw mu::ParserException(-1,-1, mu::ExceptionType::WrongHeader, "Wrong XML header");
					}
				}
				TBASE::checkBuffer();
			}while(true);
		}


		/// @brief Parses a closing element tag (</name>) and invokes onCloseNode on the consumer.
		inline void parseEndNode()
		{
			unsigned int hashID = TBASE::template parseName<true, true>();
			if (*TBASE::it != '>')
				throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token");
			if (_pLastNode->pName)
			{
				if (((unsigned int)_pLastNode->hashNo != hashID) || (_pLastNode->pName->length() != (td::UINT4) TBASE::_outBuffer.length()))
				{
					td::String str;
					str.format("Unexpected closing element name: '%s'. Expected: '%s'", TBASE::_outBuffer.c_str(), _pLastNode->pName->c_str());
					throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, str.c_str());
				}
			}
			else if (_pLastNode->hashNo != hashID)
			{
				td::String str;
				str.format("Unexpected closing element name with hash: '%s'!", TBASE::_outBuffer.c_str());
				throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, str.c_str());
			}

			consumer()->onCloseNode();

			_processingNodes.pop();
			--_nNodesOnStack;
			if (_nNodesOnStack > 0)
			{
				_pLastNode = _processingNodes.top();
				//std::cout << "Closing node :" << outBuffer.c_str() << std::endl;
				//std::cout << "          Node on stack: ";
				//pLastNode->show(std::cout);
			}
			else
			{
				_pLastNode = nullptr;
			}

			TBASE::moveNext();
		}

		/// @brief Parses text content between element tags and invokes onNodeText on the consumer.
		inline void parseNodeText()
		{
			//do
			//{
			TBASE::parseXMLText();
			//	if (*it == '!')
			//	{
			//		moveNext();
			//		if (*it == '-')
			//		{
			//			moveNext();
			//			if (*it == '-')
			//			{
			//				skipComments(2, "--");
			//				if (*it != '>')
			//				{
			//					throw ParserException(nLines, 0, E_WRONG_TOKEN, "Comments not closed! Expected '>'");
			//				}
			//				moveNext();
			//				if (*it )
			//				continue; //continue parsing text
			//			}
			//		}
			//	}
			//} while (true);
			//std::cout << "XML Text: " << outBuffer.c_str() << std::endl;
			consumer()->onNodeText();
		}

		//typename mu::Tokenizer<char, TBUFF, FileString8,  XML_PARSER, FROM_MEMORY>::Token
		/// @brief Parses all attributes of the current opening element tag.
		/// @return The terminating token (T_SLASH for self-closing, T_GT for end of opening tag).
		inline typename mu::Tokenizer<char, TBUFF, FileString8, mu::ParserType::Xml, FROM_MEMORY>::Token parseNodeAttributes()
		{
			do
			{
				typename TBASE::Token t = TBASE::template getNextToken<false>();
				if (t == TBASE::Token::T_ALPHANUM)
				{
					unsigned int hashID = TBASE::template parseName<true, true>();

					_pLastAttrib = _nodeAndAttribHash.add(hashID, TBASE::_outBuffer.length(), TBASE::_outBuffer.c_str(), consumer());
					//pLastAttrib = nodeAndAttribHash.add(hashID, TBASE::_outBuffer.c_str());
					//std::cout << "Attribute detected:" << outBuffer.c_str() << std::endl;

					t = TBASE::template getNextToken<false>();
					if (t != TBASE::Token::T_EQUAL)
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token");
					t = TBASE::template getNextToken<false>();
					if ( (t != TBASE::Token::T_SQUOTE) && (t!=TBASE::Token::T_DQUOTE))
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token");
					TBASE::parseQuotedString(t);
					//std::cout << "Attribute value:" << outBuffer.c_str() << std::endl;
					consumer()->onAttrib();
				}
				else if ( (t == TBASE::Token::T_SLASH) || (t == TBASE::Token::T_GT))
				{
					return t;
				}
				else
					throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token");

			} while (true);
		}

		/// @brief Opens a node: registers it in the hash, fires onOpenNode, and pushes it onto the stack.
		/// @tparam nodeType  The structural type of this node.
		/// @param hashID     Pre-computed hash of the node name.
		template<NodeType nodeType>
		inline void openNode(unsigned int hashID)
		{
			//std::cout << hashID << " " << outBuffer.c_str() << std::endl;
			//nodeJustOpened = true;
			//pLastNode = nodeHash.add(hashID, outBuffer.length(), outBuffer.c_str());
            const tHashEntry* pOldLastNode = _pLastNode;
            _pLastNode = _nodeAndAttribHash.add(hashID, TBASE::_outBuffer.length(), TBASE::_outBuffer.c_str(), consumer());
			//bool bOk = false;
			consumer()->onOpenNode();
			if (_makeFullCallOnDummyNode != 0)
            {
                //std::cout << "          Added node2: ";
                //pLastNode->show(std::cout);
                _processingNodes.push(_pLastNode);
                ++_nNodesOnStack;
                if (_rootDetected == 0)
                    _rootDetected = 1;
            }
            else
            {
                _pLastNode = pOldLastNode;
            }
		}

		/// @brief Parses an opening element tag (including its attributes and any inline text).
		inline void parseNode()
		{
			unsigned int hashID = TBASE::template parseName<true, true>();
			char ch = *TBASE::it;
			//if (isWhiteSpace(ch) || isNewLine(ch))
			//{
			//	TBASE::template skipWhiteSpaces<false> ();
			//	ch = *TBASE::it;
			//}
			if (ch != '>')
			{
				//check for dummy node
				//if (*TBASE::it == '/')
				if (ch == '/')
				{
					TBASE::moveNext();
					ch = *TBASE::it;
					//if (*TBASE::it != '>')
					if (ch != '>')
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token");

					//pLastNode = nodeHash.add(hashID, outBuffer.length(), outBuffer.c_str());

					//nodeHash.add(hashID, TBASE::_outBuffer.c_str());

					if (_makeFullCallOnDummyNode != 0)
						openNode<NodeType::WithAttribs>(hashID);

					consumer()->onDummyNode();

					if (_makeFullCallOnDummyNode != 0)
					{
						consumer()->onCloseNode();
						--_nNodesOnStack;

						_processingNodes.pop();
						if (_nNodesOnStack > 0)
						{
							_pLastNode = _processingNodes.top();
						}
						else
						{
							_pLastNode = nullptr;
						}
					}

					TBASE::moveNext();

					//ch = *TBASE::it;

					//processingNodes.pop();
					//pLastNode = processingNodes.top();
					return;
				}

				//std::cout << "Starting node with attribs: " << outBuffer.c_str() << std::endl;
				//consumer->onOpenNode();

				openNode<NodeType::WithAttribs>(hashID);

				typename TBASE::Token t = parseNodeAttributes();

				if (t == TBASE::Token::T_SLASH)
				{
					//moveNext();
					if (*TBASE::it == '>')
					{
						//std::cout << "Closed node with attribs. Node: " << outBuffer.c_str() << std::endl;
						consumer()->onCloseNode();
						--_nNodesOnStack;

						_processingNodes.pop();
						if (_nNodesOnStack > 0)
						{
							_pLastNode = _processingNodes.top();
						}
						else
						{
							_pLastNode = nullptr;
						}
						TBASE::moveNext();
						//openNode<NODE_WITH_ATTRIBS>();
					}
					else
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token! Expected '>'");

				}
				else if (t == TBASE::Token::T_GT)
				{
					if (*TBASE::it == '>')
						TBASE::moveNext();

					TBASE::_outBuffer.erase();
					parseNodeText();
					return;
				}
				else
					throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token");
			}
			else
			{
				//std::cout << "Open node wo attribs: " << outBuffer.c_str() << std::endl;
				//consumer->onOpenNode();
				openNode<NodeType::WithoutAttribs>(hashID);
				TBASE::_outBuffer.erase();

				if (*TBASE::it == '>')
					TBASE::moveNext();

				parseNodeText();
			}
		}

		//<?nesto x="a"?>
		/// @brief Parses an XML processing instruction (<?target attr="val"?>).
		inline void parseWorkingInstruction()
		{
			TBASE::moveNext();
			if (!TBASE::isAlpha(*TBASE::it))
				throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token. Expected working instructions name");
			TBASE::template parseName<false, true>();
			//onWorkingInstructionName
			while(true)
			{
				typename TBASE::Token t = TBASE::template getNextToken<false>();
				if (t == TBASE::Token::T_ALPHANUM)
				{
					TBASE::template parseName<false, false>();
					//onWIAttribName
					t = TBASE::template getNextToken<false>();
					if (t != TBASE::Token::T_EQUAL)
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token while parsing working instructions attribute. Expected '='");
					t = TBASE::template getNextToken<false>();
					if ( (t != TBASE::Token::T_SQUOTE) && (t != TBASE::Token::T_DQUOTE) )
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token while parsing working instructions attribute. Expected '\"' or \"'\"");
					TBASE::parseQuotedString(t);
					//onWIAttribValue
					continue;
				}
				else if (t == TBASE::Token::T_QUESTION)
				{
					if (*TBASE::it != '>')
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token while parsing working instructions attribute. Expected '>'");
					return;
				}
				throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token while parsing working instructions name");
			}
		}

		//returns true if comment was inside
		/// @brief Parses a construct starting with '!' (comment, CDATA section, or DOCTYPE).
		inline void parseExclamation()
		{
			TBASE::moveNext();
			//comments
			if (*TBASE::it == '-')
			{
				TBASE::moveNext();
				if (*TBASE::it == '-')
				{
					TBASE::moveNext();
					TBASE::template skipComments<!SKIP_COMMENTS>(2, "--");
					if constexpr (!SKIP_COMMENTS)
					{
						consumer()->onNodeComment();
					}
					if (*TBASE::it != '>')
					{
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token! Comments are not closed with '>'");
					}
					TBASE::moveNext();
					return;
					//std::cout << "Comments: " << outBuffer.c_str() << std::endl;
				}
			}
			else if (*TBASE::it == '[')
			{
				TBASE::moveNext();
				TBASE::template parseName<false, true>();
				if ( strcmp(TBASE::_outBuffer.c_str(), "CDATA") == 0 )
				{
					if (*TBASE::it == '[')
					{
						TBASE::moveNext();
						TBASE::template skipComments<true>(3, "]]>");
						//std::cout << "CDATA: " << outBuffer.c_str() << std::endl;
						consumer()->onCDATA();
						return;
					}
				}
			}
            else if (*TBASE::it == 'D')
            {
                TBASE::moveNext();
                TBASE::template parseName<false, true>();
                if ( strcmp(TBASE::_outBuffer.c_str(), "OCTYPE") == 0 )
                {
                    TBASE::moveNext();
                    TBASE::template skipComments<true>(1, ">");
                    //std::cout << "CDATA: " << outBuffer.c_str() << std::endl;
                    consumer()->onDocType();
                    return;
                }
            }
			throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token");
		}

		/// @brief Skips XML comments that appear after the root closing tag.
		inline void skipCommentsTrailingComments()
		{
			TBASE::moveNext();
			//comments
			if (*TBASE::it == '-')
			{
				TBASE::moveNext();
				if (*TBASE::it == '-')
				{
					TBASE::moveNext();
					TBASE::template skipComments<false>(2, "--");
					if (*TBASE::it != '>')
					{
						throw mu::ParserException(TBASE::_nLines, 0, mu::ExceptionType::WrongToken, "Unexpected token! Trailing comments are not closed with '>'");
					}
					_endTextDetected = 0;
					TBASE::moveNext();
					return;
					//std::cout << "Comments: " << outBuffer.c_str() << std::endl;
				}
			}
		}

		/// @brief Main parsing loop that processes the entire XML document body after the header.
		inline void parseXMLContent()
		{
			_rootDetected = 0;
			typename TBASE::Token t = TBASE::Token::T_NOT_INITIALIZED;

			while ((_nNodesOnStack != 0) || (_rootDetected==0))
			{
				//if (nodeJustOpened)
				//{
				t = TBASE::template getNextToken<true>();

				if (t == TBASE::Token::T_LT && _callOnNodeTextForWhitespace && (TBASE::_outBuffer.size() != 0))
				{
					consumer()->onNodeText();
				}

				if (t != TBASE::Token::T_LT)
				{
					parseNodeText();
					//nodeJustOpened = false;
					continue;
					//throw ParserException(-1,-1, E_WRONG_TOKEN, "Unexpected token");
				}
				else if (*TBASE::it == '!')
				{
					parseExclamation();
					//nodeJustOpened = false;
					continue;
				}
				//nodeJustOpened = false;
				//}
				//else
				//{
				//	t = getNextToken<false>();
				//	if (t != T_LT)
				//		throw ParserException(nLines,0, E_WRONG_TOKEN, "Unexpected token");
				//}

				char ch = *TBASE::it;

				if (TBASE::isAlpha(ch))
				{
					parseNode();
					continue;
				}
				else if (ch == '/')
				{
					TBASE::moveNext();
					if (TBASE::isAlpha(*TBASE::it))
					{
						parseEndNode();
						continue;
					}
					else // if (*it == '>')
					{
						throw mu::ParserException(TBASE::_nLines,0, mu::ExceptionType::WrongToken, "Unexpected token");
					}
				}
				else if (ch == '!')
				{
					parseExclamation();
				}
				else if (ch == '?')
				{
					parseWorkingInstruction();
				}
				else
					throw mu::ParserException(TBASE::_nLines,0, mu::ExceptionType::WrongToken, "Invalid element name");

			}
			//check part after the end of root
			try
			{
				do
				{
					_endTextDetected = 0;
					typename TBASE::Token t = TBASE::template getNextToken<false>();
					_endTextDetected = 1;
					if (t == TBASE::Token::T_LT)
					{
						if (*TBASE::it != '!')
						{
							break;
						}
						skipCommentsTrailingComments();
					}
					else
					{
						break;
					}
				} while (true);
			}
			catch (const mu::ParserException& e)
			{
				if (_endTextDetected != 0)
					throw e;
				return;  //end of file detected normaly
			}
			if (_endTextDetected != 0)
				throw mu::ParserException(TBASE::_nLines,0, mu::ExceptionType::TextAfterEnd, "Unexpected token at the end of xml");
		}

#pragma region writing
		/// @brief Writes a string value to the output buffer, optionally escaping XML special characters.
		/// @tparam convertSpecialCharacters  If true, escapes &, >, < and optionally " characters.
		/// @tparam checkQuoation             If true, also escapes double-quote characters.
		/// @param str  The string value to write.
		template <bool convertSpecialCharacters, bool checkQuoation>
		inline void writeValue(const td::String& str)
		{
			if (convertSpecialCharacters)
			{
				td::String::const_iterator itS = str.begin();
				td::String::const_iterator itSEnd = str.end();
				while (itS != itSEnd)
				{
					if (*itS == '&')
					{
						TBASE::_outBuffer.append(5, "&amp;");
						++itS;
						continue;
					}
					else if (*itS == '>')
					{
						TBASE::_outBuffer.append(4, "&gt;");
						++itS;
						continue;
					}
					else if (*itS == '<')
					{
						TBASE::_outBuffer.append(4, "&lt;");
						++itS;
						continue;
					}
					else
					{
						if (checkQuoation)
						{
							if (*itS == '"')
							{
								TBASE::_outBuffer.append(6, "&quot;");
								++itS;
								continue;
							}
						}
					}
					TBASE::_outBuffer += *itS;
					++itS;
				}
			}
			else
			{
				TBASE::_outBuffer.append(str.length(), str.c_str());
			}
		}
#pragma endregion
	public:
		/// @brief Constructs a SAXParser and initialises all tracking state to default values.
		inline SAXParser()
		    : _pLastNode(nullptr)
			, _pLastAttrib(nullptr)
		    , _nNodesOnStack(0)
		    , _rootDetected(0)
		    , _endTextDetected(0)
			//, consumer(0)
		{
			//nodeNames.reserve(NODEHASHSIZE);
			//attribNames.reserve(NODE_AND_ATTRIB_HASH_SIZE);
		}

		/// @brief Destructor; closes any open resources.
		inline ~SAXParser()
		{
			//if (TBASE::pInput)
				//delete TBASE::pInput;
		}

		/// @brief Returns true if the document was parsed successfully (root seen, all nodes closed, no trailing data).
		/// @return True on successful completion, false otherwise.
		inline bool isOk() const
		{
			return ((_rootDetected != 0) && (_nNodesOnStack == 0) && (_endTextDetected==0));
		}

		/// @brief Returns true if the root element opening tag has been encountered.
		/// @return True once the root element has been opened.
		inline bool isRootDetected() const
		{
			return (_rootDetected != 0);
		}

		/// @brief Returns true if the root element has been fully closed.
		/// @return True when root was detected and the node stack is empty.
		inline bool isEndOfXMLDetected() const
		{
			return (_rootDetected != 0) && (_nNodesOnStack == 0);
		}

		//template <class OSTREAM>
		//inline void showHashes(OSTREAM& o)
		//{
		//	o << "Node hashes:\n";
		//	nodeHash.show(o);
		//	o << "\n\nAttrib hashes:\n";
		//	attribHash.show(o);
		//	o << '\n';
		//}

		//template <class T_LOGSTREAM>
		/// @brief Parses an XML document from a null-terminated in-memory character string.
		/// @param pXmlData    Pointer to a null-terminated UTF-8 XML string.
		/// @param pLog        Optional logger for error messages.
		/// @param checkHeader If true, validates the XML declaration header before parsing content.
		/// @return True on success, false if a parse error occurred.
		inline bool parseMemory(const char* pXmlData, const mu::ILogger* pLog = nullptr, bool checkHeader = true)
		{
			static_assert(FROM_MEMORY, "Cannot call this method (parseMemory) on XmlFileParser");
			assert(FROM_MEMORY);

			if (pLog == nullptr)
			{
				auto pAppSet = mu::getAppSettings();
				if (pAppSet)
					pLog = pAppSet->getLogger();
			}

			//TBASE::pInput = new TBUFF;
            reset();

			TBUFF memInput;
			TBASE::_pInput = &memInput;
			TBASE::_pInput->setBufferAsInput(pXmlData);
			TBASE::initIterators();

			try
			{
				if (checkHeader)
					parseHeader();
				parseXMLContent();
			}
			catch(const mu::ParserException& e)
			{
				if (isOk())
				{
					return true;
				}
				else
				{
                    _lastException = e;
					if (pLog)
					{
						pLog->show(mu::ILogger::MsgType::Error, "SAXParser::parseMemory", td::String(e.message));
					}
					//e.show(o, "");
					return false;
				}
			}

			return true;
		}

		/// @brief Parses an XML document from a td::String in-memory string.
		/// @param strXML      String containing a complete UTF-8 XML document.
		/// @param pLog        Optional logger for error messages.
		/// @param checkHeader If true, validates the XML declaration header before parsing content.
		/// @return True on success, false if a parse error occurred.
		inline bool parseMemory(const td::String& strXML, const mu::ILogger* pLog = nullptr, bool checkHeader = true)
		{
			if (pLog == nullptr)
			{
				auto pAppSet = mu::getAppSettings();
				if (pAppSet)
					pLog = pAppSet->getLogger();
			}

			bool toRet = parseMemory(strXML.c_str(), pLog, checkHeader);
			//consumer->onParseFinished();
			return toRet;
		}

        /// @brief Resets all parser state so the object can be reused for another document.
        void reset()
        {
            TBASE::reset();
            _nNodesOnStack = 0;
            _rootDetected = 0;
            //bool nodeJustOpened;
            _endTextDetected = 0;
            _pLastNode = nullptr;
            _pLastAttrib = nullptr;
            _nodeAndAttribHash.reset();
            _processingNodes.reset();
        }

        /// @brief Parses an XML document from a mem::Buffer in-memory buffer.
        /// @param buffXML     Memory buffer containing a complete UTF-8 XML document.
        /// @param pLog        Optional logger for error messages.
        /// @param checkHeader If true, validates the XML declaration header before parsing content.
        /// @return True on success, false if a parse error occurred.
        inline bool parseMemory(mem::Buffer& buffXML, const mu::ILogger* pLog = nullptr, bool checkHeader = true)
        {
            static_assert(FROM_MEMORY, "Cannot call this method (parseMemory) on XmlFileParser");
            assert(FROM_MEMORY);

            if (pLog == nullptr)
            {
                auto pAppSet = mu::getAppSettings();
                if (pAppSet)
                    pLog = pAppSet->getLogger();
            }

            reset();

            mem::BufferReader buffReader(&buffXML);
            //TBASE::pInput = new TBUFF;
            TBUFF memInput;
            TBASE::_pInput = &memInput;
            TBASE::_pInput->setMemBufferAsInput(&buffReader);
            TBASE::initIterators();

            try
            {
                if (checkHeader)
                    parseHeader();
                parseXMLContent();
            }
            catch(const mu::ParserException& e)
            {
                if (isOk())
                {
                    return true;
                }
                else
                {
                    if (pLog)
                    {
                        pLog->show(mu::ILogger::MsgType::Error, "SAXParser::parseMemory", td::String(e.message));
                    }
                    //e.show(o, "");
                    return false;
                }
            }

            return true;
        }

//        inline bool parseMemory(const mem::Buffer& buffXML, const mu::ILogger* pLog = nullptr, bool checkHeader = true)
//        {
//            if (pLog == nullptr)
//            {
//                auto pAppSet = mu::getAppSettings();
//                if (pAppSet)
//                    pLog = pAppSet->getLogger();
//            }
//
//            bool toRet = parseMemory(xmlData.c_str(), pLog, checkHeader);
//            //consumer->onParseFinished();
//            return toRet;
//        }


		//inline bool parseFile(const td::String& fileName, const mu::ILogger* pLog = nullptr)
		//{
		//	return parseFile(fileName);
		//}

		//template <class T_LOGSTREAM>
		/// @brief Opens and parses an XML file from disk.
		/// @param fileName  Path to the XML file to parse.
		/// @param pLog      Optional logger for error messages.
		/// @return True on success, false if the file could not be opened or a parse error occurred.
		inline bool parseFile(const td::String& fileName, const mu::ILogger* pLog = nullptr)
		{
			static_assert(!FROM_MEMORY, "Cannot call this method (parseFile) on XmlMemoryParser!");
			assert(!FROM_MEMORY);

			if (pLog == nullptr)
			{
				mu::IAppSettings* pAppSet = mu::getAppSettings();
				if (pAppSet)
				{
					pLog = pAppSet->getLogger();
				}
			}

			if (_inputFile.is_open())
				_inputFile.close();

			if (!fo::openFile(_inputFile, fileName, FO_BINARY_OPEN_EXISTING))
			{
				//throw ParserException(-1,-1, E_CANNOT_OPEN_FILE, fileName.c_str());
                cnt::StringBuilderSmall sb;
                sb << "Cannot open xml file: " << fileName.c_str();
				if (pLog)
				{
                    pLog->show(mu::ILogger::MsgType::Error, "XmlFileParser::parseFile", sb.toString());
				}
				mu::dbgLog("XmlFileParser::parseFile! Cannot open xml file: %s", fileName.c_str());
                _lastException.lineNo = 0;
                _lastException.charNumber = 0;
                _lastException.message = sb.toString();
				return false;
			}

			//if (TBASE::pInput)
				//delete TBASE::pInput;

			//if (!TBASE::pInput)
            reset();

			TBUFF memInput;
			TBASE::_pInput = &memInput;

			TBASE::_pInput->setFileAsInput(&_inputFile);
			TBASE::initIterators();
			try
			{
				parseHeader();
				parseXMLContent();
			}
			catch(const mu::ParserException& e)
			{
				if (isOk())
				{
                    //consumer->onParseFinished(true);
                    _inputFile.close();
					return true;
				}
				else
				{
                    _inputFile.close();
					_lastException = e;
					if (pLog)
					{
						std::stringstream ss;
						ss << "Cannot parse xml file: " << fileName.c_str() << "\n" << e.message;
						pLog->show(mu::ILogger::MsgType::Error, "XmlFileParser::parseFile", td::String(ss.str()));
					}
					mu::dbgLog("XmlFileParser::parseFile! Cannot parse xml file: %s", fileName.c_str());

					//if (pO != nullptr)
					//	e.show(*pO, fileName.c_str());
					//consumer->onParseFinished(false);
					return false;
				}
			}
			catch (td::ZStringUTF8& str)
			{
				if (pLog)
				{
					std::stringstream ss;
					ss << "Error parsing xml file: " << fileName.c_str() << td::ENDL << str.c_str();
					pLog->show(mu::ILogger::MsgType::Error, "XmlFileParser::parseFile", td::String(ss.str()));
				}

				mu::dbgLog("XmlFileParser::parseFile! Error parsing xml file: %s -> %s", fileName.c_str(), str.c_str());

				//if (pO)
				//	*pO << str.c_str() << td::ENDL;
				//consumer->onParseFinished(false);
                _inputFile.close();
				return false;
			}
			catch (...)
			{
                _inputFile.close();
				if (pLog)
				{
					std::stringstream ss;
					ss << "Unexpected exception while parsing while parsing xml file: " << fileName.c_str();
					pLog->show(mu::ILogger::MsgType::Error, "XmlFileParser::parseFile", td::String(ss.str()));
				}

				mu::dbgLog("XmlFileParser::parseFile! Unexpected exception while parsing while parsing xml file:  %s", fileName.c_str());

				//if (pO)
					//*pO << "Unexpected exception while parsing!!!\n" << td::ENDL;
				//consumer->onParseFinished(false);
				return false;
			}

			//consumer->onParseFinished(true);
			return true;
		}

        /// @brief Opens and parses an XML file from a filesystem path object.
        /// @param filePath  Filesystem path to the XML file.
        /// @param pLog      Optional logger for error messages.
        /// @return True on success, false on error.
        inline bool parseFile(const fo::fs::path& filePath, const mu::ILogger* pLog = nullptr)
        {
            td::String strFilePath(filePath.string());
            return parseFile(strFilePath, pLog);
        }

		/// @brief Writes a human-readable description of the last error to an output stream.
		/// @tparam OSTREAM  Output stream type.
		/// @param o         Output stream to write the error description to.
		/// @param fileName  Name of the file that was being parsed (for context in the message).
		template<class OSTREAM>
		void showLastError(OSTREAM& o, const char* fileName) const
		{
			_lastException.show(o, fileName);
		}

        /// @brief Returns the error message from the last failed parse operation.
        /// @return Const reference to the last error message string.
        const td::String& getLastError() const
        {
            return _lastException.message;
        }

        /// @brief Returns a formatted error string that includes the line number when available.
        /// @return Formatted error string combining line number and message.
        const td::String getError() const
        {
            if (_lastException.lineNo >= 0)
            {
                td::String str;
                str.format("Line no=%d: %s", _lastException.lineNo, _lastException.message.c_str());
                return str;
            }
            return _lastException.message;
        }

        /// @brief Returns the 1-based line number of the last parse error.
        /// @return Line number where the last error occurred (1-based).
        int getLastErrorLineNo() const
        {
            return _lastException.lineNo+1;
        }
	};
};
