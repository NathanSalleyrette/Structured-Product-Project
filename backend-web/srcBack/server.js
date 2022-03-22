// Patches
const {inject, errorHandler} = require('express-custom-error');
inject(); // Patch express in order to use async / await syntax
const path = require('path');
// Require Dependencies
const express = require('express');
const session = require('express-session');
const cookieParser = require('cookie-parser');
const cors = require('cors');
const helmet = require('helmet');
//const fetch = require('node-fetch')
const logger = require('./util/logger');
const fs = require('fs')

const envfile = require('envfile')
// Load .env Enviroment Variables to process.env
require('mandatoryenv').load([
    'PORT',
    'SECRET'
]);
const { PORT } = process.env;
// Instantiate an Express Application
const app = express();

app.use(session({ secret: 'X7ABU6T3LMMQ', resave: false, cookie: {  sameSite: 'lax' , maxAge: 60000 } }))

// Configure Express App Instance
app.use(express.json( { limit: '50mb' } ));
app.use(express.urlencoded( { extended: true, limit: '10mb' } ));

// Configure custom logger middleware
app.use(logger.dev, logger.combined);

app.use(cookieParser());
app.use(cors());
app.use(helmet());

// This middleware adds the json header to every response
app.use('/api/*', (req, res, next) => {
    res.setHeader('Content-Type', 'application/json');
    next();
})



// Assign Routes
// app.get("/yahoo",async (req,res) => {
//     console.log("yahoo")
//     const ret=await fetch("https://query1.finance.yahoo.com/v7/finance/download/CSX?period1=1611572921&period2=1643108921&interval=1d&events=history&includeAdjustedClose=true")
//     const retourCSV = await ret.text()
//     res.send(retourCSV)
// })

app.use('/api', require('./routes/router.js'));

// app.use(express.static(__dirname+'/../buildFront'));


// app.get('/', function(req, res) {
//     res.sendFile('index.html',__dirname+'/../buildFront');
// });

// app.get('/*', function(req, res) {
//     res.sendFile('index.html',{root:__dirname+'/../buildFront'});
// });




// Handle errors
app.use(errorHandler());

// Handle not valid route
app.use('/api/*', (req, res) => {
    res
    .status(404)
    .json( {status: false, message: 'Endpoint Not Found'} );
})

// Open Server on selected Port
app.listen(
    PORT,
    () => console.info('Server listening on port ', PORT)
);


const userModel = require('./models/users.js');
const assetsModel = require('./models/assets.js');

userModel.sync();
assetsModel.sync();

userModel.create({ username:"admin", password:"admin", admin:true });


assetsModel.create({ assetName:"Toyota Motor Corporation", yahooCode:"7203.T"});
assetsModel.create({ assetName:"East Japan Railway Company", yahooCode:"9020.T"});
assetsModel.create({ assetName:"Agilent Technologies, Inc.", yahooCode:"A"});
assetsModel.create({ assetName:"AGCO Corporation", yahooCode:"AGCO"});
assetsModel.create({ assetName:"BRF S.A.", yahooCode:"BRFSN.MX"});
assetsModel.create({ assetName:"BorgWarner Inc.", yahooCode:"BWA"});
assetsModel.create({ assetName:"CF Industries Holdings, Inc.", yahooCode:"CF"});
assetsModel.create({ assetName:"Cummins Inc.", yahooCode:"CMI"});
assetsModel.create({ assetName:"Canadian National Railway Company", yahooCode:"CNR.TO"});
assetsModel.create({ assetName:"CSX Corporation", yahooCode:"CSX"});
assetsModel.create({ assetName:"Deere & Company ", yahooCode:"DE"});
assetsModel.create({ assetName:"Danaher Corporation", yahooCode:"DHR"});
assetsModel.create({ assetName:"Koninklijke DSM N.V.", yahooCode:"DSM.AS"});
assetsModel.create({ assetName:"Ecolab Inc.", yahooCode:"ECL"});
assetsModel.create({ assetName:"Emerson Electric Co.", yahooCode:"EMR"});
assetsModel.create({ assetName:"Eaton Corporation plc", yahooCode:"ETN"});
assetsModel.create({ assetName:"Flowserve Corporation", yahooCode:"FLS"});
assetsModel.create({ assetName:"Kellogg Company", yahooCode:"K"});
assetsModel.create({ assetName:"Linde plc", yahooCode:"LIN.DE"});
assetsModel.create({ assetName:"Mondelez International, Inc.", yahooCode:"MDLZ"});
assetsModel.create({ assetName:"The Mosaic Company", yahooCode:"MOS"});
assetsModel.create({ assetName:"Pentair plc", yahooCode:"PNR"});
assetsModel.create({ assetName:"Roper Technologies, Inc.", yahooCode:"ROP"});
assetsModel.create({ assetName:"Siemens Aktiengesellschaft", yahooCode:"SIE.DE"});
assetsModel.create({ assetName:"Schneider Electric S.E.", yahooCode:"SU.PA"});
assetsModel.create({ assetName:"Thermo Fisher Scientific Inc.", yahooCode:"TMO"});
assetsModel.create({ assetName:"Trinseo PLC", yahooCode:"TSE"});
assetsModel.create({ assetName:"United Utilities Group PLC", yahooCode:"UU.L"});
assetsModel.create({ assetName:"Veolia Environnement S.A.", yahooCode:"VIE.PA"});
assetsModel.create({ assetName:"Xylem Inc.", yahooCode:"XYL"});