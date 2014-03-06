/***************************************************************************//**
 * @file		evt.c
 * @author      author 
 * @brief       Events implementation file
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
 * @addtogroup  group 
 *********************************************************************//** @{ */
 
/*_________________________________________________________  Include Files  __*/
#include "evt.h"
/*_________________________________________________________  Local Defines  __*/
/*_________________________________________________________  Local Macro's  __*/
/*______________________________________________________  Local Data Types  __*/
static const uint32_t sigEntryValue = 0xFFFFFFFE;
static const uint32_t sigExitValue = 0xFFFFFFFE;
static const uint32_t sigInitValue = 0xFFFFFFFE;
/*_______________________________________________________  Local Variables  __*/
/*______________________________________________________  Global Variables  __*/
evt_T * sigEntry = (evt_T *)&sigEntryValue;
evt_T * sigExit = (evt_T *)&sigExitValue;
evt_T * sigInit = (evt_T *)&sigInitValue;
/*_____________________________________________  Local Function Prototypes  __*/
/*____________________________________________  Local Function Definitions  __*/
/*___________________________________________  Global Function Definitions  __*/
void efEvtQInit(
	evtQ_T *	evtQ, 
	evt_T **	evtBuff, 
	uint8_t		evtBuffSize) {
			
	evtQ->buff =  evtBuff;
	evtQ->size =  evtBuffSize;
	evtQ->head =  0U;
	evtQ->tail =  0U;
	evtQ->count = 0U;	
}

/*----------------------------------------------------------------------------*/
bool_T efSendEvt(
	evtQ_T *	evtQ, 
	evt_T *		evt) {
	
	E_PORT_DIS_INT();
		
	if (TRUE == efIsEvtQFull(evtQ)) {
	
		E_PORT_ENA_INT();
				
		return FALSE;
			
	} else {		
		efInsertEvt(evtQ, evt);
		
		E_PORT_ENA_INT();
		
		return TRUE;
	}
			
}

/*----------------------------------------------------------------------------*/
bool_T efSendEvtI(
	evtQ_T *	evtQ,
	evt_T *		evt) {

	if (TRUE == efIsEvtQFull(evtQ)) {
		
		return FALSE;
		
	} else {
		efInsertEvt(evtQ, evt);
		
		return TRUE;
	}
}

/*----------------------------------------------------------------------------*/
void efInsertEvt(
	evtQ_T *	evtQ,
	evt_T *		evt) {

	E_ASSERT(evtQ != NULL);
	E_ASSERT(evt != NULL);

	evtQ->buff[evtQ->head] = evt; 
	evtQ->head++;
	if (evtQ->head == evtQ->size) {
		evtQ->head = 0U;
	}
	evtQ->count++;
	
	E_ASSERT(evtQ->count < evtQ->size);
}

/*----------------------------------------------------------------------------*/
evt_T * efRetrieveEvt(
	evtQ_T *	evtQ) {
		
	evt_T * evt;

	E_PORT_DIS_INT();
	E_ASSERT(evtQ != NULL);
	
	evt = evtQ->buff[evtQ->tail];
	evtQ->tail++;	
	if (evtQ->tail == evtQ->size) {
		evtQ->tail = 0U;
	}
	evtQ->count--;
	
	E_ASSERT(((int)evtQ->count) > 0U);
	E_PORT_ENA_INT();
	
	return (evt);
}

/*----------------------------------------------------------------------------*/
bool_T efIsEvtQEmpty(
	evtQ_T * 	evtQ) {
		
	E_PORT_DIS_INT();	
	E_ASSERT(evtQ != NULL);
		
	if (0U == evtQ->count) {
		E_PORT_ENA_INT();
		
		return TRUE;
		
	} else {
		E_PORT_ENA_INT();
		
		return FALSE;
	}	
}

/*----------------------------------------------------------------------------*/
bool_T efIsEvtQFull(
	evtQ_T *	evtQ) {
	
	E_PORT_DIS_INT();
	E_ASSERT(evtQ != NULL);
	
	if (1U == (evtQ->size - evtQ->count)) {
		E_PORT_ENA_INT();
		
		return TRUE;
		
	} else {
		E_PORT_ENA_INT();
		
		return FALSE;
	}
}

/*----------------------------------------------------------------------------*/
uint8_t efGetAllocated(
	evtQ_T *	evtQ) {
	
	E_PORT_DIS_INT();
	E_ASSERT(evtQ != NULL);
	
	uint8_t all = evtQ->count;
	
	E_PORT_ENA_INT();
	
	return all;
}

/*----------------------------------------------------------------------------*/
uint8_t efGetFree(
	evtQ_T *	evtQ) {
		
	uint8_t space;
	
	E_PORT_DIS_INT();
	E_ASSERT(evtQ != NULL);
	
	space = (evtQ->size - 1U) - evtQ->count; 
	
	E_PORT_ENA_INT();
	
	return (space);
}
/** @} *//*********************************************************************/
