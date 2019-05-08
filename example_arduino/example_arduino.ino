#include "ev_statemachine.h"

#define LED_OFF()   digitalWrite( LED_BUILTIN , LOW ) 
#define LED_ON()    digitalWrite( LED_BUILTIN , HIGH ) 
#define LED_BLINK_START()   tone( LED_BUILTIN , 31 ) 
#define LED_BLINK_STOP()    noTone( LED_BUILTIN ) 

#define TIMEOUT_MS  3000 
uint32_t timeout_ticks ; 

/** State Machine Section **/
ev_state_machine_t sm_led ;

/* define events to be processed by the state machine */
typedef enum{
    sm_event_none = EVENT_NONE ,    /* returned by evsm_get_next_event if the buffer is empty */
    sm_event_button_press , 
    sm_event_timeout
} state_machine_events_t ;

/* define states using handler function prototypes */
void State_LED_Off( uint8_t event );
void State_LED_On( uint8_t event );
void State_LED_Blink( uint8_t event );

/* Event handler for LED Off State 
 *  [ Event ]        [ Action ]
 * -> Button Press  : Change state to LED ON
 * -> Timeout       : Nothing
 */
void State_LED_Off( uint8_t event ){
    switch( event ){
        case sm_event_button_press :
            LED_ON();
            timeout_ticks = millis() ;                  /* Refresh the timeout timer */
            sm_led.current_state = State_LED_On;        /* State Transition */
            break;
        default:
            /* do nothing for other events */
            break;
    }
}

/* Event handler for LED On State 
 *  [ Event ]        [ Action ]
 * -> Button Press  : Change state to LED Blink 
 * -> Timeout       : Change state to LED Off 
 */
void State_LED_On( uint8_t event ){
    switch( event ){
        case sm_event_button_press :
            timeout_ticks = millis() ;                  /* Refresh the timeout timer */
            sm_led.current_state = State_LED_Blink ;    /* State Transition */
            break ;
        case sm_event_timeout : 
            LED_OFF() ; 
            sm_led.current_state = State_LED_Off ;      /* State Transition */
            break ;
        default :
            break ;
    }
}

/* Event handler for Blinking LED State 
 *  [ Event ]        [ Action ]
 * -> Button Press  : Change state to LED Off 
 * -> Timeout       : Change state to LED Off 
 */
void State_LED_Blink( uint8_t event ){
    switch( event ){
        case sm_event_button_press :
        case sm_event_timeout : 
            LED_OFF() ;
            sm_led.current_state = State_LED_Off ;      /* State Transition */
            break ;
        default :
            break ; 
    }
}

/* Interrupt routine called when the button is pressed */
void Button_ISR(){
    evsm_queue_event( &sm_led , (uint8_t)( sm_event_button_press )) ;
}

void check_timeout( void ){
    if( TIMEOUT_MS < ( millis() - timeout_ticks )){
        timeout_ticks = millis() ;
        evsm_queue_event( &sm_led , (uint8_t)( sm_event_timeout )) ;
    }
}

/* Crude LED blink function */
void blink_led( void ){
    static uint32_t blink_ticks = 0 ;
    
    if( 125 < ( millis() - blink_ticks )){
        blink_ticks = millis() ;
        if( HIGH == digitalRead( LED_BUILTIN )){
            digitalWrite( LED_BUILTIN , LOW );
        }
        else{
            digitalWrite( LED_BUILTIN , HIGH ) ;
        }
    }
}

void setup() {
    /* initialize the state machine */
    evsm_init( &sm_led , State_LED_Off ) ;

    /* initialize the LED */
    pinMode( LED_BUILTIN , OUTPUT ) ;
    LED_OFF() ;

    /* initialize the button */
    pinMode( 2 , INPUT_PULLUP ) ;
    pinMode( 4 , OUTPUT ) ;
    digitalWrite( 4 , LOW ) ;   /* this is our button GND */

    timeout_ticks = millis() ;

    attachInterrupt( digitalPinToInterrupt(2) , Button_ISR , FALLING ) ;
}

void loop() {
    /* Check if any new events are ready */
    if( sm_led.event_count > 0 ){
        /* Call the current state event handler with the next queued event */
        (sm_led.current_state)( evsm_get_next_event( &sm_led )) ; 
    }

    /* Handle other tasks */
    check_timeout() ;

    if( State_LED_Blink == sm_led.current_state ){
        blink_led() ;
    }
}


