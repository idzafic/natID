// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "rptLib.h"
#include <td/String.h>
#include <gui/Font.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

namespace rpt
{

class IResources
{

public:
    virtual void setGroupSeparator(char sep) = 0;
    virtual td::INT1 addGroup(const td::String& name) = 0;
    virtual td::INT1 addGroup(const td::String& name, const td::String& parentFullName) = 0;
    virtual bool groupExists(const td::String& name) const = 0;
    virtual td::INT1 getGroupID(const td::String& name) const = 0;

    virtual bool addMemo(td::INT1 groupID, const td::String& key, const td::String& value) = 0;
    virtual bool addText(td::INT1 groupID, const td::String& key, const td::String& value) = 0;
    virtual bool addFont(td::INT1 groupID, const td::String& key, const gui::Font* ptr) = 0;
    virtual bool addColor(td::INT1 groupID, const td::String& key, td::ColorID id, td::BYTE alpha = 0xFF) = 0;
    virtual bool addImage(td::INT1 groupID, const td::String& key, const gui::Image* ptr) = 0;
    virtual bool addSymbol(td::INT1 groupID, const td::String& key, const gui::Symbol* ptr) = 0;
    virtual bool addMemo(const td::String& groupName, const td::String& key, const td::String& value) = 0;
    virtual bool addText(const td::String& groupName, const td::String& key, const td::String& value) = 0;
    virtual bool addFont(const td::String& groupName, const td::String& key, const gui::Font* ptr) = 0;
    virtual bool addColor(const td::String& groupName, const td::String& key, td::ColorID id, td::BYTE alpha = 0xFF) = 0;
    virtual bool addImage(const td::String& groupName, const td::String& key, const gui::Image* ptr) = 0;
    virtual bool addSymbol(const td::String& groupName, const td::String& key, const gui::Symbol* ptr) = 0;

    virtual bool loadMemoFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    virtual bool loadTextFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    //bool loadFontFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources);
    //bool loadColorFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources);
    virtual bool loadImageFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    virtual bool loadSymbolFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    virtual bool loadMemoFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;
    virtual bool loadTextFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;
    //bool loadFontFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources);
    //bool loadColorFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources);
    virtual bool loadImageFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;
    virtual bool loadSymbolFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;

    virtual bool loadMemosFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    virtual bool loadTextsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    virtual bool loadFontsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    virtual bool loadColorsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    virtual bool loadImagesFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    virtual bool loadSymbolsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    virtual bool loadMemosFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    virtual bool loadTextsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    virtual bool loadFontsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    virtual bool loadColorsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    virtual bool loadImagesFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    virtual bool loadSymbolsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;

    //reading ResourceGroup
    virtual const td::String* getMemo(td::INT1 groupID, const td::String& key) const = 0;
    virtual const td::String* getText(td::INT1 groupID, const td::String& key) const = 0;
    virtual const gui::Font* getFont(td::INT1 groupID, const td::String& key) const = 0;
    virtual td::ColorID getColorID(td::INT1 groupID, const td::String& key) const = 0;
//    virtual const ResourceGroup::Color getColor(td::INT1 groupID, const td::String& key) const = 0;
    virtual const gui::Image* getImage(td::INT1 groupID, const td::String& key) const = 0;
    virtual const gui::Symbol* getSymbol(td::INT1 groupID, const td::String& key) const = 0;
    virtual const td::String* getMemo(const td::String& groupName, const td::String& key) const = 0;
    virtual const td::String* getText(const td::String& groupName, const td::String& key) const = 0;
    virtual const gui::Font* getFont(const td::String& groupName, const td::String& key) const = 0;
//    virtual const ResourceGroup::Color getColor(const td::String& groupName, const td::String& key) const = 0;
    virtual const gui::Image* getImage(const td::String& groupName, const td::String& key) const = 0;
    virtual const gui::Symbol* getSymbol(const td::String& groupName, const td::String& key) const = 0;

    //load all resources from xmlFile
//		friend Resources loadFromXML(const td::String& xmlPath);
    virtual bool loadFromXML(const char* fileName) = 0;
    
    NATRPT_API static IResources* getManager();
    
//    virtual bool loadFromXML(const char* xmlPath) = 0;
};



}
