// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DOMParser.h
 *  @brief Document Object Model (DOM) XML parser built on top of SAXParser.
 */
#pragma once
#include <td/String.h>
#include <xml/SAXParser.h>
#include <cnt/SafeFullVector.h>
#include <cnt/List.h>
#include <td/Converter.h>
#include <mu/EnumSerializerManager.h>
#include <td/Variant.h>
#include <td/VariantDesc.h>
#include <cnt/StringBuilder.h>
#include <mem/StringMemoryManager.h>
#include <mem/CntMemoryManager.h>
#include <td/ColorID.h>
#include <td/LinePattern.h>
#include <td/Decimal.h>

namespace xml
{

/// @brief DOM parser template that builds an in-memory tree from XML input using a SAX-based backend.
/// @tparam TBUFF Buffer type used for input reading.
/// @tparam NODE_AND_ATTRIB_HASH_SIZE Size of the combined node/attribute hash table.
/// @tparam FROM_MEMORY If true, parses from an in-memory string; otherwise from a file.
template < class TBUFF, unsigned int NODE_AND_ATTRIB_HASH_SIZE, bool FROM_MEMORY >
class DOMParser : public SAXParser < DOMParser< TBUFF, NODE_AND_ATTRIB_HASH_SIZE, FROM_MEMORY >, TBUFF, NODE_AND_ATTRIB_HASH_SIZE, FROM_MEMORY >
{
    friend class node_iterator;

protected:
    typedef SAXParser < DOMParser< TBUFF, NODE_AND_ATTRIB_HASH_SIZE, FROM_MEMORY >, TBUFF, NODE_AND_ATTRIB_HASH_SIZE, FROM_MEMORY > TSAX;
    typedef DOMParser< TBUFF, NODE_AND_ATTRIB_HASH_SIZE, FROM_MEMORY > TDOM;
public:

    //typedef typename TSAX::tHashEntry Naming;
    typedef const typename TSAX::tHashEntry* PNaming;
    /// @brief Represents a single XML attribute with its name hash entry and string value.
    class Attrib
    {
    public:
        PNaming pNaming; ///< Pointer to the hash entry holding the attribute name.
        td::StringExt value; ///< The attribute value string.
        Attrib(){}

        /// @brief Returns the total usage count of this attribute name across the document.
        /// @return Total entry count from the hash entry.
        unsigned int getTotalCounter() const
        {
            return pNaming->nEntries;
        }

        /// @brief Returns the hash number of this attribute name.
        /// @return Hash number stored in the naming entry.
        unsigned int getHashNo() const
        {
            return pNaming->hashNo;
        }

        /// @brief Returns the length of the attribute name string.
        /// @return Length of the name string in characters.
        int getNameLength() const
        {
            return pNaming->pName->length();
        }

        /// @brief Returns the attribute name as a string reference.
        /// @return Const reference to the attribute name string.
        const td::StringExt& getName() const
        {
            return *(pNaming->pName);
        }

        /// @brief Returns the attribute value as a string reference.
        /// @return Const reference to the attribute value string.
        const td::StringExt& getValue() const
        {
            return value;
        }
        //void operator = (const Attrib&)
        //{
        //	//do nothing
        //}
    };

    typedef const Attrib* Attribute;

    /// @brief Represents a single XML element node containing child nodes, attributes, and text content.
    class Node
    {
    public:
        //const tHashEntry* pNaming;
        PNaming pNaming; ///< Pointer to the hash entry holding the element name.

        td::StringExt text; ///< Primary text content of this node.
        cnt::ListSL<td::StringExt, true> textLst; ///< List of all collected text segments (used when collectAllNodeTexts is enabled).
        cnt::ListSL<Node, true> nodes; ///< Singly-linked list of child nodes.

        //cnt::Stack<Attrib> attribs;
        //IDZ TODO
        //TODO, ubaci nesto umjesto PushBackVector-a. Neki own memory managed pushback vector
        //cnt::PushBackVector<Attrib> attribs;
        cnt::SafeFullVector<Attrib, true> attribs; ///< Vector of attributes belonging to this node.
        Node(){}

        /// @brief Returns the total usage counter of this node's name from the hash entry.
        /// @return Total entry count.
        unsigned int getTotalCounter() const
        {
            return pNaming->nEntries;
        }

        /// @brief Returns the hash number associated with this node's element name.
        /// @return Hash number.
        unsigned int getHashNo() const
        {
            return pNaming->hashNo;
        }

        /// @brief Returns the length of the element name string.
        /// @return Length in characters.
        int getNameLength() const
        {
            return pNaming->pName->length();
        }

        /// @brief Returns the element name as a string reference.
        /// @return Const reference to the element name.
        const td::StringExt& getName() const
        {
            return *(pNaming->pName);
        }

        /// @brief Returns the primary text content of this node.
        /// @return Const reference to the text string.
        const td::StringExt& getValue() const
        {
            return text;
        }

        /// @brief Concatenates all text segments collected in textLst into a single string.
        /// @return A new td::String containing all text segments joined together.
        td::String getWholeNodeString() const
        {
            td::UINT4 nLen = 0;
            {
                auto it = textLst.cbegin();
                auto itEnd = textLst.cend();
                while (it != itEnd)
                {
                    nLen += it->length();
                    ++it;
                }
            }

            if (nLen == 0)
                return td::String();

            td::String str;
            str.reserve(nLen);
            char* pToCopy = str.begin();

            auto it = textLst.cbegin();
            auto itEnd = textLst.cend();

            while (it != itEnd)
            {
                size_t nToCopy = it->length();
                memcpy(pToCopy, it->c_str(), nToCopy);
                pToCopy += nToCopy;
                ++it;
            }
            pToCopy[0] = 0;
            return str;
        }

        /// @brief Returns a raw C-string pointer to the node's text content.
        /// @return Pointer to the null-terminated text string.
        const char* getValueStrPtr() const
        {
            return text.c_str();
        }


        /// @brief Clears all child nodes and attributes, releasing their memory.
        void clean()
        {
            nodes.clean();
            attribs.clean();
        }

