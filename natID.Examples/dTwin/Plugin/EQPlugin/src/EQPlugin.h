#pragma once
#include <compiler/Definitions.h>
#include <sc/IPlugin.h>
#include <gui/LineEdit.h>

#ifdef MU_WINDOWS
	#ifdef PLUGIN_EXPORTS
	#define PLUGIN_API __declspec(dllexport)
	#else
	#define PLUGIN_API __declspec(dllimport)
	#endif
#else
	#ifdef PLUGIN_EXPORTS
    #define PLUGIN_API __attribute__((visibility("default")))
	#else
    #define PLUGIN_API
	#endif
#endif


using Options = struct _Options
{
    td::String modelName;
    td::String accelorometerData;
    td::INT4 maxIter;
    float dTime;
    float endTime;
    bool useAccelorometerData;
};

void onClosedPluginWindow();

bool createModel(const td::String& inputFileName, const td::String& outFileName, sc::IPlugin* pIPlugin, const Options& options, gui::LineEdit& status); //in EQPlugin.cpp
