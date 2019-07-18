# Bootloader for stm32f10 microcontroller

### States:-
This bootloader has 3 states
  *Waiting:- In this state the bootloader is waiting to recieve the first line of the application's HEX file which would be written to FLASH.
  *Writing:- In this state the bootloader is in the middle of recieving the HEX file of the application
  *Finished:- The bootloader finished execution and ready to jump to application
