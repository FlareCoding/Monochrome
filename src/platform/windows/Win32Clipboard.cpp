#include <utils/Clipboard.h>
#include <Windows.h>

namespace mc::utils {
    void Clipboard::saveToClipboard(const std::string& data) {
        if (!OpenClipboard(0)) {
            return;
        }

        // Empty existing clipboard buffer
        EmptyClipboard();

        // Allocate memory for the clipboard content
        auto newClipboardContent = GlobalAlloc(GMEM_DDESHARE, data.size() + 1);
        if (!newClipboardContent) {
            CloseClipboard();
            return;
        }

        // Lock and get the address of the clipboard buffer
        char* clipboardBuffer = static_cast<char*>(GlobalLock(newClipboardContent));
        if (clipboardBuffer) {
            memcpy_s(clipboardBuffer, data.size(), &data[0], data.size());
        }

        // Unlock the buffer and set the new clipboard content
        GlobalUnlock(newClipboardContent);
        SetClipboardData(CF_TEXT, newClipboardContent);

        // Close the clipboard handle
        CloseClipboard();
    }

    std::string Clipboard::getClipboardText() {
        if (!OpenClipboard(0)) {
            return "";
        }

        // Get the handle to the clipboard contents
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (!hData) {
            CloseClipboard();
            return "";
        }

        // Lock and get the address of the clipboard buffer
        char* clipboardBuffer = static_cast<char*>(GlobalLock(hData));
        if (!clipboardBuffer) {
            CloseClipboard();
            return "";
        }

        std::string result(clipboardBuffer);

        // Unlock the buffer and set the new clipboard content
        GlobalUnlock(hData);

        // Close the clipboard handle
        CloseClipboard();

        return result;
    }
} // namespace mc::utils
