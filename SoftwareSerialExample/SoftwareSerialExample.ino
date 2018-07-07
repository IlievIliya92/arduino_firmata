/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 */
 
#include <SoftwareSerial.h>

#include <stdlib.h>
#include "fsm_comm.h"

/*Initialize array of structure with states and event with proper handler */
sStateMachine asStateMachine [] = {
  {Idle_State,Detect_Trigger,init_handler},
  {Data_State,Data_Event,data_handler},
  {Reset_State,Reset_Event,reset_handler},
};

SoftwareSerial mySerial(10, 11); /* RX, TX */

void serial_comm_init (void);

void setup(void) 
{
  serial_comm_init();
  /* Initialize the state machine */
  eSystemState eNextState = Idle_State;

  

  return;
}

void loop(void) 
{ 
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

//Api read the event
  eSystemEvent eNewEvent = read_event();
  if((eNextState < last_State) && (eNewEvent < last_Event)&& (asStateMachine[eNextState].eStateMachineEvent == eNewEvent) && (asStateMachine[eNextState].pfStateMachineEvnentHandler != NULL))
  {
    // function call as per the state and event and return the next state of the finite state machine
    eNextState = (*asStateMachine[eNextState].pfStateMachineEvnentHandler)();
  }
  else
  {
    usage_fsm();
  }

  return;
}

void serial_comm_init (void)
{
   /* Open serial communications and wait for port to open */
  /* Serial Console of the Arduino */
  Serial.begin(57600,SERIAL_8N1);
  while (!Serial) {
    ; /*  wait for serial port to connect. Needed for native USB port only */
  }

  Serial.println("Serial communication has been initialized!");
  
  /* set the data rate for the SoftwareSerial port */
  mySerial.begin(115200);
  mySerial.println("Serial communication has been initialized!");

  return;
}

void usage_fsm (void)
{

  exit(1);
}

eSystemState init_handler(void)
{
  Serial.println("Init Handler");
  return Data_State;
}

eSystemState data_handler(void)
{
  Serial.println("Data Handler");
  return Reset_State;
}

eSystemState reset_handler(void)
{
  Serial.println("Reset Handler");
  return Idle_State;
}

eSystemEvent read_event (void)
{
  eSystemEvent event;
  int user_in = 0;

  switch (user_in)
  {
    case 0:
      event = Detect_Trigger;
      break;
    case 1:
      event = Data_Event;
      break;
    case 2:
      event = Reset_Event;
      break;

    default:
      event = Error_Event;
      usage_fsm();
      break;
  }

  return event;
}

