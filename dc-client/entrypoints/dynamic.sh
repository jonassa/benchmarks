#!/bin/bash
set -ex

cd /usr/src/memcached/memcached_client/
# echo "dc-server, 11211" > "servers.txt"

# for i in $(seq $N_ROWS); do
#     echo "DYNAMIC RPS: $rps"
# done

# Use mounted load.cfg
[[ -r /load.cfg ]] && cp -f /load.cfg load.cfg

./loader \
	-a ../twitter_dataset/twitter_dataset_30x \
	-s servers.txt \
	-c ${CONNECTIONS:=216} -w ${CLIENT_WORKERS:=18} -e -t ${RUNTIME:=120} -T ${STATS_INTERVAL:=1}
