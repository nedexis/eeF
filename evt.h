/***************************************************************************//**
 * @file		evt.h
 * @author      Dejan Ristic 
 * @brief       Events Include file
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
#ifndef EVT_H_
#define EVT_H_
/*_________________________________________________________  Include Files  __*/
#include "port.h"
#include <stdint.h>
#include <stddef.h>
/*_______________________________________________________________  Defines  __*/
/**@brief			Basic System signals that causes predefined actions				  
 * @description		Event processor (EP) send these signals to event 
 *						processing engine (EPE) in order to fulfill ENTRY,
 *						EXIT and INIT actions
 *@note				EPE uses these signals to detect the basic actions,
 *						but EPE must not use these signals to be sent 
 *						to another EPE 
 */
#define SIG_ENTRY	sigEntry
#define SIG_EXIT	sigExit
#define SIG_INIT	sigInit
/*_______________________________________________________________  Macro's  __*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif
/*_____________________________________________________________  Data Types  __*/
//typedef void evt_T;	

 typedef struct evt_tag {
 	uint8_t		id;	
} evt_T;

/**@brief			Declaration of system predefined signal type
 */ 
typedef uint32_t sysSig_T;
 /**@brief			Declaration of event queue (EQ) used for storing event
  */
typedef struct evtQ_tag {
/**@brief			Pointer to buffer which is previously 
  *						statically or dinamically allocated in memory
  */	
	evt_T **	buff;
/**@brief size		Size of buffer for EPE events expressed
 *						as the number of elements of a given type 
 */	
	uint8_t		size;
/**@brief			Index that point to top of EQ buffer which is realised 
 *						as circular buffer
 */	
	uint8_t 	head;
/**@brief			Index that point to bottom of EQ buffer which is realised 
 *						as circular buffer
 */		
	uint8_t		tail;
/**@brief			Number of elements currently stored in buffer
 */		
	uint8_t		count;
} evtQ_T;
/*_______________________________________________________  Global Variables  __*/
extern evt_T * sigEntry;
extern evt_T * sigExit;
extern evt_T * sigInit;
/*____________________________________________________  Function Prototypes  __*/
/**@brief				Initialise event queue (EQ) used for storing events 
 * @param evtQ			Pointer to EQ that can be statically or dinamically allocated
 * @param evtBuff		Pointer to buffer used by EQ, described in @ref evtQ_tag
 * @param evtBuffSize	Size of buffer, described in @ref evtQ_tag  			
 */
void efEvtQInit(
		evtQ_T *	evtQ, 		
		evt_T **	evtBuff, 
		uint8_t		evtBuffSize);
/**@brief			Sends event to EPE
 *@description      This function check EQ of an EPE, 
 *						if EQ is not full function insert event an event
 *@param evtQ       Pointer to EQ of an EPE
 *@param evt		Event that will be sent
 *@retval			TRUE if event successfully sent, FALSE if not
 */		
bool_T efSendEvt(
		evtQ_T *	evtQ, 
		evt_T *		evt);
/**@brief			Sends event to EPE
 *@description      This function check EQ of an EPE, 
 *						if EQ is not full function insert event an event
 *@param evtQ       Pointer to EQ of an EPE
 *@param evt		Event that will be sent
 *@retval			TRUE if event successfully sent, FALSE if not
 *@annotation		This function performs atomic access to the EQ
 */
bool_T efSendEvtI(
		evtQ_T *	evtQ,
		evt_T *		evt);
/**@brief			Insert event to EPE
 *@param evtQ       Pointer to EQ of an EPE
 *@param evt		Event that will be insert
 *@annotation		This function is used by the @ref efSendEvt and @ref efSendEvtI
 *						user can use this function frelly, but this function not perform
 *						check fullness of EQ, and atomic acces to EQ
 *							
 */	
void efInsertEvt(
		evtQ_T *	evtQ,
		evt_T *		evt);
/**@brief			Retreive event from EQ 
 *@param evtQ       Pointer to EQ of an EPE
 *@note				This function is for internally use only
 */		
evt_T * efRetrieveEvt(
		evtQ_T *	evtQ);	
/**@brief			Perform check emptiness of an EQ
 *@param evtQ       Pointer to EQ of an EPE
 *@retval			TRUE if EQ is empty, FALSE if not
 */				
bool_T efIsEvtQEmpty(
		evtQ_T *	evtQ);			
/**@brief			Perform check fullness of an EQ
 *@param evtQ       Pointer to EQ of an EPE
 *@retval			TRUE if EQ is full, FALSE if not
 */				
bool_T efIsEvtQFull(
		evtQ_T *	evtQ);
/**@brief			Get fullness of an EQ expresed as the number of 
 *						elements of a given type
 *@param evtQ       Pointer to EQ of an EPE
 *@retval			Number of existing elements in EQ
 */						
uint8_t efGetAllocated(
		evtQ_T *	evtQ);
/**@brief			Get emptyness of an EQ expresed as the number of 
 *						elements of a given type
 *@param evtQ       Pointer to EQ of an EPE
 *@retval			Number of elements that can be inserted in EQ
 */								
uint8_t efGetFree(
		evtQ_T *	evtQ);								
/** @} *//**********************************************************************/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------  C++ extern end  --*/
#endif 