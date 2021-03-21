#!/usr/bin/env python
# Author: Yunqi Zhang
# Email: yunqi@umich.edu

import argparse

LOAD_TIMELINE = [
    62,
    68,
    66.5,
    69,
    66,
    63.5,
    55,
    50,
    43,
    48,
    49.5,
    50,
    55,
    62,
    63,
    69,
    71,
    75,
    70,
    72,
    71.5,
    69,
    62,
    60,
    ]
MAX_CAPACITY = 75

def generateLoad(maxload):
  for load in LOAD_TIMELINE:
    current_load = float(maxload) * float(load) / float(MAX_CAPACITY)
    print("60,{0}".format(int(current_load)))
  for load in LOAD_TIMELINE:
    current_load = float(maxload) * float(load) / float(MAX_CAPACITY)
    print int(current_load),

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument("-m", "--maxload",
                      help="Maximal amount of throughput")
  args = parser.parse_args()
  if not args.maxload:
    print("Please specify the maximal amount of throughput")
    exit(0)

  generateLoad(int(args.maxload))

if __name__ == "__main__":
  main()
