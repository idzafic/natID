// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Font.h @brief GPU-accelerated bitmap font renderer for drawing text in a GLX rendering context. */
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
	/// @brief Key structure identifying a render pipeline by its color and depth pixel formats.
	struct PipelineConfig
	{
		PixelFormat colorFormat; ///< Pixel format used for the color attachment.
		PixelFormat depthFormat; ///< Pixel format used for the depth attachment.
		mutable Device device; // Not compared, used for pipeline creation

		/// @brief Compares two PipelineConfig instances for equality based on pixel formats.
		/// @param other The other PipelineConfig to compare against.
		/// @return True if both color and depth formats match.
		bool operator==(const PipelineConfig& other) const
		{
			return colorFormat == other.colorFormat && depthFormat == other.depthFormat;
		}
	};

	/// @brief GPU-accelerated font renderer that uploads glyph geometry into a consolidated buffer
	///        and draws text using a Metal render pipeline.
	class NATGLX_API Font
	{
	public:
		/// @brief Horizontal alignment options for text layout.
		enum class HorizontalAlignment
		{
			Left,   ///< Align text to the left edge.
			Center, ///< Center text around the given x coordinate.
			Right   ///< Align text to the right edge.
		};

		/// @brief Per-glyph metrics stored in the glyph atlas.
		struct Glyph
		{
			float left;    ///< Left-side bearing in texture coordinates.
			float width;   ///< Glyph width in texture coordinates.
			float advance; ///< Horizontal advance to the next glyph.
			float tx;      // texture coordinate
		};

		/// @brief Computed size metrics for the loaded glyph set at the current DPI.
		struct GlyphSize
		{
			float PixelsPerTx = 0.0f;        // how many pixels on the monitor to take up per one pixel in the texture
			float heightLogPixels = 0.0f;    // Height in logical pixels
			float widthLogPixels = 0.0f;     // Width in logical pixels
			float spaceWidth = 0.0f;         //
			float spaceBetweenGlyphs = 0.0f; //
			float backingFactor = 1.0f;      ///< HiDPI backing scale factor.
		};

		/// @brief Vertex layout for a single text quad corner sent to the GPU.
		struct TextVertex
		{
			float position[2];    // vec2
			float texCoord[2];    // vec2
			float color[4];       // vec4
		};

		/// @brief Uniform data block passed to the font vertex shader.
		struct FontUniforms
		{
			glm::mat4 mvp;  // Model-View-Projection matrix
		};

		// Structure to manage single consolidated buffer
		/// @brief Manages a single GPU buffer that stores uniform, vertex, and index data contiguously.
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
			size_t currentVertexCount = 0;    ///< Number of vertices currently written into the buffer.
			size_t currentIndexCount = 0;     ///< Number of indices currently written into the buffer.
			size_t maxChars = 0;              // Maximum number of characters
		};

	private:
		// Static shared resources (same for all Font instances)
		static SamplerState _staticTextureSampler;     ///< Shared sampler state for the font texture atlas.
		static bool _staticResourcesInitialized;       ///< Tracks whether shared GPU resources have been created.
		static int _instanceCount;  // Track number of Font instances
		static ObjectCache<PipelineConfig, RenderPipeline(*)(const PipelineConfig&), void(*)(const PipelineConfig&, RenderPipeline&)>* _pipelineCache; ///< Cache of render pipelines keyed by pixel format configuration.

		// Static initialization and cleanup functions
		/// @brief Initialises shared GPU resources such as the sampler and pipeline cache.
		/// @param device The GPU device to create resources on.
		/// @param drawableFormat The pixel format of the drawable surface.
		/// @return True on success, false otherwise.
		static bool initializeStaticResources(Device& device, PixelFormat drawableFormat);

		/// @brief Releases all shared GPU resources and resets the instance count.
		static void releaseStaticResources();

		// Static helper to get DPI (dots per inch)
		/// @brief Returns the display DPI used for point-to-pixel conversion.
		/// @return The current display dots-per-inch value.
		static float getDPI();

		// Pipeline creation function
		/// @brief Creates a render pipeline for the given pixel format configuration.
		/// @param config The color and depth pixel format configuration.
		/// @return A fully compiled render pipeline object.
		static RenderPipeline createPipelineForConfig(const PipelineConfig& config);

		// Private method to update MVP matrix when viewport changes
		/// @brief Recomputes the orthographic MVP matrix to match the current viewport dimensions.
		void updateMVPMatrix();

		// Helper method to add a single character
		/// @brief Appends the quad for a single character to the CPU-side buffer.
		/// @param x Current pen x-position in logical pixels; advanced by this call.
		/// @param y Current pen y-position in logical pixels.
		/// @param glyphIndex Index into the _glyphs array for the character to draw.
		/// @param color RGBA color to apply to the character vertices.
		void addChar(double& x, double& y, size_t glyphIndex, td::Color color);

		// Private addText method with dryRun parameter - returns bounding box size
		/// @brief Lays out text starting at (x, y) and optionally appends vertices to the buffer.
		/// @param utf8Text UTF-8 encoded string to lay out.
		/// @param x Horizontal starting position in logical pixels.
		/// @param y Vertical starting position in logical pixels.
		/// @param color RGBA color for the text.
		/// @param dryRun When true, measures the bounding box without writing vertex data.
		/// @return The bounding size of the laid-out text.
		glx::Size addText(const td::String& utf8Text, double x, double y, td::Color color, bool dryRun);


		Device _device = nullptr;       ///< The GPU device used for buffer and pipeline creation.
		CommandQueue _commandQueue;     ///< Command queue for submitting rendering work.
		bool _initialised = false, _retainedExternal = false; ///< Initialisation and external-retention flags.

		Texture _texture;               ///< GPU texture holding the font glyph atlas.

		// Consolidated buffer manager
		BufferManager _bufferCpu;       ///< CPU-visible consolidated buffer for uniforms, vertices, and indices.

		float _viewportWidth = 0.0f;    ///< Current viewport width in pixels, used for MVP computation.
		float _viewportHeight = 0.0f;   ///< Current viewport height in pixels, used for MVP computation.

		std::vector<Glyph> _glyphs;                     ///< Array of per-glyph metrics indexed by atlas position.
		std::map<td::UINT4, td::UINT2> _unicodeToPos;   ///< Maps Unicode code points to glyph atlas positions.

		float _fontSize = 0.0f;         ///< Font size in points as loaded from the font file.

		td::UINT4 _textureWidth = 0;    ///< Width of the glyph atlas texture in pixels.
		td::UINT4 _textureHeight = 0;   ///< Height of the glyph atlas texture in pixels.

		GlyphSize _glyphSize;  // Calculated glyph size information

		/// @brief Releases all GPU and CPU resources held by this instance.
		void release();

		/// @brief Submits the accumulated text geometry for rendering.
		/// @param activeEncoder The render command encoder to record draw commands into.
		/// @param viewportSize The current viewport size in pixels.
		void draw(glx::RenderCommandEncoder &activeEncoder, glx::Size viewportSize);
		friend class RenderCommandEncoder;

	public:
		/// @brief Constructs an uninitialised Font object.
		Font();

		/// @brief Destructor; releases GPU resources.
		~Font();

		// Disable copy and move operations
		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;
		Font(Font&&) = delete;
		Font& operator=(Font&&) = delete;

		// Initialization and cleanup - added maxChars parameter
		/// @brief Loads a font from a file and creates all required GPU resources.
		/// @param commandQueue The command queue used for texture upload operations.
		/// @param drawableFormat The pixel format of the render target.
		/// @param filePath Path to the font resource file on disk.
		/// @param pixelsHeight Desired glyph height in pixels.
		/// @param maxChars Maximum number of characters that can be rendered per frame.
		/// @return True if the font was loaded successfully, false otherwise.
		bool init(CommandQueue commandQueue, PixelFormat drawableFormat, td::String filePath, int pixelsHeight, size_t maxChars);
		//bool init(CommandQueue commandQueue, PixelFormat drawableFormat, const char* file, double pt, size_t maxChars);

		/// @brief Clears all pending text, resetting the vertex and index counts to zero.
		void reset();

		/// @brief Returns whether the font has been successfully initialised.
		/// @return True if the font is ready to use.
		bool isOk() const;

		/// @brief Returns the loaded font size in points.
		/// @return Font size in points.
		float getFontSize() const { return _fontSize; }

		/// @brief Returns the width of a space character in logical pixels.
		/// @return Space character advance width in logical pixels.
		float getSpaceWidth() const { return _glyphSize.spaceWidth; }

		/// @brief Provides read access to the glyph atlas texture.
		/// @return Const reference to the GPU texture containing the glyph atlas.
		const Texture& getTexture() const { return _texture; }

		/// @brief Provides read access to the consolidated CPU buffer.
		/// @return Const reference to the GPU buffer holding vertex and index data.
		const Buffer& getCPUBuffer() const { return _bufferCpu.buffer; }

		/// @brief Returns the number of vertices currently queued for rendering.
		/// @return Vertex count for the current frame.
		size_t getVertexCount() const { return _bufferCpu.currentVertexCount; }

		/// @brief Queues text for rendering at the specified position with the given alignment.
		/// @param text UTF-8 encoded string to render.
		/// @param x Horizontal anchor position in logical pixels.
		/// @param y Vertical baseline position in logical pixels.
		/// @param color RGBA color for the rendered text.
		/// @param alignment Horizontal alignment relative to the anchor x position.
		/// @return The bounding size of the rendered text.
		glx::Size addText(const td::String& text, double x, double y, td::Color color, HorizontalAlignment alignment = HorizontalAlignment::Left);

		/// @brief Draws all queued text into the specified view.
		/// @param pView Pointer to the target view; must not be null.
		/// @param clearScreen When true, the view is cleared before drawing text.
		void draw(View* pView, bool clearScreen);

	};


}
