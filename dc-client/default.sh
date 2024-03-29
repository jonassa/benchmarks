#!/bin/bash

set -ex

cd /usr/src/memcached/memcached_client/

# Use default load.cfg
./loader \
	-a ../twitter_dataset/twitter_dataset_30x \
	-s servers.txt \
	-c ${CONNECTIONS:=216} -w ${CLIENT_WORKERS:=18} -e -y -t ${RUNTIME:=120} -T ${STATS_INTERVAL:=1}
