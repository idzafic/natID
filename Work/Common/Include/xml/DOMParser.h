// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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
    class Attrib
    {
    public:
        PNaming pNaming;
        td::StringExt value;
        Attrib(){}

        unsigned int getTotalCounter() const
        {
            return pNaming->nEntries;
        }

        unsigned int getHashNo() const
        {
            return pNaming->hashNo;
        }

        int getNameLength() const
        {
            return pNaming->pName->length();
        }

        const td::StringExt& getName() const
        {
            return *(pNaming->pName);
        }

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

    class Node
    {
    public:
        //const tHashEntry* pNaming;
        PNaming pNaming;
        
        td::StringExt text;
        cnt::ListSL<td::StringExt, true> textLst;
        cnt::ListSL<Node, true> nodes;

        //cnt::Stack<Attrib> attribs;
        //IDZ TODO
        //TODO, ubaci nesto umjesto PushBackVector-a. Neki own memory managed pushback vector
        //cnt::PushBackVector<Attrib> attribs;
        cnt::SafeFullVector<Attrib, true> attribs;
        Node(){}

        unsigned int getTotalCounter() const
        {
            return pNaming->nEntries;
        }

        unsigned int getHashNo() const
        {
            return pNaming->hashNo;
        }

        int getNameLength() const
        {
            return pNaming->pName->length();
        }

        const td::StringExt& getName() const
        {
            return *(pNaming->pName);
        }

        const td::StringExt& getValue() const
        {
            return text;
        }

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

        const char* getValueStrPtr() const
        {
            return text.c_str();
        }


        void clean()
        {
            nodes.clean();
            attribs.clean();
        }
        
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

    class node_iterator
    {
    protected:
        //const tHashEntry* pNaming;
        PNaming pNaming;
        TDOM*	pParser;
        typename cnt::ListSL<Node, true>::const_iterator _it;
    public:
        node_iterator()
            : pNaming(nullptr)
            , pParser(nullptr)
        {
        }

        node_iterator(const node_iterator& it)
            : pNaming(it.pNaming)
            , pParser(it.pParser)
            , _it(it._it)
        {
        }

        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList)
            : pNaming(nullptr)
            , pParser(nullptr)
            , _it(itList)
        {
        }

        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList, TDOM* parser)
            : pNaming(nullptr)
            , pParser(parser)
            , _it(itList)
        {
        }

        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList, PNaming pName)
            : pNaming(pName)
            , pParser(nullptr)
            , _it(itList)
        {
        }

        node_iterator(const typename cnt::ListSL<Node, true>::const_iterator& itList, TDOM* parser, PNaming pName)
            : pNaming(pName)
            , pParser(parser)
            , _it(itList)
        {
        }

        void operator = (const node_iterator& it)
        {
            _it = it._it;
            pParser = it.pParser;
            pNaming = it.pNaming;
        }

        bool operator == (const node_iterator& it) const
        {
            return (_it == it._it);
        }

        bool operator != (const node_iterator& it) const
        {
            return (it != it._it);
        }

        const attrib_iterator& getAttribs() const
        {
            return _it->attribs.begin();
        }
        
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

        bool getAttribValue(td::UINT4 hashNo, td::Date& dt)
        {
            return getAttribValueDateTime(hashNo, dt);
        }

        bool getAttribValue(td::UINT4 hashNo, td::Time& dt)
        {
            return getAttribValueDateTime(hashNo, dt);
        }

        bool getAttribValue(td::UINT4 hashNo, td::DateTime& dt)
        {
            return getAttribValueDateTime(hashNo, dt);
        }

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

        template <typename TENUM>
        const mu::EnumSerializer* getEnumSerializer(int enumPos) const
        {
            assert(pParser);
            assert(pParser->pEnumManager);
                            return  pParser->pEnumManager->template getEnum<TENUM>(enumPos);
        }

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

        const td::StringExt* getAttribValueStrPtr(td::UINT4 hashNo) const
        {
            Attribute a = getAttrib(hashNo);
            if (a)
            {
                return &(a->getValue());
            }
            return 0;
        }


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

        
        bool getAttribValue(const char* attribName, td::Date& dt)
        {
            return getAttribValueDateTime(attribName, dt);
        }

        bool getAttribValue(const char* attribName, td::Time& dt)
        {
            return getAttribValueDateTime(attribName, dt);
        }

        bool getAttribValue(const char* attribName, td::DateTime& dt)
        {
            return getAttribValueDateTime(attribName, dt);
        }

        template <class TD>
        bool getAttribValueDateTime(const char* attribName, TD& dt)
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                dt.fromString(a->value.c_str());
                return true;
            }
            return false;
        }
        
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

        const td::StringExt* getAttribValueStrPtr(const char* attribName) const
        {
            Attribute a = getAttrib(attribName);
            if (a)
            {
                return &a->value;
            }
            return 0;
        }

        

        inline int getAttribCount() const
        {
            return _it->attribs.size();
        }

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

        const Node& operator* () const
        {
            return _it.operator*();
        }

        const Node* operator ->() const
        {
            return _it.operator->();
        }
        
        bool end() const
        {
            if (_it._getPtr() == 0)
                return true;
            return false;
        }

        bool isOk() const
        {
            if (_it._getPtr() != 0)
                return true;
            return false;
        }

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

        node_iterator getChildNode() const
        {
            typename cnt::ListSL<Node, true>::const_iterator itNodes(_it->nodes.begin());
            return node_iterator(itNodes, pParser);
        }
    };

