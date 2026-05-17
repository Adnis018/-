#include "ti_msp_dl_config.h"

void Delay_ms(uint32_t ms)
{
    delay_cycles(ms * 32000);
}

void Delay_us(uint32_t us) {
    delay_cycles(us * 32);
}