# NodeMCU setup
## Flashing the firmware
Depending on the packages you want to use on NodeMCU, you must build and install a new firmware.
The file firmware.bin is the firmware currently used for this project.
To install it, make sure tou have python3 and pip installed. Then run the following commands

```bash
pip install -r firmware-installation-python-requirements.txt
esptool.py --port <serial-port-of-ESP8266> write_flash -fm dio 0x00000 firmware.bin
```

Obs.: if you are not using ESP32 or ESP12, change "dio" for "qio"

## Setting up the LFS
To setup the Lua FLash Store, we will be using the nodemcu-tool. To install it, run

```
npm i -g nodemcu-tool
```

After that, you can use 

```
#Lists the available NodeMCUs
nodemcu-tool devices

#Prepares the LFS
nodemcu-tool mkfs
```

then you will have the appropriate setup to upload the code.


## Uploading the code to NodeMCU
To upload the code, run

```
nodemcu-tool upload <filename>
```

If the code is composed by more than one file, remember to also upload the other files!

# Information about the firmware
The current firmware was built on https://nodemcu-build.com/ and includes the following modules:
- file 
- gpio 
- i2c 
- net 
- node 
- spi 
- tmr 
- uart 
- wifi

The firmware uses integer types.