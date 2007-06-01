#include <Network/RL_netapi.h>

static int environment_connection = 0;

void rlSetEnvironmentConnection(int connection) {
  if (environment_connection)
    rlClose(environment_connection);
  
  environment_connection = connection;
}
