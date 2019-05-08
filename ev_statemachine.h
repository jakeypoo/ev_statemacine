#ifdef __cplusplus
extern "C" {
#endif

#ifndef _EV_STATEMACHINE_H_ 
#define _EV_STATEMACHINE_H_ 

#include <stdint.h>
#include <stdlib.h>

#define EVENT_QUEUE_SIZE    32

#define EVENT_NONE          0 

typedef void ( *p_state_event_handler )( uint8_t event ) ;

typedef struct{
    p_state_event_handler   current_state ;                     /* pointer to current state handler */
    volatile uint8_t        event_queue[ EVENT_QUEUE_SIZE ] ;   /* Event FIFO buffer */
    volatile uint8_t        event_count ;                       /* number of unhandled events in the buffer */
    volatile uint8_t        queue_pos ;                         /* read position of the event buffer */
} ev_state_machine_t ;

uint8_t evsm_queue_event( ev_state_machine_t * sm , uint8_t event ) ;
uint8_t evsm_get_next_event( ev_state_machine_t * sm ) ;
uint8_t evsm_init( ev_state_machine_t * sm , p_state_event_handler initial_state ) ;

#endif 

#ifdef __cplusplus
} // extern "C"
#endif
