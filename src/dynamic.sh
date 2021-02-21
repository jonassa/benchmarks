#!/bin/bash
set -ex

echo "dc-server, 11211" > "/usr/src/memcached/memcached_client/servers.txt"

benchmark_step() {
    local rps=$1

    printf "\n\nRPS: $rps\nTimeout: $STEP_TIME\n =======================\n"
    /usr/src/memcached/memcached_client/loader \
        -a /usr/src/memcached/twitter_dataset/twitter_dataset_30x \
        -s /usr/src/memcached/memcached_client/servers.txt \
        -c $CONNECTIONS -w $CLIENT_WORKERS -e -r $rps -t $STEP_TIME -T $STATS_INTERVAL
    }

if [[ -n $1 ]]; then
    RPS=$1

    STEP_TIME=$(($RUNTIME / 5))
    STATS_INTERVAL=$(($STATS_INTERVAL / 5))

    RPS_LOW=$(($RPS / 4)) # 25 %
    RPS_MID=$(($RPS / 2)) # 50 %
    RPS_HIGH=$RPS         # 100 %

    # Five-step benchmark: 25 -> 50 -> 100 -> 50 -> 25
    RPS_STEPS="$RPS_LOW $RPS_MID $RPS_HIGH $RPS_MID $RPS_LOW"

    for rps in $RPS_STEPS; do
        benchmark_step "$rps" "$STEP_TIME"
    done
else
    bash
fi
