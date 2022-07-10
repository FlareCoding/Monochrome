#include "D2DGraphics.h"
#include <string>
#include <system_error>

namespace mc
{
	static bool s_initialized = false;

	ComPtr<ID2D1Factory>		D2DGraphics::s_D2DFactory;
	ComPtr<IDWriteFactory>		D2DGraphics::s_writeFactory;
	ComPtr<IWICImagingFactory>  D2DGraphics::s_wicImagingFactory;

	void D2DGraphics::initialize() {
		if (s_initialized) return; // resources are already initialized

		HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, s_D2DFactory.GetAddressOf());
		CORE_ASSERT(result == S_OK, "Failed to create Direct2D Factory Resource");

		result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)s_writeFactory.GetAddressOf());
		CORE_ASSERT(result == S_OK, "Failed to create DirectWrite Factory Resource");

		auto _ = CoInitialize(NULL);
		result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)s_wicImagingFactory.GetAddressOf());
		CORE_ASSERT(result == S_OK, "Failed to create WICImagingFactory");

		s_initialized = true;
	}

	bool D2DGraphics::isInitialized() {
		return s_initialized;
	}

	ComPtr<ID2D1HwndRenderTarget> D2DGraphics::createWindowRenderTarget(HWND hwnd) {
		if (!s_initialized) {
			throw "Direct2D resources have not yet been initialized\n";
		}

		ComPtr<ID2D1HwndRenderTarget> renderTarget;

		RECT rect;
		GetClientRect(hwnd, &rect);

		HRESULT result = s_D2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rect.right, rect.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
			renderTarget.GetAddressOf()
		);

		CORE_ASSERT(result == S_OK, "Failed to create Direct2D render target");

		renderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_FORCE_DWORD);
		renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

		return renderTarget;
	}
	
	ComPtr<ID2D1BitmapRenderTarget> D2DGraphics::createBitmapRenderTarget(ID2D1HwndRenderTarget* hwndRenderTarget) {
		if (!s_initialized) {
			throw "Direct2D resources have not yet been initialized\n";
		}

		ComPtr<ID2D1BitmapRenderTarget> renderTarget;
		HRESULT result = hwndRenderTarget->CreateCompatibleRenderTarget(renderTarget.GetAddressOf());

		CORE_ASSERT(result == S_OK, "Failed to create Direct2D bitmap render target");
		return renderTarget;
	}

	ComPtr<ID2D1Bitmap> D2DGraphics::createBitmap(
		ComPtr<ID2D1HwndRenderTarget>& renderTarget,
		uint32_t width,
		uint32_t height
	) {
		
		ComPtr<ID2D1Bitmap> bitmap;

		D2D1_PIXEL_FORMAT pixelFormat;
		pixelFormat.alphaMode = D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED;
		pixelFormat.format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;

		D2D1_BITMAP_PROPERTIES bitmapProperties;
		bitmapProperties.dpiX = 96.0f;
		bitmapProperties.dpiY = 96.0f;
		bitmapProperties.pixelFormat = pixelFormat;

		HRESULT result = renderTarget->CreateBitmap(
			D2D1::SizeU(width, height),
			bitmapProperties,
			bitmap.GetAddressOf()
		);

		CORE_ASSERT(result == S_OK, "Failed to create bitmap, error code: " + std::system_category().message(result));
		return bitmap;
	}
}