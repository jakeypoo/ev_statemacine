#ifdef __cplusplus
extern "C" {
#endif

#ifndef _EV_STATEMACHINE_H_ 
#define _EV_STATEMACHINE_H_ 

#include <stdint.h>
#include <stdlib.h>

#define EVENT_QUEUE_SIZE    32

#define EVENT_NONE          0 

/** State event handler function 
 *  This function will receive and process events as they occur.
 *  Create a handler function for each state in your state machine. 
 */
typedef void ( *p_state_event_handler )( uint8_t event ) ;

/** State machine instance 
 *  Contains information on the current state and a buffer for incoming events. 
 */
typedef struct{
    p_state_event_handler   current_state ;                     /**< pointer to current state handler */
    volatile uint8_t        event_queue[ EVENT_QUEUE_SIZE ] ;   /**< Event FIFO buffer */
    volatile uint8_t        event_count ;                       /**< number of unhandled events in the buffer */
    volatile uint8_t        queue_pos ;                         /**< read position of the event buffer */
} ev_state_machine_t ;

/** Add a new event to the event FIFO buffer
 *  @param  sm      state machine instance      
 *  @param  event   event to add to the buffer
 *
 *  @retval 0       success
 *  @retval 1       the buffer is full 
 */
uint8_t evsm_queue_event( ev_state_machine_t * sm , uint8_t event ) ;

/** Get the next event from the FIFO buffer
 *  @param  sm      state machine instance      
 *
 *  @retval 0       the buffer is empty (EVENT_NONE)
 *  @retval event   the next event if event_count is not zero
 */
uint8_t evsm_get_next_event( ev_state_machine_t * sm ) ;

/** Initialize the state machine instance
 *  @param sm               state machine instance to initialzie 
 *  @param initial_state    event handler function of the starting state 
 */
uint8_t evsm_init( ev_state_machine_t * sm , p_state_event_handler initial_state ) ;

#endif 

#ifdef __cplusplus
} /* extern "C" */
#endif
