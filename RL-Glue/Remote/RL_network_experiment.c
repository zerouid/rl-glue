#include <Network/RLnet.h>

static int experiment_connection = 0;

void rlSetExperimentConnection(int connection) {
  if (experiment_connection)
    rlClose(experiment_connection);
  
  experiment_connection = connection;
}
