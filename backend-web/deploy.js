const { execSync } = require('child_process');
// const stdout = execSync('unzip pnl-1.10.4')

// console.log(stdout.toString())

// console.log(execSync('mkdir pnl-1.10.4/build').toString())

// console.log(execSync('cmake ..' , {cwd : "pnl-1.10.4/build"}).toString())

// console.log(execSync('make' , {cwd : "pnl-1.10.4/build"}).toString())

// console.log(execSync('make install' , {cwd : "pnl-1.10.4/build"}).toString())



console.log(execSync('apt-get install g++').toString())

console.log(execSync('npm install -g cmake-js' , {cwd : "AddonJS"}).toString())

console.log(execSync('npm install' , {cwd : "AddonJS"}).toString())

console.log(execSync('node srcBack/server.js').toString())

