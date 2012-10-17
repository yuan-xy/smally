#!/bin/bash
mongodump -h 192.168.1.22 -d dface
cd dump/dface
rm system.js.*
cd ..
tar -jcv -f mongo.tar.bz2 dface

