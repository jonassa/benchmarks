#!/bin/bash
set -ex

rps=$1

cd /usr/src/memcached/memcached_client/
# echo "dc-server, 11211" > "servers.txt"

echo "0,0" > load.cfg
echo "${RUNTIME:=60},${rps:=450000}" >> load.cfg

./loader \
	-a ../twitter_dataset/twitter_dataset_30x \
	-s servers.txt \
	-c ${CONNECTIONS:=216} -w ${CLIENT_WORKERS:=18} -e -t $RUNTIME -T ${STATS_INTERVAL:=1}
