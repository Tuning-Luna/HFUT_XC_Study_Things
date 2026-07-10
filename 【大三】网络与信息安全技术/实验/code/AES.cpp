#include <iostream>
#include <string>
using namespace std;

string aes_encrypt(string plaintext, string key) {
  string cipher = plaintext;
  for (int i = 0; i < plaintext.size(); i++) {
    cipher[i] = plaintext[i] ^ key[i % key.size()];
  }
  return cipher;
}

string aes_decrypt(string cipher, string key) {
  return aes_encrypt(cipher, key); // 对称
}

int main() {
  string text = "HelloAES";
  string key = "key123";

  string enc = aes_encrypt(text, key);
  string dec = aes_decrypt(enc, key);

  cout << "Plain: " << text << endl;
  cout << "Encrypted: " << enc << endl;
  cout << "Decrypted: " << dec << endl;
}