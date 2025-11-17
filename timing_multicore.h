/**************************************************************************
--             PICO2 RISC-V Multicore timing macros
-- 
--           Copyright (C) 2025 By Ulrik Hørlyk Hjort
--
--  This Program is Free Software; You Can Redistribute It and/or
--  Modify It Under The Terms of The GNU General Public License
--  As Published By The Free Software Foundation; Either Version 2
--  of The License, or (at Your Option) Any Later Version.
--
--  This Program is Distributed in The Hope That It Will Be Useful,
--  But WITHOUT ANY WARRANTY; Without Even The Implied Warranty of
--  MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE.  See The
--  GNU General Public License for More Details.
--
-- You Should Have Received A Copy of The GNU General Public License
-- Along with This Program; if not, See <Http://Www.Gnu.Org/Licenses/>.
***************************************************************************/
#ifndef INC_TIMING_H_
#define INC_TIMING_H_
#include "hardware/structs/sio.h"

absolute_time_t start_global_0 = 0;
absolute_time_t start_global_1 = 0;

#define TIME_START() {\
	if (sio_hw->cpuid == 0) \
      start_global_0 = get_absolute_time(); \
	else  \
      start_global_1 = get_absolute_time(); \
	}

#define TIME_US_STOP(label) {\
		if (sio_hw->cpuid == 0) \
			printf("%s: (Core %ld) %lld us\r\n", label, sio_hw->cpuid, (absolute_time_diff_us(start_global_0, get_absolute_time()))); \
		else \
 			printf("%s: (Core %ld) %lld us\r\n", label, sio_hw->cpuid, (absolute_time_diff_us(start_global_1, get_absolute_time()))); \
		}

#define TIME_MS_STOP(label)	{ \
	if (sio_hw->cpuid == 0)	 \
		printf("%s: (Core %ld) %lld ms\r\n", label, sio_hw->cpuid, (absolute_time_diff_us(start_global_0, get_absolute_time()) / 1000)); \
    else \
		printf("%s: (Core %ld) %lld ms\r\n", label, sio_hw->cpuid, (absolute_time_diff_us(start_global_1, get_absolute_time()) / 1000)); \
    }


#define TIME_MEASURE_US(func) \
{                             \
 absolute_time_t start_local = get_absolute_time(); \
 func;    \
 printf(#func ": (Core: %ld) %lld us\r\n", sio_hw->cpuid, absolute_time_diff_us(start_local, get_absolute_time())); \
}

#define TIME_MEASURE_MS(func) \
{                             \
 absolute_time_t start_local = get_absolute_time(); \
 func;                 \
 printf(#func ": (Core: %ld) %lld ms\r\n", sio_hw->cpuid, (absolute_time_diff_us(start_local, get_absolute_time()) / 1000)); \
}

#endif /* INC_TIMING_H_ */
