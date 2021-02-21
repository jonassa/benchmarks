#!/bin/bash
set -ex
#set -x

echo "dc-server, 11211" > "/usr/src/memcached/memcached_client/servers.txt"

if [[ -n $1 ]]; then
    RPS=$1

    # test -f /usr/src/memcached/twitter_dataset/twitter_dataset_30x || (echo "Scaled dataset not available"; exit)

    printf "\n\nRPS: $RPS\nTimeout: $RUNTIME\n =======================\n"

    # Benchmark
    /usr/src/memcached/memcached_client/loader \
        -a /usr/src/memcached/twitter_dataset/twitter_dataset_30x \
        -s /usr/src/memcached/memcached_client/servers.txt \
        -c $CONNECTIONS -w $CLIENT_WORKERS -e -r $RPS -t $RUNTIME -T $STATS_INTERVAL
else
    bash
fi
