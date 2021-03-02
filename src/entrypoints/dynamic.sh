#!/bin/bash
# set -ex

echo "dc-server, 11211" > "/usr/src/memcached/memcached_client/servers.txt"

if [[ -n $1 ]]; then
    rps=$1

    echo "RPS: $rps"
    echo "Timeout: $STEP_TIME"
    echo "======================="

    # for i in $(seq $N_ROWS); do
    #     echo "DYNAMIC RPS: $rps"
    # done

    /usr/src/memcached/memcached_client/loader \
        -a /usr/src/memcached/twitter_dataset/twitter_dataset_30x \
        -s /usr/src/memcached/memcached_client/servers.txt \
        -c $CONNECTIONS -w $CLIENT_WORKERS -e -r $rps -t $STEP_TIME -T $STEP_INTERVAL
else
    bash
fi