protected:
    mu::EnumSerializerManager* _pEnumManager;
    Node _root;
    typename TSAX::tHashEntry _attrCDATA;
    Node* _pCurrentNode;
    cnt::Stack<Node*, 16> _processingDOMNodes;
    mem::StringMemoryManager<> _stringPool; //memory allocator za sve stringove (attribute and node text)
    mem::CntMemoryManager < cnt::ListSL<Node, true> > _nodeMemMgr;
    mem::CntMemoryManager < cnt::SafeFullVector<Attrib, true> > _attribMemMgr;
    mem::CntMemoryManager < cnt::ListSL<td::StringExt, true> > _nodeTextMemMgr; //will be populated in case when _collectAllNodeTexts is true
    td::String _docType;
    bool _collectAllNodeTexts = false;
    
    //cnt::ListSL<Attrib, true> attribs;
public:
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

    const td::String& getDocType() const
    {
        return _docType;
    }
    
    void collectAllNodeTexts(bool collect = true)
    {
        _collectAllNodeTexts = collect;
    }
            
    inline td::StringExt* allocStringObject(td::UINT4 len, const char* pStr)
    {
        return _stringPool.allocObject(len, pStr);
    }

    void setEnumManager(mu::EnumSerializerManager* pEnumMgr)
    {
        _pEnumManager = pEnumMgr;
    }

    PNaming getNodeNaming(const char* name) const
    {
        return TSAX::getNodeHashEntry(name);
    }

    PNaming getAttribNaming(const char* name) const
    {
        return TSAX::getAttribHashEntry(name);
    }

    node_iterator getRootNode()
    {
        return node_iterator(_root.nodes.begin(), this);
    }

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

    node_iterator getChildNode(const node_iterator& it)
    {
        typename cnt::ListSL<Node, true>::const_iterator itNodes(it->nodes.begin());
        return node_iterator(itNodes, this);
    }

    void onDummyNode()
    {
#ifdef _DEBUG_SAX
        std::cout << "Added DUMMY node:" << TSAX::_outBuffer.c_str() << std::endl;
#endif
        //_nodeMemMgr.push_back(pCurrentNode->nodes);
        //Node* pNode = &pCurrentNode->nodes.push();
        //pNode->pNaming = TSAX::pLastNode;
    }

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
    void writeEndNode(const Node* pNode)
    {
        TSAX::_outBuffer.append(2, "</");
        TSAX::_outBuffer.append(pNode->pNaming->pName->length(), pNode->pNaming->pName->c_str());
        TSAX::_outBuffer.append(2, ">\n");
    }

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

    bool saveToFile(const td::UTF8* fileName)
    {
        std::ofstream o;
        if (!fo::openFile(o, fileName, FO_BINARY_OPEN_TRUNCATE))
            return false;
        return save(o);
    }

    void onParseFinished([[maybe_unused]] bool bOK)
    {
        //_currentNodeText.clean();
    }
};

typedef DOMParser<FileBuffer4k, 0x000003FF, false> FileParser;
typedef DOMParser<StringWrapper8, 0x000000FF, true> MemoryParser;
	//StringWrapper8
};


inline bool operator! (const xml::FileParser::node_iterator& ni)
{
	return ni.end();
}

inline bool operator! (const xml::MemoryParser::node_iterator& ni)
{
	return ni.end();
}