        /// @brief Serializes this node and all its children to a writer object.
        /// @tparam TWriter Writer type that accepts startNode/attribute/endNode calls.
        /// @param w The writer instance to write into.
        template <class TWriter>
        void toWriter(TWriter& w) const
        {
            w.startNode(getName());
            for (const Attrib& a: attribs)
            {
                w.attribute(a.getName(), a.getValue());
            }

            for (const Node& n: nodes)
            {
                n.toWriter(w);
            }
            w.endNode();
        }
        //pristup atributima ide preko NodeIteratora

        //const Attribute* getAttribute(const td::String& attributeName) const
        //{
        //	Stack<Attrib>::const_iterator it = attribs.begin();
        //	Stack<Attrib>::const_iterator itEnd = attribs.end();
        //	while (it != itEnd)
        //	{
        //		if (*it->getName() == attributeName)
        //			return it;
        //	}
        //	return 0;
        //}


        //void operator = (const Node&)
        //{
        //	//do nothing for operator =
        //}
    };

    //typedef typename cnt::Stack<Attrib>::const_iterator attrib_iterator;
    typedef typename cnt::SafeFullVector<Attrib, true>::const_iterator attrib_iterator;

    /// @brief Iterator for traversing child nodes in a DOM tree, optionally filtered by element name.
    class node_iterator
    {
    protected:
        //const tHashEntry* pNaming;
        PNaming pNaming; ///< Optional name filter; if set, only nodes matching this hash are visited.
        TDOM*	pParser; ///< Pointer to the owning parser instance.
        typename cnt::ListSL<Node, true>::const_iterator _it; ///< Underlying list iterator pointing at the current node.
    public:
        /// @brief Default constructor; creates an empty/invalid iterator.
        node_iterator()
            : pNaming(nullptr)
            , pParser(nullptr)
        {
        }

        /// @brief Copy constructor.
        /// @param it The node_iterator to copy from.
        node_iterator(const node_iterator& it)
            : pNaming(it.pNaming)
            , pParser(it.pParser)
            , _it(it._it)
        {
        }

        /// @brief Constructs from a raw list iterator without name filter or parser.
        /// @param itList The list iterator to wrap.
        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList)
            : pNaming(nullptr)
            , pParser(nullptr)
            , _it(itList)
        {
        }

        /// @brief Constructs from a list iterator and parser pointer.
        /// @param itList The list iterator to wrap.
        /// @param parser Pointer to the owning DOMParser.
        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList, TDOM* parser)
            : pNaming(nullptr)
            , pParser(parser)
            , _it(itList)
        {
        }

