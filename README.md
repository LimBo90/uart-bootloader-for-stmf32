# Bootloader for stm32f10 microcontroller

## States:-
This bootloader has 3 states
  * __Waiting__:- In this state the bootloader is waiting to recieve the first line of the application's HEX file which would be written to FLASH.
  * __Writing__:- In this state the bootloader is in the middle of recieving the HEX file of the application
  * __Finished__:- The bootloader finished execution and ready to jump to application
  
![Bootloader Finite State Machine](https://i.imgur.com/tzVfmog.png)

## How it works :-
1. The flash is seperated into two regions
   * __Bootloader region__ starts at `FLASH_START_ADDRESS`
   * __Application region__ starts at `FLASH_START_ADDRESS + APP_OFFSET`
1. The bootloader is the first program to run when microcontroller is booted. It waits for a new HEX file (application) to be recieved through UART.
   * if there's an application ready in application region, the bootloader keeps waiting until timeout `BOOTLOADER_TIMEOUT_DURATION` then jumps to the application.
   * if there's no application ready in application region, the bootloader keeps waiting indefinitely for a new application.
1. Hex file is sent line by line.
   * if a line is recieved, parsed and written to flash correctly the bootloader responds with `ok\r`.The sender should wait for `ok\r` before sending the next line.
   * if an error occurred during parsing the line or writing it to flash, the bootloader responds with `ERR: <cause of error>\r`
 
