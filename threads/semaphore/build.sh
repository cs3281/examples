#!/bin/bash
g++ shm_produce.cpp -lrt -lpthread -o shm_produce
g++ shm_consume.cpp -lrt -lpthread -o shm_consume
