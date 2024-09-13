'use strict';

module.exports = options => {
	if (process.platform === 'win32') {
		return require('./src/win32/index.js')(options);
	}

	return Promise.reject(new Error('macOS, Linux, and Windows only'));
};