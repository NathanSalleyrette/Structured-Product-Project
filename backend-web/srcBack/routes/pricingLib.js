const express = require('express');
const router = express.Router();
const pricingLib = require('../controllers/pricingLib.js');

router.get('/yahoo', pricingLib.getYahoo);
router.get('/vanillaCall', pricingLib.getVanillaCall);
router.get('/simple', pricingLib.getSimple);
router.post('/simpleRates', pricingLib.simpleRates);
router.post('/getDeltas', pricingLib.getDeltas);
router.get('/performance', pricingLib.getPerf);
router.get('/CSV', pricingLib.updateDATA);
router.get('/limitDate', pricingLib.getLimitDate);

module.exports = router;
