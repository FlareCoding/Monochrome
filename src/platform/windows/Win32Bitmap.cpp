#include <rendering/Bitmap.h>
#include "D2DGraphics.h"
#include "Win32RenderTarget.h"
#include <utils/TextUtils.h>
#include <WinInet.h>
#include <vector>

namespace mc {
    Shared<Bitmap> _createD2D1Bitmap(IWICBitmapDecoder* decoder, ID2D1RenderTarget* target) {
        IWICBitmapFrameDecode* wicFrame;
        HRESULT hr = decoder->GetFrame(0, &wicFrame);
        if (FAILED(hr)) {
            decoder->Release();
            return nullptr;
        }

        IWICFormatConverter* wicConverter;
        hr = D2DGraphics::getWICImagingFactory()->CreateFormatConverter(&wicConverter);
        if (FAILED(hr)) {
            wicFrame->Release();
            decoder->Release();
            CORE_ASSERT(false, "Failed to create WIC format converter");
            return nullptr;
        }

        hr = wicConverter->Initialize(
            wicFrame,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.0,
            WICBitmapPaletteTypeCustom
        );

        if (FAILED(hr)) {
            wicConverter->Release();
            wicFrame->Release();
            decoder->Release();
            CORE_ASSERT(false, "Failed to initialize WIC converter");
            return nullptr;
        }

        ID2D1Bitmap* bmp;
        hr = target->CreateBitmapFromWicBitmap(wicConverter, NULL, &bmp);
        if (FAILED(hr)) {
            wicConverter->Release();
            wicFrame->Release();
            decoder->Release();
            CORE_ASSERT(false, "Failed to create bitmap from WIC Bitmap");
            return nullptr;
        }

        auto bmpSize = bmp->GetSize();
        auto bitmap = MakeRef<Bitmap>(
            static_cast<uint32_t>(bmpSize.width),
            static_cast<uint32_t>(bmpSize.height),
            static_cast<void*>(bmp)
        );

        wicConverter->Release();
        wicFrame->Release();
        decoder->Release();

        return bitmap;
    }

    Shared<Bitmap> _createD2D1BitmapFromData(void* data, uint32_t size) {
        IStream* stream = SHCreateMemStream((const BYTE*)data, size);
        if (!stream) {
            CORE_ASSERT(false, "Failed to create memory stream");
            return nullptr;
        }

        IWICBitmapDecoder* wicDecoder;
        HRESULT hr = D2DGraphics::getWICImagingFactory()->CreateDecoderFromStream(
            stream,
            NULL,
            WICDecodeMetadataCacheOnLoad,
            &wicDecoder
        );

        if (FAILED(hr)) {
            CORE_ASSERT(false, "Failed to create WIC decoder from memory stream");
            return nullptr;
        }

        // wicDecoder->Release will be called inside _createD2D1Bitmap
        return _createD2D1Bitmap(wicDecoder, Win32RenderTarget::getSharedRenderTarget());
    }

    Bitmap::~Bitmap() {}

    Shared<Bitmap> Bitmap::loadFromFile(const std::string& path) {
        IWICBitmapDecoder* wicDecoder;
        HRESULT hr = D2DGraphics::getWICImagingFactory()->CreateDecoderFromFilename(
            utils::convertToWideString(path).c_str(),
            NULL,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &wicDecoder
        );

        if (FAILED(hr)) {
            CORE_ASSERT(false, "Failed to create WIC decoder from filename");
            return nullptr;
        }

        // wicDecoder->Release will be called inside _createD2D1Bitmap
        return _createD2D1Bitmap(wicDecoder, Win32RenderTarget::getSharedRenderTarget());
    }

    Shared<Bitmap> Bitmap::loadFromWebUrl(const std::string& url) {
        std::vector<char> data;

        HINTERNET hInternetSession;
        HINTERNET hURL;

        // Make internet connection.
        hInternetSession = InternetOpen(L"tes", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        hURL = InternetOpenUrl(
            hInternetSession,
            utils::convertToWideString(url).c_str(),
            NULL,
            0, 0, 0
        );

        DWORD dwBytesRead = 1;
        for (; dwBytesRead > 0;) {
            char buffer[1024];
            InternetReadFile(hURL, buffer, 1024, &dwBytesRead);
            data.insert(data.end(), buffer, buffer + dwBytesRead);
        }

        // Close down connections.
        InternetCloseHandle(hURL);
        InternetCloseHandle(hInternetSession);

        return _createD2D1BitmapFromData(&data[0], (uint32_t)data.size());
    }
} // namespace mc
