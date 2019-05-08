
#include "ev_statemachine.h"


uint8_t evsm_queue_event( ev_state_machine_t * sm , uint8_t new_event ){
    uint8_t i; 
    /* make sure the input queue is not full */
    if(sm->event_count < EVENT_QUEUE_SIZE){
        i = sm->queue_pos + sm->event_count; 
        if( i >= EVENT_QUEUE_SIZE ){
            i -= EVENT_QUEUE_SIZE ;
        }
        sm->event_queue[i] = new_event ;
        sm->event_count++;

        return 0 ;
    }
    return 1 ;
}

uint8_t evsm_get_next_event( ev_state_machine_t * sm ){
    uint8_t next_event ;
    if(sm->event_count == 0){
        /* Input queue is empty */
        next_event = EVENT_NONE ;
    }
    else{
        next_event = sm->event_queue[sm->queue_pos];
        sm->queue_pos++;
        if(sm->queue_pos >= EVENT_QUEUE_SIZE)
            sm->queue_pos = 0 ;
        sm->event_count--;
    }
    return next_event ;
}

uint8_t evsm_init( ev_state_machine_t * sm , p_state_event_handler initial_state ){
    if( initial_state == NULL ){
        return 1 ;
    }

    sm->event_count = 0 ;
    sm->queue_pos   = 0 ;
    sm->current_state = initial_state ;

    return 0 ;
}
