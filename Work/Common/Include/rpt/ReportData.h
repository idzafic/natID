// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <dp/IDataSet.h>
#include <rpt/ShapeType.h>
#include <cnt/SafeFullVector.h>
#include <rpt/EntityData.h>
#include <rpt/AutoElement.h>
#include <td/String.h>
#include <gui/Frame.h>

namespace rpt
{
#define NREPDATA_CNT_ELEMS 3

	typedef struct
	{
		rpt::ContainerType cntType;
		rpt::ShapeType shapeType;
		const char* name;
	} ElemDef;

	class ContainerData
	{	
		friend class ReportData;

	protected:
		int _layoutNo;
		ElemDef _elemDef[NREPDATA_CNT_ELEMS + 1]; 
		dp::IDataSet* _pDP;
		td::String* _pNote = nullptr;
	public:
		ContainerData()
			: _layoutNo(0)			
			, _pDP(nullptr)
		{
			for (int i = 0; i < NREPDATA_CNT_ELEMS + 1; ++i)
			{
				_elemDef[i].cntType = rpt::ContainerType::CNT_None;
				_elemDef[i].shapeType = rpt::ShapeType::SHP_None;
				_elemDef[i].name = nullptr;
			}				
		}
		inline int getLayoutNo() const
		{
			return _layoutNo;
		}
		inline dp::IDataSet* getDP()
		{
			return _pDP;
		}

		inline td::String* getNote()
		{
			return _pNote;
		}
	};

	//TODO: Finish full descriptor
	class ReportData : public cnt::SafeFullVector<ContainerData>
	{
	protected:
		const rpt::DocumentData& _docData;
		const rpt::CompanyData& _companyData;
		const rpt::ObjectData& _objectData;
        const rpt::SignatureData* _pSignatures = nullptr;
        
		dp::IDatabase* _pDB = nullptr;
		const char* translationExtension = nullptr;		
		//const mu::RTStringManager* rtStringsManager = nullptr;
		const cnt::SafeFullVector<int>* _pLayoutsToBeRemovedFromConfig = nullptr; //0-based, RESTRICTION: first layout (0) cannot be removed
		gui::Frame* _pContextManager = nullptr;
		//QString _note;
	public:
		class iterator
		{
			friend class ReportData;
		protected:
			ReportData& _rd;
			int _layoutNo = 0;
			size_t _pos = 0;
			int _k = -1;
			bool _editing = true;
			iterator(ReportData& rd, size_t pos = 0, int k = -1)
				: _rd(rd) 
				, _pos(pos)
				, _k(k)
			{
			}
		public:
			iterator(const iterator& it)
				: _rd(it._rd)
				, _pos(it._pos)
				, _k(it._k)
			{
				if (!_editing)					
					_layoutNo = _rd[_pos]._layoutNo;
			}
			
			void operator ++ ()
			{
				++_pos;
				_k = -1;
				assert(_pos < _rd.size());
				if (_pos >= _rd.size())
					return;
				if (_editing)
					_rd[_pos]._layoutNo = _layoutNo;
				else
					_layoutNo = _rd[_pos]._layoutNo;
			}

			void nextField()
			{
				++_k;
			}

			const ContainerData& operator -> () const
			{
				return _rd[_pos];
			}

			const ElemDef& fieldBlock() const
			{
				return _rd[_pos]._elemDef[_k];
			}

			void nextLayout()
			{
				++_layoutNo;
				if (_editing)
					_rd[_pos]._layoutNo = _layoutNo;
				
			}

			iterator& operator << (rpt::ContainerType cntType)
			{				
				++_k;
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert(_rd[_pos]._elemDef[_k].shapeType == rpt::ShapeType::SHP_None);
				_rd[_pos]._elemDef[_k].cntType = cntType;
				return *this;
			}

			iterator& operator << (rpt::ShapeType shapeType)
			{
				++_k;
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert(_rd[_pos]._elemDef[_k].cntType == rpt::ContainerType::CNT_None);
				_rd[_pos]._elemDef[_k].shapeType = shapeType;
				return *this;
			}

			iterator& operator << (const char* shapeOrCntName)
			{				
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert((_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None));
				_rd[_pos]._elemDef[_k].name = shapeOrCntName;
				return *this;
			}

			iterator& operator << (dp::IDataSet* pDP)
			{
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert((_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None));
				_rd[_pos]._pDP = pDP;
				return *this;
			}

			iterator& operator << (dp::IDataSetPtr& pDP)
			{
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert((_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None));
				pDP->addRef();
				_rd[_pos]._pDP = pDP.getPtr();
				return *this;
			}

			iterator& operator << (td::String* pNote)
			{
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert((_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None));
				_rd[_pos]._pNote = pNote;
				return *this;
			}

			inline bool isOk() const
			{
				return (_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None);
			}
		};

		class const_iterator
		{
			friend class ReportData;
		protected:
			const ReportData& _rd;
			int _layoutNo = 0;
			size_t _pos = 0;
			int _k = -1;	

			const_iterator(const ReportData& rd, size_t pos = 0, int k = -1)
				: _rd(rd)
				, _pos(pos)
				, _k(k)
			{
			}

