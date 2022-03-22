const Sequelize = require('sequelize');
const db = require('./database.js');
const users = db.define('users', {
    id: {
        primaryKey: true,
        type: Sequelize.INTEGER,
        autoIncrement: true,
        allowNull: false
    },
    username: {
        type: Sequelize.STRING,
        unique: true,
        allowNull: false,
        validate: {
            max: 16
        }
    },
    password: {
        type: Sequelize.STRING,
        allowNull: false,
        validate: {
            max: 16
        }
    },
    admin: {
        type: Sequelize.BOOLEAN,
        allowNull: false
    },
})
module.exports = users;
