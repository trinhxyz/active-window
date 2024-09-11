
#include <napi.h>
#include <Cocoa/Cocoa.h>
#include <vector>
#include <map>

namespace WinPeek
{

    // Typedefs for the watch callback system
    typedef int watch_t;
    typedef std::function<void(const char *)> watch_callback;

    class WinPeek
    {
    public:
        // Constructor and Destructor
        WinPeek();
        ~WinPeek();

        // Method to register observers for active window notifications
        void registerObservers();

        // Method to get the currently active window
        const char *getActiveWindow();

        // Handle the notification when a new window becomes active
        void handleNotification();

        void runLoop();

    private:
        // Vector to store observer IDs for easy removal
        std::vector<id> observers;

        // Map to store the watch callbacks
        std::map<watch_t, watch_callback> watches;
    };

} // namespace WinPeek

#endif // WINPEEK_H
