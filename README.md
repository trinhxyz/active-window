# **winpeek**

A Node.js native module that allows you to get the name of the window you currently have in focus.


## **Installation**

You can install winpeek via npm by running:
  ```bash
  npm install winpeek
  ```

## **Usage**
  ```javascript
  import { getActiveWindow } from 'winpeek'
  
  const activeWindowName = getActiveWindow();
  if (activeWindowName) {
    console.log(`Active window: ${activeWindowName}`);
  } else {
    console.log('No active window found');
  }
  ```

## **Methods**
`getActiveWindow()`
 - Returns the name of the active window as a string. If no window is active, or if the window is hidden or cloaked, it returns null.

## **Compatibility**
- Windows: This addon is designed to work on both 32-bit and 64-bit windows platforms.
