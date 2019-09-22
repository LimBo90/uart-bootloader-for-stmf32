# Bootloader for stm32f10xx microcontroller

## 1. States:-
This bootloader has 3 states
  * __Waiting__:- In this state the bootloader is waiting to recieve the first line of the application's HEX file which would be written to FLASH.
  * __Writing__:- In this state the bootloader is in the middle of recieving the HEX file of the application
  * __Finished__:- The bootloader finished execution and ready to jump to application
  
![Bootloader Finite State Machine](https://i.imgur.com/tzVfmog.png)

## 2. How it works :-
1. The flash is seperated into two regions
   * __Bootloader region__ starts at `FLASH_START_ADDRESS`
   * __Application region__ starts at `FLASH_START_ADDRESS + APP_OFFSET`
1. The bootloader is the first program to run when microcontroller is booted. It waits for a new HEX file (application) to be recieved through UART.
   * if there's an application ready in application region, the bootloader keeps waiting until timeout `BOOTLOADER_TIMEOUT_DURATION` then jumps to the application.
   * if there's no application ready in application region, the bootloader keeps waiting indefinitely for a new application.
1. Hex file is sent line by line.
   * if a line is recieved, parsed and written to flash correctly the bootloader responds with `ok\r`.The sender should wait for `ok\r` before sending the next line.
   * if an error occurred during parsing the line or writing it to flash, the bootloader responds with `ERR: <cause of error>\r`.
   * The bootloader also sends logs about what's currently doing these logs will have the format `LOG: <log message>\r`.
1. After all lines are written sucessfully and the bootloader receives `EOF` the boot loader jumps to application.

__Note__:-
 pin PC13 is an indicator. As long as the bootloader is running PC13 is HIGH. return to LOW when the bootloader is jumping to application. 
  ### Errors returned by bootloader
   * `ERR: Checksum error\r` checksum calculated by bootloader doesn't match the one in hex record.
   * `ERR: Invalid record. Missing ':' at beginning of record\r` each record has to start with `:`
   * `ERR: Can't write record to flash. Tried to write into a region not previously erased\r` 
   * `ERR: Can't write record to flash. Tried to write into a write protected region\r`
   * `ERR: Can't write record to flash. Flash took too long to program(timeout error).\r` took too long to write record to flash
   * `ERR: Can't write record to flash. Invalid address for record.\r` address in record is not in application region
   * `ERR: Couldn't erase application region in flash. (timeout error)\r`
   * `ERR: Record has an unknown type field\r`
  ### Logs sent by bootloader
   * `LOG: Waiting\r` bootloader entered waiting state and is waiting for a new HEX file
   * `LOG: New hex file incoming \r` first line in a hex file is recieved
   * `LOG: Erasing FLASH...\r`  bootloader is erasing flash
   * `LOG: Application detected in FLASH\r` bootloader detected an application in application region that's ready to run
   * `LOG: No application detected in FLASH\r` bootloader didn't detect an application in application region
   * `LOG: BootLooder finished execution \r`  bootloader is in finished state
   * `LOG: BootLoader Exiting...\r` bootloader is exiting (de-iniatlizing peripherals used and jumping to application)

## 3. Configuration
There are 4 values that's configured in `Bootloader_config.h`
   * `FLASH_START_ADD` start address of the flash.
   * `APP_OFFSET` Offset of the application region relative to the start address of flash
   * `APP_END_ADDRESS` the end address of the application region (used when erasing the application region) 
   * `BOOTLOADER_TIMEOUT_DURATION` The duration the bootloader will wait after which the bootloader will jump to application if one exists
   #### Note 
   The application HEX file must have start address = `FLASH_START_ADD` + `APP_OFFSET`. This can be done by configuring the application's linker script.

## 4. Sending a HEX file
  sendHexFile.py in is a python script to send a HEX file to bootloader.
  #### Usage
>python sendHexFile.py [com-port] [baudrate] [file-location]

  __Example__:-
>python sendHexFile.py com3 57600 ".\examples\BLINKING_LED.hex"
     
