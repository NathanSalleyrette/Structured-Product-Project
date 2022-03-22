const status = require('http-status');
const spotsModel = require('../models/spots.js');
const has = require('has-keys');
const { decAndVerifToken } = require('./token.js');

function verifyLog(req) {
    if (!has(req.params, 'username'))
        return false;
    let { username } = req.params;
    // verifions si le token est le bon

    let dataToken = decAndVerifToken(req);
    if (dataToken != username) {
        return false;
    }
    return true;
}

module.exports = {

    async getSpots(req, res) {
        let data = await spotsModel.findAll();

        res.json({ status: true, message: 'Returning spots', data });
    },

    async newSpot(req, res) {
        //console.log(req.body);
        if (!has(req.body, ['data'])) {
            throw { code: status.BAD_REQUEST, message: 'You must specify the data' };
        }
        let data = JSON.parse(req.body.data);
        //console.log(data);
        if (!has(data, ['date']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the date' };
        if (!has(data, ['assetName']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the assetName' };
        let { date, assetName } = data;

        if (await spotsModel.findOne({ where: { date, assetName } }))
            throw { code: status.BAD_REQUEST, message: 'The spot for this date et assetName is already in the database' };

        if (!has(data, ['spot']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the spot' };

        let { spot } = data;

        await spotsModel.create({ date, assetName, spot });
        res.json({ status: true, message: 'Spot Added' });

    },

    async deleteSpot(req, res) {
        if (!has(req.body, ['data'])) {
            throw { code: status.BAD_REQUEST, message: 'You must specify the data' };
        }
        let data = JSON.parse(req.body.data);
        if (!has(data, ['date']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the date' };
        if (!has(data, ['assetName']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the assetName' };
        let { date, assetName } = data;

        await userModel.destroy({ where: { date, assetName } });
        res.json({ status: true, message: 'User deleted' });
    },

}
