// @ts-nocheck

function rotl(x, n) {
  return ((x << n) | (x >>> (32 - n))) >>> 0
}

function sbox(x) {
  return x ^ 0xa5
}

function tau(A) {
  let B = 0
  for (let i = 0; i < 4; i++) {
    let byte = (A >>> (24 - i * 8)) & 0xff
    B |= sbox(byte) << (24 - i * 8)
  }
  return B >>> 0
}

function L(B) {
  return B ^ rotl(B, 2) ^ rotl(B, 10) ^ rotl(B, 18) ^ rotl(B, 24)
}

function T(x) {
  return L(tau(x))
}

function sm4Encrypt(X, rk) {
  for (let i = 0; i < 32; i++) {
    let tmp = X[1] ^ X[2] ^ X[3] ^ rk[i]
    let newX = X[0] ^ T(tmp)
    X = [X[1], X[2], X[3], newX >>> 0]
  }
  return X.reverse()
}

// ===== 测试 =====
let plaintext = [0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210]
let rk = new Array(32).fill(0x11111111)

let cipher = sm4Encrypt(plaintext, rk)

console.log(
  "Cipher:",
  cipher.map((x) => x.toString(16))
)
