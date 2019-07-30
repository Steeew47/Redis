#!/bin/bash

for slot in {0..5461}; do redis-cli -h 192.168.99.111 -p 32770 CLUSTER ADDSLOTS $slot > /dev/null; done;
for slot in {5462..10923}; do redis-cli -h 192.168.99.112 -p 32770 CLUSTER ADDSLOTS $slot ; done;
for slot in {10924..16383}; do redis-cli -h 192.168.99.113 -p 32770 CLUSTER ADDSLOTS $slot ; done;

for slot in {10924..16383}; do redis-cli -h 192.168.99.111 -p 32770 CLUSTER DELSLOTS $slot ; done;

