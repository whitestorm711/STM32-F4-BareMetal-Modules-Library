
#ifndef F411_DRV3WHEEL_H
#define F411_DRV3WHEEL_H

#include "f411_motor.h"

typedef struct
{
    F411_Motor *motor[3];
    /* 3x3 fixed-point transform, Q15. Output = M * [vx, vy, wz]. */
    int16_t mix_q15[3][3];
} F411_Drive3Wheel;

void F411_Drive3Wheel_Init(F411_Drive3Wheel *drive,
                           F411_Motor *m0,
                           F411_Motor *m1,
                           F411_Motor *m2);
void F411_Drive3Wheel_SetMix(F411_Drive3Wheel *drive,
                             const int16_t mix_q15[3][3]);
void F411_Drive3Wheel_SetWheelSpeeds(F411_Drive3Wheel *drive, int16_t s0, int16_t s1, int16_t s2);
void F411_Drive3Wheel_SetVector(F411_Drive3Wheel *drive, int16_t vx, int16_t vy, int16_t wz);
void F411_Drive3Wheel_Stop(F411_Drive3Wheel *drive);
void F411_Drive3Wheel_Enable(F411_Drive3Wheel *drive, uint8_t enable);

#endif
