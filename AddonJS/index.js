const testAddon = require('./build/Release/testaddon.node');

var obj = testAddon.createObject('../../peps/pricer_skel/data/callVanilleProf.csv')


function sleep(ms) {
    return new Promise((resolve) => {
      setTimeout(resolve, ms);
    });
  }   

async function init() {
    console.log('pnl', testAddon.pnl(4, 7));
    console.log('hello', testAddon.hello());
    console.log('null', testAddon.null());
    console.log('pnl_inside', testAddon.pnl_inside());
  }
  

async function Spots() {
  console.log(obj.Spots())
}

async function Dates() {
  console.log(obj.Dates())

}



//obj.CalculPnl()

//obj.Simple('../../peps/pricer_skel/data/DATA')

// obj.SimpleRates('../../peps/pricer_skel/data/RATE', '../../peps/pricer_skel/data/DATA', 97, 10, "2021-12-10", "2021-12-15", 5000, false)

// console.log(obj.Dates())
// console.log(obj.Spots())
// console.log(obj.Prices())
// console.log(obj.DatesString())
// console.log("uwu")


obj.ComputeDeltas('../../peps/pricer_skel/data/RATE', '../../peps/pricer_skel/data/DATA', 10, "2021-12-10", "2021-12-15", 5000)
console.log(obj.Deltas())
console.log(obj.StocksString())


// obj.CalculPnl(function () {
//   console.log(Array.from(arguments));
// })

module.exports = testAddon;