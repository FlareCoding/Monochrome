#pragma once
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "wininet.lib")

#include <core/Core.h>
#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <exception>
#include <Shlwapi.h>

#include <wincodec.h> // NOLINT
#include <wrl/client.h> // NOLINT
using namespace Microsoft::WRL; // NOLINT

namespace mc {
class D2DGraphics {
public:
    static void initialize();
    static bool isInitialized();

    static ComPtr<ID2D1HwndRenderTarget> createWindowRenderTarget(HWND hwnd);
    static ComPtr<ID2D1BitmapRenderTarget> createBitmapRenderTarget(
        ID2D1HwndRenderTarget* hwndRenderTarget
    );

    static ComPtr<ID2D1Bitmap> createBitmap(
        ComPtr<ID2D1HwndRenderTarget>& renderTarget,
        uint32_t width,
        uint32_t height
    );

    static ComPtr<ID2D1Factory>& getD2DFactory() { return s_D2DFactory; }
    static ComPtr<IDWriteFactory>& getDWriteFactory() { return s_writeFactory; }
    static ComPtr<IWICImagingFactory>& getWICImagingFactory() { return s_wicImagingFactory; }

private:
    static ComPtr<ID2D1Factory>         s_D2DFactory;
    static ComPtr<IDWriteFactory>       s_writeFactory;
    static ComPtr<IWICImagingFactory>   s_wicImagingFactory;
};
} // namespace mc
