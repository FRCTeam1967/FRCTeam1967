#!/bin/sh

TOPDIR=`pwd`

# build ntcore
mkdir -p ntcore/build
cd ntcore/build
cmake ..
echo "Building ntcore"
make
echo "Done"
cd ${TOPDIR}

# build cscore
mkdir -p cscore/build
cd cscore/build
cmake ..
echo "Building cscore"
make
echo "Done"
cd ${TOPDIR}

# build cv-capture
mkdir -p cv-capture/build
cd cv-capture/build
cmake ..
echo "Building cv-capture"
make
echo "Done"
cd ${TOPDIR}
