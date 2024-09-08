const addon = require('bindings')('winpeek');

module.exports = {
  getActiveWindow: addon.getActiveWindow
};
