# Memcached benchmarks

## Requirements
- Docker
- Docker images: cloudsuite/data-caching, jonassa/benchmarks
- Intel Platform Resource Manager
- numactl
- moreutils
- jq

## Running benchmarks interactively
This starts the memcached server and a client workload generator, but does nothing else

`client`: use the default.sh script in the client (default dynamic load, 120 seconds)
`client dynamic <load>`: use a dynamic load specified by <load>
`client static <rps>`: benchmark at static <rps> for 60 seconds
`client extended <rps>`: benchmark at static <rps> with no timeout
`client shell`: run the client interactively with bash
`client mount`: run the client interactively with bash, also mount entrypoints dir and default load

## Running batches and processing output

Specify options in `config/<name>.cfg`.
Run a batch with `batch_run <name>`.
Overwrite `output/name` if it exists with `batch_run -f <name>`.
Skip warmup with `batch_run -n <name>`

