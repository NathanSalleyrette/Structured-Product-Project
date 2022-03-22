const Sequelize = require('sequelize');
const db = require('./database.js');
const assets = db.define('assets', {
    id: {
        primaryKey: true,
        type: Sequelize.INTEGER,
        autoIncrement: true,
        allowNull: false
    },
    assetName: {
        type: Sequelize.STRING,
        allowNull: false,
    },
    yahooCode: {
        unique:true,
        type: Sequelize.STRING,
        allowNull: true
    },
})
module.exports = assets;