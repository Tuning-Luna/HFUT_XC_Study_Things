// @ts-nocheck

let LFSR = new Array(16).fill(0x12345678)

function F(x, y) {
  return (x ^ y) + ((x << 3) | (x >>> 29))
}

function zucGenerate() {
  let x = LFSR[0]
  let y = LFSR[11]

  let z = F(x, y) >>> 0

  for (let i = 0; i < 15; i++) {
    LFSR[i] = LFSR[i + 1]
  }
  LFSR[15] = z & 0x7fffffff

  return z
}

// ===== 测试 =====
for (let i = 0; i < 5; i++) {
  console.log("Keystream:", zucGenerate().toString(16))
}
