#!/bin/bash
# set -ex

cd /usr/src/memcached/memcached_client/
# echo "dc-server, 11211" > "servers.txt"

# Use mounted load.cfg
[[ -r /load.cfg ]] && cp -f /load.cfg load.cfg

./loader \
	-a ../twitter_dataset/twitter_dataset_30x \
	-s servers.txt \
	-c ${CONNECTIONS:=450} -w ${CLIENT_WORKERS:=18} -e -y -T ${STATS_INTERVAL:=1}
