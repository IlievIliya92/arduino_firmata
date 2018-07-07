#ifndef   _FSMLT_H_
#define   _FSMLT_H_

typedef enum
{
  Idle_State,
  Data_State,
  Reset_State,
  Error_State,
  last_State,
}eSystemState;

typedef enum
{
  Detect_Trigger,
  Data_Event,
  Reset_Event,
  Error_Event,
  last_Event,
}eSystemEvent;

//typedef of function pointer
typedef eSystemState (*pfEventHandler)(void);

//structure of state and event with event handler
typedef struct
{
  eSystemState eStateMachine;
  eSystemEvent eStateMachineEvent;
  pfEventHandler pfStateMachineEvnentHandler;
}sStateMachine;

void usage_fsm (void);

eSystemState init_handler(void);
eSystemState data_handler(void);
eSystemState reset_handler(void);
eSystemEvent read_event (void);

#endif /* end _FSMLT_H_ */

