const addon = require('winpeek');

setInterval(() => {    
        const activeWindow = addon.getActiveWindow();
        console.log(activeWindow)
    }, 1000
)
