
# stregsystem Kernel Module
https://github.com/f-klubben/stregsystemet

ever wish you could buy your beer by echoing what you want directly into your kernel? no i didn't either.

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
echo "admin øl:10" > /dev/stregsystem
```
## Unload
`sudo rmmod stregsystem`
