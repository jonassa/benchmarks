#!/bin/bash

RPS=$1

cd /usr/src/memcached/memcached_client/

./loader \
	-a ../twitter_dataset/twitter_dataset_30x \
	-s servers.txt \
	-c ${CONNECTIONS:=216} -w ${CLIENT_WORKERS:=18} -e -r ${RPS:=450000} -t ${RUNTIME:=60} -T ${RUNTIME:=60}
