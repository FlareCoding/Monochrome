#include "UIImage.h"
#include <graphics/Graphics.h>

#ifdef _WIN32
#include <Windows.h>
#include <WinInet.h>
#elif defined(__APPLE__)
#include <platform/osx/OSXGraphics.h>
#endif

namespace mc
{
#if defined(_WIN32)
	static void LoadWebImageData(const std::string& url, std::vector<char>& data);
#endif

	UIImage::UIImage()
	{
		layer.frame = Frame(40, 40, 128, 128);
	}

	UIImage::UIImage(Frame frame) : UIView(frame) {}

	bool UIImage::LoadImageFromFile(const std::string& path)
	{
		m_Bitmap = Graphics::CreateBitmapFromFile(path);
		return m_Bitmap.get();
	}

	bool UIImage::LoadWebImage(const std::string& url)
	{
#if defined(_WIN32)
		std::vector<char> data;
		LoadWebImageData(url, data);

		m_Bitmap = Graphics::CreateBitmap(&data[0], (uint32_t)data.size());
		return m_Bitmap.get();
#elif defined(__APPLE__)
		m_Bitmap = OSXGraphics::CreateBitmapFromURL(url.c_str());
		return m_Bitmap.get();
#else
		return false;
#endif
	}

	void UIImage::Draw()
	{
		if (m_Bitmap)
		{
			Graphics::DrawBitmapImage(
				m_Bitmap,
				layer.frame.position.x,
				layer.frame.position.y,
				layer.frame.size.width,
				layer.frame.size.height,
				Opacity
			);
		}
	}

	float UIImage::GetWidth()
	{
		return m_Bitmap.get() ? m_Bitmap->GetWidth() : 0.0f;
	}
	
	float UIImage::GetHeight()
	{
		return m_Bitmap.get() ? m_Bitmap->GetHeight() : 0.0f;
	}

	void LoadWebImageData(const std::string& url, std::vector<char>& data)
	{
#if defined(_WIN32)
		HINTERNET hInternetSession;
		HINTERNET hURL;

		// Make internet connection.
		hInternetSession = InternetOpen(L"tes", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		hURL = InternetOpenUrl(hInternetSession, std::wstring(url.begin(), url.end()).c_str(), NULL, 0, 0, 0);

		DWORD dwBytesRead = 1;
		for (; dwBytesRead > 0;)
		{
			char buffer[1024];
			InternetReadFile(hURL, buffer, 1024, &dwBytesRead);
			data.insert(data.end(), buffer, buffer + dwBytesRead);
		}

		// Close down connections.
		InternetCloseHandle(hURL);
		InternetCloseHandle(hInternetSession);
#endif
	}
}
