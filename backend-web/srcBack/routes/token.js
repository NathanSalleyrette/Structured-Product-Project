const express = require('express');
const router = express.Router();
const token = require('../controllers/token.js');

router.get('/getToken/:username', token.getToken);
router.get('/whoami', token.decodeToken);
router.get('/verify', token.verifyAuth);

module.exports = router;