        /// @brief Constructs from a list iterator and a name filter entry.
        /// @param itList The list iterator to wrap.
        /// @param pName Pointer to the hash entry used as name filter.
        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList, PNaming pName)
            : pNaming(pName)
            , pParser(nullptr)
            , _it(itList)
        {
        }

        /// @brief Constructs from a list iterator, parser pointer, and name filter entry.
        /// @param itList The list iterator to wrap.
        /// @param parser Pointer to the owning DOMParser.
        /// @param pName Pointer to the hash entry used as name filter.
        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList, TDOM* parser, PNaming pName)
            : pNaming(pName)
            , pParser(parser)
            , _it(itList)
        {
        }

        /// @brief Assignment operator.
        /// @param it The node_iterator to assign from.
        void operator = (const node_iterator& it)
        {
            _it = it._it;
            pParser = it.pParser;
            pNaming = it.pNaming;
        }

        /// @brief Equality comparison operator.
        /// @param it The other iterator to compare with.
        /// @return True if both iterators point to the same list position.
        bool operator == (const node_iterator& it) const
        {
            return (_it == it._it);
        }

        /// @brief Inequality comparison operator.
        /// @param it The other iterator to compare with.
        /// @return True if the iterators point to different list positions.
        bool operator != (const node_iterator& it) const
        {
            return (it != it._it);
        }

        /// @brief Returns the beginning of the attribute collection of the current node.
        /// @return Const reference to the attribute iterator at the start.
        const attrib_iterator& getAttribs() const
        {
            return _it->attribs.begin();
        }

        /// @brief Finds an attribute on the current node by its hash entry pointer.
        /// @param name Pointer to the naming hash entry to search for.
        /// @return Pointer to the matching Attrib, or null if not found.
        Attribute getAttrib(PNaming name) const
        {
            attrib_iterator it(_it->attribs.begin());
            const attrib_iterator itE(_it->attribs.end());

            while (it != itE)
            {
                //if ( (it->getHashNo() == name->hashNo) && (it->getNameLength() == name->pName->length()) )
                if (it->getHashNo() == name->hashNo)
                {
#ifdef _DEBUG_SAX
                    assert (*(it->pNaming->pName) == *(name->pName));
#endif
                    return &(*it);
                }
                ++it;
            }
            return 0;
        }

        /// @brief Finds an attribute on the current node by its pre-computed hash number.
        /// @param hashNo The hash number of the attribute name to find.
        /// @return Pointer to the matching Attrib, or null if not found.
        inline Attribute getAttrib(td::UINT4 hashNo) const
        {
            attrib_iterator it = _it->attribs.begin();
            const attrib_iterator itE(_it->attribs.end());
            while (it != itE)
            {
                if (it->getHashNo() == hashNo)
                {
                    return &(*it);
                }
                ++it;
            }
            return 0;
        }

        //template <typename TINT>
        //bool getAttribValueInt(td::UINT4 hashNo, TINT& val) const
        //{
        //	Attribute a = getAttrib(hashNo);
        //	if (a)
        //	{
        //		val = td::toInt<TINT>(a->getValue().c_str());
        //		return true;
        //	}
        //	return false;
        //}

        /// @brief Retrieves a single-character attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param chVal Output character value.
        /// @return True if the attribute was found and the value is non-empty.
        bool getAttribValue(td::UINT4 hashNo, char& chVal) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                const auto& val = a->getValue();
                if (val.length() > 0)
                {
                    chVal = val.getAt(0);
                    if (chVal < ' ')
                        chVal = 0;
                }
                else
                    chVal = 0;
                return true;
            }
            return false;
        }

        /// @brief Retrieves a float attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output float value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, float& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toFloat();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a double attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output double value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, double& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toDouble();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a StringExt attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output StringExt value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::StringExt& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a boolean attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output boolean value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, bool& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toBoolean(); //td::toBoolean(a->getValue().c_str());
                return true;
            }
            return false;
        }

        /// @brief Retrieves a Date attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param dt Output Date value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::Date& dt)
        {
            return getAttribValueDateTime(hashNo, dt);
        }

        /// @brief Retrieves a Time attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param dt Output Time value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::Time& dt)
        {
            return getAttribValueDateTime(hashNo, dt);
        }

        /// @brief Retrieves a DateTime attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param dt Output DateTime value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::DateTime& dt)
        {
            return getAttribValueDateTime(hashNo, dt);
        }

        /// @brief Generic helper that retrieves a date/time typed attribute value by hash.
        /// @tparam TD A date/time type supporting fromString().
        /// @param hashNo Hash number of the attribute name.
        /// @param dt Output date/time value.
        /// @return True if the attribute was found and parsed successfully.
        template <class TD>
        bool getAttribValueDateTime(td::UINT4 hashNo, TD& dt)
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                dt.fromString(a->getValue().c_str());
                return true;
            }
            return false;
        }


        /// @brief Retrieves an unsigned 64-bit integer attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output LUINT8 value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::LUINT8& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toUint8();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a signed 64-bit integer attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output LINT8 value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::LINT8& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toInt8();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a signed 32-bit integer attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output INT4 value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::INT4& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toINT4();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a signed 16-bit integer attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output INT2 value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::INT2& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toINT2();
                return true;
            }
            return false;
        }

        /// @brief Retrieves an unsigned byte attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output BYTE value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::BYTE& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toBYTE();
                return true;
            }
            return false;
        }

        /// @brief Retrieves an unsigned 16-bit word attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output WORD value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::WORD& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toWORD();
                return true;
            }
            return false;
        }

        /// @brief Retrieves an unsigned 32-bit integer attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output UINT4 value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::UINT4& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                val = a->getValue().toUINT4();
                return true;
            }
            return false;
        }

        //bool getAttribValue(td::UINT4 hashNo, bool& val) const
        //{
        //	Attribute a = getAttrib(hashNo);
        //	if (a)
        //	{
        //		val = td::toBoolean(a->getValue().c_str());
        //		return true;
        //	}
        //	return false;
        //}
        //----------

        /// @brief Retrieves the enum serializer for a specific enum type and position.
        /// @tparam TENUM The enum type to look up.
        /// @param enumPos Position/index of the enum in the serializer manager.
        /// @return Pointer to the EnumSerializer for the given enum type.
        template <typename TENUM>
        const mu::EnumSerializer* getEnumSerializer(int enumPos) const
        {
            assert(pParser);
            assert(pParser->pEnumManager);
                            return  pParser->pEnumManager->template getEnum<TENUM>(enumPos);
        }

        /// @brief Retrieves an enum attribute value by hash, using the registered enum serializer.
        /// @tparam TENUM The enum type to deserialize into.
        /// @param hashNo Hash number of the attribute name.
        /// @param enumPos Position of the enum in the serializer manager.
        /// @param val Output enum value.
        /// @return True if the attribute was found and deserialized successfully.
        template <typename TENUM>
        bool getAttribEnumValue(td::UINT4 hashNo, int enumPos, TENUM& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                //val = td::toBoolean(a->getValue().c_str());
                //return true;
                assert(pParser);
                assert(pParser->pEnumManager);

                                    const mu::EnumSerializer* pEnum = pParser->pEnumManager->template getEnum<TENUM>(enumPos);
                assert(pEnum);
                val = pEnum->getValue<TENUM>(a->getValue());
                return true;
            }
            return false;
        }

        /// @brief Returns a pointer to the StringExt value of an attribute found by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @return Pointer to the attribute value string, or null if not found.
        const td::StringExt* getAttribValueStrPtr(td::UINT4 hashNo) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                return &(a->getValue());
            }
            return 0;
        }


        /// @brief Finds an attribute on the current node by its name string.
        /// @param name Null-terminated C-string attribute name.
        /// @return Pointer to the matching Attrib, or null if not found.
        Attribute getAttrib(const char* name) const
        {
            attrib_iterator it(_it->attribs.begin());
            const attrib_iterator itE(_it->attribs.end());
            while (it != itE)
            {
                if ( it->getName().cCompare(name) == 0 )
                    return &(*it);
                ++it;
            }
            return 0;
        }

        //template <typename TINT>
        //bool getAttribValueInt(const char* attribName, TINT& val) const
        //{
        //	Attribute a = getAttrib(attribName);
        //	if (a)
        //	{
        //		val = td::toInt<TINT>(a->getValue().c_str());
        //		return true;
        //	}
        //	return false;
        //}

        /// @brief Retrieves x/y float point coordinates from attributes named "x" and "y".
        /// @tparam TPOINT A point type with public float members x and y.
        /// @param val Output point value populated from x/y attributes.
        /// @return True if at least one coordinate attribute was found.
        template <typename TPOINT>
        bool getAttribPointValueFloat(TPOINT& val) const
        {
#ifdef _DEBUG_SAX
            //based on used hash algorythm
            assert(120 == mu::Utils::calcHashNo("x"));
            assert(121 == mu::Utils::calcHashNo("y"));
#endif
            //x
            Attribute a0 = getAttrib(120U);
            if (a0)
            {
                val.x = td::toFloat(a0->getValue().c_str());
                return true;
            }

            //y
            Attribute a1 = getAttrib(121U);
            if (a1)
            {
                val.y = td::toFloat(a1->getValue().c_str());
                return true;
            }
            return false;
        }

        /// @brief Retrieves a single-character attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param chVal Output character value.
        /// @return True if the attribute was found and the value is non-empty.
        bool getAttribValue(const char* attribName, char& chVal) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                const auto& val = a->getValue();
                if (val.length() > 0)
                {
                    chVal = val.getAt(0);
                    if (chVal < ' ')
                        chVal = 0;
                }
                else
                    chVal = 0;
                return true;
            }
            return false;
        }

        /// @brief Retrieves a float attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output float value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, float& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->getValue().toFloat();
                return true;
            }
            return false;
        }
        /// @brief Retrieves a complex<float> attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output complex float value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, std::complex<float>& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->getValue().toCmplxF();
                return true;
            }
            return false;
        }
        /// @brief Retrieves a complex<double> attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output complex double value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, std::complex<double>& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->getValue().toCmplx();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a ColorID attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output ColorID value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::ColorID& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                const char* colorName = a->getValue().c_str();
                val = td::toColorID(colorName);
                return true;
            }
            return false;
        }

        /// @brief Retrieves a ColorID attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output ColorID value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::ColorID& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                const char* colorName = a->getValue().c_str();
                val = td::toColorID(colorName);
                return true;
            }
            return false;
        }

        /// @brief Retrieves a LinePattern attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output LinePattern value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::LinePattern& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                const char* colorName = a->getValue().c_str();
                val = td::toLinePattern(colorName);
                return true;
            }
            return false;
        }

        /// @brief Retrieves a LinePattern attribute value by hash.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output LinePattern value.
        /// @return True if the attribute was found.
        bool getAttribValue(td::UINT4 hashNo, td::LinePattern& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                const char* colorName = a->getValue().c_str();
                val = td::toLinePattern(colorName);
                return true;
            }
            return false;
        }

        /// @brief Retrieves a Decimal attribute value by hash.
        /// @tparam TDECHOLDER Underlying storage type for the decimal.
        /// @tparam NDEC Number of decimal places.
        /// @param hashNo Hash number of the attribute name.
        /// @param val Output Decimal value.
        /// @return True if the attribute was found.
        template <typename TDECHOLDER, int NDEC>
        bool getAttribValue(td::UINT4 hashNo, td::Decimal<TDECHOLDER, NDEC>& val) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                const char* decVal = a->getValue().c_str();
                val.fromString(decVal);
                return true;
            }
            return false;
        }

        /// @brief Retrieves a Decimal attribute value by name string.
        /// @tparam TDECHOLDER Underlying storage type for the decimal.
        /// @tparam NDEC Number of decimal places.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output Decimal value.
        /// @return True if the attribute was found.
        template <typename TDECHOLDER, int NDEC>
        bool getAttribValue(const char* attribName, td::Decimal<TDECHOLDER, NDEC>& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                const char* decVal = a->getValue().c_str();
                val.fromString(decVal);
                return true;
            }
            return false;
        }

        /// @brief Retrieves a double attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output double value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, double& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->getValue().toDouble();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a StringExt attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output StringExt value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::StringExt& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->getValue();
                //val = tmp.clone();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a td::String attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output td::String value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::String& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                td::StringExt valExt = a->getValue();
                //val = tmp.clone();
                val = valExt;
                return true;
            }
            return false;
        }

        /// @brief Retrieves a fixed-size char array attribute value by name string.
        /// @tparam N Maximum number of characters for the SmallChFix buffer.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output SmallChFix value.
        /// @return True if the attribute was found.
        template <size_t N>
        bool getAttribValue(const char* attribName, td::SmallChFix<N>& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                td::StringExt valExt = a->getValue();
                //val = tmp.clone();
                val = valExt.c_str();
                return true;
            }
            return false;
        }



        /// @brief Retrieves a ZStringUTF8 attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output ZStringUTF8 value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::ZStringUTF8& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                //const char* pStr = a->getValue().c_str();
                val = a->getValue().c_str();
                return true;
            }
            return false;
        }


        /// @brief Retrieves a Date attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param dt Output Date value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::Date& dt) const
        {
            return getAttribValueDateTime(attribName, dt);
        }

        /// @brief Retrieves a Time attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param dt Output Time value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::Time& dt) const
        {
            return getAttribValueDateTime(attribName, dt);
        }

        /// @brief Retrieves a DateTime attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param dt Output DateTime value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::DateTime& dt) const
        {
            return getAttribValueDateTime(attribName, dt);
        }

        /// @brief Generic helper that retrieves a date/time typed attribute value by name string.
        /// @tparam TD A date/time type supporting fromString().
        /// @param attribName Null-terminated attribute name.
        /// @param dt Output date/time value.
        /// @return True if the attribute was found.
        template <class TD>
        bool getAttribValueDateTime(const char* attribName, TD& dt) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                dt.fromString(a->value.c_str());
                return true;
            }
            return false;
        }

        /// @brief Returns the raw string value of an attribute found by hash, or an empty string.
        /// @param hashNo Hash number of the attribute name.
        /// @return The attribute value as a td::String, or an empty string if not found.
        td::String getAttribRawValue(td::UINT4 hashNo) const
        {
            Attribute a = getAttrib(hashNo);
            td::String toRet;
            if (a)
            {
                toRet = a->getValue();
            }
            return toRet;
        }

        /// @brief Returns the raw string value of an attribute found by name, or an empty string.
        /// @param attribName Null-terminated attribute name.
        /// @return The attribute value as a td::String, or an empty string if not found.
        td::String getAttribRawValue(const char* attribName) const
        {
            Attribute a = getAttrib(attribName);
            td::String toRet;
            if (a)
            {
                toRet = a->getValue();
            }
            return toRet;
        }

        /// @brief Retrieves an unsigned 64-bit integer attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output LUINT8 value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::LUINT8& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toUint8();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a signed 64-bit integer attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output LINT8 value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::LINT8& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toInt8();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a signed 32-bit integer attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output INT4 value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::INT4& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toINT4();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a signed 16-bit integer attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output INT2 value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::INT2& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toINT2();
                return true;
            }
            return false;
        }

        /// @brief Retrieves an unsigned byte attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output BYTE value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::BYTE& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toBYTE();
                return true;
            }
            return false;
        }

        /// @brief Retrieves an unsigned 16-bit word attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output WORD value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::WORD& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toWORD();
                return true;
            }
            return false;
        }

        /// @brief Retrieves an unsigned 32-bit integer attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output UINT4 value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, td::UINT4& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toUINT4();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a boolean attribute value by name string.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output boolean value.
        /// @return True if the attribute was found.
        bool getAttribValue(const char* attribName, bool& val) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val = a->value.toBoolean();
                return true;
            }
            return false;
        }

        /// @brief Retrieves a Variant attribute value by name string; the variant must have a pre-set type.
        /// @param attribName Null-terminated attribute name.
        /// @param val Output Variant value (must have its type pre-initialized).
        /// @return True if the attribute was found and parsed into the variant.
        bool getAttribValue(const char* attribName, td::Variant& val) const
        {
            assert(val.getType() != td::TD_NONE);
            Attribute a = getAttrib(attribName);
            if (a)
            {
                val.setValueFromString(a->value.c_str());
                return true;
            }
            return false;
        }

        /// @brief Returns a pointer to the StringExt value of an attribute found by name string.
        /// @param attribName Null-terminated attribute name.
        /// @return Pointer to the attribute value string, or null if not found.
        const td::StringExt* getAttribValueStrPtr(const char* attribName) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                return &a->value;
            }
            return 0;
        }



        /// @brief Returns the number of attributes on the current node.
        /// @return Attribute count.
        inline int getAttribCount() const
        {
            return _it->attribs.size();
        }

        /// @brief Fills a vector of VariantDesc descriptors from all attributes of the current node.
        /// @param vDesc Output vector to fill with attribute name descriptors.
        /// @return True if the node has at least one attribute.
        bool getAttribDescriptions(cnt::SafeFullVector<td::VariantDesc>& vDesc) const
        {
            attrib_iterator it(_it->attribs.begin());
            attrib_iterator itE(_it->attribs.end());
            int nAttribs = getAttribCount();
            if (nAttribs <= 0)
                return false;
            vDesc.reserve(nAttribs);
            int i = 0;
            while (it != itE)
            {
                vDesc[i].posIn = i;
                vDesc[i].posOut = -1;
                vDesc[i].outType = td::TD_NONE;
                vDesc[i].desc = it->getName();
                ++i;
                ++it;
            }
            return true;
        }

        /// @brief Reads attribute values into a pre-populated vector of VariantDesc descriptors.
        /// @param vDesc Vector of VariantDesc instances that describe which attributes to read and how.
        void getAttribValues(cnt::SafeFullVector<td::VariantDesc>& vDesc) const
        {
            auto it(vDesc.begin());
            const auto itE(vDesc.end());
            while (it != itE)
            {
                if (it->isPaired())
                {
                    //TODO -> fixiraj ovo da uzmemo string jednostavije, koristenjem td::StringExt
                    it->fromString(_it->attribs[it->posIn].getValue().c_str());
                    //*it = _it->attribs[it->posIn].getValue();

                }
                ++it;
            }
        }
        /// @brief Counts how many sibling nodes match the current name filter (or all siblings if no filter).
        /// @return Count of matching nodes from the current position to the end.
        unsigned int getCounter() const
        {
//            if (!pNaming)
//                return 0;
            typename cnt::ListSL<Node, true>::const_iterator itTmp(_it);
            unsigned int i = 0;

            while (itTmp._getPtr())
            {
                if (!pNaming)
                {
                    ++i;
                }
                //if ( (pNaming->hashNo == itTmp->pNaming->hashNo)/* && (pNaming->pName->length() == _it->pNaming->pName->length())*/ )
                else if (pNaming->hashNo == itTmp->pNaming->hashNo && (pNaming->pName->length() == itTmp->pNaming->pName->length()))
                {
#ifdef _DEBUG_SAX
                    assert(*(pNaming->pName) == *(_it->pNaming->pName));
#endif
                    ++i;
                }

                ++itTmp;
            };
            return i;
        }

        /// @brief Advances the iterator to the next node, skipping those that don't match the name filter.
        void operator ++ ()
        {
            ++_it;
            if (!pNaming)
            {
                return;
            }

            while (_it._getPtr())
            {

                //if ( (pNaming->hashNo == _it->pNaming->hashNo)/* && (pNaming->pName->length() == _it->pNaming->pName->length())*/ )
                if (pNaming->hashNo == _it->pNaming->hashNo)
                {
#ifdef _DEBUG_SAX
                    assert(*(pNaming->pName) == *(_it->pNaming->pName));
#endif

                    return;
                }
                ++_it;
            };
        }

        /// @brief Dereferences the iterator to get a const reference to the current node.
        /// @return Const reference to the current Node.
        const Node& operator* () const
        {
            return _it.operator*();
        }

        /// @brief Arrow operator to access members of the current node.
        /// @return Const pointer to the current Node.
        const Node* operator ->() const
        {
            return _it.operator->();
        }

        /// @brief Checks whether the iterator has reached the end of the node list.
        /// @return True if the iterator is past the last node.
        bool end() const
        {
            if (_it._getPtr() == 0)
                return true;
            return false;
        }

        /// @brief Checks whether the iterator points to a valid node.
        /// @return True if the iterator is valid (not at the end).
        bool isOk() const
        {
            if (_it._getPtr() != 0)
                return true;
            return false;
        }

        /// @brief Returns an iterator to the first child node with the given element name.
        /// @param name Null-terminated element name to search for among child nodes.
        /// @return A node_iterator pointing to the first matching child, or an empty iterator if not found.
        node_iterator getChildNode(const char* name) const
        {
            //const tHashEntry* pEntry = getNodeHashEntry(name);
            assert(pParser);
            PNaming pEntry = pParser->getNodeHashEntry(name);
            typename cnt::ListSL<Node, true>::const_iterator itN = _it->nodes.begin();
            if (pEntry)
            {
                while (itN._getPtr())
                {
                    //if ( (itN->pNaming->hashNo == pEntry->hashNo) && ( *(itN->pNaming->pName) == *(pEntry->pName) ) )
                    if ( itN->pNaming->hashNo == pEntry->hashNo )
                        return node_iterator(itN, pParser, pEntry);
                    ++itN;
                }
            }
            return node_iterator();
        }

        /// @brief Returns an iterator to the first child node with the given element name hash.
        /// @param hashNo Pre-computed hash number of the element name.
        /// @return A node_iterator pointing to the first matching child, or an empty iterator if not found.
        node_iterator getChildNode(td::UINT4 hashNo) const
        {
            //const tHashEntry* pEntry = getNodeHashEntry(name);
            assert(pParser);
            PNaming pEntry = pParser->getNodeHashEntry(hashNo);
            typename cnt::ListSL<Node, true>::const_iterator itN (_it->nodes.begin());
            while (itN._getPtr())
            {
                if ( itN->pNaming->hashNo == hashNo )
                    return node_iterator(itN, pParser, pEntry);
                ++itN;
            }
            return node_iterator();
        }

        /// @brief Returns an iterator to the first child node of the current node without name filtering.
        /// @return A node_iterator pointing to the first child node.
        node_iterator getChildNode() const
        {
            typename cnt::ListSL<Node, true>::const_iterator itNodes(_it->nodes.begin());
            return node_iterator(itNodes, pParser);
        }
    };

