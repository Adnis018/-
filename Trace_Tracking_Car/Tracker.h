#ifndef __TRACKER_H__
#define __TRACKER_H__

#include <stdint.h>

void Tracker_Init(void);
uint8_t Tracker_GetRawData(void);
float Tracker_GetWeightedError(void);

#endif /* __TRACKER_H__ */