#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
using namespace std;

uint32_t rotl(uint32_t x, int n) {
  return (x << n) | (x >> (32 - n));
}

// 简化 Sbox
uint8_t sbox(uint8_t x) {
  return x ^ 0xA5;
}

uint32_t tau(uint32_t A) {
  uint32_t B = 0;
  for (int i = 0; i < 4; i++) {
    uint8_t byte = (A >> (24 - i * 8)) & 0xFF;
    B |= (sbox(byte) << (24 - i * 8));
  }
  return B;
}

uint32_t L(uint32_t B) {
  return B ^ rotl(B, 2) ^ rotl(B, 10) ^ rotl(B, 18) ^ rotl(B, 24);
}

uint32_t T(uint32_t x) {
  return L(tau(x));
}

void sm4_encrypt(uint32_t X[4], uint32_t rk[32]) {
  for (int i = 0; i < 32; i++) {
    uint32_t tmp = X[1] ^ X[2] ^ X[3] ^ rk[i];
    uint32_t newX = X[0] ^ T(tmp);

    X[0] = X[1];
    X[1] = X[2];
    X[2] = X[3];
    X[3] = newX;
  }
}

int main() {
  // ===== 输入 =====
  uint32_t plaintext[4] = {
    0x01234567,
    0x89ABCDEF,
    0xFEDCBA98,
    0x76543210
  };

  uint32_t rk[32];
  for (int i = 0; i < 32; i++) {
    rk[i] = 0x11111111; // 简化轮密钥
  }

  // ===== 加密 =====
  sm4_encrypt(plaintext, rk);

  // ===== 输出 =====
  cout << "SM4 Ciphertext: ";
  for (int i = 0; i < 4; i++) {
    cout << hex << setw(8) << setfill('0') << plaintext[i] << " ";
  }
  cout << endl;

  return 0;
}