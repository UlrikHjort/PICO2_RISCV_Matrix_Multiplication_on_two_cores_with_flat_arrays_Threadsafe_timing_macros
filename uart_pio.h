/**************************************************************************
--           PICO2 RISC-V Threadsafe UART TX PIO SM
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

#ifndef INC_PIO_BITBANG_H_
#define INC_PIO_BITBANG_H_

void init_uart(uint32_t baud_rate, uint tx_pin_);

#endif /* INC_PIO_BITBANG_H_*/
