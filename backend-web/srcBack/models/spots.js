const Sequelize = require('sequelize');
const db = require('./database.js');
const spots = db.define('spots', {
    date: {
        type: Sequelize.DATE,
        allowNull: false,
        primaryKey: true
    },
    assetName: {
        type: Sequelize.STRING,
        allowNull: false,
        primaryKey: true
    },
    spot: {
        type: Sequelize.INTEGER,
        allowNull: true
    },
})
module.exports = spots;