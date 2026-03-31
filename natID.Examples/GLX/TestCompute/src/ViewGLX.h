#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>
#include <gui/FileDialog.h>
#include <gui/Texture.h>
#include <gui/Alert.h>
#include <mu/DebugConsoleLog.h>

class ViewGLX : public glx::View
{
protected:
    Renderer* _pRenderer = nullptr;  // Keep reference to our renderer
    gui::Texture _frameTexture; //for frame export
    
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
    }
    

public:
    ViewGLX()
    {
    }
    

    void exportFrame()
    {
        _frameTexture.setExportJPGQuality(80); //80%
        
        // Note: readFrameBuffer method name might be different for GLX
        // Let's comment this out for now to avoid compilation error
        /*
        readFrameBuffer(_frameTexture, [this]()
        {
            _frameTexture.mirrorVertically();
            
            td::UINT4 dlgID = 6666; //should be unique dlg ID
            
            gui::SaveFileDialog::show(this, "Export Frame", {{"JPEG file with losses", "*.jpg"}, {"PNG file (no losses)", "*.png"},{"TGA file (no losses)", "*.tga"},{"Bitmap file (no losses)", "*.bmp"}}, dlgID, [this](gui::FileDialog* pFD)
            {
              if (pFD->getStatus() == gui::FileDialog::Status::OK)
              {
                  const char* pExt = pFD->getFileExtension(); //just for debug
                  if (_frameTexture.saveToFile(pFD->getFileName()))
                  {
                      gui::Alert::show("Export Frame OK", pFD->getFileName());
                  }
              }
            }, "Frame");
        });
        */
        
        mu::DebugConsoleLog::info() << "Export frame functionality not yet implemented for GLX";
    }
};
