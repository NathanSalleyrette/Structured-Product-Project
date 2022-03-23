
const { execSync, exec } = require('child_process');

console.log(exec('node srcBack/server.js' , {cwd : "backend-web"}).toString())
console.log(exec('npm start' , {cwd : "Frontend"}).toString())
