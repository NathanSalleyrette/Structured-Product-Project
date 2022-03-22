const status = require('http-status');
const assetsModel = require('../models/assets.js');
const has = require('has-keys');
const { decAndVerifToken } = require('./token.js');

function verifyLog(req) {
    if (!has(req.params, 'assetname'))
        return false;
    let { assetname } = req.params;
    // verifions si le token est le bon

    let dataToken = decAndVerifToken(req);
    if (dataToken != assetname) {
        return false;
    }
    return true;
}

module.exports = {

    async getAssets(req, res) {
        let data = await assetsModel.findAll();
        res.json({ status: true, message: 'Returning assets', data });
    },

    async newAsset(req, res) {
        console.log(req.body);
        if (!has(req.body, ['data'])) {
            throw { code: status.BAD_REQUEST, message: 'You must specify the data' };
        }
        let data = JSON.parse(req.body.data);
        console.log(data);

        if (!has(data, ['assetName']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the assetName' };
        if (!has(data, ['yahooCode']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the yahooCode' };
        let { assetName, yahooCode } = data;

        if (await assetsModel.findOne({ where: { assetName } }))
            throw { code: status.BAD_REQUEST, message: 'This asset is already in the database' };

        const newAsset = await assetsModel.create({ assetName, yahooCode });
        console.log(newAsset.dataValues.id);
        res.json({ status: true, message: 'Asset Added' , id:newAsset.dataValues.id });

    },

    async deleteAsset(req, res) {
            if (!has(req.params, 'id'))
                throw { code: status.BAD_REQUEST, message: 'You must specify the id' };
            let { id } = req.params;
            await assetsModel.destroy({ where: { id } });
            res.json({ status: true, message: 'Asset deleted' });
        },

        async modifyAsset(req, res) {
            // if (!(await verifyAdmin(req))) {
            //     throw { code: status.BAD_REQUEST, message: 'Token problem' }
            // }
    
            if (!has(req.params, 'id'))
                throw { code: status.BAD_REQUEST, message: 'You must specify the id in req' };
            let { id } = req.params;
            let asset = await assetsModel.findOne({ where: { id } });
            if (!asset)
                throw { code: status[304], message: 'There no asset in the database with this id' };
            else {
                let data = JSON.parse(req.body.data);
                if (!has(data, ['assetName']))
                    throw { code: status.BAD_REQUEST, message: 'In data, you must specify the assetname' };
    
                if (!has(data, ['yahooCode']))
                    throw { code: status.BAD_REQUEST, message: 'In data, you must specify the password' };
    
                let {assetName, yahooCode } = data;
    
                await asset.update({ assetName, yahooCode });
                res.json({ status: true, message: 'Asset updated'});
            }
        },
}
