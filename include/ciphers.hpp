#pragma once

#include "util.hpp"

class Cipher {
    protected:
        vector<uint32_t> *enc_key = NULL;
        vector<uint32_t> *dec_key = NULL;
        uint32_t keyLength;

    public:
        
        virtual void encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext) {}
        virtual void decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext) {}
        virtual void generateDecKey() {
            enc_key = dec_key;
        }
        virtual void setKey(vector<uint32_t> *k) {
            enc_key = k;
            keyLength = k->size();
            generateDecKey();
        }
};

class SimpleSubCipher : public Cipher {
    protected:
        uint32_t modulus;
        uint32_t keyLength;
    
    public:
        SimpleSubCipher(vector<uint32_t> *k, uint32_t N);
        virtual void encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext);
        virtual void decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext);
        virtual void generateDecKey();
};

class CaesarCipher : public SimpleSubCipher {
    public:
        CaesarCipher(uint32_t k, uint32_t N);
};

class AffineSubCipher : public SimpleSubCipher {
    public:
        AffineSubCipher(uint32_t a, uint32_t b, uint32_t N);
};

class VigenereCipher : public Cipher {
    protected:
        uint32_t modulus;
        uint32_t keyLength;
    
    public:
        VigenereCipher(vector<uint32_t> *k, uint32_t N);
        virtual void encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext);
        virtual void decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext);
        virtual void generateDecKey();
};

class BeaufortCipher : public VigenereCipher {
    public:
        BeaufortCipher(vector<uint32_t> *k, uint32_t N);
        virtual void encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext);
        virtual void decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext);
};


class RunningKeyCipher : public Cipher {
    protected:
        Cipher *baseCipher;
    
    public:
        RunningKeyCipher(Cipher *base);
        virtual void encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext);
        virtual void decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext);
};

vector<uint32_t> *createAffineKey(uint32_t a, uint32_t b, uint32_t N);
