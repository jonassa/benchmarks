#!/bin/python3

import numpy as np
import math
from argparse import ArgumentParser, FileType

parser = ArgumentParser(description='Generate sinusoidal loads for dynamic memcached load generator')
parser.add_argument('name', help='name of the load file to generate', type=str)
parser.add_argument('-t', '--runtime', help='total duration of the run in seconds, and the period of the sine function', type=int, default=600)
parser.add_argument('-a', '--peak-rps', help='peak RPS/range of the sine function ', type=int, default=500000)
parser.add_argument('-b', '--trough-rps', help='trough RPS/range of the sine function ', type=int, default=380000)

args = parser.parse_args()

period = args.runtime
omega = 2 * math.pi / period
amplitude = (args.peak_rps - args.trough_rps) / 2
step = 5

t = np.arange(0, period, step)
r = ((amplitude * np.sin(omega * t + 3*math.pi/2)) + amplitude + args.trough_rps).astype(int)

with open(f'/workspace/memcached/loads/{args.name}.cfg', 'w') as f:
    f.writelines('5,%d\n' % rps for rps in r)