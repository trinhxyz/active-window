'use strict';

const getActiveWindow = async () => {
  if (process.platform === 'win32') {
    return require('./src/win32/index.js')();
  }
  if (process.platform === 'darwin') {
    return require('./src/macos/index.js')();   
  }

  throw new Error('Only Windows and macOS are supported.');
};

const initialize = () => {
  if (process.platform === 'darwin') {
    return require('./src/macos/index.js').runLoop();
  }
};

module.exports = {
  getActiveWindow,
  initialize
};

module.exports.default = module.exports;
