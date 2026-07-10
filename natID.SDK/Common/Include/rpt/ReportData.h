// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ReportData.h
    @brief Defines ContainerData and ReportData classes for aggregating report layout and dataset information. */
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

	/// @brief Descriptor for a single report element defining its container type, shape type, and name.
	typedef struct
	{
		rpt::ContainerType cntType; ///< Container type of the element.
		rpt::ShapeType shapeType; ///< Shape type of the element.
		const char* name; ///< Name identifier of the element.
	} ElemDef;

	/// @brief Holds layout and dataset information for a single report container entry.
	class ContainerData
	{
		friend class ReportData;

	protected:
		int _layoutNo; ///< Layout index associated with this container.
		ElemDef _elemDef[NREPDATA_CNT_ELEMS + 1]; ///< Array of element definitions for this container.
		dp::IDataSet* _pDP; ///< Pointer to the data set supplying data for this container.
		td::String* _pNote = nullptr; ///< Optional note string associated with this container.
	public:
		/// @brief Default constructor, initializes layout to 0 and all element definitions to empty.
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
		/// @brief Returns the layout number for this container.
		/// @return The layout index.
		inline int getLayoutNo() const
		{
			return _layoutNo;
		}
		/// @brief Returns the data set pointer associated with this container.
		/// @return Pointer to the IDataSet, or nullptr if none assigned.
		inline dp::IDataSet* getDP()
		{
			return _pDP;
		}

		/// @brief Returns the note string pointer associated with this container.
		/// @return Pointer to the note string, or nullptr if none assigned.
		inline td::String* getNote()
		{
			return _pNote;
		}
	};

	//TODO: Finish full descriptor
	/// @brief Aggregates all container data entries for a report, holding document, company, object, and signature metadata.
	class ReportData : public cnt::SafeFullVector<ContainerData>
	{
	protected:
		const rpt::DocumentData& _docData; ///< Reference to the document-level data for the report.
		const rpt::CompanyData& _companyData; ///< Reference to the company-level data for the report.
		const rpt::ObjectData& _objectData; ///< Reference to the object-level data for the report.
        const rpt::SignatureData* _pSignatures = nullptr; ///< Optional pointer to signature data.

		dp::IDatabase* _pDB = nullptr; ///< Pointer to the database used by this report.
		const char* translationExtension = nullptr; ///< Extension string used for translation resource lookup.
		//const mu::RTStringManager* rtStringsManager = nullptr;
		const cnt::SafeFullVector<int>* _pLayoutsToBeRemovedFromConfig = nullptr; ///< Pointer to a list of layout indices to be removed from configuration (0-based, first layout cannot be removed).
		gui::Frame* _pContextManager = nullptr; ///< Pointer to the GUI frame that manages context for this report.
		//QString _note;
	public:
		/// @brief Mutable iterator for traversing and building report container data.
		class iterator
		{
			friend class ReportData;
		protected:
			ReportData& _rd; ///< Reference to the owning ReportData.
			int _layoutNo = 0; ///< Current layout index tracked by this iterator.
			size_t _pos = 0; ///< Current position in the container data vector.
			int _k = -1; ///< Current field index within the container element definitions.
			bool _editing = true; ///< Indicates whether this iterator is in editing mode.
			/// @brief Constructs an iterator for the given ReportData at the specified position and field index.
			/// @param rd Reference to the owning ReportData.
			/// @param pos Initial position index.
			/// @param k Initial field index.
			iterator(ReportData& rd, size_t pos = 0, int k = -1)
				: _rd(rd)
				, _pos(pos)
				, _k(k)
			{
			}
		public:
			/// @brief Copy constructor.
			/// @param it Source iterator to copy.
			iterator(const iterator& it)
				: _rd(it._rd)
				, _pos(it._pos)
				, _k(it._k)
			{
				if (!_editing)
					_layoutNo = _rd[_pos]._layoutNo;
			}

			/// @brief Advances the iterator to the next container entry.
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

			/// @brief Advances the field index to the next element definition slot.
			void nextField()
			{
				++_k;
			}

			/// @brief Returns a const reference to the current ContainerData entry.
			/// @return Const reference to the current ContainerData.
			const ContainerData& operator -> () const
			{
				return _rd[_pos];
			}

			/// @brief Returns the current element definition block.
			/// @return Const reference to the current ElemDef.
			const ElemDef& fieldBlock() const
			{
				return _rd[_pos]._elemDef[_k];
			}

			/// @brief Advances the layout number and updates the current container entry.
			void nextLayout()
			{
				++_layoutNo;
				if (_editing)
					_rd[_pos]._layoutNo = _layoutNo;

			}

			/// @brief Assigns a container type to the next element definition slot.
			/// @param cntType The container type to assign.
			/// @return Reference to this iterator.
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

			/// @brief Assigns a shape type to the next element definition slot.
			/// @param shapeType The shape type to assign.
			/// @return Reference to this iterator.
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

			/// @brief Assigns a name to the current element definition slot.
			/// @param shapeOrCntName Name string to assign.
			/// @return Reference to this iterator.
			iterator& operator << (const char* shapeOrCntName)
			{
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert((_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None));
				_rd[_pos]._elemDef[_k].name = shapeOrCntName;
				return *this;
			}

			/// @brief Assigns a raw data set pointer to the current container entry.
			/// @param pDP Pointer to the IDataSet to assign.
			/// @return Reference to this iterator.
			iterator& operator << (dp::IDataSet* pDP)
			{
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert((_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None));
				_rd[_pos]._pDP = pDP;
				return *this;
			}

			/// @brief Assigns a reference-counted data set pointer to the current container entry.
			/// @param pDP Smart pointer to the IDataSet to assign (reference count is incremented).
			/// @return Reference to this iterator.
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

			/// @brief Assigns a note string pointer to the current container entry.
			/// @param pNote Pointer to the note string.
			/// @return Reference to this iterator.
			iterator& operator << (td::String* pNote)
			{
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;

				assert((_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None));
				_rd[_pos]._pNote = pNote;
				return *this;
			}

			/// @brief Checks whether the current element definition slot is valid (has a container or shape type).
			/// @return True if the current slot has a valid type, false otherwise.
			inline bool isOk() const
			{
				return (_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None);
			}
		};

		/// @brief Read-only iterator for traversing report container data.
		class const_iterator
		{
			friend class ReportData;
		protected:
			const ReportData& _rd; ///< Const reference to the owning ReportData.
			int _layoutNo = 0; ///< Current layout index tracked by this iterator.
			size_t _pos = 0; ///< Current position in the container data vector.
			int _k = -1; ///< Current field index within the container element definitions.

			/// @brief Constructs a const_iterator for the given ReportData at the specified position and field index.
			/// @param rd Const reference to the owning ReportData.
			/// @param pos Initial position index.
			/// @param k Initial field index.
			const_iterator(const ReportData& rd, size_t pos = 0, int k = -1)
				: _rd(rd)
				, _pos(pos)
				, _k(k)
			{
			}

		public:

			/// @brief Copy constructor.
			/// @param it Source const_iterator to copy.
			const_iterator(const const_iterator& it)
				: _rd(it._rd)
				, _pos(it._pos)
				, _k(it._k)
			{
					_layoutNo = _rd[_pos]._layoutNo;
			}
			/// @brief Advances the iterator to the next container entry.
			void operator ++ ()
			{
				++_pos;
				_k = -1;
				assert(_pos < _rd.size());
				if (_pos >= _rd.size())
					return;
				_layoutNo = _rd[_pos]._layoutNo;
			}

			/// @brief Advances the field index to the next element definition slot.
			void nextField()
			{
				++_k;
			}

			/// @brief Returns a mutable reference to the current ContainerData entry (const_cast).
			/// @return Reference to the current ContainerData.
			ContainerData& data () const
			{
				ReportData& rd = const_cast<ReportData&>(_rd);
				return rd[_pos];
			}

			/// @brief Returns the current element definition block.
			/// @return Const reference to the current ElemDef.
			const ElemDef& fieldBlock() const
			{
				return _rd[_pos]._elemDef[_k];
			}

			/// @brief Overrides the current layout number stored in this iterator.
			/// @param layoutNo New layout number to set.
			void setLayoutNo(int layoutNo)
			{
				_layoutNo = layoutNo;
			}

			/// @brief Reads the next element definition into elemDef.
			/// @param elemDef Output variable that receives the next element definition.
			/// @return Reference to this iterator.
			const_iterator& operator >> (ElemDef& elemDef)
			{
				++_k;
				assert(_k <= NREPDATA_CNT_ELEMS);
				if (_k > NREPDATA_CNT_ELEMS)
					return *this;
				elemDef = _rd[_pos]._elemDef[_k];
				return *this;
			}

			/// @brief Checks whether the current element definition slot is valid.
			/// @return True if the current slot has a valid container or shape type, false otherwise.
			inline bool isOk() const
			{
				return (_rd[_pos]._elemDef[_k].cntType != rpt::ContainerType::CNT_None) || (_rd[_pos]._elemDef[_k].shapeType != rpt::ShapeType::SHP_None);
			}

			/// @brief Builds and returns the full hierarchical name string for the current container entry.
			/// @return String representing the full name including layout prefix and element type suffixes.
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
		/// @brief Releases all data set references held by the container entries.
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
		/// @brief Constructs a ReportData with the specified number of container entries and associated metadata.
		/// @param nElems Number of container entries to allocate.
		/// @param docData Reference to document-level entity data.
		/// @param companyData Reference to company-level entity data.
		/// @param objectData Reference to object-level entity data.
		/// @param pDB Pointer to the database.
		/// @param translExtension Translation extension string for resource lookup.
		ReportData(size_t nElems, const rpt::DocumentData& docData, const rpt::CompanyData& companyData, const rpt::ObjectData& objectData, dp::IDatabase* pDB, const char* translExtension)
			: cnt::SafeFullVector<ContainerData>(nElems)
			, _docData(docData)
			, _companyData(companyData)
			, _objectData(objectData)
			, _pDB(pDB)
			, translationExtension(translExtension)
		{}

		/// @brief Destructor; releases all data set references.
		~ReportData()
		{
			cleanDP();
		}

		/// @brief Returns a mutable iterator positioned at the first container entry.
		/// @return Iterator at the beginning of the container data.
		iterator begin()
		{
			return iterator(*this);
		}

		/// @brief Returns a mutable iterator positioned at the specified container index.
		/// @param pos Zero-based index of the target container entry.
		/// @return Iterator positioned at the given index.
		iterator at(int pos)
		{
			iterator it(*this, pos, -1);
			return it;
		}

		/// @brief Returns a read-only iterator positioned at the specified container index with field index 0.
		/// @param pos Zero-based index of the target container entry.
		/// @return Const iterator positioned at the given index.
		const_iterator dataBlock(int pos) const
		{
			const_iterator it(*this, pos, 0);
			return it;
		}

		/// @brief Returns the translation extension string used for resource file lookup.
		/// @return Pointer to the translation extension C-string, or nullptr if not set.
		const char* getTranslationExtension() const
		{
			return translationExtension;
		}

		/// @brief Returns the database pointer associated with this report.
		/// @return Pointer to the IDatabase, or nullptr if not set.
		dp::IDatabase* getDB() const
		{
			return _pDB;
		}

		/// @brief Returns a const reference to the document-level entity data.
		/// @return Const reference to DocumentData.
		const rpt::DocumentData& getDocData() const
		{
			return _docData;
		}

		/// @brief Returns a const reference to the company-level entity data.
		/// @return Const reference to CompanyData.
		const rpt::CompanyData& getCompanyData() const
		{
			return _companyData;
		}

		/// @brief Returns a const reference to the object-level entity data.
		/// @return Const reference to ObjectData.
		const rpt::ObjectData& getObjectData() const
		{
			return _objectData;
		}

        /// @brief Returns a pointer to the signature data, if set.
        /// @return Const pointer to SignatureData, or nullptr if not set.
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

		/// @brief Sets the list of layout indices that should be removed from the configuration.
		/// @param pLayoutsToBeRemovedFromConfig Pointer to a vector of layout indices (0-based; the first layout (0) cannot be removed).
		void setLayoutsToBeRemovedFromConfig(const cnt::SafeFullVector<int>* pLayoutsToBeRemovedFromConfig)
		{
			_pLayoutsToBeRemovedFromConfig = pLayoutsToBeRemovedFromConfig;
		}

		/// @brief Returns the list of layout indices to be removed from the configuration.
		/// @return Pointer to the vector of layout indices, or nullptr if not set.
		const cnt::SafeFullVector<int>* getLayoutsToBeRemovedFromConfig() const
		{
			return _pLayoutsToBeRemovedFromConfig;
		}

		/// @brief Sets the GUI frame used as the context manager for this report.
		/// @param pConsumer Pointer to the gui::Frame context manager.
		void setContextManager(gui::Frame* pConsumer)
		{
			_pContextManager = pConsumer;
		}

		/// @brief Returns the GUI frame used as the context manager for this report.
		/// @return Const pointer to the gui::Frame context manager, or nullptr if not set.
		const gui::Frame* getContextManager() const
		{
			return _pContextManager;
		}

        /// @brief Sets the signature data for this report.
        /// @param pSign Pointer to the SignatureData to assign.
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
