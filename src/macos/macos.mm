#include "macos.h"
#include <CoreGraphics/CoreGraphics.h> // Needed for CGWindowListCopyWindowInfo

namespace WinPeek {
    WinPeek::WinPeek() {
        this->registerObservers();
    }

    WinPeek::~WinPeek() {
        NSWorkspace* workspace = [NSWorkspace sharedWorkspace];
        NSNotificationCenter* notificationCenter = [workspace notificationCenter];

        for (std::vector<id>::iterator it = this->observers.begin(); it != this->observers.end(); it++) {
            [notificationCenter removeObserver:*it];
        }
    }

    void WinPeek::registerObservers() {
        NSWorkspace* workspace = [NSWorkspace sharedWorkspace];
        NSNotificationCenter* notificationCenter = [workspace notificationCenter];

        this->observers.push_back([notificationCenter addObserverForName:NSWorkspaceDidActivateApplicationNotification object:nil queue:nil usingBlock:^(NSNotification* notification) {
            this->handleNotification();
        }]);
    }

    const char* WinPeek::getActiveWindow() {
        NSWorkspace* workspace = [NSWorkspace sharedWorkspace];
        NSRunningApplication* app = [workspace frontmostApplication];

        if (app == nil) {
            return NULL; // No active application
        }

        // Check if any windows of the frontmost application are minimized
        CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
        bool allWindowsMinimized = true;

        for (CFIndex i = 0; i < CFArrayGetCount(windowList); i++) {
            CFDictionaryRef windowInfo = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
            int64_t windowOwnerPID = 0;
            CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(windowInfo, kCGWindowOwnerPID), kCFNumberSInt64Type, &windowOwnerPID);

            if (windowOwnerPID == app.processIdentifier) {
                CFBooleanRef isMinimized = (CFBooleanRef)CFDictionaryGetValue(windowInfo, kCGWindowIsOnscreen);
                if (isMinimized == kCFBooleanTrue) {
                    allWindowsMinimized = false;
                    break;
                }
            }
        }

        CFRelease(windowList);

        if (allWindowsMinimized) {
            return NULL; // Return NULL if all windows of the frontmost app are minimized
        }

        return [app.localizedName UTF8String]; // Return the name of the active window
    }

    void WinPeek::handleNotification() {
        const char* info = this->getActiveWindow();

        for (std::map<watch_t, watch_callback>::iterator it = this->watches.begin(); it != this->watches.end(); it++) {
            try {
                it->second(info);  
            }
            catch (...) { }
        }
    }

    void WinPeek::runLoop() {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.0001, true);
    }
}
