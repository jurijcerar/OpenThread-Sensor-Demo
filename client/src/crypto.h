#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdint.h>

// Extern declaration of AES key
extern uint8_t aes_key[16];

// Function prototype
int encrypt_data(const uint8_t *input, size_t input_len, uint8_t *output);

#endif // CRYPTO_H

