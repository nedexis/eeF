/***************************************************************************//**
 * @file		epe.h
 * @author      Dejan Ristic 
 * @brief       Event processing engine include file
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
#ifndef EPE_H_
#define EPE_H_
/*_________________________________________________________  Include Files  __*/
#include "evt.h"
/*_______________________________________________________________  Defines  __*/
/**@brief			Uncoment this option if you want use scheduling based 
 *						on epe priority
 *@annotation       This option afect event processor (ep)
 *@see				ep.h file
 */
//#define EPE_USE_PRIORITY
/*_______________________________________________________________  Macro's  __*/
/**@brief			This macro is used by the event processor (ep)
 *@note				For internal use only
 */ 
#define GET_FIRST_EPE()														   \
			firstEpe
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif
/*_____________________________________________________________  Data Types  __*/
/**@brief			Forward declaration of event processing engine (epe)
 */
typedef struct epe_tag epe_T;
/**@brief			Forward declaration of epe states
 */
typedef void (* epeStates_T)(evt_T * evt);
/**@brief			Declaration of event processing engine (epe) execution states
 * @descrition		Execution of an epe is divided in four states (phases)
 *						those is declared in @Ref exeStates_tag
 *@note				Those states is used by the event processor (ep) and 
 *						for internal use only	  
 */
typedef enum exeStates_tag {
	EPE_WAIT = 0,
	INIT_TRAN,
	HANDLE_TRAN,
	HANDLE_EVT	
} exeStates_T;
/**@brief			Declaration of event processing engine (epe)
 */
struct epe_tag { 
/**@brief			Next epe in execution linked list 
 */	
	epe_T *			next;
/**@brief			Pointer to queue for storing epe events			
 */		
	evtQ_T *		evtQ;
/**@brief			Current state of epe 
 */			
	epeStates_T 	currState; 			
/**@brief			Previous state of epe internally used for transition
 */				
	epeStates_T 	prevState;
/**@brief			Epe execution states described in @ref exeStates_tag
 */			
	exeStates_T		exeState;
#if defined(EPE_USE_PRIORITY) || defined(__DOXYGEN__)	
/**@brief			Priority of epe used by priority scheduling method
 * @note            There are 256 priority levels, maximal priority is 0
 *						minimal priority is 255
 */		
	uint8_t			priority;
#endif	
};
/*_______________________________________________________  Global Variables  __*/
extern epe_T * firstEpe;
/*____________________________________________________  Function Prototypes  __*/
/**@brief				Epe initialisation
  *@param epe			Pointer to event processing engine (epe)
  *@param evtQ			Pointer to queue for epe events	 
  *@param evtBuff		Pointer to buffer for epe events (used by epe queue)
  *@param evtBuffSize   Size of buffer for epe events expressed 
  *							as the number of elements of a given type
  *@param priority		Epe priority described in @ref epe_tag
  *@param initState		State that will be first executed by event processssor (ep)
  *							after EP_START command described in @ref epCmd_Tag
  *							located in ep.h file
 */
void efEpeInit(
		epe_T *			epe,	
		evtQ_T *		evtQ, 
		evt_T **		evtBuff, 
		size_t			evtBuffsize, 
#if defined(EPE_USE_PRIORITY) || defined(__DOXIGEN__)
		uint8_t			priority,
#endif				
		epeStates_T 	initState);		
/**@brief				Epe deinitialisation
  *@param epe			Pointer to event processing engine (epe)
 */		
void efEpeDeinit(
		epe_T *			epe);
						
/** @} *//**********************************************************************/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------  C++ extern end  --*/
#endif 