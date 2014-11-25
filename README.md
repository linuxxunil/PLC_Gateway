PLC_Gateway
===========
The program is used to act as Software PLC.

Install Step.
1. Change work path
vim Makefile.inc
Find "HOME = /home/PLC_Gateway" change to your path

2. Install automake
apt-get install automake
ln -s /usr/bin/automake /usr/bin/automake-1.14
ln -s /usr/bin/aclocal  /usr/bin/aclocal-1.14

3. Install libmodbus-3.0.6 to System
cd $path/modbus_server/libmodbus-3.0.6
make clean ; ./configure ; make ; make install

4. Make modbus_server
cd $path
make clean
make
