#include "../../include/ciphers.hpp"

/* Standard Vigenere */
VigenereCipher::VigenereCipher(vector<uint32_t> *k, uint32_t N) {
    modulus = N;
    setKey(k);
}

void VigenereCipher::generateDecKey() {
    dec_key = new vector<uint32_t>;
    for (int i = 0; i < keyLength; i++) {
        dec_key->push_back((modulus - (*enc_key)[i]));
    }
}

void VigenereCipher::encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext) {
    for (int i = 0; i < plaintext->size(); i++) 
        ciphertext->push_back(combineMod((*plaintext)[i], (*enc_key)[i % modulus], modulus));
}

void VigenereCipher::decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext) {
    for (int i = 0; i < ciphertext->size(); i++)
        plaintext->push_back(combineMod((*ciphertext)[i], (*dec_key)[i % modulus], modulus));
}

/* Beaufort */

BeaufortCipher::BeaufortCipher(vector<uint32_t> *k, uint32_t N) :
    VigenereCipher(k, N) 
    { }

void BeaufortCipher::encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext) {
    for (int i = 0; i < plaintext->size(); i++) 
        ciphertext->push_back(combineMod(modulus - (*plaintext)[i], (*enc_key)[i % modulus], modulus));
}

void BeaufortCipher::decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext) {
    for (int i = 0; i < ciphertext->size(); i++)
        plaintext->push_back(combineMod(modulus - (*ciphertext)[i], (*enc_key)[i % modulus], modulus));
}