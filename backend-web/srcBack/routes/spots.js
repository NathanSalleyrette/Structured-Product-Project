const express = require('express');
const router = express.Router();
const spots = require('../controllers/spots.js');

router.get('/spots', spots.getSpots);
router.post('/spots', spots.newSpot);
router.delete('/spots', spots.deleteSpot);
//router.delete('/users', user.deleteAll);

module.exports = router;
