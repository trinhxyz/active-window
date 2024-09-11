#include "macos.h"

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
            return NULL;
        }

        return [app.localizedName UTF8String];
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
