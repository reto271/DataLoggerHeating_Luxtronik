#!/bin/bash

# Working for influx DB
# ---------------------
# Creates the data base
#curl -i -X POST 'http://ds218plus.local:8086/query' --data-urlencode "q=CREATE DATABASE heatingdb"

#Posts successful
curl -i -X POST 'http://ds218plus.local:8086/write?db=heatingdb' --data-binary 'cpu_load_short,host=server01,region=us-west value=0.3'
curl -i -X POST 'http://ds218plus.local:8086/write?db=heatingdb' --data-binary 'cpu_load_short,host=server01,region=us-east value=0.5'
#Timed entry in ns
#curl -i -X POST 'http://ds218plus.local:8086/write?db=heatingdb' --data-binary 'cpu_load_short,host=server01,region=us-east value=0.6 1591275600000000000'