protected:
    mu::EnumSerializerManager* _pEnumManager; ///< Pointer to the enum serializer manager used for enum attribute deserialization.
    Node _root; ///< Root node of the DOM tree; its children are the top-level XML elements.
    typename TSAX::tHashEntry _attrCDATA; ///< Hash entry used for CDATA section nodes.
    Node* _pCurrentNode; ///< Pointer to the node currently being built during parsing.
    cnt::Stack<Node*, 16> _processingDOMNodes; ///< Stack tracking the ancestor chain of the node being parsed.
    mem::StringMemoryManager<> _stringPool; //memory allocator za sve stringove (attribute and node text)
    mem::CntMemoryManager < cnt::ListSL<Node, true> > _nodeMemMgr; ///< Memory manager for node list entries.
    mem::CntMemoryManager < cnt::SafeFullVector<Attrib, true> > _attribMemMgr; ///< Memory manager for attribute vector entries.
    mem::CntMemoryManager < cnt::ListSL<td::StringExt, true> > _nodeTextMemMgr; //will be populated in case when _collectAllNodeTexts is true
    td::String _docType; ///< Stores the DOCTYPE declaration string if present in the XML document.
    bool _collectAllNodeTexts = false; ///< When true, all text segments of each node are collected into textLst.

    //cnt::ListSL<Attrib, true> attribs;
