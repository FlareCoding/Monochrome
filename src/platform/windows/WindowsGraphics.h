#pragma once
#include <graphics/Graphics.h>
#include <Windows.h>

namespace mc
{
	class WindowsGraphics
	{
	public:
		static bool Initialize(HWND hwnd);
		static bool IsInitialized();

		static void Shutdown();

		static void SetActiveTarget(HWND hwnd);

	public:
		static void BeginFrame();
		static void EndFrame();
		static void ClearScreenColor(uint32_t r, uint32_t g, uint32_t b);
		static void Update(const Color& background, SceneManager& sm, bool clearBackgroundColor = true);

	public:
		static void PushLayer(float x, float y, float width, float height);
		static void PopLayer();

	public:
		static void ResizeRenderTarget(HWND hwnd);

	public:
		/// Draws a straight line.
		/// @param x1 x-coordinate of the start point
		/// @param y1 y-coordinate of the start point
		/// @param x2 x-coordinate of the end point
		/// @param y2 y-coordinate of the end point
		///
		static void DrawLine(
			float x1,
			float y1,
			float x2,
			float y2,
			Color color = Color::black,
			float stroke = 2
		);

		/// Draws a rectangle.
		/// @param x x-coordinate of the location of the top left corner relative to the parent UIView
		/// @param y y-coordinate of the location of the top left corner relative to the parent UIView
		/// @param width Width of the rectangle
		/// @param height Height of the rectangle
		/// @param color Color to fill the rectangle with
		/// @param corner_radius Specifies the radius of each corner of the rectangle
		/// @param filled Specifies if the rectangle should be filled
		/// @param stroke Specifies the border thickness
		///
		static void DrawRectangle(
			float x,
			float y,
			float width,
			float height,
			Color color = Color::gray,
			float corner_radius = 0,
			bool filled = true,
			float stroke = 2
		);

		/// Draws a circle.
		/// @param x x-coordinate of the location of the circle's center relative to the parent UIView
		/// @param y y-coordinate of the location of the circle's center relative to the parent UIView
		/// @param radius Radius of the circle
		/// @param color Color to fill the circle with
		/// @param filled Specifies if the circle should be filled
		/// @param stroke Specifies the border thickness
		///
		static void DrawCircle(
			float x,
			float y,
			float radius,
			Color color = Color::gray,
			bool filled = true,
			float stroke = 2
		);

		/// Draws an arc.
		/// @param start_x x-coordinate of the location of the first endpoint
		/// @param start_y y-coordinate of the location of the first endpoint
		/// @param end_x x-coordinate of the location of the second endpoint
		/// @param end_y y-coordinate of the location of the second endpoint
		/// @param angle angle between the start and end points
		/// @param size The x-radius and y-radius of the arc.
		/// @param color Color to fill the arc with
		/// @param clockwise Specifies whether the arc should bend in the clockwise direction
		/// @param large_arc Specifies whether the arc should be calculated using the larger or the smaller angle
		/// @param stroke Border thickness
		///
		static void DrawArc(
			float start_x,
			float start_y,
			float end_x,
			float end_y,
			float angle,
			float size = 4,
			Color color = Color::gray,
			bool clockwise = true,
			bool large_arc = false,
			float stroke = 2
		);

		/// Draws a text string.
		/// @param x x-coordinate of the location of the top left corner relative to the parent UIView
		/// @param y y-coordinate of the location of the top left corner relative to the parent UIView
		/// @param width Width of the text rectangle
		/// @param height Height of the text rectangle
		/// @param text Text to draw
		/// @param text_props Properties describing the text font and style
		/// @param color Color to fill the rectangle with
		///
		static void DrawTextWideString(
			float x,
			float y,
			float width,
			float height,
			const std::wstring& text,
			TextProperties text_props = TextProperties(),
			Color color = Color::black
		);

		/// Draws a text string.
		/// @param x x-coordinate of the location of the top left corner relative to the parent UIView
		/// @param y y-coordinate of the location of the top left corner relative to the parent UIView
		/// @param width Width of the text rectangle
		/// @param height Height of the text rectangle
		/// @param text Text to draw
		/// @param text_props Properties describing the text font and style
		/// @param color Color to fill the rectangle with
		///
		static void DrawTextString(
			float x,
			float y,
			float width,
			float height,
			const std::string& text,
			TextProperties text_props = TextProperties(),
			Color color = Color::black
		);

		/// Calculates metrics of the text based on the text properties and layout.
		/// @param text Text to calculate the metrics of
		/// @param text_props Properties describing the text font and style
		/// @param max_width Maximum width of the text layout container
		/// @param max_height Maximum height of the text layout container
		/// @returns Structure containing calculated metrics of the inputted text
		///
		static TextMetrics CalculateTextMetrics(
			const std::string& text,
			TextProperties text_props,
			float max_width,
			float max_height
		);

		/// Calculates how many characters would fit on a single line.
		/// @param text_props Properties describing the text font and style
		/// @param container_width Width of the container holding the text
		/// @param container_height Height of the container holding the text
		/// @returns Number of characters that would fit on the line
		static uint32_t GetLineCharacterLimit(
			TextProperties text_props,
			float container_width,
			float container_height
		);

		/// Returns a Bitmap object holding the image data from reading a file.
		static Ref<Bitmap> CreateBitmapFromFile(const std::string& path);

		/// Returns a Bitmap object holding the image data provided.
		/// @param data Raw data of the image.
		/// @param size Size of the image data in bytes.
		static Ref<Bitmap> CreateBitmap(const char* data, uint32_t size);

		/// Draws a bitmap image.
		/// @param bmp Reference to a shared_ptr<Bitmap>
		/// @param x x-coordinate of the location of the top left corner relative to the parent UIView
		/// @param y y-coordinate of the location of the top left corner relative to the parent UIView
		/// @param width Width of the image
		/// @param height Height of the image
		/// @param opacity Specifies the value of the alpha channel of the draw image
		///
		static void DrawBitmapImage(
			Ref<Bitmap>& bmp,
			float x,
			float y,
			float width,
			float height,
			float opacity = 1.0f
		);

	private:
		static Ref<RenderTarget> m_RenderTarget;
		static std::map<HWND, Ref<RenderTarget>> m_RenderTargetMap;
	};
}
