const express = require('express');
const router = express.Router();
const user = require('../controllers/user.js');

router.get('/users/:id', user.getUserById);
router.get('/users', user.getUsers);
router.post('/users', user.newUser);
router.delete('/users/:id', user.deleteUser);
router.delete('/users', user.deleteAll);
router.put('/users/modify/:id', user.modifyUser);
router.put('/users/:id', user.changeGrade);
router.post('/login', user.login)
router.get('/logout', user.logout)

module.exports = router;