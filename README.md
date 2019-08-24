
### Overview 

Event-Driven State Machines are useful in applications that sit around waiting for something to happen before taking action.  
The need for a state machine usually comes from the need to respond to an input/event differntly depending on the current context (state) of the application.

This library and code template is useful for creating an organized state machine implementation in C that's easy to maintain.

### Using this library

It's best to start with your state machine organized into a table that outlines what action to take when an event occurs in each state. This should also include state changes.

|  | EVENT 1 | EVENT 2 | EVENT 3 |   
|---|---|---|---|  
| **STATE X** | ACTION A ; new state = Y | ACTION B ; new state = Z  | ACTION C ; no state change |  
| **STATE Y** | ACTION D ; new state = X | ACTION B ; new state = Z | no action ; no state change |
| **STATE Z** | ACTION D ; new state = X | no action ; no state chagne | ACTION A : new state = Y |  

The library itself doesn't have much to it, just folow this template to implement your state machine.

```c

ev_state_machine_t my_sm ;  /* create a state machine instance */

/* define the events */
typedef enum {
  EVENT_0 = EVENT_NONE ,  /* used by the library functions */
  EVENT_1 , 
  EVENT_2 , 
  EVENT_3 
} state_machine_events_t ;

/* create an event handler function for each state */
void state_x( uint8_t event ) ;
void state_y( uint8_t event ) ;
void state_z( uint8_t event ) ;

/* create the handler functions to follow the table above */
void state_x( uint8_t event ){
  switch( event ){
    case EVENT_1 :
      action_A() ;
      my_sm.current_state = state_y ;
      break; 
    case EVENT_2 : 
      action_B() ;
      my_sm.current_state = state_z ;
      break; 
    case EVENT_3 : 
      action_C() ;
      break; 
  }
}

void state_y( uint8_t event ){
  switch( event ){
    case EVENT_1 :
      action_D() ;
      my_sm.current_state = state_x ;
      break; 
    case EVENT_2 : 
      action_B() ;
      my_sm.current_state = state_z ;
      break; 
  }
}


void state_z( uint8_t event ){
  switch( event ){
    case EVENT_1 :
      action_D() ;
      my_sm.current_state = state_x ;
      break; 
    case EVENT_3 : 
      action_A() ;
      my_sm.current_state = state_y ;
      break; 
  }
}
```
Pass events to the state machine using the `evsm_queue_event` function.  
```c
  evsm_queue_event( &my_sm , (uint8_t)( EVENT_1 )) ;  
```
Run the state machine by passing the events to the current state's event handler.
```c 
evsm_init( &my_sm , state_x ) ; /* start the state machine in STATE X */

while(1){
  if( my_sm.event_count > 0 ){
    (my_sm.current_state)( evsm_get_next_event( &my_sm )) ; 
  }
}
```

### Arduino Example: Blinking LED

See `example_arduino/` for an example that implements the following state machine 

|  | Button Press | Timeout |   
|---|---|---|  
| **LED Off** | Turn On LED | Turn Off LED  |  
| **LED On** | Blink LED | Turn Off LED |
| **LED Blink** | Turn On LED | do nothing |  
