// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include <glx/Texture.h>
#include <glx/RenderPipeline.h>
#include <glx/Buffer.h>
#include <glx/SamplerState.h>
#include <vector>
#include <map>
#include <functional>
#include <td/Types.h>
#include "natGLX.h"
#include <glm/glm.hpp>
#include <glx/CommandQueue.h>

// Forward declare ObjectCache template
template<typename Key, typename FactoryFnc, typename CleanupFnc>
class ObjectCache;

namespace glx
{
	class Device;

	class View;

	class RenderCommandEncoder;

	// Pipeline configuration key for ObjectCache
	struct PipelineConfig
	{
		PixelFormat colorFormat;
		PixelFormat depthFormat;
		mutable Device device; // Not compared, used for pipeline creation

		bool operator==(const PipelineConfig& other) const
		{
			return colorFormat == other.colorFormat && depthFormat == other.depthFormat;
		}
	};

	class NATGLX_API Font
	{
	public:
		enum class HorizontalAlignment
		{
			Left,
			Center,
			Right
		};

		struct Glyph
		{
			float left;
			float width;
			float advance;
			float tx;  // texture coordinate
		};

		struct GlyphSize
		{
			float PixelsPerTx = 0.0f;        // how many pixels on the monitor to take up per one pixel in the texture
			float heightLogPixels = 0.0f;    // Height in logical pixels
			float widthLogPixels = 0.0f;     // Width in logical pixels
			float spaceWidth = 0.0f;         //
			float spaceBetweenGlyphs = 0.0f; //
			float backingFactor = 1.0f;     
		};

		struct TextVertex
		{
			float position[2];    // vec2
			float texCoord[2];    // vec2
			float color[4];       // vec4
		};

		struct FontUniforms
		{
			glm::mat4 mvp;  // Model-View-Projection matrix
		};

		// Structure to manage single consolidated buffer
		struct BufferManager
		{
			Buffer buffer;                    // Single managed buffer containing all data
			size_t totalSize = 0;             // Total buffer size
			
			// Uniform data section
			size_t uniformOffset = 0;         // Offset to uniform data
			size_t uniformSize = 0;           // Size of uniform data
			
			// Vertex data section
			size_t vertexDataOffset = 0;      // Offset to vertex data
			size_t vertexDataSize = 0;        // Size of vertex data section
			
			// Index data section
			size_t indexDataOffset = 0;       // Offset to index data
			size_t indexDataSize = 0;         // Size of index data section
			
			// Current counts
			size_t currentVertexCount = 0;
			size_t currentIndexCount = 0;
			size_t maxChars = 0;              // Maximum number of characters
		};

	private:
		// Static shared resources (same for all Font instances)
		static SamplerState _staticTextureSampler;
		static bool _staticResourcesInitialized;
		static int _instanceCount;  // Track number of Font instances
		static ObjectCache<PipelineConfig, RenderPipeline(*)(const PipelineConfig&), void(*)(const PipelineConfig&, RenderPipeline&)>* _pipelineCache;

		// Static initialization and cleanup functions
		static bool initializeStaticResources(Device& device, PixelFormat drawableFormat);
		static void releaseStaticResources();
		
		// Static helper to get DPI (dots per inch)
		static float getDPI();
		
		// Pipeline creation function
		static RenderPipeline createPipelineForConfig(const PipelineConfig& config);

		// Private method to update MVP matrix when viewport changes
		void updateMVPMatrix();
		
		// Helper method to add a single character
		void addChar(double& x, double& y, size_t glyphIndex, td::Color color);
		
		// Private addText method with dryRun parameter - returns bounding box size
		glx::Size addText(const td::String& utf8Text, double x, double y, td::Color color, bool dryRun);


		Device _device = nullptr;
		CommandQueue _commandQueue;
		bool _initialised = false, _retainedExternal = false;

		Texture _texture;
		
		// Consolidated buffer manager
		BufferManager _bufferCpu;

		float _viewportWidth = 0.0f;
		float _viewportHeight = 0.0f;

		std::vector<Glyph> _glyphs;
		std::map<td::UINT4, td::UINT2> _unicodeToPos;

		float _fontSize = 0.0f;

		td::UINT4 _textureWidth = 0;
		td::UINT4 _textureHeight = 0;
		
		GlyphSize _glyphSize;  // Calculated glyph size information
		
		void release();

		void draw(glx::RenderCommandEncoder &activeEncoder, glx::Size viewportSize);
		friend class RenderCommandEncoder;

	public:
		Font();
		~Font();

		// Disable copy and move operations
		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;
		Font(Font&&) = delete;
		Font& operator=(Font&&) = delete;

		// Initialization and cleanup - added maxChars parameter
		bool init(CommandQueue commandQueue, PixelFormat drawableFormat, td::String filePath, int pixelsHeight, size_t maxChars);
		//bool init(CommandQueue commandQueue, PixelFormat drawableFormat, const char* file, double pt, size_t maxChars);
		void reset();

		bool isOk() const;
		float getFontSize() const { return _fontSize; }
		float getSpaceWidth() const { return _glyphSize.spaceWidth; }
		const Texture& getTexture() const { return _texture; }
		const Buffer& getCPUBuffer() const { return _bufferCpu.buffer; }
		size_t getVertexCount() const { return _bufferCpu.currentVertexCount; }

		glx::Size addText(const td::String& text, double x, double y, td::Color color, HorizontalAlignment alignment = HorizontalAlignment::Left);
		void draw(View* pView, bool clearScreen);
		
	};


}