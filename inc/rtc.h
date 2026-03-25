#ifndef RTC_H_
#define RTC_H_

#include "main.h"

void RTC_init();
void RTC_setTime(uint8_t h,uint8_t m);
void RTC_getTime(uint8_t *hour,uint8_t *min,uint8_t *sec);
void RTC_alarmSec();

#endif /* RTC_H_ */
