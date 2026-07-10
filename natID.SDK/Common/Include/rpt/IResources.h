// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IResources.h
    @brief Interface for managing report resources such as fonts, images, colors, and text strings. */
#pragma once
#include "rptLib.h"
#include <td/String.h>
#include <gui/Font.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

namespace rpt
{

/// @brief Abstract interface for a report resource manager that stores and retrieves named assets.
class IResources
{

public:
    /// @brief Sets the separator character used to delimit group hierarchy levels.
    /// @param sep The character to use as a group separator.
    virtual void setGroupSeparator(char sep) = 0;
    /// @brief Adds a top-level resource group with the given name.
    /// @param name Name of the new group.
    /// @return The assigned group identifier.
    virtual td::INT1 addGroup(const td::String& name) = 0;
    /// @brief Adds a resource group as a child of an existing group.
    /// @param name Name of the new group.
    /// @param parentFullName Full hierarchical name of the parent group.
    /// @return The assigned group identifier.
    virtual td::INT1 addGroup(const td::String& name, const td::String& parentFullName) = 0;
    /// @brief Checks whether a group with the specified name exists.
    /// @param name Name of the group to check.
    /// @return True if the group exists, false otherwise.
    virtual bool groupExists(const td::String& name) const = 0;
    /// @brief Retrieves the identifier of a named group.
    /// @param name Name of the group.
    /// @return The group identifier, or a sentinel value if not found.
    virtual td::INT1 getGroupID(const td::String& name) const = 0;

    /// @brief Adds a memo (multi-line text) resource to a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param value Memo text value.
    /// @return True on success, false otherwise.
    virtual bool addMemo(td::INT1 groupID, const td::String& key, const td::String& value) = 0;
    /// @brief Adds a single-line text resource to a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param value Text value.
    /// @return True on success, false otherwise.
    virtual bool addText(td::INT1 groupID, const td::String& key, const td::String& value) = 0;
    /// @brief Adds a font resource to a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param ptr Pointer to the font object.
    /// @return True on success, false otherwise.
    virtual bool addFont(td::INT1 groupID, const td::String& key, const gui::Font* ptr) = 0;
    /// @brief Adds a color resource to a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param id Color identifier.
    /// @param alpha Alpha (opacity) component, default is fully opaque (0xFF).
    /// @return True on success, false otherwise.
    virtual bool addColor(td::INT1 groupID, const td::String& key, td::ColorID id, td::BYTE alpha = 0xFF) = 0;
    /// @brief Adds an image resource to a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param ptr Pointer to the image object.
    /// @return True on success, false otherwise.
    virtual bool addImage(td::INT1 groupID, const td::String& key, const gui::Image* ptr) = 0;
    /// @brief Adds a symbol resource to a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param ptr Pointer to the symbol object.
    /// @return True on success, false otherwise.
    virtual bool addSymbol(td::INT1 groupID, const td::String& key, const gui::Symbol* ptr) = 0;
    /// @brief Adds a memo resource to a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param value Memo text value.
    /// @return True on success, false otherwise.
    virtual bool addMemo(const td::String& groupName, const td::String& key, const td::String& value) = 0;
    /// @brief Adds a single-line text resource to a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param value Text value.
    /// @return True on success, false otherwise.
    virtual bool addText(const td::String& groupName, const td::String& key, const td::String& value) = 0;
    /// @brief Adds a font resource to a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param ptr Pointer to the font object.
    /// @return True on success, false otherwise.
    virtual bool addFont(const td::String& groupName, const td::String& key, const gui::Font* ptr) = 0;
    /// @brief Adds a color resource to a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param id Color identifier.
    /// @param alpha Alpha (opacity) component, default is fully opaque (0xFF).
    /// @return True on success, false otherwise.
    virtual bool addColor(const td::String& groupName, const td::String& key, td::ColorID id, td::BYTE alpha = 0xFF) = 0;
    /// @brief Adds an image resource to a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param ptr Pointer to the image object.
    /// @return True on success, false otherwise.
    virtual bool addImage(const td::String& groupName, const td::String& key, const gui::Image* ptr) = 0;
    /// @brief Adds a symbol resource to a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param ptr Pointer to the symbol object.
    /// @return True on success, false otherwise.
    virtual bool addSymbol(const td::String& groupName, const td::String& key, const gui::Symbol* ptr) = 0;

    /// @brief Loads a memo resource from a file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadMemoFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    /// @brief Loads a text resource from a file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadTextFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    //bool loadFontFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources);
    //bool loadColorFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources);
    /// @brief Loads an image resource from a file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadImageFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    /// @brief Loads a symbol resource from a file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadSymbolFromFile(td::INT1 groupID, const td::String& key, const td::String& filepathInResources) = 0;
    /// @brief Loads a memo resource from a file into a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadMemoFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;
    /// @brief Loads a text resource from a file into a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadTextFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;
    //bool loadFontFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources);
    //bool loadColorFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources);
    /// @brief Loads an image resource from a file into a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadImageFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;
    /// @brief Loads a symbol resource from a file into a named group.
    /// @param groupName Name of the target group.
    /// @param key Resource key.
    /// @param filepathInResources Path to the file within the resource system.
    /// @return True on success, false otherwise.
    virtual bool loadSymbolFromFile(const td::String& groupName, const td::String& key, const td::String& filepathInResources) = 0;

