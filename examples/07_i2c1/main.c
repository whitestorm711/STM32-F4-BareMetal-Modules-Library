
#include "f411_i2c1.h"

int main(void)
{
    uint8_t id;
    F411_I2C1_Init(16000000U, 100000U);

    while (1)
    {
        (void)F411_I2C1_MemRead8(0x68U, 0x75U, &id, 1U, 100000U);
        (void)id;
    }
}