		public:

			const_iterator(const const_iterator& it)
				: _rd(it._rd)
				, _pos(it._pos)
				, _k(it._k)
			{				
					_layoutNo = _rd[_pos]._layoutNo;
			}		
			void operator ++ ()
			{
				++_pos;
				_k = -1;
				assert(_pos < _rd.size());
				if (_pos >= _rd.size())
					return;
				_layoutNo = _rd[_pos]._layoutNo;
			}

			void nextField()
			{
				++_k;
			}

			ContainerData& data () const
			{
				ReportData& rd = const_cast<ReportData&>(_rd);
				return rd[_pos];
			}

			const ElemDef& fieldBlock() const
			{
				return _rd[_pos]._elemDef[_k];
			}

			void setLayoutNo(int layoutNo)
			{
				_layoutNo = layoutNo;
			}

			const_iterator& operator >> (ElemDef& elemDef)
			{
				++_k;
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;
				elemDef = _rd[_pos]._elemDef[_k];
				return *this;
			}

			inline bool isOk() const
			{
				return (_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None);
			}

			td::String getFullName() const
			{
				cnt::StringBuilderSmall str;
				str << "L" << _layoutNo;
				for (int i = 0; i <= NREPDATA_CNT_ELEMS; ++i)
				{
					rpt::ContainerType cnt = _rd[_pos]._elemDef[i].cntType;
					rpt::ShapeType shp = _rd[_pos]._elemDef[i].shapeType;
					if (cnt == rpt::ContainerType::CNT_None && shp == rpt::ShapeType::SHP_None)
						break;
					if (cnt != rpt::ContainerType::CNT_None)
					{
						switch (cnt)
						{
                            case ContainerType::CNT_Header: str << "_H"; break;
                            case ContainerType::CNT_DocHeader: str << "_DH"; break;
                            case ContainerType::CNT_Body: str << "_B"; break;
                            case ContainerType::CNT_Footer: str << "_F"; break;
                            case ContainerType::CNT_Group: str << "_G"; break;
                            case ContainerType::CNT_Table: str << "_T"; break;
                            case ContainerType::CNT_Row: str << "_R"; break;
                            case ContainerType::CNT_DocFooter: str << "_DF"; break;
                            case ContainerType::CNT_Grouper: str << "_Grp"; break;
						default:
							str << "_X";							
						}
					}
					const char* pName = _rd[_pos]._elemDef[i].name;
					if (pName)
						str << "(" << pName << ")";
				}
				return str.toString();
			}
		};

	protected:
		void cleanDP()
		{
			size_t nElems = size();
			for (size_t i = 0; i < nElems; ++i)
			{
				if (_data[i]._pDP != nullptr)
				{
					_data[i]._pDP->release();
					_data[i]._pDP = nullptr;
				}
			}
		}

	public:	        
		ReportData(size_t nElems, const rpt::DocumentData& docData, const rpt::CompanyData& companyData, const rpt::ObjectData& objectData, dp::IDatabase* pDB, const char* translExtension)
			: cnt::SafeFullVector<ContainerData>(nElems)			
			, _docData(docData)
			, _companyData(companyData)
			, _objectData(objectData)
			, _pDB(pDB)
			, translationExtension(translExtension)
		{}

		~ReportData()
		{
			cleanDP();
		}

		iterator begin()
		{
			return iterator(*this);
		}

		iterator at(int pos)
		{
			iterator it(*this, pos, -1);
			return it;
		}

		const_iterator dataBlock(int pos) const
		{
			const_iterator it(*this, pos, 0);
			return it;
		}

		const char* getTranslationExtension() const
		{
			return translationExtension;
		}

		dp::IDatabase* getDB() const
		{
			return _pDB;
		}

		const rpt::DocumentData& getDocData() const
		{
			return _docData;
		}

		const rpt::CompanyData& getCompanyData() const
		{
			return _companyData;
		}

		const rpt::ObjectData& getObjectData() const
		{
			return _objectData;
		}
        
        const rpt::SignatureData* getSignatureData() const
        {
            return _pSignatures;
        }

		//void setRTStringManager(const mu::RTStringManager* stringManager)
		//{
		//	//rtStringsManager = stringManager;
		//}

		//const mu::RTStringManager* getRTStringManager() const
		//{
  //          return nullptr;
		//	//return rtStringsManager;
		//}

		void setLayoutsToBeRemovedFromConfig(const cnt::SafeFullVector<int>* pLayoutsToBeRemovedFromConfig)
		{
			_pLayoutsToBeRemovedFromConfig = pLayoutsToBeRemovedFromConfig;
		}

		const cnt::SafeFullVector<int>* getLayoutsToBeRemovedFromConfig() const
		{
			return _pLayoutsToBeRemovedFromConfig;
		}

		void setContextManager(gui::Frame* pConsumer)
		{
			_pContextManager = pConsumer;
		}

		const gui::Frame* getContextManager() const
		{
			return _pContextManager;
		}
        
        void setSignatureData(SignatureData* pSign)
        {
            _pSignatures = pSign;
        }

		//void setNote(const QString& note)
		//{
		//	_note = note;
		//}
		
	};
}
