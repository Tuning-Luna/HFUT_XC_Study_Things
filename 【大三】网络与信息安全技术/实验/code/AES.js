// @ts-nocheck

function aesEncrypt(plaintext, key) {
  let result = ""
  for (let i = 0; i < plaintext.length; i++) {
    result += String.fromCharCode(
      plaintext.charCodeAt(i) ^ key.charCodeAt(i % key.length)
    )
  }
  return result
}

function aesDecrypt(cipher, key) {
  return aesEncrypt(cipher, key)
}

const text = "HelloAES"
const key = "key123"

const enc = aesEncrypt(text, key)
const dec = aesDecrypt(enc, key)

console.log("Plain:", text)
console.log("Encrypted:", enc)
console.log("Decrypted:", dec)
