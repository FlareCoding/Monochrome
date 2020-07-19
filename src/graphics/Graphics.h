#pragma once
#include <core/Core.h>
#include <Windows.h>
#include "Color.h"
#include "Shapes.h"
#include "TextProperties.h"

namespace mc
{
	class RenderTarget;
	class SceneManager;
	
	class Graphics
	{
	public:
		static bool Initialize(HWND hwnd);
		static bool IsInitialized();

	public:
		static void BeginFrame();
		static void EndFrame();
		static void ClearScreenColor(uint32_t r, uint32_t g, uint32_t b);
		static void Update(const Color& background, SceneManager& sm);

	public:
		static void PushLayer(float x, float y, float width, float height);
		static void PopLayer();

	public:
		static void ResizeRenderTarget(HWND hwnd);

	public:
		static void DrawLine(
			float x1,
			float y1,
			float x2,
			float y2,
			Color color = Color::black,
			float stroke = 2
		);

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

		static void DrawCircle(
			float x,
			float y,
			float radius,
			Color color = Color::gray,
			bool filled = true,
			float stroke = 2
		);

		static void DrawArc(
			float start_x,
			float start_y,
			float end_x,
			float end_y,
			float size = 4,
			Color color = Color::gray,
			bool clockwise = true,
			bool large_arc = false,
			float stroke = 2
		);

		static void DrawTextWideString(
			float x,
			float y,
			float width,
			float height,
			const std::wstring& text,
			TextProperties text_props = TextProperties(),
			Color color = Color::black
		);

		static void DrawTextString(
			float x,
			float y,
			float width,
			float height,
			const std::string& text,
			TextProperties text_props = TextProperties(),
			Color color = Color::black
		);

	private:
		static Ref<RenderTarget> m_RenderTarget;
	};
}
