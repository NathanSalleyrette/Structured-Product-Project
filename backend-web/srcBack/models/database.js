const Sequelize = require('sequelize');
// const db = new Sequelize(process.env.DATABASE_URL, {
//     dialectOptions: {
//       ssl: {
//         require: true,
//         rejectUnauthorized: false
//       }
//     }
//   }
// );
const db = new Sequelize({
  dialect: 'sqlite',
  storage: 'bmt.sqlite',
  logging: (...msg) => console.log(msg)
});

module.exports = db;
