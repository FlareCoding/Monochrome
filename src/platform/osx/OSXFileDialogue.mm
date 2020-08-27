#include "OSXFileDialogue.h"
#include "OSXNativeCore.h"

namespace mc
{
    Ref<UIFileDialogue> UIFileDialogue::Create()
	{
		return Ref<UIFileDialogue>(new OSXFileDialogue());
	}

	std::string OSXFileDialogue::ChooseDirectoryDialogue()
	{
		return "";
	}

	std::string OSXFileDialogue::ChooseFileDialogue()
	{
		std::string result = "";

		// dispatch_semaphore_t sema = dispatch_semaphore_create(0);
		// dispatch_async(dispatch_get_main_queue(), ^(void) {
		// 	NSOpenPanel* openDlg = [NSOpenPanel openPanel];
		// 	[openDlg setFloatingPanel:YES];
		// 	[openDlg setCanChooseDirectories:NO];
		// 	[openDlg setCanChooseFiles:YES];
		// 	[openDlg setAllowsMultipleSelection:NO];

		// 	if ([openDlg runModal] == NSModalResponseOK)
		// 	{
		// 		NSArray* filePaths = [openDlg URLs];
		// 		NSURL* fileUrl = [filePaths objectAtIndex:0];

		// 		const char* path = [[fileUrl path] UTF8String];
		// 		printf("ReePath: %s\n", path);
		// 	}

		// 	dispatch_semaphore_signal(sema);
		// });

		// dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);

		// if (![NSThread isMainThread]) {
		// 	dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
		// } else {
		// 	while (dispatch_semaphore_wait(sema, DISPATCH_TIME_NOW)) { 
		// 		[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:0]]; 
		// 	}
		// }

		// dispatch_async(dispatch_get_main_queue(), ^(void) {
		// 	NSOpenPanel* openDlg = [NSOpenPanel openPanel];
		// 	[openDlg setFloatingPanel:YES];
		// 	[openDlg setCanChooseDirectories:NO];
		// 	[openDlg setCanChooseFiles:YES];
		// 	[openDlg setAllowsMultipleSelection:NO];
		// 	//[openDlg setAllowedFileTypes:@[@"ped"]];

		// 	if ([openDlg runModal] == NSModalResponseOK)
		// 	{
		// 		NSArray* filePaths = [openDlg URLs];
		// 		NSURL* fileUrl = [filePaths objectAtIndex:0];

		// 		const char* path = [[fileUrl path] UTF8String];
		// 	}
		// });

		return result;
	}

	std::string OSXFileDialogue::SaveFileDialogue()
	{
		return "";
	}
}
