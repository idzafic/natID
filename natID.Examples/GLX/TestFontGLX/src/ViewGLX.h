#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <mu/DebugConsoleLog.h>

class ViewGLX : public glx::View
{
protected:
    Renderer* _pRenderer = nullptr;  // Keep reference to our renderer
    
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
        td::UINT4 dlgID = 6666;
        auto pFD = gui::SaveFileDialog::show(this, "Export Frame", 
            {
                {"PNG file (no losses)", "*.png"}, 
                {"JPEG file with losses", "*.jpg"},
                {"BMP file (no losses)", "*.bmp"},
                {"TGA file (no losses)", "*.tga"}
            }, 
            dlgID, 
            [this](gui::FileDialog* pFD)
            {
                td::String path = pFD->getFileName();
                if (!path.isNull())
                {
                    mu::DebugConsoleLog::info() << "Exporting frame to: " << path.c_str();
                    
                    // Determine format based on file extension
                    glx::ImageFormat format = glx::ImageFormat::PNG;
                    if (path.endsWith(".jpg") || path.endsWith(".jpeg"))
                    {
                        format = glx::ImageFormat::JPG;
                    }
                    else if (path.endsWith(".bmp"))
                    {
                        format = glx::ImageFormat::BMP;
                    }
                    else if (path.endsWith(".tga"))
                    {
                        format = glx::ImageFormat::TGA;
                    }
                    
                    // Get the current drawable and save it
                    glx::Drawable drawable = currentDrawable();
                    glx::CommandQueue cmdQueue = _pRenderer->getCommandQueue();
                    bool success = saveDrawable(path, cmdQueue, format);
                    
                    if (success)
                    {
                        mu::DebugConsoleLog::ok() << "Frame exported successfully to: " << path.c_str();
                        gui::Alert::show("Export Success", path);
                    }
                    else
                    {
                        mu::DebugConsoleLog::error() << "Failed to export frame to: " << path.c_str();
                        gui::Alert::show("Export Error", "Failed to save frame");
                    }
                }
            });
    }
};
