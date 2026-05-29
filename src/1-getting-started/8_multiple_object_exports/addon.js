const addon = require('bindings')('addon')

const obj = new addon.MyObject(5)

obj.multiply(3)
obj.plusOne()

console.log(obj.value())

const other = new addon.OtherObject("Hello, ")

other.multiply(3)
other.append("NAPI!")

console.log(other.value())
