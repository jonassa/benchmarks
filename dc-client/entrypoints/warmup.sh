#!/bin/bash
# set -ex
# TODO: warmup does not work (exits immediately with -ex)

echo "Warming up the server"

cd /usr/src/memcached/memcached_client/

./loader \
	-a ../twitter_dataset/twitter_dataset_30x \
	-s servers.txt \
    -c ${CONNECTIONS:=450} -w ${CLIENT_WORKERS:=18} -e -D ${SERVER_MEMORY:=65442} -j -T 1000 \

echo "Warmup completed"

