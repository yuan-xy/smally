#!/bin/bash
#/home/dooo/mongodb/bin/mongod --fork --dbpath /home/dooo/mongodb/db --logpath /home/dooo/mongodb/mongodb.log
/home/dooo/mongodb/bin/mongod --fork --dbpath /home/dooo/mongodb/db --logpath /home/dooo/mongodb/mongodb.log --replSet dface --bind_ip 192.168.1.22
/home/dooo/mongodb/bin/mongod --fork --dbpath /home/dooo/mongodb/arbiter --logpath /home/dooo/mongodb/mongo-arbiter.log  --replSet dface --port 40000 --bind_ip 192.168.1.22

