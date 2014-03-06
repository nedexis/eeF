/***************************************************************************//**
 * @file		eef_avr.h
 * @author      Dejan Ristic 
 * @brief       AVR 8bit GCC Port include file
 * @details     detalis
 * 
 * Copyright (C) 2013  Dejan Ristic
 *
 * PROJECT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * PROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *//************************************************************************//**
 * @addtogroup  ep 
 *********************************************************************//** @{ */
#ifndef PORT_H_
#define PORT_H_
/*_________________________________________________________  Include Files  __*/
#include <stdbool.h>
#include <avr/interrupt.h>
/*_______________________________________________________________  Defines  __*/
/**@brief			If user want use assert EEF_USE_ASSERT must be uncomented
 */
//#define EEF_USE_ASSERT
/**@brief			Definition of boolean type values
 */
#define TRUE	true
#define FALSE	false
/*_______________________________________________________________  Macro's  __*/
#ifdef EEF_USE_ASSERT
# define E_ASSERT(exp)														   \
	if((exp) == false)														   \
	{																		   \
		while(1);															   \
	}
# else
#define E_ASSERT(exp)
#endif

#define E_PORT_DIS_INT()													   \
			cli()

#define E_PORT_ENA_INT()													   \
			sei()			
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif
/*_____________________________________________________________  Data Types  __*/
typedef bool    bool_T;
/*_______________________________________________________  Global Variables  __*/
/*____________________________________________________  Function Prototypes  __*/

/** @} *//**********************************************************************/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------  C++ extern end  --*/
#endif 