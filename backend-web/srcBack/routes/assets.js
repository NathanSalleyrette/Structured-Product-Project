const express = require('express');
const router = express.Router();
const assets = require('../controllers/assets.js');

router.get('/assets', assets.getAssets);
router.post('/assets', assets.newAsset);
router.delete('/assets/:id', assets.deleteAsset);
router.put('/assets/modify/:id', assets.modifyAsset);

module.exports = router;
