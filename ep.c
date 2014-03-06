/***************************************************************************//**
 * @file		ep.c
 * @author      Dejan Ristic 
 * @brief       Event Processor implemetation file
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
	 
/*_________________________________________________________  Include Files  __*/
#include "ep.h"
/*_________________________________________________________  Local Defines  __*/
/*_________________________________________________________  Local Macro's  __*/
/*______________________________________________________  Local Data Types  __*/
typedef void (* eefProcess_T)(void);
/*_______________________________________________________  Local Variables  __*/
static eefProcess_T eefProcess = NULL; 
/*______________________________________________________  Global Variables  __*/
/*_____________________________________________  Local Function Prototypes  __*/
static inline void dispatch(
	epe_T * epe, 
	evt_T *  evt);  
		
static void schedule(
	void);	
	
static void stop(
	void);
	
static void start(
	void);
/*____________________________________________  Local Function Definitions  __*/
static inline void dispatch(
	epe_T * epe, 
	evt_T *  evt) {	
		
	switch (epe->exeState) {
		case EPE_WAIT: {
			if (epe->prevState != epe->currState) {
	
				epe->exeState = HANDLE_TRAN;
				goto HTRAN;
				
			} else {
WAIT:			if (evt != NULL) {
					
					epe->exeState = HANDLE_EVT;					
					goto HEVT;
				}
			}		
				
			return;
		}
			
		case INIT_TRAN : {
			epe->currState(SIG_ENTRY);
			
			epe->exeState = EPE_WAIT;
			
			return;
		}
			
HTRAN:		case HANDLE_TRAN : {	
#ifdef USE_SIG_EXIT	
			epe->prevState(SIG_EXIT);
#endif		
			epe->currState(SIG_ENTRY);
			epe->prevState = epe->currState;
			
			epe->exeState = EPE_WAIT;
			
			goto WAIT;
		}
			
HEVT:	case HANDLE_EVT : {
			epe->currState(evt);
			
			epe->exeState = EPE_WAIT;						
			
			return;
		}
			
		default : {
			epe->exeState = EPE_WAIT;
		}
	}		
}

/*----------------------------------------------------------------------------*/
static void schedule(
	void) { 
	
#ifdef EPE_MAX_PRIORITY
	epe_T * epe;
	epe_T * tmpEpe = NULL;
	uint8_t maxPriority = 255;

	epe = GET_FIRST_EPE();
	
	E_ASSERT(epe != NULL);
	
	while (epe != NULL) {
		if (FALSE == efIsEvtQEmpty(epe->evtQ)) {
			if (epe->priority < maxPriority) {
				maxPriority = epe->priority;
				tmpEpe = epe;
			}
		}		
		dispatch(
			epe,
			NULL);
		epe = epe->next;
	}
	if (tmpEpe != NULL) {
		evt_T * evt;
	
		evt = efRetrieveEvt(tmpEpe->evtQ);
				
		dispatch(
		tmpEpe,
		evt);
	}
#endif

#ifdef EPE_MAX_FILLED_QUEUE
	epe_T * epe;
	epe_T * tmpEpe = NULL;
	uint8_t maxFilled = 0;
	uint8_t currFilled;

	epe = GET_FIRST_EPE();
	tmpEpe = epe;
	currFilled = efGetAllocated(epe->evtQ);

	while (epe != NULL) {
		currFilled = efGetAllocated(epe->evtQ);
		if (maxFilled < currFilled) {
			maxFilled = currFilled;
			tmpEpe = epe;
		}
		dispatch(
			epe,
			NULL);		
		epe = epe->next;
	}
	if (tmpEpe != NULL) {
		evt_T * evt;
		
		evt = efRetrieveEvt(tmpEpe->evtQ);		
		dispatch(
			tmpEpe,
			evt);		
	}		
#endif	

#ifdef EPE_ROUND_ROBIN
	epe_T * epe;

	epe = GET_FIRST_EPE();
	while (epe != NULL) {
		if (efIsEvtQEmpty(epe->evtQ) == FALSE) {
			evt_T * evt;
		
			evt = efRetrieveEvt(epe->evtQ);
			dispatch(
			epe,
			evt);
		} else {
			dispatch(
			epe,
			NULL);
		}
		epe = epe->next;
	}
#endif
}

/*----------------------------------------------------------------------------*/
static void stop(
	void) {
	;	
}

/*----------------------------------------------------------------------------*/
static void start(
	void) {

	epe_T * epe;
	
	epe = GET_FIRST_EPE();
	while (epe != NULL) {
		dispatch(
			epe, 
			NULL);
		epe = epe->next;
	}	
	
	eefProcess = schedule;	
}	 
/*___________________________________________  Global Function Definitions  __*/
void efEpCmd(
	epCmd_T cmd) {
	
	switch (cmd) {
		case EP_STOP : {
			eefProcess = stop;
			
			return;
		}
		
		case EP_START : {
			eefProcess = start;
			
			return;
		}
		
		case EP_RESUME : {
			eefProcess = schedule;
			
			return;
		}
		
		default : {
			;
		}		
	}		
}

/*----------------------------------------------------------------------------*/
void efProcessing() {
	
	E_ASSERT(eefProcess != NULL);
	
	eefProcess();
}

/** @} *//*********************************************************************/
