#include <string.h>
#include <stdio.h>

#define MAX_PASSWORD_LENGTH 8

/* same lookup table used in encoding */
const char *lookup = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";

int reverse_lookup(char c) {
    for (int i = 0; i < 64; i++) {
        if (lookup[i] == c) {
            return i;
        }
    }
    return -1; // should never happen if the encoded string is valid
}

char *wol_apgar_decode(const char *encoded)
{
    static char out[MAX_PASSWORD_LENGTH + 1]; // output string (max length 8)
    size_t elen = strlen(encoded);
    int i;

    for (i = 0; i < MAX_PASSWORD_LENGTH; i++)
    {
        if (i >= elen) {
            out[i] = '\0'; // If input string is shorter than 8 characters, fill with '\0'
            continue;
        }

        // Get the index from the lookup table
        int lookup_idx = reverse_lookup(encoded[i]);
        unsigned char left = 0;
        unsigned char right = 0;

        // Reverse the encoding process
        for (left = 0; left < 256; left++) {
            for (right = 0; right < 256; right++) {
                if ((left & 1) > 0) {
                    if (((left << 1) & right) == lookup_idx) {
                        out[i] = left;
                        break;
                    }
                } else {
                    if ((left ^ right) == lookup_idx) {
                        out[i] = left;
                        break;
                    }
                }
            }
            if (out[i] != 0) {
                break;
            }
        }
    }

    out[MAX_PASSWORD_LENGTH] = '\0'; // null-terminate the output string
    return out;
}

int main() {
    const char *encoded = "zotclot9"; // Replace with the actual encoded string
    char *decoded = wol_apgar_decode(encoded);
    printf("Decoded string: %s\n", decoded);
    return 0;
}
