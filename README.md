
### Overview 

Event-Driven State Machines are useful in applications that sit around waiting for something to happen before taking action.  
The need for a state machine usually comes from the need to respond to an input/event differntly depending on the current context (state) of the application.

This library and code template is useful for creating an organized state machine implementation in C that's easy to maintain.

### Using this library

It's best to start with your state machine organized into a table that outlines what action to take when an event occurs in each state. This should also include state changes.

  | EVENT 1 | EVENT 2 | EVENT 3 
---|---|---|---
STATE X | ACTION A ; new state = Y | ACTION B ; new state = Z  | ACTION C ; no state change
STATE Y | ACTION D ; new state = X | ACTION B ; new state = Z | no action ; no state change 
STATE Z | ACTION D ; new state = X | no action ; no state chagne | ACTION A : new state = Y 

####

The library is 

```c

```