public:
    /// @brief Default constructor; initializes the CDATA hash entry and sets up the processing stack.
    DOMParser()
        : TSAX ()
        , _pEnumManager(nullptr)
    {
        _attrCDATA.pName = _stringPool.allocObject(5, "CDATA"); //&_strAttrDataName; //new td::String("CDATA");
        _attrCDATA.hashNo = 0;
        _root.pNaming = 0;
        _pCurrentNode = &_root;
        _processingDOMNodes.push(_pCurrentNode);
        _processingDOMNodes.push(_pCurrentNode);  //do it twice to remove unneeded checking by top()
    }

    /// @brief Destructor; cleans up the DOM tree, memory managers, and the string pool.
    ~DOMParser()
    {
        _root.clean();
        _nodeMemMgr.clean();
        _processingDOMNodes.clean();
        _attribMemMgr.clean();
        _stringPool.clean();  //_stringPool mora biti zadnji izbrisan!!!
        //int g = 5;
        //++g;
        //delete attrCDATA.pName;
    }

    /// @brief Returns the DOCTYPE declaration string found during parsing.
    /// @return Const reference to the doctype string.
    const td::String& getDocType() const
    {
        return _docType;
    }

    /// @brief Enables or disables collection of all text segments for each node into textLst.
    /// @param collect If true, all text segments are accumulated; if false, only the first non-whitespace segment is kept.
    void collectAllNodeTexts(bool collect = true)
    {
        _collectAllNodeTexts = collect;
    }

    /// @brief Allocates a new StringExt object in the string pool with the given content.
    /// @param len Length of the string to allocate.
    /// @param pStr Pointer to the source character data.
    /// @return Pointer to the newly allocated StringExt object in the pool.
    inline td::StringExt* allocStringObject(td::UINT4 len, const char* pStr)
    {
        return _stringPool.allocObject(len, pStr);
    }

    /// @brief Sets the enum serializer manager used for deserializing enum-typed attributes.
    /// @param pEnumMgr Pointer to the EnumSerializerManager instance.
    void setEnumManager(mu::EnumSerializerManager* pEnumMgr)
    {
        _pEnumManager = pEnumMgr;
    }

    /// @brief Looks up the naming hash entry for a node element name.
    /// @param name Null-terminated element name string.
    /// @return Pointer to the matching hash entry, or null if not found.
    PNaming getNodeNaming(const char* name) const
    {
        return TSAX::getNodeHashEntry(name);
    }

    /// @brief Looks up the naming hash entry for an attribute name.
    /// @param name Null-terminated attribute name string.
    /// @return Pointer to the matching hash entry, or null if not found.
    PNaming getAttribNaming(const char* name) const
    {
        return TSAX::getAttribHashEntry(name);
    }

    /// @brief Returns an iterator to the first top-level child node of the document root.
    /// @return A node_iterator pointing to the first root-level element.
    node_iterator getRootNode()
    {
        return node_iterator(_root.nodes.begin(), this);
    }

    /// @brief Returns an iterator to the first child of the given iterator's node that matches the given name.
    /// @param it The parent node iterator.
    /// @param name Null-terminated element name to search for.
    /// @return A node_iterator pointing to the first matching child, or an empty iterator if not found.
    node_iterator getChildNode(const node_iterator& it, const char* name)
    {
        //const tHashEntry* pEntry = getNodeHashEntry(name);
        PNaming pEntry = TSAX::getNodeHashEntry(name);
        typename cnt::ListSL<Node, true>::const_iterator itN(it->nodes.begin());
        if (pEntry)
        {
            while (itN._getPtr())
            {
                if ( (itN->pNaming->hashNo == pEntry->hashNo) && ( *(itN->pNaming->pName) == *(pEntry->pName) ) )
                    return node_iterator(itN, this, pEntry);
                ++itN;
            }
        }
        return node_iterator();
    }

    /// @brief Returns an iterator to the first child of the given iterator's node without name filtering.
    /// @param it The parent node iterator.
    /// @return A node_iterator pointing to the first child node.
    node_iterator getChildNode(const node_iterator& it)
    {
        typename cnt::ListSL<Node, true>::const_iterator itNodes(it->nodes.begin());
        return node_iterator(itNodes, this);
    }

    /// @brief SAX callback invoked when a self-closing (dummy) node is encountered.
    void onDummyNode()
    {
#ifdef _DEBUG_SAX
        std::cout << "Added DUMMY node:" << TSAX::_outBuffer.c_str() << std::endl;
#endif
        //_nodeMemMgr.push_back(pCurrentNode->nodes);
        //Node* pNode = &pCurrentNode->nodes.push();
        //pNode->pNaming = TSAX::pLastNode;
    }

    /// @brief SAX callback invoked when an opening XML element tag is encountered; creates a new child node.
    /// @return Always returns true.
    bool onOpenNode()
    {
#ifdef _DEBUG_SAX
        std::cout << "Open node       :" << TSAX::_outBuffer.c_str() << std::endl;
#endif

        //pCurrentNode = &pCurrentNode->nodes.push();
        _pCurrentNode = _nodeMemMgr.push_back(_pCurrentNode->nodes);
        //pCurrentNode = &.push_back()->data;
        _pCurrentNode->pNaming = TSAX::_pLastNode;
        _processingDOMNodes.push(_pCurrentNode);
        return true;
    }

    /// @brief SAX callback invoked when a closing XML element tag is encountered; pops the current node.
    void onCloseNode()
    {
#ifdef _DEBUG_SAX
        std::cout << "Closed node     :" << TSAX::_pLastNode->pName->c_str() << std::endl;
#endif
        //td::UINT4 nChars = _currentNodeText.length();
        //
        //if (nChars > 0)
        //{
        //	pCurrentNode->text = _stringPool.alloc(nChars);
        //	_currentNodeText.storeToPreallocated(pCurrentNode->text);
        //}
        //_currentNodeText.reset();
        _processingDOMNodes.pop();
        _pCurrentNode = _processingDOMNodes.top();
    }

    /// @brief SAX callback invoked when an attribute is parsed; appends a new Attrib to the current node.
    void onAttrib()
    {
#ifdef _DEBUG_SAX
        std::cout << "Attribute name  :" << TSAX::_pLastAttrib->pName->c_str() << std::endl;
        std::cout << "Attribute value :" << TSAX::_outBuffer.c_str() << std::endl;
#endif

        //Attrib* pAtt = &pCurrentNode->attribs.push();
        Attrib* pAtt = _attribMemMgr.push_back(_pCurrentNode->attribs);
        pAtt->pNaming = TSAX::_pLastAttrib;
        //pAtt->value.setString(TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
        _stringPool.allocDataHolder(pAtt->value, TSAX::_outBuffer.length(), TSAX::_outBuffer.c_str());
    }

    /// @brief SAX callback invoked when a CDATA section is parsed; stores its content as the current node's text.
    void onCDATA()
    {
#ifdef _DEBUG_SAX
        std::cout << "CDATA           : " << TSAX::_outBuffer.c_str() << std::endl;
#endif

        //_currentNodeText.appendString(TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
        if (!TSAX::_outBuffer.isWhiteSpaceOnly())
        {
            _stringPool.allocDataHolder(_pCurrentNode->text, TSAX::_outBuffer.length(), TSAX::_outBuffer.c_str());
        }

    }

    /// @brief SAX callback invoked when a DOCTYPE declaration is parsed; stores it in _docType.
    void onDocType()
    {
#ifdef _DEBUG_SAX
        std::cout << "DOCTYPE           : " << TSAX::_outBuffer.c_str() << std::endl;
#endif

        //_currentNodeText.appendString(TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
        if (!TSAX::_outBuffer.isWhiteSpaceOnly())
        {
            _docType = TSAX::_outBuffer.c_str();
        }
    }

    /// @brief SAX callback invoked when text content between XML tags is parsed; stores it in the current node.
    void onNodeText()
    {
#ifdef _DEBUG_SAX
        std::cout << "Node Text       :" << TSAX::_outBuffer.c_str() << std::endl;
#endif
        auto nLen = TSAX::_outBuffer.length();
        //pCurrentNode->text.appendString(TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
        if (nLen > 0)
        {

            const char* pStr = TSAX::_outBuffer.c_str();
            if (_collectAllNodeTexts)
            {
                //called by getString
                td::StringExt str;
                _stringPool.allocDataHolder(str, nLen, pStr);
                td::StringExt* pStr = _nodeTextMemMgr.push_back(_pCurrentNode->textLst);
                *pStr = str;

                if (!TSAX::_outBuffer.isWhiteSpaceOnly())
                {
                    //keep here for nodes without this need (we could call getValue on such nodes
                    _pCurrentNode->text = str;
                }

            }
            else
            {
                if (!TSAX::_outBuffer.isWhiteSpaceOnly())
                {
                    //_currentNodeText.appendString(TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
                    _stringPool.allocDataHolder(_pCurrentNode->text, nLen, pStr);
                }
            }

        }
        //_currentNodeText.appendString(TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
    }
