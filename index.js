let activeWindowModule;

if (process.platform === 'darwin') {
  activeWindowModule = require('./build/Release/macos.node');
} else if (process.platform === 'win32') {
  activeWindowModule = require('./build/Release/win32.node');
} else {
  throw new Error('Unsupported platform: ' + process.platform);
}

module.exports = {
  getActiveWindow: activeWindowModule.getActiveWindow
};
