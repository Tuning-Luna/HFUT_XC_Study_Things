// @ts-nocheck

function modexp(base, exp, mod) {
  let result = 1n
  base = BigInt(base) % BigInt(mod)
  exp = BigInt(exp)
  mod = BigInt(mod)

  while (exp > 0n) {
    if (exp % 2n === 1n) result = (result * base) % mod
    base = (base * base) % mod
    exp /= 2n
  }
  return result
}

const p = 61n
const q = 53n
const n = p * q
const e = 17n
const d = 2753n

const m = 65n

const c = modexp(m, e, n)
const decrypted = modexp(c, d, n)

console.log("n =", n.toString())
console.log("Public key:", `(${e}, ${n})`)
console.log("Private key:", `(${d}, ${n})`)
console.log("Encrypted:", c.toString())
console.log("Decrypted:", decrypted.toString())
