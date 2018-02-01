# node-cryptonight
> node bindings for cryptonight hashing

### Requirements

node-cryptonight requires [Boost](http://www.boost.org)

##### Ubuntu

    sudo apt-get install libboost-all-dev

##### Mac

    brew install boost

### Installation

    npm install --save node-cryptonight
   
### Testing

Code is linted with [standard](https://github.com/standard/standard) and tested with [Jest](https://github.com/facebook/jest). Run `npm test` to lint and run tests.

### Usage Examples

##### Synchronous Hashing

```js
const cryptonight = require('node-cryptonight').hash
const hash = cryptonight(Buffer.from('This is a test'))
console.log(hash) // <Buffer a0 84 f0 1d 14 37 ..>
```

##### Asynchronous Hashing

```js
const cryptonight = require('node-cryptonight').asyncHash
cryptonight(Buffer.from('This is a test'), hash => {
  console.log(hash) // <Buffer a0 84 f0 1d 14 37 ..>
})
```

##### Promise Wrapper Example

```js
function cryptonight(data) {
  return new Promise((resolve, reject) => {
    require('node-cryptonight').asyncHash(data, hash => {
      resolve(hash)
    })
  })
}

cryptonight(Buffer.from('This is a test'))
  .then(console.log) // <Buffer a0 84 f0 1d 14 37 ..>
```

### See Also

* [node-cryptonight-lite](https://github.com/ExcitableAardvark/node-cryptonight-lite)

### License

Released under the 3-Clause BSD License. Contains code from the Monero project. See `LICENSE` for more information.
