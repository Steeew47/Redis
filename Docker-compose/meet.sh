#!/bin/bash
#./meet.sh 7001 6 redis-cluster-m-
readonly STARTING_PORT=${1:-0}
readonly NUM_MASTERS=${2:-0}
readonly NAME_PREFIX=${3:-"redis-cluster-m-"}
readonly LOCAL_CONTAINER_ID=$(docker ps -f name="$NAME_PREFIX" -q | head -n 1)
readonly LOCAL_PORT=$(docker inspect --format='{{index .Config.Labels "com.docker.swarm.service.name"}}' "$LOCAL_CONTAINER_ID" | sed 's|.*-||')
# From local service task, meet all other nodes
for ((port = STARTING_PORT, endPort = STARTING_PORT + NUM_MASTERS; port < endPort; port++)) do
  if [ "$LOCAL_PORT" == $port ]; then
    continue
  fi
  meet_node="$NAME_PREFIX$port"
  meet_ip=$(docker service inspect -f '{{(index .Endpoint.VirtualIPs 1).Addr}}' "$meet_node" | sed 's|/.*||')
  echo "CLUSTER MEET $meet_ip $port"
  docker exec -it "$LOCAL_CONTAINER_ID"\
    redis-cli -p "$LOCAL_PORT"\
      CLUSTER MEET "$meet_ip" "$port"
done
docker exec -it "$LOCAL_CONTAINER_ID"\
  redis-cli -p "$LOCAL_PORT"\
    CLUSTER NODES
exit 0