protected:
    //2020.01.05
//		template <class OSTREAM>
//		void writeEndNode(OSTREAM& o, const Node* pNode)
//		{
//			TSAX::_outBuffer.append(2, "</");
//			TSAX::_outBuffer.append(pNode->pNaming->pName->length(), pNode->pNaming->pName->c_str());
//			TSAX::_outBuffer.append(2, ">\n");
//		}
    /// @brief Appends the XML closing tag for the given node to the output buffer.
    /// @param pNode Pointer to the Node whose closing tag should be written.
    void writeEndNode(const Node* pNode)
    {
        TSAX::_outBuffer.append(2, "</");
        TSAX::_outBuffer.append(pNode->pNaming->pName->length(), pNode->pNaming->pName->c_str());
        TSAX::_outBuffer.append(2, ">\n");
    }

    /// @brief Writes the opening tag and content of a node to the output buffer.
    /// @param pNode Pointer to the Node to serialize.
    /// @return True if child nodes should be pushed onto the save stack; false if the node was self-closed.
    bool writeNode(const Node* pNode)
    {
        if ( (pNode->pNaming->hashNo == 0) && (strcmp(pNode->pNaming->pName->c_str(), "CDATA") == 0))
        {
            //write CDATA
            TSAX::_outBuffer.append(9, "<![CDATA[");
            TSAX::_outBuffer.append(pNode->text.length(), pNode->text.c_str());
            TSAX::_outBuffer.append(4, "]]>\n");
            return false;
        }
        bool noNodes = pNode->nodes.isEmpty();
        bool noText = (pNode->text.length() == 0);

        TSAX::_outBuffer += '<';
        TSAX::_outBuffer.append(pNode->pNaming->pName->length(), pNode->pNaming->pName->c_str());
        if (!pNode->attribs.isEmpty())
        {
            //typename cnt::Stack<Attrib>::const_iterator itA(pNode->attribs.begin());
            //typename cnt::Stack<Attrib>::const_iterator itAEnd (pNode->attribs.end());
            auto itA(pNode->attribs.begin());
            auto itAEnd (pNode->attribs.end());
            while (itA != itAEnd)
            {
                TSAX::_outBuffer += ' ';
                TSAX::_outBuffer.append(itA->pNaming->pName->length(), itA->pNaming->pName->c_str());
                TSAX::_outBuffer.append(2, "=\"");

                TSAX::template writeValue<true, true>(itA->value);

                TSAX::_outBuffer += '"';
                ++itA;
            }
        }
        if (noNodes && noText)
        {
            TSAX::_outBuffer.append(3, "/>\n");
            return false;
        }
        if (!noText)
        {
            TSAX::_outBuffer += '>';
            TSAX::template writeValue<true, false>(pNode->text);
        }
        if (noNodes)
        {
            TSAX::_outBuffer.append(2, "</");
            TSAX::_outBuffer.append(pNode->pNaming->pName->length(), pNode->pNaming->pName->c_str());
            TSAX::_outBuffer.append(2, ">\n");
            return false;
        }
        else if (noText)
        {
            TSAX::_outBuffer.append(2, ">\n");
        }
        return true;
    }

