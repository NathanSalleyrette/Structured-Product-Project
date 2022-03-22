const router = require('express').Router();
router.use(require('./user'));
router.use(require('./token'));
router.use(require('./spots'));
router.use(require('./assets'));
router.use(require('./pricingLib'));
module.exports = router;
