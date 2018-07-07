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

/* Global defines*/
//#define LOOPBACKTEST
//#define DEBUG_PRINT
#define MAX_USER_INPUT    20

/*Initialize array of structure with states and event with proper handler */
sStateMachine asStateMachine [] = {
  {Idle_State,Detect_Trigger_Event,trigger_handler},
  {Data_State,Data_Event,data_handler},
  {Reset_State,Reset_Event,reset_handler},
  {Idle_State,Error_Event,error_handler},
  {Data_State,Error_Event,error_handler},
  {Reset_State,Error_Event,error_handler},
};

/*Global Variables */
SoftwareSerial mySerial(10, 11); /* RX, TX */
eSystemState eNextState;
char tranfer_buffer[MAX_USER_INPUT] = {0};
uint8_t buff_idx = 0;

void setup(void) 
{
  serial_comm_init();
  /* Initialize the state machine */
  eNextState = Idle_State;
  usage_fsm();
  return;
}

void loop(void) 
{ 
#ifdef LOOPBACKTEST
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
#endif
  
  //Api read the event
  eSystemEvent eNewEvent = read_event();

  if((eNextState < last_State) && (eNewEvent < last_Event)&& (asStateMachine[eNextState].eStateMachineEvent == eNewEvent) && (asStateMachine[eNextState].pfStateMachineEvnentHandler != NULL))
  {
    // function call as per the state and event and return the next state of the finite state machine
    eNextState = (*asStateMachine[eNextState].pfStateMachineEvnentHandler)();
  }
  else
  {
    ;
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
  String info_header;
  
  info_header = String (__FUNCTION__);
  info_header = "[" + info_header + "]" "\t";
  
  mySerial.print(info_header);
  mySerial.println("Allowed sequence: t (trigger event) - d (data event) - r (reset event) Options: h - for help");
}

eSystemState trigger_handler(void)
{ 
  mySerial.println(__FUNCTION__);
  return Data_State;
}

eSystemState data_handler(void)
{
  uint32_t i = 0;
  
  const String awaiting_for_data_msgs[4] = {
    "Awaiting for datA",
    "Awaiting for daTa",
    "Awaiting for dAta",
    "Awaiting for Data",
  };

  const String gathering_data_msgs[4] = {
    "Gathering datA",
    "Gathering daTa",
    "Gathering dAta",
    "Gathering Data",
  };
  
  mySerial.println(__FUNCTION__);

  while ( buff_idx < MAX_USER_INPUT )
  {

    mySerial.print("\033[2J");
    mySerial.println(awaiting_for_data_msgs[i]);
    i++;
    if (i > 3)
    {
      i = 0;
    }

    delay(200);

    if (mySerial.available()) 
    {
      tranfer_buffer[buff_idx] = mySerial.read();
      buff_idx++;

      for (i = 0 ; i < 4 ; ++i)
      {
        mySerial.print("\033[2J");
        mySerial.print(gathering_data_msgs[i]);
        delay(200);
      }
      i = 0;
    }
  }

  mySerial.print("\033[2J");
  
  while (buff_idx > 0)
  {
    buff_idx--;
    mySerial.write(tranfer_buffer[buff_idx]);
  }
  
  mySerial.println();
  delay(100);

  buff_idx = 0;

  return Reset_State;
}

eSystemState reset_handler(void)
{
  mySerial.println(__FUNCTION__);
    
  return Idle_State;
}

eSystemState error_handler(void)
{
  return Idle_State;
}

eSystemEvent read_event (void)
{
  uint8_t user_in = 0;
  eSystemEvent event;

  if (mySerial.available()) {
    user_in = mySerial.read();
#ifdef DEBUG_PRINT
    mySerial.println(user_in);
#endif
  }
  
 
  switch (user_in)
  {
    case 't':
      event = Detect_Trigger_Event;
      break;
    case 'd':
      event = Data_Event;
      break;
    case 'r':
      event = Reset_Event;
      break;
    case 'h':
      usage_fsm();
      break;
    default:
      event = Error_Event;
      break;
  }

  return event;
}

