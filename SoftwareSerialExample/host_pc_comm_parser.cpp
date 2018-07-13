#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SoftwareSerial.h>

#include "host_pc_comm_parser.h"
#include "global_defines.h"

int8_t comm_rcv( char *comm )
{
  int8_t status = 0;

  if (strcmp(comm, "cwrot") == 0 )
  {
    return 1;
  }
  else if ((strcmp(comm, "ccwrot") == 0 ))
  {
    return 2;
  }
  else
  {
    status = -1;
  }
  
  return status;
}

