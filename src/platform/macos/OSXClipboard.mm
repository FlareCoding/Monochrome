#include <utils/Clipboard.h>
#include <Cocoa/Cocoa.h>

namespace mc::utils
{
    void Clipboard::saveToClipboard(const std::string& data) {
        NSString* nsData = [NSString stringWithUTF8String: data.c_str()];

        [[NSPasteboard generalPasteboard] clearContents];
        [[NSPasteboard generalPasteboard] setString:nsData forType:NSPasteboardTypeString];
    }

    std::string Clipboard::getClipboardText() {
        NSPasteboard* pb = [NSPasteboard generalPasteboard];
        NSString* pasteboardContent = [pb stringForType:NSPasteboardTypeString];

        return std::string([pasteboardContent UTF8String]);
    }
}
