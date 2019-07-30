#!/bin/bash
#./createMasters.sh 7001 6 redis-cluster-m- redis
readonly STARTING_PORT=${1:-0}
readonly NUM_MASTERS=${2:-0}
readonly NAME_PREFIX=${3:-"redis-cluster-m-"}
readonly IMAGE=${4:-"redis"}
for ((port = STARTING_PORT, endPort = port + NUM_MASTERS; port < endPort; port++)) do
  name="$NAME_PREFIX$port"
  docker service create\
    --name "$name"\
    --network visualizer_default\
    --publish "$port:$port"/tcp\
    --restart-condition any\
    --stop-grace-period 60s\
      "$IMAGE"\
        --appendfsync everysec\
        --appendonly yes\
        --auto-aof-rewrite-percentage 20\
        --cluster-enabled yes\
        --cluster-node-timeout 60000\
        --cluster-require-full-coverage no\
        --logfile "$name".log\
        --port "$port"\
        --protected-mode no\
        --repl-diskless-sync yes\
        --save ''''
done
exit 0