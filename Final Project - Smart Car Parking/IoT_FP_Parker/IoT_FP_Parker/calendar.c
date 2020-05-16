/*
 * calendar.c
 *
 *  Created on: 2016. okt. 12.
 *      Author: arkalvac
 */

#include "em_rtcc.h"
#include "calendar.h"

//static PACKSTRUCT (struct calendar_maintain
//{
//uint16_t cal_year;
//uint8_t cal_month;
//uint8_t cal_day;
//uint8_t cal_weekday;
//uint8_t cal_hour;
//uint8_t cal_min;
//uint8_t cal_sec;
//uint16_t cal_ms;
//uint16_t cal_tick;
//int8_t  cal_time_zone;
//uint8_t cal_dst;
//uint8 para_slot_len;
//uint8 para_slot_data[];
//}) maintain;

static uint32_t last_RTCC_value;

void updateCalendar()
{
	ps_load();

	uint32_t ticks,ticks_since_last;
	uint8_t diff_day;
	uint8_t diff_hour;
	uint8_t diff_min;
	uint8_t diff_sec;
	uint16_t diff_tick;

	ticks = RTCC_CounterGet();
	ticks_since_last = ticks - last_RTCC_value;
	last_RTCC_value = ticks;

	diff_tick     = (ticks_since_last) % 128;
	diff_sec      = (ticks_since_last / 128) % 60;
	diff_min      = (ticks_since_last / 128 / 60) % 60;
	diff_hour     = (ticks_since_last / 128 / 60 / 60) % 24;
	diff_day      = (ticks_since_last / 128 / 60 / 60 / 24);

	maintain.cal_tick += diff_tick;
	if (maintain.cal_tick >= 128)
	{
		maintain.cal_tick -= 128;
		maintain.cal_sec++;
	}
	maintain.cal_ms = maintain.cal_tick * 1000 / 128;
	maintain.cal_sec += diff_sec;
	if (maintain.cal_sec >= 60)
	{
		maintain.cal_sec -= 60;
		maintain.cal_min++;
	}
	maintain.cal_min += diff_min;
	if (maintain.cal_min >= 60)
	{
		maintain.cal_min -= 60;
		maintain.cal_hour++;
	}
	maintain.cal_hour += diff_hour;
	if (maintain.cal_hour >= 24)
	{
		maintain.cal_hour -= 24;
		maintain.cal_day++;
		maintain.cal_weekday++;
	}
	maintain.cal_weekday += diff_day;
	if (maintain.cal_weekday > 7)
	{
		maintain.cal_weekday -= 7;
	}
	maintain.cal_day += diff_day;
	switch (maintain.cal_month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
			if (maintain.cal_day > 31)
			{
				maintain.cal_day -= 31;
				maintain.cal_month++;
			}
		break;

		case 12:
			if (maintain.cal_day > 31)
			{
				maintain.cal_day -= 31;
				maintain.cal_month = 1;
				maintain.cal_year++;
			}
		break;

		case 4:
		case 6:
		case 9:
		case 11:
			if (maintain.cal_day > 30)
			{
				maintain.cal_day -= 30;
				maintain.cal_month++;
			}
		break;

		case 2:
			if (maintain.cal_year % 4 == 0 && (maintain.cal_year % 100 != 0 || maintain.cal_year % 400 == 0))
			{
				if (maintain.cal_day > 29)
				{
					maintain.cal_day -= 29;
					maintain.cal_month++;
				}
			}
			else
			{
				if (maintain.cal_day > 28)
				{
					maintain.cal_day -= 28;
					maintain.cal_month++;
				}
			}
		break;
	}
	ps_save();
}

void ps_save(void)
{
	struct gecko_msg_flash_ps_save_rsp_t* pSave;
	pSave = gecko_cmd_flash_ps_save(0x4004, sizeof(struct calendar_maintain), (const uint8*)&maintain);
	  if (pSave->result)
	    printf("PS save failed, code %x\r\n", pSave->result);
}

void ps_load(void)
{
	struct gecko_msg_flash_ps_load_rsp_t* pLoad;
	pLoad = gecko_cmd_flash_ps_load(0x4004);
	if (pLoad->result)
		{
		printf("PS load failed, code %x\r\n", pLoad->result);
		//load date if ps load failed
		}
	else
	{
	memcpy(&maintain, pLoad->value.data, pLoad->value.len);
	}	//shift to after case statements
}

void setDateAndTime(uint16_t year, uint8_t month, uint8_t day, uint8_t weekday, uint8_t hour, uint8_t min, uint8_t sec, uint16_t ms)
{
	maintain.cal_year    = year;
	maintain.cal_month   = month;
	maintain.cal_day     = day;
	maintain.cal_weekday = weekday;
	maintain.cal_hour    = hour;
	maintain.cal_min     = min;
	maintain.cal_sec     = sec;
	maintain.cal_ms      = ms;

	ps_save();

	last_RTCC_value = RTCC_CounterGet();
}

void getDateAndTime(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* weekday, uint8_t* hour, uint8_t* min, uint8_t* sec, uint16_t* ms)
{
	updateCalendar();
	*year = maintain.cal_year;
	*month = maintain.cal_month;
	*day = maintain.cal_day;
	*weekday = maintain.cal_weekday;
	*hour = maintain.cal_hour;
	*min = maintain.cal_min;
	*sec = maintain.cal_sec;
	*ms = maintain.cal_ms;
}

void setTimeZone(int8_t time_zone)
{
	maintain.cal_time_zone = time_zone;
}

int8_t getTimeZone()
{
	return maintain.cal_time_zone;
}

void setDst(uint8_t dst)
{
	maintain.cal_dst = dst;
}

uint8_t getDst()
{
	return maintain.cal_dst;
}
