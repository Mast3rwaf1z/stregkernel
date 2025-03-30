
# stregsystem Kernel Module
https://github.com/f-klubben/stregsystemet

ever wish you could buy your beer by echoing what you want directly into your kernel? no i didn't either.

## Features
- Creates `/dev/fklub/quickbuy` and `/dev/fklub/balance`.
- send a multibuy via a character device under `/dev` to [stregsystemet](https://github.com/f-klubben/stregsystemet)

## Build
`make`

or nixos:

`nix build`

## Load
```sh
sudo insmod stregkernel.ko
```
or nixos:
```sh
nix shell nixpkgs#linux.dev
nix run .#insert
```

## Use
```sh
echo "admin øl:10" > /dev/fklub/quickbuy
```
## Unload
`sudo rmmod stregkernel`
