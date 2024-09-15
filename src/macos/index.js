const addon = require('./binding.js');

module.exports = async () => addon.getActiveWindow();

module.exports.runLoop = () => {
    return addon.runLoop();
}
