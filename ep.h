/***************************************************************************//**
 * @file		ep.h
 * @author      Dejan Ristic 
 * @brief       Event Processor include file
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
#ifndef EP_H_
#define EP_H_
/*_________________________________________________________  Include Files  __*/
#include "epe.h"
/*_______________________________________________________________  Defines  __*/
/**@brief			Choose one of the strategies for scheduling
 */
//#define EPE_MAX_PRIORITY								
//#define EPE_MAX_FILLED_QUEUE
#define EPE_ROUND_ROBIN

/**@brief			If this option commented an EPE will not make an exit action 
 *						during transition
 *@annotation		This feature speeds up the processing of events
 */ 	
//#define USE_SIG_EXIT
/*_______________________________________________________________  Macro's  __*/			
/**@brief			This macro switches betwen two states
 * @param epe       Pointer to current event processing engine (epe) 
 * @param nextState State in which the transition should lead us  
 * @note			With atomic data access
 */
#define EF_EP_TRAN(epe, nextState)											   \
			E_PORT_DIS_INT();												   \
			(epe)->currState = nextState;									   \
			E_PORT_ENA_INT()	
			
/**@brief			This macro switches betwen two states
 * @param epe       Pointer to current event processing engine (epe) 
 * @param nextState State in which the transition should lead us  
 * @note			For use in interrupt rutines
 */
#define EF_EP_TRAN_I(epe, nextState)										   \
			(epe)->currState = nextState									   
			
			
#if (defined(EPE_MAX_PRIORITY) && defined(EPE_MAX_FILLED_QUEUE)) ||			   \
	(defined(EPE_MAX_PRIORITY) && defined(EPE_ROUND_ROBIN)) ||				   \
	(defined(EPE_MAX_FILLED_QUEUE) && defined(EPE_ROUND_ROBIN))
# error "you may choose just one scheduling strategy, option provided in this file"
#endif

#if defined(EPE_MAX_PRIORITY) && (!defined(EPE_USE_PRIORITY))
# error if "you choose EPE_MAX_PRIORITY you must uncoment EPE_USE_PRIORITY it is located in epe.h file"
#endif
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif
/*_____________________________________________________________  Data Types  __*/
/**@brief			Basic commands of event processor (ep)                  
 */ 
typedef enum epCmd_tag {
/**@brief			Immediately stops event processing (epe execution)
  */
	EP_STOP = 0U,
/**@brief			Starts event processing 
 */	
	EP_START,
/**@brief			Resume event processing, use this in conjunction with STOP command			
 */		
	EP_RESUME
} epCmd_T;
/*_______________________________________________________  Global Variables  __*/
/*____________________________________________________  Function Prototypes  __*/
/**@brief			Control event processing
 * @param cmd       Commands described in @ref epCmd_Tag
 *  @arg			EP_STOP			  
 *	@arg			EP_START
 *	@arg			EP_RESUME
 */ 
void efEpCmd(
	epCmd_T cmd);
/**@brief			The main function of event processing 
 * @note			This function need to be placed in main loop
 */	
void efProcessing();
/** @} *//**********************************************************************/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------  C++ extern end  --*/
#endif 