/**
 * @type {import('../type')}
 */
const bindings = require("bindings")("addon");

bindings.doHeavyMath(2, 1).then(console.log);
