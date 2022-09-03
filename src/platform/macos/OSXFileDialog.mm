#include "OSXFileDialog.h"

namespace mc::utils {
    Shared<FileDialog> FileDialog::create() {
        return Shared<FileDialog>(new OSXFileDialog());
    }

    std::string OSXFileDialog::chooseFolderDialog() {
        // Create a File Open Dialog class.
        NSOpenPanel* openDlg = [NSOpenPanel openPanel];

        // Set array of file types
        if (d_filter.hasFilters()) {
            NSMutableArray<NSString*>* allowedFileTypes = [[NSMutableArray alloc] init];
            for (auto& [name, filetypes] : d_filter.getFilters()) {
                for (auto& filetype : filetypes) {
                    NSString* nsFiletype = [NSString stringWithCString:filetype.c_str() 
                                    encoding:[NSString defaultCStringEncoding]];
                    [allowedFileTypes addObject: nsFiletype];
                }
            }

            [openDlg setAllowedFileTypes:allowedFileTypes];
        }

        // Enable options in the dialog.
        [openDlg setCanChooseFiles:NO];
        [openDlg setCanChooseDirectories:YES];
        [openDlg setCanCreateDirectories:YES];

        std::string result = "";

        // Display the dialog box.  If OK is pressed, process the result
        if ([openDlg runModal] == NSModalResponseOK) {
            NSURL* url = [openDlg URLs][0];

            result = std::string([[url path] UTF8String]);
        }

        return result;
    }

    std::string OSXFileDialog::openFileDialog() {
        // Create a File Open Dialog class.
        NSOpenPanel* openDlg = [NSOpenPanel openPanel];

        // Set array of file types
        if (d_filter.hasFilters()) {
            NSMutableArray<NSString*>* allowedFileTypes = [[NSMutableArray alloc] init];
            for (auto& [name, filetypes] : d_filter.getFilters()) {
                for (auto& filetype : filetypes) {
                    NSString* nsFiletype = [NSString stringWithCString:filetype.c_str() 
                                    encoding:[NSString defaultCStringEncoding]];
                    [allowedFileTypes addObject: nsFiletype];
                }
            }

            [openDlg setAllowedFileTypes:allowedFileTypes];
        }

        // Enable options in the dialog.
        [openDlg setCanChooseFiles:YES];

        std::string result = "";

        // Display the dialog box.  If OK is pressed, process the result
        if ([openDlg runModal] == NSModalResponseOK) {
            NSURL* url = [openDlg URLs][0];

            result = std::string([[url path] UTF8String]);
        }

        return result;
    }

    std::string OSXFileDialog::saveFileDialog() {
        // Create a File Open Dialog class.
        NSSavePanel* saveDlg = [NSSavePanel savePanel];

        // Set array of file types
        if (d_filter.hasFilters()) {
            NSMutableArray<NSString*>* allowedFileTypes = [[NSMutableArray alloc] init];
            for (auto& [name, filetypes] : d_filter.getFilters()) {
                for (auto& filetype : filetypes) {
                    NSString* nsFiletype = [NSString stringWithCString:filetype.c_str() 
                                    encoding:[NSString defaultCStringEncoding]];
                    [allowedFileTypes addObject: nsFiletype];
                }
            }

            [saveDlg setAllowedFileTypes:allowedFileTypes];
        }

        std::string result = "";

        // Display the dialog box.  If OK is pressed, process the result
        if ([saveDlg runModal] == NSModalResponseOK) {
            NSURL* url = [saveDlg URL];

            result = std::string([[url path] UTF8String]);
        }

        return result;
    }
} // namespace mc::utils
