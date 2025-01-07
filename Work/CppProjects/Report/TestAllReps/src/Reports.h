#pragma once
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <mu/IAppSettings.h>
#include <gui/Image.h>
#include <gui/Frame.h>

void showSingleCard(const char* repConfigName);

void racunV1(const gui::Image*);
void racunV2(const gui::Image*);
void racunV3(const gui::Image* pImage);
void racunV4(const gui::Image* pImage);
void racunV5(const gui::Image* pImage, bool forExport);
void racunParallelTables(const gui::Image* pImage);

void karticaReport1(const gui::Image* pImage);
void karticaReport4(const gui::Image* pImage);
void landscapeTest(const gui::Image* pImage, const char* reportName);
void port2LandscapeRotation(const gui::Image* pImage);
void landScapeWithFormulaTest(const gui::Image* pImage);
void multiLayout(const gui::Image* pImage, int);

void karticaMultiReport(const gui::Image* pImage);
void karticaMultiReport2(const gui::Image* pImage);
void karticaMultiReport_WF(const gui::Image* pImage);

void itemsForStoreWindow6x3(const gui::Image* pImage);
void itemsForStoreWindow7x3(const gui::Image* pImage);

void showWaterfallSimple(const gui::Image* pImage);
void showWaterfallAdvanced(const gui::Image* pImage);
void showWaterfallAdvanced2(const gui::Image* pImage);
void dynamicBodyWithParallelTables(const gui::Image* pImage, gui::Frame* pContextManger);

