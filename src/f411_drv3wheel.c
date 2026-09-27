
#include "f411_drv3wheel.h"

static int16_t clamp_speed(int32_t value)
{
    if (value > 1000) return 1000;
    if (value < -1000) return -1000;
    return (int16_t)value;
}

void F411_Drive3Wheel_Init(F411_Drive3Wheel *drive,
                           F411_Motor *m0,
                           F411_Motor *m1,
                           F411_Motor *m2)
{
    if (drive == NULL) return;
    drive->motor[0] = m0;
    drive->motor[1] = m1;
    drive->motor[2] = m2;
    for (uint32_t r = 0U; r < 3U; r++)
        for (uint32_t c = 0U; c < 3U; c++)
            drive->mix_q15[r][c] = 0;
}

void F411_Drive3Wheel_SetMix(F411_Drive3Wheel *drive,
                             const int16_t mix_q15[3][3])
{
    if (drive == NULL || mix_q15 == NULL) return;
    for (uint32_t r = 0U; r < 3U; r++)
        for (uint32_t c = 0U; c < 3U; c++)
            drive->mix_q15[r][c] = mix_q15[r][c];
}

void F411_Drive3Wheel_SetWheelSpeeds(F411_Drive3Wheel *drive, int16_t s0, int16_t s1, int16_t s2)
{
    int16_t speeds[3] = { clamp_speed(s0), clamp_speed(s1), clamp_speed(s2) };
    if (drive == NULL) return;
    for (uint32_t i = 0U; i < 3U; i++)
        if (drive->motor[i] != NULL) F411_Motor_SetSpeed(drive->motor[i], speeds[i]);
}

void F411_Drive3Wheel_SetVector(F411_Drive3Wheel *drive, int16_t vx, int16_t vy, int16_t wz)
{
    int32_t input[3] = { vx, vy, wz };
    int32_t out[3];
    int32_t max_abs = 0;

    if (drive == NULL) return;

    for (uint32_t r = 0U; r < 3U; r++)
    {
        int64_t sum = 0;
        for (uint32_t c = 0U; c < 3U; c++) sum += (int32_t)drive->mix_q15[r][c] * input[c];
        out[r] = (int32_t)(sum >> 15);
        if (out[r] < 0 ? -out[r] > max_abs : out[r] > max_abs)
            max_abs = (out[r] < 0) ? -out[r] : out[r];
    }

    if (max_abs > 1000)
    {
        for (uint32_t r = 0U; r < 3U; r++) out[r] = (out[r] * 1000) / max_abs;
    }

    F411_Drive3Wheel_SetWheelSpeeds(drive,
                                    (int16_t)out[0],
                                    (int16_t)out[1],
                                    (int16_t)out[2]);
}

void F411_Drive3Wheel_Stop(F411_Drive3Wheel *drive)
{
    if (drive == NULL) return;
    for (uint32_t i = 0U; i < 3U; i++)
        if (drive->motor[i] != NULL) F411_Motor_Stop(drive->motor[i]);
}

void F411_Drive3Wheel_Enable(F411_Drive3Wheel *drive, uint8_t enable)
{
    if (drive == NULL) return;
    for (uint32_t i = 0U; i < 3U; i++)
        if (drive->motor[i] != NULL) F411_Motor_Enable(drive->motor[i], enable);
}
