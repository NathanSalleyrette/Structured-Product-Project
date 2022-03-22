const status = require('http-status');
const userModel = require('../models/users.js');
const has = require('has-keys');
const jws = require('jws');
require('mandatoryenv').load(['SECRETJWS'])

module.exports = {
    async getToken(req, res){
        if(!has(req.params, 'username'))
            throw {code: status.BAD_REQUEST, message: 'You must specify the username'};
        let {username} = req.params;
        if (!(await userModel.findOne({where: {username}})))
            throw {code: status.BAD_REQUEST, message: 'The username is not present in database'};
        const signature = jws.sign({
            header: { alg: 'HS256' },
            payload: username,
            secret: 'Nicolas est beau',
            });
        let data = {token: signature };
        res.json({status: true, message: 'Returning Token', data});
    },

    getTokenFromUsername: (username) => {
        const signature = jws.sign({
            header: { alg: 'HS256' },
            payload: username,
            secret: process.env.SECRETJWS,
            });
        return signature;
    },

    decAndVerifToken(req) {
        let token = req.headers['x-access-token']
        if (!jws.verify(token, 'HS256', 'Nicolas est beau'))
            throw false;
        return jws.decode(token).payload;
    },

    async decodeToken(req, res){
        if(!has(req.headers, 'x-access-token'))
            throw {code: status.BAD_REQUEST, message: 'You must specify the token'};
        let token = req.headers['x-access-token'];
        if (!jws.verify(token, 'HS256', 'Nicolas est beau')) 
            throw {code: status.BAD_REQUEST, message: 'Token is incorrect'};
        res.json({status: true, message: 'login décodé', data: jws.decode(token).payload});
    },

     verifyAuth(req, res) {
         if (!req.session.loggedIn) req.session.loggedIn = false;
         if (!req.session.isAdmin) req.session.isAdmin = false;
        res.json({auth : req.session.loggedIn, admin : req.session.isAdmin})
    }
}
