#!/bin/bash

PREVIOUS_DIR=$(realpath .)

cd nhan_flash_config
openocd -f ./openocd-stlink.ocd -f ./flash_bootloader_app.ocd

exit 1
cd $PREVIOUS_DIR
