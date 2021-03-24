#!/bin/bash
# set -ex

RPS=$1

cd /usr/src/memcached/memcached_client/

# Benchmark with no timeout
./loader \
	-a ../twitter_dataset/twitter_dataset_30x \
	-s servers.txt \
	-c ${CONNECTIONS:=216} -w ${CLIENT_WORKERS:=18} -e -r ${RPS:=450000} -T ${STATS_INTERVAL:=600}
