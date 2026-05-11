
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/ComboBox.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <fo/FileOperations.h>
#include <sc/IPlugin.h>
#include <syst/LibraryLoader.h>
#include <arch/MemoryOut.h>
#include <gui/Application.h>

class MainView : public gui::View
{
    using FuncType = sc::IPlugin*(*)();
    
    using PluginDesc = struct _PluginDesc
    {
        td::String path;
        sc::IPlugin* pIPlugin = nullptr;
    };
private:
    gui::Label _lblName;
    gui::ComboBox _cmbPlugins;
    gui::Label _lblLocation;
    gui::LineEdit _leLocation;
    gui::Label _lblVisualModel;
    gui::TextEdit _teDigitalModel;
    gui::TextEdit _teVisualModel;
    gui::GridLayout _gl;
    
    cnt::PushBackVector<PluginDesc> _plugins;
protected:
    void handleUserActions()
    {
        _cmbPlugins.onChangedSelection([this](){
            int iSel = _cmbPlugins.getSelectedIndex();
            if (iSel >= 0)
            {
                const td::String& location = _plugins[iSel].path;
                _leLocation = location;
            }
        });
    }
    
    void populatePlugins()
    {
        fo::fs::path setupPluginsPath = PLUGIN_DIR;
        
        const char* pluginExtension = fo::getSharedLibDefaultExtension();
        //check system (setup) plugins
        cnt::PushBackVector<td::String> userPluginFiles;
        userPluginFiles.reserve(16);
        fo::getFileNamesInFolder(userPluginFiles, setupPluginsPath, pluginExtension);
        
        int nPlugins = 0;
        for (const td::String& pluginName: userPluginFiles)
        {
            syst::LibraryLoader ll;
            if (!ll.load(pluginName))
                continue;
            
            FuncType f = ll.getFunctionPtr<FuncType>("getPluginInterface");
            if (!f)
                continue;
            sc::IPlugin* pPlugin = f();
            if (!pPlugin)
                continue;
            auto str = pPlugin->getMenuName();
            if (str.isEmpty())
                continue;
            
            auto& plugDesc =_plugins.push_back();
            plugDesc.path = pluginName;
            _cmbPlugins.addItem(str);
            ++nPlugins;
        }
        
        if (nPlugins > 0)
        {
            _cmbPlugins.selectIndex(0);
        }
        
    }
public:
    MainView()
    : _lblName(tr("Name:"))
    , _lblLocation(tr("Location:"))
    , _lblVisualModel(tr("Visual model:"))
    , _gl(5,2)
    {
        gui::Application* pApp = gui::getApplication();
        auto resPath = pApp->getResPath();
        auto testDataPath = resPath / "testData/Input_EQ_Model.txt";
        if (fo::fileExists(testDataPath))
        {
            td::String strNote;
            strNote.format("Note: If combo box is not empty, click on the Open button from the toolbar.\n\nThen select this file as an input for the conversion:\n%s\n\nThe content of the test input file is given below...", testDataPath.c_str());
            _teDigitalModel.setText(strNote);
            td::String strTestInputContent;
            fo::loadBinaryFile(testDataPath, strTestInputContent);
            _teVisualModel.setText(strTestInputContent);
        }
        _leLocation.setAsReadOnly();
        _teDigitalModel.setAsReadOnly();
        _teVisualModel.setAsReadOnly();
        
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblName) << _cmbPlugins;
        gc.appendRow(_lblLocation) << _leLocation;
        gc.appendRow(_teDigitalModel, 0);
        gc.appendRow(_lblVisualModel, 0);
        gc.appendRow(_teVisualModel, 0);
        setLayout(&_gl); //set the layout of this view
        handleUserActions();
        populatePlugins();
    }
    
    //menu and toolbar actions
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        if (menuID == 20 && actionID == 10)
        {
            //show the plugin
            int iSel = _cmbPlugins.getSelectedIndex();
            if (iSel < 0)
                return true; //no plugin detected, action handled -> return true
            
            sc::IPlugin* pPlugin = _plugins[iSel].pIPlugin;
            if (!pPlugin)
            {
                const td::String& pluginPath = _plugins[iSel].path;
                syst::LibraryLoader ll(false);
                if (!ll.load(pluginPath))
                    return true;
                
                FuncType f = ll.getFunctionPtr<FuncType>("getPluginInterface");
                if (!f)
                    return true;
                pPlugin = f();
                if (!pPlugin)
                    return true;
                _plugins[iSel].pIPlugin = pPlugin;
            }
            
            sc::IPlugin::MemoryArchiveContainer archives;
            archives[0] = arch::MemoryOut::allocate(arch::MemoryOut::PageSize::Normal);
            archives[1] = arch::MemoryOut::allocate(arch::MemoryOut::PageSize::Small);
            
            if (!archives[0]->open(nullptr))
                return true;
            
            if (!archives[1]->open(nullptr))
                return true;

            auto cleanPlugin = [archives](){
                for (size_t i=0; i<2; ++i)
                    archives[i]->release();
            };
            
            gui::Window* parentWnd = getParentWindow();
            td::UINT4 wndID = 10000 + td::UINT4(iSel);
            pPlugin->show(parentWnd, archives, wndID, cleanPlugin,
                          [this, cleanPlugin](sc::IPlugin* pIPlugin){
                td::String strFileName = pIPlugin->getOutFileName();
                if (fo::fileExists(strFileName))
                {
                    td::String strDigitalModelContent;
                    if (fo::loadFileContent(strFileName, strDigitalModelContent))
                        _teDigitalModel.setText(strDigitalModelContent);
                    
                    td::String visualModelName = fo::replaceFileExtension<false>(strFileName, ".vmodl");
                    if (fo::fileExists(visualModelName))
                    {
                        td::String strVisualModelContent;
                        if (fo::loadFileContent(visualModelName, strVisualModelContent))
                            _teVisualModel.setText(strVisualModelContent);
                    }
                    return;
                }
            });
            return true;
        }
        //always call the parent implemenation to enable proper message propagation
        return gui::View::onActionItem(aiDesc);
    }
};
