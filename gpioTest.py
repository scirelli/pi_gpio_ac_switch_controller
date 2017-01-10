#!/usr/bin/python
import json
import os
from sys import stdin, exit
from subprocess import call

PIN = '0'
ON = '1'
OFF = '0'
p = call(['gpio','mode', PIN, 'down'])
p = call(['gpio','write', PIN, OFF])
