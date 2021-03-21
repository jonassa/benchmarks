FROM ubuntu:16.04
LABEL maintainer="Jonas Sandberg <jonas.sandb@gmail.com>"

# add our user and group first to make sure their IDs get assigned consistently, regardless of whatever dependencies get added
RUN groupadd -r memcache && useradd -r -g memcache memcache

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
	libevent-2.0-5 \
	vim \
	&& rm -rf /var/lib/apt/lists/*

RUN buildDeps='curl gcc libc6-dev libevent-dev make' \
	&& set -x \
	&& apt-get update && apt-get install -y --no-install-recommends $buildDeps \
	&& rm -rf /var/lib/apt/lists/*

# COPY memcached_dyn.tar.gz /memcached_dyn.tar.gz

RUN mkdir -p /usr/src/memcached
COPY memcached_dyn/twitter_dataset /usr/src/memcached/twitter_dataset
COPY twitter_dataset_30x /usr/src/memcached/twitter_dataset/twitter_dataset_30x
COPY memcached_dyn/memcached_client /usr/src/memcached/memcached_client

RUN cd /usr/src/memcached/memcached_client \
	&& make \
	&& chown -R memcache:memcache /usr/src/memcached
	# && apt-get purge -y --auto-remove $buildDeps

# RUN buildDeps='curl gcc libc6-dev libevent-dev make' \
# 	&& set -x \
# 	&& apt-get update && apt-get install -y --no-install-recommends $buildDeps \
# 	&& rm -rf /var/lib/apt/lists/* \
# 	&& mkdir -p /usr/src/memcached \
# 	&& tar -xzf memcached_dyn.tar.gz -C /usr/src/memcached --strip-components=1 \
# 	&& rm memcached_dyn.tar.gz \
# 	&& cd /usr/src/memcached/memcached_client \
# 	&& make \
# 	&& chown -R memcache:memcache /usr/src/memcached \
# 	&& apt-get purge -y --auto-remove $buildDeps

COPY default.sh /default.sh
USER memcache
CMD /default.sh
