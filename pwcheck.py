#!/usr/bin/python3

import argparse
import sys
from itertools import groupby

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('level', type=int)
    parser.add_argument('param', type=int)
    parser.add_argument('--stats', action='store_true')
    return parser.parse_args()

def isspecial(string):
    return not string.isalnum() and 32 <= ord(string) <= 126

def main():
    args = parse_args()

    stats = {'chars': [], 'lengths': []}

    for line in sys.stdin.readlines():
        line = line.rstrip('\n')

        stats['lengths'].append(len(line))
        for char in line:
            if char not in stats['chars']:
                stats['chars'].append(char)

        if args.level >= 1:
            if not any(map(str.isupper, line)) or not any(map(str.islower, line)):
                continue
        if args.level >= 2:
            groups = (int(any(map(str.isupper, line))) +
                      int(any(map(str.islower, line))) +
                      int(any(map(str.isdecimal, line))) +
                      int(any(map(isspecial, line))))
            if groups < args.param and groups != 4:
                continue
        if args.level >= 3:
            groups = [''.join(g) for _, g in groupby(line)]
            if any(len(sequence) >= args.param for sequence in groups):
                continue
        if args.level >= 4:
            substrings = [line[x:x+args.param] for x in range(len(line) + 1 - args.param)]
            found = False
            for x, substring in enumerate(substrings, start=1):
                if substring in line[x:]:
                    found = True
                    break
            if found:
                continue

        print(line)

    if args.stats:
        print("Statistika:")
        print(f"Ruznych znaku: {len(stats['chars'])}")
        print(f"Minimalni delka: {min(stats['lengths'])}")
        print(f"Prumerna delka: {round(sum(stats['lengths']) / len(stats['lengths']), 1)}")


if __name__ == '__main__':
    main()
