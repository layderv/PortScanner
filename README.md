### PortScanner ###
C++ port scanner for Linux

### Options ###
* -s: set starting port (1); min 1
* -e: set ending port (2^16-1); max 2^16-1
* -t: target hostname or ip address
* -d: delay in ms to connect (0); [0,10^6)

### Examples ###
* ./PortScanner -t 127.0.0.1
* ./PortScanner -t 127.0.0.1 -s 1024 -d 100
