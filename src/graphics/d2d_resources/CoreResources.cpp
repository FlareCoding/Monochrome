#include "CoreResources.h"

namespace mc
{
	static bool s_Initialized = false;

	ComPtr<ID2D1Factory>	CoreResources::s_Factory;
	ComPtr<IDWriteFactory>  CoreResources::s_WriteFactory;

	void CoreResources::Initialize()
	{
		if (s_Initialized) return; // resources are already initialized

		HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, s_Factory.GetAddressOf());
		if (result != S_OK)
		{
			MessageBoxA(0, "Failed to create Direct2D Factory Resource", "CoreResources", 0);
			return;
		}

		result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)s_WriteFactory.GetAddressOf());
		if (result != S_OK)
		{
			MessageBoxA(0, "Failed to create DirectWrite Factory Resource", "CoreResources", 0);
			return;
		}

		s_Initialized = true;
	}

	bool CoreResources::IsInitialized()
	{
		return s_Initialized;
	}

	ComPtr<ID2D1HwndRenderTarget> CoreResources::CreateRenderTarget(HWND hwnd)
	{
		if (!s_Initialized)
		{
			throw "Direct2D resources have not yet been initialized\n";
			return ComPtr<ID2D1HwndRenderTarget>();
		}

		ComPtr<ID2D1HwndRenderTarget> render_target;

		RECT rect;
		GetClientRect(hwnd, &rect);

		HRESULT result = s_Factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rect.right, rect.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
			render_target.GetAddressOf()
		);

		if (result != S_OK)
		{
			MessageBoxA(0, "Failed to create render target", "CoreResources", 0);
			return nullptr;
		}

		render_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_FORCE_DWORD);
		render_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

		return render_target;
	}
}
