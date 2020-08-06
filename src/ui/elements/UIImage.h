#pragma once
#include <ui/UIView.h>
#include <graphics/Bitmap.h>

namespace mc
{
	class UIImage : public UIView
	{
	public:
		UIImage();
		~UIImage() = default;
		UIImage(Frame frame);

		/// Loads an image file from a device.
		bool LoadImageFromFile(const std::string& path);

		/// Loads an image from the web given an appropriate url.
		/// @param url String containing the web url to the image on the internet.
		bool LoadWebImage(const std::string& url);

		// Inherited via IDrawable
		virtual void Draw() override;

		/// Alpha channel of the image.
		float Opacity = 1.0f;

		/// Returns whether or not an image was successfully loaded.
		bool IsImageLoaded() const { return m_Bitmap.get(); }

		/// Returns width of the image.
		/// Will return 0 if image was not successfully loaded.
		float GetWidth();

		/// Returns height of the image.
		/// Will return 0 if image was not successfully loaded.
		float GetHeight();

	private:
		Ref<Bitmap> m_Bitmap = nullptr;
	};
}
