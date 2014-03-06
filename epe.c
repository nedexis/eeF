/***************************************************************************//**
 * @file		epe.c
 * @author      Dejan Ristic 
 * @brief       Event processing engine implemetation file
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
 * @addtogroup  epe 
 *********************************************************************//** @{ */
 
/*_________________________________________________________  Include Files  __*/
#include "epe.h"
/*_________________________________________________________  Local Defines  __*/
/*_________________________________________________________  Local Macro's  __*/
/*______________________________________________________  Local Data Types  __*/
/*_______________________________________________________  Local Variables  __*/
/*______________________________________________________  Global Variables  __*/
epe_T * firstEpe = NULL;
/*_____________________________________________  Local Function Prototypes  __*/
/*____________________________________________  Local Function Definitions  __*/
/*___________________________________________  Global Function Definitions  __*/
void efEpeInit(
	epe_T *			epe,
	evtQ_T *		evtQ,
	evt_T **		evtBuff,
	size_t			evtBuffsize,
#ifdef EPE_USE_PRIORITY
	uint8_t			priority,
#endif	
	epeStates_T 	initState) {
	
	epe_T * currEpe;
	
	E_ASSERT(epe != NULL);
	E_ASSERT(evtQ != NULL);
	E_ASSERT(evtBuffsize != 0);
	E_ASSERT(initState != NULL);
	
	efEvtQInit(
				evtQ,
				evtBuff,
				evtBuffsize);
	
	epe->next = NULL;
	epe->evtQ = evtQ;	
	epe->currState = initState;
	epe->prevState = initState;
	epe->exeState = INIT_TRAN;
		
#ifdef EPE_USE_PRIORITY	
	epe->priority = priority;
#endif	
	
	if (NULL == firstEpe) {
		firstEpe = epe;
	} else {
		currEpe = firstEpe; 
		while (currEpe != NULL) {
			if (NULL == currEpe->next) {
				currEpe->next = epe;	
							
				break;		
						
			} else {
				currEpe = currEpe->next;
			}
		}		
	}				
}

/*----------------------------------------------------------------------------*/
void efEpeDeinit(
	epe_T *			epe) {

	epe_T * currEpe;
	epe_T * prevEpe;
	
	E_ASSERT(epe != NULL);	
	
	E_PORT_DIS_INT();
	
	epe->evtQ->count = 0;														/* Clear queue */
	
	if (NULL == firstEpe) {
				
		E_ASSERT(firstEpe != NULL);												/* Cant deinit unexisting epe */	
		
	} else {
		currEpe = firstEpe;
		prevEpe = firstEpe;		
		while (currEpe != NULL) {
			if (currEpe == epe) {
				prevEpe->next = currEpe->next;	
							
				break;
				
			} else {
				prevEpe = currEpe;
				currEpe = currEpe->next;
			}
		}
	}
	E_PORT_ENA_INT();			
}

/** @} *//*********************************************************************/
