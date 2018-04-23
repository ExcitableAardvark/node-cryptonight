// Copyright (c) 2017 ExcitableAardvark <excitableaardvark@tutanota.de>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/* eslint-env jest */

const cryptonight = require('./')

test('async hash of empty string', done => {
  return cryptonight.asyncHash(Buffer.from(''), data => {
    expect(data.toString('hex'))
      .toBe('eb14e8a833fac6fe9a43b57b336789c46ffe93f2868452240720607b14387e11')
    done()
  })
})

test('async hash of empty string with variant 0', done => {
  return cryptonight.asyncHash(Buffer.from(''), 0, data => {
    expect(data.toString('hex'))
      .toBe('eb14e8a833fac6fe9a43b57b336789c46ffe93f2868452240720607b14387e11')
    done()
  })
})

test('async hash of empty string with variant 1', () => {
  expect(() => cryptonight.asyncHash(Buffer.from(''), 1, data => {}))
    .toThrow(/43/)
})

test('async hash of test string', done => {
  return cryptonight.asyncHash(Buffer.from('This is a test'), data => {
    expect(data.toString('hex'))
      .toBe('a084f01d1437a09c6985401b60d43554ae105802c5f5d8a9b3253649c0be6605')
    done()
  })
})

test('async hash of test string with variant 1', done => {
  return cryptonight.asyncHash(Buffer.from('This is a test which as at least 43 bytes ...'), 1, data => {
    expect(data.toString('hex'))
      .toBe('bf1b87e049bfe1c668c44f2dc1bb689abcc729a704fc8088917cfbca202fc3cb')
    done()
  })
})

test('async hash of test string with variant 0', done => {
  return cryptonight.asyncHash(Buffer.from('This is a test'), 0, data => {
    expect(data.toString('hex'))
      .toBe('a084f01d1437a09c6985401b60d43554ae105802c5f5d8a9b3253649c0be6605')
    done()
  })
})

test('async invalid argument throws exception', () => {
  expect(() => cryptonight.asyncHash('not a buffer'))
    .toThrow(/buffer/)
})

test('async no argument throws exception', () => {
  expect(() => cryptonight.asyncHash())
    .toThrow(/buffer/)
})

test('invalid arguments throws exception', () => {
  expect(() => cryptonight.asyncHash(Buffer.from(''), { invalid: true }, () => {}))
    .toThrow(/number/)
})

test('extra arguments throws exception', () => {
  expect(() => cryptonight.asyncHash(Buffer.from(''), 0, () => {}, { invalid: true }))
    .toThrow(/Invalid/)
})

test('sync hash of empty string', () => {
  expect(cryptonight.hash(Buffer.from('')).toString('hex'))
    .toBe('eb14e8a833fac6fe9a43b57b336789c46ffe93f2868452240720607b14387e11')
})

test('sync hash of empty string with variant 0', () => {
  expect(cryptonight.hash(Buffer.from(''), 0).toString('hex'))
    .toBe('eb14e8a833fac6fe9a43b57b336789c46ffe93f2868452240720607b14387e11')
})

test('sync hash of empty string with variant 1', () => {
  expect(() => cryptonight.hash(Buffer.from(''), 1))
    .toThrow(/43/)
})

test('sync hash of test string with variant 1', () => {
  expect(cryptonight.hash(Buffer.from('This is a test which as at least 43 bytes ...'), 1).toString('hex'))
    .toBe('bf1b87e049bfe1c668c44f2dc1bb689abcc729a704fc8088917cfbca202fc3cb')
})

test('sync invalid argument throws exception', () => {
  expect(() => cryptonight.hash('not a buffer'))
    .toThrow(/buffer/)
})

test('sync no argument throws exception', () => {
  expect(() => cryptonight.hash())
    .toThrow(/buffer/)
})

test('sync extra arguments throws exception', () => {
  expect(() => cryptonight.hash(Buffer.from(''), { invalid: true }))
    .toThrow(/number/)
})

test('sync extra arguments throws exception', () => {
  expect(() => cryptonight.hash(Buffer.from(''), 0, { invalid: true }))
    .toThrow(/Invalid/)
})
