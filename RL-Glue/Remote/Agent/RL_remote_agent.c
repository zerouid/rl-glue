#include <Remote/RL_netapi.h>

static int agent_connection = 0;

void rlSetAgentConnection(int connection) {
  if (agent_connection)
    rlClose(agent_connection);
  
  agent_connection = connection;
}
