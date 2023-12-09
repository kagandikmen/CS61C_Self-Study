#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    
    unsigned to_msb = ((((*reg << 15) >> 15)^((*reg << 13) >> 15))^((*reg << 12) >> 15))^((*reg << 10) >> 15);
    *reg = (*reg >> 1) | (to_msb << 15);

}

