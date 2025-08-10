#include "crypto.h"
#include <tinycrypt/aes.h>
#include <tinycrypt/constants.h>
#include <zephyr/sys/printk.h>

#define AES_KEY_SIZE 16
#define AES_BLOCK_SIZE 16

// 16-byte AES key
uint8_t aes_key[AES_KEY_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

int encrypt_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    struct tc_aes_key_sched_struct aes;

    if (tc_aes128_set_encrypt_key(&aes, aes_key) == TC_CRYPTO_FAIL) {
        printk("Failed to set AES encryption key!\n");
        return -1;
    }

    for (size_t i = 0; i < input_len; i += AES_BLOCK_SIZE) {
        if (tc_aes_encrypt(output + i, input + i, &aes) == TC_CRYPTO_FAIL) {
            printk("AES encryption failed!\n");
            return -1;
        }
    }

    return 0;
}
