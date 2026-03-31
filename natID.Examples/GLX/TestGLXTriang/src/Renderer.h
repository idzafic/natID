#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#define NUM_INSTANCES 1

#include <glx/IRenderer.h>
#include <glx/Device.h>
#include <glx/CommandQueue.h>
#include <glx/RenderPipeline.h>
#include <glx/Buffer.h>
#include <glx/Library.h>
#include <glx/Error.h>
#include <glx/Resource.h>
#include <glx/View.h>
#include <glx/Function.h>
#include <glx/VertexDescriptor.h>
#include <td/Point.h>
#include <td/Size.h>
#include <gui/InputDevice.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstring>
#include <vector>
#include <deque>
#include <mu/DebugConsoleLog.h>
#include <mu/Utils.h>
#include "InstancedObject.h"
#include <gui/gl/Triang2D.h>
#include <cnt/PushBackVector.h>
#include <math/math.h>

struct ObjectVertex {
    float position[2];
};

struct InstanceData {
    glm::vec4 color;
    glm::vec2 translation;
    float scale;
};

struct UniformBufferObject {
    glm::mat4 projection;
};

// Convert triangle fan vertices to triangle-list indices
inline void fanToTriangleIndices(uint16_t baseVertex, uint16_t vertexCount, std::vector<uint16_t>& indices)
{
    for (uint16_t i = 1; i + 1 < vertexCount; ++i)
    {
        indices.push_back(baseVertex);
        indices.push_back(static_cast<uint16_t>(baseVertex + i));
        indices.push_back(static_cast<uint16_t>(baseVertex + i + 1));
    }
}

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _uniformBuffer;

    glx::View* _pView = nullptr;

    gui::Size _viewportSize;

    // Pan and zoom
    glm::vec2 _panOffset = glm::vec2(0.0f, 0.0f);
    float _zoom = 1.0f;
    static constexpr float _zoomMin = 0.1f;
    static constexpr float _zoomMax = 50.0f;
    static constexpr float _scrollSpeed = 0.3f;
    static constexpr float _zoomKeyFactor = 1.1f;

    // Drag tracking
    glm::vec2 _lastDragPos = glm::vec2(0.0f, 0.0f);

    using InstancedObject = ::InstancedObject<ObjectVertex, InstanceData>;
    std::deque<InstancedObject> _objects;

    // Track which objects are letter E outline (drawn as line loop)
    InstancedObject* _letterEOutline = nullptr;

