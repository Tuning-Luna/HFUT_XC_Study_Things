#include <iostream>
#include <cstdint>
#include <iomanip>
using namespace std;

uint32_t LFSR[16];

uint32_t F(uint32_t x, uint32_t y) {
  return (x ^ y) + ((x << 3) | (x >> 29));
}

uint32_t zuc_generate() {
  uint32_t x = LFSR[0];
  uint32_t y = LFSR[11];

  uint32_t z = F(x, y);

  // 更新 LFSR
  for (int i = 0; i < 15; i++) {
    LFSR[i] = LFSR[i + 1];
  }
  LFSR[15] = z & 0x7FFFFFFF;

  return z;
}

int main() {
  // ===== 初始化输入 =====
  for (int i = 0; i < 16; i++) {
    LFSR[i] = 0x12345678; // 初始状态
  }

  cout << "ZUC Keystream:" << endl;

  // ===== 生成密钥流 =====
  for (int i = 0; i < 5; i++) {
    uint32_t ks = zuc_generate();
    cout << hex << setw(8) << setfill('0') << ks << endl;
  }

  return 0;
}