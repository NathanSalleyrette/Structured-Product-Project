const status = require('http-status');
const userModel = require('../models/users.js');
const assetsModel = require('../models/assets.js');
const has = require('has-keys');
const jws = require('jws');
const axios = require("axios");
const csv = require('fast-csv');
const fs = require('fs');
const testAddon = require('../../../AddonJS/build/Release/testaddon.node')
require('mandatoryenv').load(['SECRETJWS'])
var obj = testAddon.createObject('../../PEPS/peps/pricer_skel/data/callVanilleProf.csv')
const { readdirSync, readFileSync } = require('fs')

function getEndDateFromFiles() {
    const getFiles = source =>
        readdirSync(source, { withFileTypes: true })
            .map(file => file.name)
    files = getFiles("./DATA")
    dates = []
    files.forEach(element => {
        data = readFileSync('./DATA/'+element, 'utf-8')
        var lines = data.trim().split('\n');
        var lastLine = lines.slice(-1)[0];
        var fields = lastLine.split(',');
        dates.push(fields[0])
    })
    return dates.sort()[0]
}

async function getFromYahooToCSV(yahooCode, end_date) {
    //console.log("yahoo")
    const YC = yahooCode;
    // const freq = req.query.frequency; // 1 day
    // const begind = req.query.begin_date; // 01/01/2014
    const endd = end_date;
    const url = "https://query1.finance.yahoo.com/v7/finance/download/" + YC + "?period1=1388530800"  + "&period2="+ endd +"&interval=1d" + "&events=history&includeAdjustedClose=true"

    const ret= await axios(url)
    const retourCSV = ret.data;
    var dir = './DATA/'
    if (YC.includes("=X")) dir = './RATE/'

    if (!fs.existsSync(dir)){
        fs.mkdirSync(dir);
    }
    const filePath = dir + YC + ".csv"


    const writableCSV = fs.createWriteStream(filePath)
    writableCSV.write('Date,Open,High,Low,Close,Adj Close,Volume\n')
    const { Readable } = require("stream")
    const readable = Readable.from(retourCSV)
    readable.pipe(csv.parse({headers: true}))
    .on('error', error => {
        writableCSV.close()
        console.error(error)
    })
    .on('data', row => {
        if (row['Close'] != 'null' && !(row['Date'] == '2016-10-28' && YC == 'JPYEUR=X')) {
            writableCSV.write(row['Date'] + ',' +  row['Open']
            + ',' +  row['High']+ ',' +  row['Low']+ ',' +  row['Close']
            + ',' +  row['Adj Close']+ ',' +  row['Volume'] + '\n')
        }
    })
    .on('end', () => {
        writableCSV.close()
    });
}

module.exports = {
    async getYahoo(req,res) {
        //console.log("yahoo")
        const YC = req.query.yahooCode;
        const freq = req.query.frequency;
        const begind = req.query.begin_date;
        const endd = req.query.end_date;
        const url = "https://query1.finance.yahoo.com/v7/finance/download/" + YC + "?period1=" + begind + "&period2="+ endd +"&interval=" + freq + "&events=history&includeAdjustedClose=true"

        const ret= await axios(url)
        const retourCSV = ret.data;
        var closes = []
        var dates = []
        const { Readable } = require("stream")
        const readable = Readable.from(retourCSV)
        readable.pipe(csv.parse({headers: true}))
        .on('error', error => console.error(error))
        .on('data', row => {
            if (row['Close'] != 'null') {
            closes.push(row['Close'])
            dates.push(row['Date'])
            }
        })
        .on('end', () => {
            res.json({status: "success", date : dates, close : closes});
        });
    },


    async getVanillaCall(req, res) {
        console.log(process.cwd())
        obj.CalculPnl()
        res.json({status: "success", date : obj.Dates(), port : obj.Spots(), price: obj.Prices()})
    },

    async getSimple(req, res) {
        console.log(process.cwd())
        obj.Simple('./DATA')
        res.json({status: "success", date : obj.Dates(), port : obj.Spots(), price: obj.Prices()})
    },

    async simpleRates(req, res) {
        if (!has(req.body, ['data'])) 
            throw { code: status.BAD_REQUEST, message: 'You must specify the data' };
        let data = JSON.parse(req.body.data);
        if (!has(data, ['numberOfRebalancing']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the numberOfRebalancing' };
        if (!has(data, ['startDate']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the startDate' };
        if (!has(data, ['nbMCSamples']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the nbMCSamples' };

        let { numberOfRebalancing, startDate, nbMCSamples } = JSON.parse(req.body.data);
        console.log(numberOfRebalancing);
        console.log(startDate)
        console.log(nbMCSamples)
        console.log(process.cwd())

        let simu = startDate > "2014-07-18" ? false : true;
        let startD = simu ? "2014-07-18" : startDate;
        console.log(simu)
        obj.SimpleRates('./RATE', './DATA', parseInt(numberOfRebalancing), 10, "2021-12-10", startD, parseInt(nbMCSamples), simu)
        console.log(obj.DatesString())
        res.json({status: "success", date : obj.DatesString(), port : obj.Spots(), price: obj.Prices()})
    },

    async getPerf(req, res) {

        var endDate = getEndDateFromFiles()
        //var endDate = "2021-12-15"
        var perf = obj.PerformanceProduct('./DATA', endDate)
        var date = obj.DatesString()
        console.log(perf)
        console.log(date)
        res.json({status: "success", date, perf})
    },

    async getDeltas(req, res) {
        if (!has(req.body, ['data'])) 
            throw { code: status.BAD_REQUEST, message: 'You must specify the data' };
        let data = JSON.parse(req.body.data);
        if (!has(data, ['date']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the startDate' };
        if (!has(data, ['nbMCSamples']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the nbMCSamples' };

        let { date, nbMCSamples } = JSON.parse(req.body.data);

        console.log(date)

        console.log(process.cwd())
        obj.ComputeDeltas('./RATE', './DATA', 10, "2021-12-10", date, parseInt(nbMCSamples))
        const deltas = obj.Deltas()
        const stockStrings = obj.StocksString()
        const assetsModel = require('./../models/assets.js');
        returnData = [];
        for (i = 0; i < deltas.length; i++){
            let a = await assetsModel.findOne({where:{"yahooCode":stockStrings[i]}});
            returnData.push({stockName:a.dataValues.assetName, delta:deltas[i]});
        }
        res.json({status: "success", repData : returnData})
    },

    async getLimitDate(req, res){
        res.json({status: "success", limitDate : getEndDateFromFiles()})
    },

    async updateDATA(req, res){
        let data = await assetsModel.findAll();
        await data.forEach(d => {
            getFromYahooToCSV(d.yahooCode, 1647903600)
        });

        let databis =['CADEUR=X', 'EUR=X', 'GBPEUR=X', 'JPYEUR=X', 'MXNEUR=X']
        await databis.forEach(d => {
            getFromYahooToCSV(d, 1647903600)
        });
        res.json({status: "success"})
    }

   
}