protected:

    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");

        library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load shader library from :shaders: " << err.c_str();
            assert(false);
            throw std::runtime_error("Failed to load shader library from :shaders");
        }

        glx::Function vertexFn = library.newFunction("mainVS");
        glx::Function fragFn = library.newFunction("mainFS");

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vertexFn);
        desc.setFragmentFunction(fragFn);

        setupVertexDescriptor(desc);

        glx::RenderPipeline::ColorAttachments clrAttachments = desc.colorAttachments();
        glx::RenderPipeline::ColorAttachment clrAtt = clrAttachments[0];
        clrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm);

        _RP = _device.newRenderPipelineState(desc, error);
        if (!_RP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create render pipeline";
            assert(false);
        }

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();

        mu::DebugConsoleLog::info() << "Ortho polygon pipeline created";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        // Per-vertex: position (vec2)
        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float2);
        posAttr.setOffset(offsetof(ObjectVertex, position));
        posAttr.setBufferIndex(0);

        // Per-instance: color (vec4)
        glx::VertexAttribute colorAttr = attributes[1];
        colorAttr.setFormat(glx::VertexFormat::Float4);
        colorAttr.setOffset(offsetof(InstanceData, color));
        colorAttr.setBufferIndex(1);

        
        glx::VertexAttribute transAttr = attributes[2];
        transAttr.setFormat(glx::VertexFormat::Float2);
        transAttr.setOffset(offsetof(InstanceData, translation));
        transAttr.setBufferIndex(1);

       
        glx::VertexAttribute scaleAttr = attributes[3];
        scaleAttr.setFormat(glx::VertexFormat::Float);
        scaleAttr.setOffset(offsetof(InstanceData, scale));
        scaleAttr.setBufferIndex(1);

        // Layout 0: per-vertex data
        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(ObjectVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

       
        glx::VertexBufferLayout layout1 = layouts[1];
        layout1.setStride(sizeof(InstanceData));
        layout1.setStepRate(1);
        layout1.setStepFunction(glx::VertexStepFunction::PerInstance);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }

    // Helper: add an InstancedObject from Triang2D fan output (vertices only, no indices from triangulator)
    void addFanObject(const cnt::PushBackVector<gui::gl::Point2d>& allVerts, uint16_t startVert, uint16_t nVerts, const glm::vec4& color)
    {
        std::vector<ObjectVertex> verts;
        for (uint16_t i = 0; i < nVerts; ++i)
        {
            const auto& p = allVerts[startVert + i];
            verts.push_back({{p.x, p.y}});
        }
        std::vector<uint16_t> indices;
        fanToTriangleIndices(0, nVerts, indices);
        _objects.emplace_back(verts, indices, NUM_INSTANCES);
        _objects.back().addInstance(InstanceData{color, glm::vec2(0.0f, 0.0f), 1.0f});
    }

    // Helper: add an InstancedObject from Triang2D indexed output (lines, complex polygons)
    void addIndexedObject(const cnt::PushBackVector<gui::gl::Point2d>& allVerts, const cnt::PushBackVector<td::UINT4>& allIndices,
                          uint16_t startVert, uint16_t nVerts, uint32_t startIdx, uint32_t nIndices, const glm::vec4& color)
    {
        std::vector<ObjectVertex> verts;
        for (uint16_t i = 0; i < nVerts; ++i)
        {
            const auto& p = allVerts[startVert + i];
            verts.push_back({{p.x, p.y}});
        }
        std::vector<uint16_t> indices;
        const td::UINT4* idxPtr = allIndices.begin() + startIdx;
        for (uint32_t i = 0; i < nIndices; ++i)
        {
            // Remap: original index references global vertex array, convert to local
            indices.push_back(static_cast<uint16_t>(idxPtr[i] - startVert));
        }
        _objects.emplace_back(verts, indices, NUM_INSTANCES);
        _objects.back().addInstance(InstanceData{color, glm::vec2(0.0f, 0.0f), 1.0f});
    }

    void buildObjects()
    {
        cnt::PushBackVector<gui::gl::Point2d> vboPoints;
        vboPoints.reserve(1024);

        cnt::PushBackVector<td::UINT4> eboIndices;
        eboIndices.reserve(1024);

        gui::gl::Triang2D tr(1024);

        // ---- Quad (red) ----
        {
            const gui::gl::Point2d quad[] = {{ 585, 360 }, { 630, 135 }, { 405, 180 }, { 360, 315 }};
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(quad, 4, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        // ---- Pentagon 1 (counter-clockwise, red) ----
        {
            const gui::gl::Point2d pentagon[] = {{ 100, 450 }, { 200, 450 }, { 90, 360 }, { 200, 180 }, { 45, 360 }};
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        // ---- Pentagon 2 (counter-clockwise, green) ----
        {
            gui::gl::Point2d pentagon[] = {{ 200, 450 }, { 90, 360 }, { 200, 180 }, { 45, 360 }, { 100, 450 }};
            math::translate(pentagon, 5, 150.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }

        // ---- Pentagon 3 (counter-clockwise, blue) ----
        {
            gui::gl::Point2d pentagon[] = {{ 90, 360 }, { 200, 180 }, { 45, 360 }, { 100, 450 }, { 200, 450 }};
            math::translate(pentagon, 5, 300.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }

        // ---- Pentagon 4 (counter-clockwise, yellow) ----
        {
            gui::gl::Point2d pentagon[] = {{ 200, 180 }, { 45, 360 }, { 100, 450 }, { 200, 450 }, { 90, 360 }};
            math::translate(pentagon, 5, 450.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        // ---- Pentagon 5 (clockwise, red) ----
        {
            gui::gl::Point2d pentagon[] = {{ 45, 360 }, { 200, 180 }, { 90, 360 }, { 200, 450 }, { 100, 450 }};
            math::translate(pentagon, 5, 600.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        // ---- Pentagon 6 (clockwise, green) ----
        {
            gui::gl::Point2d pentagon[] = {{ 100, 450 }, { 45, 360 }, { 200, 180 }, { 90, 360 }, { 200, 450 }};
            math::translate(pentagon, 5, 750.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }

        // ---- Pentagon 7 (clockwise, blue) ----
        {
            gui::gl::Point2d pentagon[] = {{ 200, 450 }, { 100, 450 }, { 45, 360 }, { 200, 180 }, { 90, 360 }};
            math::translate(pentagon, 5, 900.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }

        // ---- Pentagon 8 (clockwise, yellow) ----
        {
            gui::gl::Point2d pentagon[] = {{ 90, 360 }, { 200, 450 }, { 100, 450 }, { 45, 360 }, { 200, 180 }};
            math::translate(pentagon, 5, 1050.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            auto [triangType, nIndices] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            uint16_t nVerts = (uint16_t)(vboPoints.size() - startVert);
            addFanObject(vboPoints, startVert, nVerts, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        // ---- Lines (clockwise order) ----
        float lineWidth = 5.0f;
        bool bClosedLine = false;

        // Line1 (red)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }};
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 2, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        // Line2 (green)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }};
            math::translate(line, 3, 150.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 3, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }

        // Line3 (blue)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }, { 220, 550 }, { 230, 750 }};
            math::translate(line, 5, 300.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 5, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }

        // Line4 (white, open rect)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            math::translate(line, 4, 450.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }

        // Line5 (yellow, closed rect)
        {
            bClosedLine = true;
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            math::translate(line, 4, 600.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        // ---- Lines (counter-clockwise order) ----
        bClosedLine = false;

        // Line1CC (red)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }};
            mu::reverse(line, 2);
            math::translate(line, 2, 750.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 2, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        // Line2CC (green)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }};
            mu::reverse(line, 3);
            math::translate(line, 3, 900.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 3, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }

        // Line3CC (blue)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }, { 220, 550 }, { 230, 750 }};
            mu::reverse(line, 5);
            math::translate(line, 5, 1050.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 5, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }

        // Line4CC (white, open rect)
        {
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            mu::reverse(line, 4);
            math::translate(line, 4, 1200.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }

        // Line5CC (yellow, closed rect)
        {
            bClosedLine = true;
            gui::gl::Point2d line[] = {{ 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            mu::reverse(line, 4);
            math::translate(line, 4, 1350.0f, 0.0f);
            uint16_t startVert = (uint16_t)vboPoints.size();
            uint32_t startIdx = (uint32_t)eboIndices.size();
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            addIndexedObject(vboPoints, eboIndices, startVert, nVertices, startIdx, nIndices, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        // ---- Letter E (blue, with white outline) ----
        {
            const float h = 100;
            const float h1 = h / 5;
            const float h2 = h * 2 / 5;
            const float h3 = h * 3 / 5;
            const float h4 = h * 4 / 5;
            const float w = h / 2;
            const float w1 = w / 3;
            const float w2 = w * 4 / 5;

            gui::gl::Point2d letterE[] = {
                {0, 0}, {0, h}, {w, h}, {w, h4}, {w1, h4}, {w1, h3},
                {w2, h3}, {w2, h2}, {w1, h2}, {w1, h1}, {w, h1}, {w, 0}
            };
            td::UINT4 nVertsE = 12;
            for (td::UINT4 i = 0; i < nVertsE; ++i)
            {
                letterE[i].x += 200;
                letterE[i].y += 200;
            }

            uint16_t startVertE = (uint16_t)vboPoints.size();
            uint32_t startIdxE = (uint32_t)eboIndices.size();
            auto [triangTypeE, nIndicesE] = tr.triangulate(letterE, nVertsE, vboPoints, eboIndices);
            uint16_t nVertsAdded = (uint16_t)(vboPoints.size() - startVertE);
            addIndexedObject(vboPoints, eboIndices, startVertE, nVertsAdded, startIdxE, nIndicesE, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

            // Letter E outline (white) — use original letterE vertices as a line loop
            // We create this as a line strip with the vertices forming the outline
            std::vector<ObjectVertex> outlineVerts;
            for (td::UINT4 i = 0; i < nVertsE; ++i)
                outlineVerts.push_back({{letterE[i].x, letterE[i].y}});
            // Close the loop by repeating first vertex
            outlineVerts.push_back({{letterE[0].x, letterE[0].y}});
            // Create line indices for line strip as individual line segments
            std::vector<uint16_t> outlineIndices;
            for (uint16_t i = 0; i < (uint16_t)nVertsE; ++i)
            {
                outlineIndices.push_back(i);
                outlineIndices.push_back(static_cast<uint16_t>(i + 1));
            }
            _objects.emplace_back(outlineVerts, outlineIndices, NUM_INSTANCES);
            _letterEOutline = &_objects.back();
            _letterEOutline->addInstance(InstanceData{glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 1.0f});
        }

        const size_t uniformDataSize = sizeof(UniformBufferObject);
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);

        mu::DebugConsoleLog::info() << "Triangulated objects created: " << _objects.size() << " objects";
    }

    void updateProjection()
    {
        float height = _viewportSize.height / _zoom;
        float width =  _viewportSize.width  / _zoom;   

        float left   =  _panOffset.x;
        float right  =  _panOffset.x + width;
        float bottom =  _panOffset.y + height;
        float top    =  _panOffset.y;

        glm::mat4 proj = glm::ortho(left, right, bottom, top, 0.0f, 1.0f);

        UniformBufferObject ubo;
        ubo.projection = proj;

        memcpy(_uniformBuffer.contents(), &ubo, sizeof(UniformBufferObject));
        glx::Buffer::Range range(0, sizeof(UniformBufferObject));
        _uniformBuffer.didModifyRange(range);
    }

public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _pView = pView;
        _commandQueue = _device.newCommandQueue();
        pView->getSize(_viewportSize);

        InstancedObject::initializeBuffers(_device);

        buildShaders();
        buildObjects();
        updateProjection();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);

        mu::DebugConsoleLog::info() << "Ortho polygon renderer initialized";
    }

    ~Renderer()
    {
        InstancedObject::releaseBuffers();

        _uniformBuffer.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
		static bool firstResize = true;
        if(firstResize)
        {
            firstResize = false;
            mu::DebugConsoleLog::info() << "Initial viewport size: " << sz.width << "x" << sz.height;
		}
        _viewportSize = sz;
        updateProjection();

        mu::DebugConsoleLog::debug() << "Viewport resized to: " << sz.width << "x" << sz.height;
    }

    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }

    void handleKeyPressed(const gui::Key& key)
    {
        if (key.isCtrlPressed())
        {
            td::UINT4 ch = key.getCode();
            static int cnt = 0;
            if (cnt++ < 5)
                return;
            mu::DebugConsoleLog::debug() << "Pressed: " << ch;
            gui::Point centerPoint(_viewportSize.width / 2, _viewportSize.height / 2);
            if (ch == '+' || ch == 187)
            {
                handleZoom(_zoomKeyFactor, centerPoint);
            }
            else if (ch == '-' || ch == 189)
            {
                handleZoom(1.0 / _zoomKeyFactor, centerPoint);
            }
        }
    }

    void handleScroll(const gui::Point& scrollDelta, bool shiftPressed)
    {
        float speed = _scrollSpeed / _zoom;
        if (shiftPressed)
            _panOffset.x -= static_cast<float>(scrollDelta.y) * speed;
        else
            _panOffset.y -= static_cast<float>(scrollDelta.y) * speed;
        updateProjection();
    }

    void handleZoom(double scale, const gui::Point& framePoint)
    {
        scale = std::abs(scale);

		double newZoom = glm::clamp(_zoom * static_cast<float>(scale), _zoomMin, _zoomMax);

        double offsetX = (_viewportSize.width / _zoom - _viewportSize.width / (newZoom));
        double offsetY = (_viewportSize.height / _zoom -_viewportSize.height / (newZoom));

		_panOffset.x += offsetX * (framePoint.x/_viewportSize.width);
        _panOffset.y += offsetY * ((_viewportSize.height - framePoint.y) / _viewportSize.height);

        mu::DebugConsoleLog::debug() << "Zoom to point: " << framePoint.x << ", " << _viewportSize.height - framePoint.y << " | New zoom: " << newZoom;

        _zoom = newZoom;

        updateProjection();
    }

    void handleMousePressed(const gui::Point& framePoint)
    {
        _lastDragPos = glm::vec2(static_cast<float>(framePoint.x), static_cast<float>(framePoint.y));
    }

    void handleMouseDragged(const gui::Point& framePoint, bool shiftPressed)
    {
        glm::vec2 currentPos(static_cast<float>(framePoint.x), static_cast<float>(framePoint.y));

        if (shiftPressed)
        {
            float aspect = _viewportSize.width / _viewportSize.height;
            float halfHeight = 0;// _baseHalfHeight / _zoom;
            float halfWidth = halfHeight * aspect;

            float dx = (currentPos.x - _lastDragPos.x) / _zoom;
            float dy = (currentPos.y - _lastDragPos.y) / _zoom;

            _panOffset.x -= dx;
            _panOffset.y += dy;
            updateProjection();
        }

        _lastDragPos = currentPos;
    }

    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        cmdEnc.setRenderPipelineState(_RP);
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 6);

        // Draw all objects
        for (auto& obj : _objects)
        {
            if (&obj == _letterEOutline)
            {
                obj.draw(cmdEnc, 0, 1, glx::PrimitiveType::Line);
            }
            else
            {
                obj.draw(cmdEnc, 0, 1);
            }
        }

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
