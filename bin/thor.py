#!/usr/bin/env python3

# Used for "hammering" the web server with requests

import multiprocessing
import os
import requests
import sys
import time

# Globals

PROCESSES = 1
REQUESTS  = 1
VERBOSE   = False
URL       = None

# Functions

def usage(status=0):
    print('''Usage: {} [-p PROCESSES -r REQUESTS -v] URL
    -h              Display help message
    -v              Display verbose output

    -p  PROCESSES   Number of processes to utilize (1)
    -r  REQUESTS    Number of requests per process (1)
    '''.format(os.path.basename(sys.argv[0])))
    sys.exit(status)

def do_request(pid):
    ''' Perform REQUESTS HTTP requests and return the average elapsed time. '''
    times = []
    for r in range(REQUESTS):
        t_begin = time.time()
        response = requests.get(URL)
        t_end = time.time()
        if (VERBOSE):
            print(response.text)
        print("Process: {}, Request: {}, Elapsed Time: {}".format(pid, r, round((t_end - t_begin), 2)))
        times.append(t_end - t_begin)

    average = sum(times) / len(times)
    print("Process: {}, AVERAGE:  , Elapsed Time: {}".format(pid, round(average, 2)))
    return average

# Main execution

if __name__ == '__main__':
    # Parse command line arguments
    args = sys.argv[1:]
    while len(args) and args[0].startswith('-') and (len(args[0]) > 1):
        arg = args.pop(0)
        if arg == '-h':
            usage(0)
        elif arg == '-v':
            VERBOSE = True
        elif arg == '-p':
            PROCESSES = int(args.pop(0))
        elif arg == '-r':
            REQUESTS = int(args.pop(0))
        else:
            usage(1)

    if len(args) == 0:
        usage(1)
    else:
        URL = args.pop(0)

    # Create pool of workers and perform requests
    pool = multiprocessing.Pool(PROCESSES)
    times = pool.map(do_request, range(PROCESSES))
    print("TOTAL AVERAGE ELAPSED TIME: {}".format(round(sum(times) / len(times), 2)))

# vim: set sts=4 sw=4 ts=8 expandtab ft=python:
