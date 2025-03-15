
# stregsystem Kernel Module

## Features
- Creates `/dev/stregsystem`.
- GET /1/ to fetch CSRF and POST /1/sale/ with quickbuy command.
- Fully done in kernel space (C), no userspace helper.

## Build
`make`

## Load
```sh
sudo insmod stregsystem.ko
```
## Use
```sh

sudo mknod /dev/stregsystem c <major> 0
echo "tester 1:10" > /dev/stregsystem
```
## Unload
`sudo rmmod stregsystem`