public:

    /// @brief Saves the parsed DOM tree as an XML document to the given output stream.
    /// @tparam OSTREAM Output stream type supporting write operations.
    /// @param o The output stream to write the XML content into.
    /// @return True on success.
    template <class OSTREAM>
    bool save(OSTREAM& o)
    {
        //FO_BINARY_OPEN_TRUNCATE
        //const unsigned char utf8Encoding[3] = {0xEF, 0xBB, 0xBF};
        //fo::write(o, (const char*) utf8Encoding, 3);
        fo::write(o, "<?xml version='1.0' encoding='UTF-8'?>\n", 39);


        cnt::Stack<const Node*, 16> savingDOMNodes;

        const Node* pNode = &_root.nodes.first();

        TSAX::_outBuffer.erase();
        bool putOnStack = writeNode(pNode);
        fo::write(o, TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
        if (putOnStack)
        {
            savingDOMNodes.push(pNode);
        }
        else
        {
            return true;
        }

        int nDepth = 1;
        typename cnt::ListSL<Node, true>::const_iterator itN = pNode->nodes.begin();
        typename cnt::ListSL<Node, true>::const_iterator itNEnd;

        cnt::Stack< typename cnt::ListSL<Node, true>::const_iterator , 16> stackIterators;
        stackIterators.push(itN);
        pNode = &(*itN);

        //processingDOMNodes.push(pCurrentNode);
        while (!savingDOMNodes.isEmpty())
        {
            while (itN != itNEnd)
            {
                TSAX::_outBuffer.erase();
                for (int i=0; i<nDepth; ++i)
                    TSAX::_outBuffer += '\t';
                pNode = &(*itN);
                putOnStack = writeNode(pNode);
                fo::write(o, TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
                if (putOnStack)
                {
                    stackIterators.push(itN);
                    savingDOMNodes.push(pNode);
                    itN = itN->nodes.begin();
                    pNode = &(*itN);
                    ++nDepth;
                    continue;
                }
                ++itN;
            }
            --nDepth;
            TSAX::_outBuffer.erase();
            pNode = savingDOMNodes.top();
            for (int i=0; i<nDepth; ++i)
                TSAX::_outBuffer += '\t';
            writeEndNode(pNode);
            fo::write(o, TSAX::_outBuffer.c_str(), TSAX::_outBuffer.length());
            itN = stackIterators.top();
            ++itN;
            stackIterators.pop();
            pNode = savingDOMNodes.top();
            savingDOMNodes.pop();
        }
        return true;
    }

    /// @brief Saves the parsed DOM tree as an XML file to the specified file path.
    /// @param fileName Null-terminated UTF-8 file path.
    /// @return True if the file was opened and saved successfully; false otherwise.
    bool saveToFile(const td::UTF8* fileName)
    {
        std::ofstream o;
        if (!fo::openFile(o, fileName, FO_BINARY_OPEN_TRUNCATE))
            return false;
        return save(o);
    }

    /// @brief SAX callback invoked after parsing is complete.
    /// @param bOK Indicates whether parsing succeeded.
    void onParseFinished([[maybe_unused]] bool bOK)
    {
        //_currentNodeText.clean();
    }
};

/// @brief Convenience typedef for a file-based DOM parser with a 4KB buffer and medium-sized hash table.
typedef DOMParser<FileBuffer4k, 0x000003FF, false> FileParser;
/// @brief Convenience typedef for an in-memory DOM parser with a small hash table.
typedef DOMParser<StringWrapper8, 0x000000FF, true> MemoryParser;
	//StringWrapper8
};


/// @brief Negation operator that returns true when the FileParser node_iterator has reached the end.
/// @param ni The node_iterator to test.
/// @return True if the iterator is at the end.
inline bool operator! (const xml::FileParser::node_iterator& ni)
{
	return ni.end();
}

/// @brief Negation operator that returns true when the MemoryParser node_iterator has reached the end.
/// @param ni The node_iterator to test.
/// @return True if the iterator is at the end.
inline bool operator! (const xml::MemoryParser::node_iterator& ni)
{
	return ni.end();
}
