#pragma once
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <exception>
#include <wincodec.h>

#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace mc
{
	class CoreResources
	{
	public:
		static void Initialize();
		static bool IsInitialized();

		static ComPtr<ID2D1HwndRenderTarget> CreateRenderTarget(HWND hwnd);

		static ComPtr<ID2D1Factory>& GetFactory() { return s_Factory; }
		static ComPtr<IDWriteFactory>& GetWriteFactory() { return s_WriteFactory; }

	private:
		static ComPtr<ID2D1Factory>	   s_Factory;
		static ComPtr<IDWriteFactory>  s_WriteFactory;
	};
}
