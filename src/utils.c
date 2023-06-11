#include "utils.h"

#include <stdio.h>

void dump(uint8_t* buf, size_t len) {
    for (size_t i=0; i<len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}