
# stregsystem Kernel Module
https://github.com/f-klubben/stregsystemet

ever wish you could buy your beer by echoing what you want directly into your kernel? no i didn't either.

## Features
- Creates `/dev/fklub/quickbuy` and `/dev/fklub/balance`.
- send a multibuy via a character device under `/dev` to [stregsystemet](https://github.com/f-klubben/stregsystemet)

## Build
`make`

## Load
```sh
sudo insmod stregsystem.ko
```
## Use
```sh
echo "admin øl:10" > /dev/fklub/quickbuy
```
## Unload
`sudo rmmod stregsystem`
