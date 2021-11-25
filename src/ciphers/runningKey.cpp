#include "../../include/ciphers.hpp"

RunningKeyCipher::RunningKeyCipher(Cipher *base) {
    baseCipher = base;
}

void RunningKeyCipher::encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext) {
    uint32_t keyLength = plaintext->size() / 2;
    vector<uint32_t> *key = new vector<uint32_t>(plaintext->size() / 2, 0);
    vector<uint32_t> *pt = new vector<uint32_t>(plaintext->size() - key->size(), 0);
    copy(plaintext->begin(), plaintext->begin() + keyLength + 1, key->begin());
    copy(plaintext->begin() + keyLength, plaintext->end(), pt->begin());
    baseCipher->setKey(key);
    baseCipher->encrypt(pt, ciphertext);
    free(pt);
}

void RunningKeyCipher::decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext) {
    baseCipher->decrypt(plaintext, ciphertext);
}