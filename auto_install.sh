#!/bin/sh
path=`pwd`

# Change work path
sed -i "s#HOME = /home/PLC_Gateway#HOME =$path#g" Makefile.inc

# Install automake
apt-get install automake
ln -s /usr/bin/automake /usr/bin/automake-1.14
ln -s /usr/bin/aclocal  /usr/bin/aclocal-1.14

# Install libmodbus-3.0.6 to System
cd $path/modbus_server/libmodbus-3.0.6
make clean ; ./configure ; make ; make install

# Make modbus_server
cd $path
make clean 
make
