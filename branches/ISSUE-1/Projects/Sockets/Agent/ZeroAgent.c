#include <stdlib.h>
#include <stdio.h>
#include "ZeroAgent.h"

Action theAction;
Observation theObservation;

void agent_init(Task_specification task_spec)
{
  /*  fprintf(stderr, "%s agent_init\n", __FILE__); */

  theAction.numInts = 1;
  theAction.numDoubles = 0;
  theAction.intArray = (int*)calloc(theAction.numInts, sizeof(int));
  theAction.doubleArray = 0;
}

Action agent_start(Observation o)
{
  /* fprintf(stderr, "%s agent_start\n", __FILE__); */

  theObservation = o;
  theAction.intArray[0] = rand();
  return theAction;
}

Action agent_step(Reward r, Observation o)
{
  /* fprintf(stderr, "%s agent_step\n", __FILE__); */

  theObservation = o;
  theAction.intArray[0] = rand();
  return theAction;
}

void agent_end(Reward r)
{
  /* fprintf(stderr, "%s agent_end\n", __FILE__); */
}

void agent_cleanup()
{
  /* fprintf(stderr, "%s agent_cleanup\n", __FILE__); */
  free (theAction.intArray);
}
