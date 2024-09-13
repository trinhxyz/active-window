const addon = require('./binding.js');

module.exports = async () => addon.getActiveWindow();
