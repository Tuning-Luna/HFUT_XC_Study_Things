#include <iostream>
using namespace std;

long long modexp(long long base, long long exp, long long mod) {
  long long res = 1;
  base %= mod;
  while (exp > 0) {
    if (exp % 2) res = (res * base) % mod;
    base = (base * base) % mod;
    exp /= 2;
  }
  return res;
}

int main() {
  long long p = 61, q = 53;
  long long n = p * q; // 3233
  long long e = 17;
  long long d = 2753;

  long long m = 65;

  long long c = modexp(m, e, n);
  long long decrypted = modexp(c, d, n);

  cout << "n=" << n << endl;
  cout << "Public key: (" << e << "," << n << ")" << endl;
  cout << "Private key: (" << d << "," << n << ")" << endl;
  cout << "Encrypted: " << c << endl;
  cout << "Decrypted: " << decrypted << endl;
}