    /// @brief Loads all memo resources from an XML file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadMemosFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    /// @brief Loads all text resources from an XML file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadTextsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    /// @brief Loads all font resources from an XML file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadFontsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    /// @brief Loads all color resources from an XML file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadColorsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    /// @brief Loads all image resources from an XML file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadImagesFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    /// @brief Loads all symbol resources from an XML file into a group by ID.
    /// @param groupID Identifier of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadSymbolsFromXML(td::INT1 groupID, const td::String& xmlPath) = 0;
    /// @brief Loads all memo resources from an XML file into a named group.
    /// @param groupName Name of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadMemosFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    /// @brief Loads all text resources from an XML file into a named group.
    /// @param groupName Name of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadTextsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    /// @brief Loads all font resources from an XML file into a named group.
    /// @param groupName Name of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadFontsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    /// @brief Loads all color resources from an XML file into a named group.
    /// @param groupName Name of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadColorsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    /// @brief Loads all image resources from an XML file into a named group.
    /// @param groupName Name of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadImagesFromXML(const td::String& groupName, const td::String& xmlPath) = 0;
    /// @brief Loads all symbol resources from an XML file into a named group.
    /// @param groupName Name of the target group.
    /// @param xmlPath Path to the XML resource file.
    /// @return True on success, false otherwise.
    virtual bool loadSymbolsFromXML(const td::String& groupName, const td::String& xmlPath) = 0;

    //reading ResourceGroup
    /// @brief Retrieves a memo resource from a group by ID and key.
    /// @param groupID Identifier of the group.
    /// @param key Resource key.
    /// @return Pointer to the memo string, or nullptr if not found.
    virtual const td::String* getMemo(td::INT1 groupID, const td::String& key) const = 0;
    /// @brief Retrieves a text resource from a group by ID and key.
    /// @param groupID Identifier of the group.
    /// @param key Resource key.
    /// @return Pointer to the text string, or nullptr if not found.
    virtual const td::String* getText(td::INT1 groupID, const td::String& key) const = 0;
    /// @brief Retrieves a font resource from a group by ID and key.
    /// @param groupID Identifier of the group.
    /// @param key Resource key.
    /// @return Pointer to the font object, or nullptr if not found.
    virtual const gui::Font* getFont(td::INT1 groupID, const td::String& key) const = 0;
    /// @brief Retrieves a color identifier from a group by ID and key.
    /// @param groupID Identifier of the group.
    /// @param key Resource key.
    /// @return The color identifier associated with the key.
    virtual td::ColorID getColorID(td::INT1 groupID, const td::String& key) const = 0;
//    virtual const ResourceGroup::Color getColor(td::INT1 groupID, const td::String& key) const = 0;
    /// @brief Retrieves an image resource from a group by ID and key.
    /// @param groupID Identifier of the group.
    /// @param key Resource key.
    /// @return Pointer to the image object, or nullptr if not found.
    virtual const gui::Image* getImage(td::INT1 groupID, const td::String& key) const = 0;
    /// @brief Retrieves a symbol resource from a group by ID and key.
    /// @param groupID Identifier of the group.
    /// @param key Resource key.
    /// @return Pointer to the symbol object, or nullptr if not found.
    virtual const gui::Symbol* getSymbol(td::INT1 groupID, const td::String& key) const = 0;
    /// @brief Retrieves a memo resource from a named group by key.
    /// @param groupName Name of the group.
    /// @param key Resource key.
    /// @return Pointer to the memo string, or nullptr if not found.
    virtual const td::String* getMemo(const td::String& groupName, const td::String& key) const = 0;
    /// @brief Retrieves a text resource from a named group by key.
    /// @param groupName Name of the group.
    /// @param key Resource key.
    /// @return Pointer to the text string, or nullptr if not found.
    virtual const td::String* getText(const td::String& groupName, const td::String& key) const = 0;
    /// @brief Retrieves a font resource from a named group by key.
    /// @param groupName Name of the group.
    /// @param key Resource key.
    /// @return Pointer to the font object, or nullptr if not found.
    virtual const gui::Font* getFont(const td::String& groupName, const td::String& key) const = 0;
//    virtual const ResourceGroup::Color getColor(const td::String& groupName, const td::String& key) const = 0;
    /// @brief Retrieves an image resource from a named group by key.
    /// @param groupName Name of the group.
    /// @param key Resource key.
    /// @return Pointer to the image object, or nullptr if not found.
    virtual const gui::Image* getImage(const td::String& groupName, const td::String& key) const = 0;
    /// @brief Retrieves a symbol resource from a named group by key.
    /// @param groupName Name of the group.
    /// @param key Resource key.
    /// @return Pointer to the symbol object, or nullptr if not found.
    virtual const gui::Symbol* getSymbol(const td::String& groupName, const td::String& key) const = 0;

    //load all resources from xmlFile
//		friend Resources loadFromXML(const td::String& xmlPath);
    /// @brief Loads all resources from an XML file.
    /// @param fileName Path to the XML file.
    /// @return True on success, false otherwise.
    virtual bool loadFromXML(const char* fileName) = 0;

    /// @brief Returns the singleton resource manager instance.
    /// @return Pointer to the IResources manager.
    NATRPT_API static IResources* getManager();

//    virtual bool loadFromXML(const char* xmlPath) = 0;
};



}
