#!/bin/bash

REDIS_CONFIG='port 6379
cluster-enabled yes
cluster-config-file nodes.conf
cluster-node-timeout 5000
appendonly yes'

network=mynet

docker service create --name redis \
  --network $network \
  --replicas=6 \
  -e REDIS_CONFIG="$REDIS_CONFIG" \
  -e REDIS_CONFIG_FILE="/usr/local/etc/redis/redis.conf" \
  redis:3.2.6-alpine sh -c 'mkdir -p $(dirname $REDIS_CONFIG_FILE) && echo "$REDIS_CONFIG" > $REDIS_CONFIG_FILE && cat $REDIS_CONFIG_FILE && redis-server $REDIS_CONFIG_FILE'

sleep 2
docker service ps redis --no-trunc

# run the redis-trib.rb script  (the docker inspect runs on the host and the echo output is passed the along to the ruby container)
docker run -it --rm --net $network ruby sh -c "\
  gem install redis --version 3.2 \
  && wget http://download.redis.io/redis-stable/src/redis-trib.rb \
  && ruby redis-trib.rb create --replicas 1 \
  \$(getent hosts tasks.redis | awk '{print \$1 \":6379\"}') "