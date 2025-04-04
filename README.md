# Stregsystemets Linux Kernel Module
<p align="center">
    <img src=https://github.com/f-klubben/logo/blob/master/logo-white-circle-background.png width=150px> 
</p>
<p align="center">
https://github.com/f-klubben/stregsystemet | https://fklub.dk
</p>
<p align="center">
ever wish you could buy your beer by echoing what you want directly into your kernel? no i didn't either.
</p>

Note: this project was developed as an entry for [Facking](https://fklub.dk/aktiviteter/fhacking)

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
sudo insmod result/lib/modules/*/misc/stregkernel.ko
```

## Use
```sh
echo "admin øl:10" > /dev/fklub/quickbuy
```

If running using the vm flake output, then a few utils have been included:
```sh
# buy something
quickbuy admin øl:10

# display the flogo
flogo

# set a setting
set-setting address 127.0.0.1
set-setting port 8080
```

## Unload
`sudo rmmod stregkernel`

## Development
This project was developed on NixOS, which means that clangd and subsequently the compile_commands.json file are not trivial to configure.

for this i have created the derivation at flake output `.#mkCompileCommands` to generate a compile_commands file based on the NixOS kernel and copy it to the project root for clangd to use for language server support for the linux kernel module.

NOTE: this derivation will also work for any other kernel module, feel free to use it in other projects.
