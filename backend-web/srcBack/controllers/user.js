const status = require('http-status');
const userModel = require('../models/users.js');
const has = require('has-keys');
const { decAndVerifToken, getTokenFromUsername } = require('./token.js');


async function verifyAdmin(req) {
    // verifions si le token est le bon
    let dataToken = decAndVerifToken(req);
    let find = await userModel.findOne({ where: { username : dataToken, admin : true} });
    //console.log("dzdz" , find);
    if (find === null) {
        return false;
    }
    return true;
}

module.exports = {

    async getUserById(req, res) {
        if (!has(req.params, 'id'))
            throw { code: status.BAD_REQUEST, message: 'You must specify the id' };
        let { id } = req.params;
        let data = await userModel.findOne({ where: { id } });
        if (!data)
            throw { code: status.BAD_REQUEST, message: 'User not found' };
        res.json({ status: true, message: 'Returning user', data });
    },
    async getUsers(req, res) {
        let data = await userModel.findAll();

        res.json({ status: true, message: 'Returning users', data });
    },
    async newUser(req, res) {
        if (!has(req.body, ['data'])) {
            throw { code: status.BAD_REQUEST, message: 'You must specify the data' };
        }
        let data = JSON.parse(req.body.data);
        if (!has(data, ['username']))
            throw { code: status.BAD_REQUEST, message: 'In data, you must specify the username' };

        let { username } = data;

        if (await userModel.findOne({ where: { username } }))
            throw { code: status.BAD_REQUEST, message: 'The user is already in the database' };

        if (!has(data, ['password']))
            throw { code: status.BAD_REQUEST, message: 'In d ata, you must specify the password' };

        let { password } = data;
        let admin = false;

        await userModel.create({ username, password, admin });
        res.json({ status: true, message: 'User Added' });

    },

    async deleteUser(req, res) {
        if (!has(req.params, 'id'))
            throw { code: status.BAD_REQUEST, message: 'You must specify the id' };

        let { id } = req.params;
        await userModel.destroy({ where: { id } });
        res.json({ status: true, message: 'User deleted' });
    },

    async deleteAll(req, res) {
        await userModel.destroy({ where: {} });
        res.json({ status: true, message: 'All users deleted' });
    },

    async changeGrade(req, res) {
        // if (!(await verifyAdmin(req))) {
        //     throw { code: status.BAD_REQUEST, message: 'Token problem' }
        // }

        if (!has(req.params, 'id'))
            throw { code: status.BAD_REQUEST, message: 'You must specify the id or no username given in req' };
        let { id } = req.params;
        let user = await userModel.findOne({ where: { id } });
        if (!user)
            throw { code: status[304], message: 'There no user in the database with this id' };
        else {
            let newGrade = !user.admin;
            await user.update({ admin: newGrade });
            res.json({ status: true, message: 'Permission updated to ' +  newGrade});
        }
    },

    async modifyUser(req, res) {
        // if (!(await verifyAdmin(req))) {
        //     throw { code: status.BAD_REQUEST, message: 'Token problem' }
        // }

        if (!has(req.params, 'id'))
            throw { code: status.BAD_REQUEST, message: 'You must specify the id or no username given in req' };
        let { id } = req.params;
        let user = await userModel.findOne({ where: { id } });
        if (!user)
            throw { code: status[304], message: 'There no user in the database with this id' };
        else {
            let data = JSON.parse(req.body.data);
            if (!has(data, ['username']))
                throw { code: status.BAD_REQUEST, message: 'In data, you must specify the username' };

            if (!has(data, ['password']))
                throw { code: status.BAD_REQUEST, message: 'In data, you must specify the password' };
            
            if (!has(data, ['admin']))
                throw { code: status.BAD_REQUEST, message: 'In data, you must specify the rôle' };

            let {username, password, admin } = data;

            await user.update({ username, password, admin });
            res.json({ status: true, message: 'User updated'});
        }
    },

    async login(req, res) {
        console.debug('Connection sur /login')
        console.debug(req.body.data)
        let { username, password } = JSON.parse(req.body.data);
        console.debug(`${username} avec le mot de passe ${password}`)
        let user = await userModel.findOne({where: {username, password}});
        if (!user) {
            res.send({ message: 'login failed' }); 
        } else{
            req.session.loggedIn = true;
            req.session.username = username;
            req.session.isAdmin = user.admin;
            const token = getTokenFromUsername(username);
            // res.cookie('token', token, {httpOnly : true});
            res.send({ message: 'login success' , token, admin:user.admin }); return 
        }
    },

    logout(req, res) {
        req.session.destroy((err)=>{})
        res.send({message: 'Thank you, visit again !'})
    }
}
