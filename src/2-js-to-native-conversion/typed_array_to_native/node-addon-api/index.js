/**
 * @type {import('./type')}
 */
const binding = require('bindings')('typed_array_to_native');

{
    const byteArray = Uint8Array.from([10, 20, 30, 50, 25, 17]);
    console.log("byteArray: ", byteArray);
    binding.AcceptByteArray(byteArray);
}
console.log("---------");

{
    const array = [7, 15, 26, 58, 64];
    console.log("array: ", array);
    const byteArray = binding.CreateByteArray(array);
    console.log("byteArray: ", byteArray);
}