#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdint.h>

// Function prototype
int decrypt_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);

#endif // CRYPTO_H
