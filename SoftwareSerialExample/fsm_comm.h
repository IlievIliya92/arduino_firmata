#ifndef _FSMLT_H_
#define _FSMLT_H_

typedef enum
{
  Idle_State,
  Data_State,
  Reset_State,
  last_State,
}eSystemState;

typedef enum
{
  Detect_Trigger_Event,
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
void serial_comm_init (void);

eSystemState trigger_handler(void);
eSystemState data_handler(void);
eSystemState reset_handler(void);
eSystemEvent read_event (void);
eSystemState error_handler(void);

#endif /* end _FSMLT_H_ */

