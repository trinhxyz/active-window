const addon = require('./build/Release/winpeek');

setInterval(() => {    
        const activeWindow = addon.getActiveWindow();
        console.log(activeWindow)
    }, 1000
)
