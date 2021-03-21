#!/bin/bash
#set -ex
set -x

echo "dc-server, 11211" > "/usr/src/memcached/memcached_client/servers.txt"

echo "Warming up the server"

/usr/src/memcached/memcached_client/loader \
    -a /usr/src/memcached/twitter_dataset/twitter_dataset_30x \
    -s /usr/src/memcached/memcached_client/servers.txt \
    -w $CLIENT_WORKERS -S 1 -c $CONNECTIONS -D $SERVER_MEMORY -j -T 1000 \
    #> /dev/null 2>&1

echo "Warmup completed"

