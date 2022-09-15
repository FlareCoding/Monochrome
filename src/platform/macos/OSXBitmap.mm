#include <rendering/Bitmap.h>
#include <Cocoa/Cocoa.h>

@interface ImageUtil : NSObject
+(NSImage*)flipImage:(NSImage*)image;
@end

@implementation ImageUtil
+(NSImage*)flipImage:(NSImage*)image
{
    NSImage *existingImage = image;
    NSSize existingSize = [existingImage size];
    NSSize newSize = NSMakeSize(existingSize.width, existingSize.height);
    NSImage *flipedImage = [[[NSImage alloc] initWithSize:newSize] autorelease];

    [flipedImage lockFocus];
    [[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];

    NSAffineTransform *t = [NSAffineTransform transform];
    [t translateXBy:existingSize.width yBy:existingSize.height];
    [t scaleXBy:-1 yBy:-1];
    [t concat];

    [existingImage drawAtPoint:NSZeroPoint fromRect:NSMakeRect(0, 0, newSize.width, newSize.height) operation:NSCompositingOperationSourceOver fraction:1.0];

    [flipedImage unlockFocus];

    return flipedImage;
}
@end

namespace mc {
    Shared<Bitmap> Bitmap::loadFromFile(const std::string& path) {
        NSString* nsPath = [NSString stringWithCString:path.c_str() 
                                   encoding:[NSString defaultCStringEncoding]];

        NSImage* nsImage = [[NSImage alloc]initWithContentsOfFile:nsPath];
        if (!nsImage) {
            return MakeRef<Bitmap>(0, 0, nullptr);
        }

        NSSize imageSize = [nsImage size];
		return MakeRef<Bitmap>(imageSize.width, imageSize.height, static_cast<void*>(nsImage));
    }

    Shared<Bitmap> Bitmap::loadFromWebUrl(const std::string& url) {
        NSURL* imageURL = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
		NSImage* nsImage = [[NSImage alloc] initWithContentsOfURL:imageURL];
        if (!nsImage) {
            return MakeRef<Bitmap>(0, 0, nullptr);
        }

        NSSize imageSize = [nsImage size];
		return MakeRef<Bitmap>(imageSize.width, imageSize.height, static_cast<void*>(nsImage));
    }
} // namespace mc
