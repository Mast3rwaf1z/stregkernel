{
    inputs.nixpkgs.url = "nixpkgs/nixos-24.11";
    inputs.stregsystemet = {
        url = "github:Mast3rwaf1z/stregsystemet/nix-updates";
        inputs.nixpkgs.follows = "nixpkgs";
    };
    outputs = { self, nixpkgs, stregsystemet, ... }: let
        system = "x86_64-linux";
        pkgs = import nixpkgs { inherit system; };
        
    in {
        packages.${system} = {
            default = pkgs.callPackage ./nix/module.nix { kernel = pkgs.linux.dev; };
            vm = (nixpkgs.lib.nixosSystem {
                inherit system;
                modules = [
                    stregsystemet.nixosModules.default
                    ({ pkgs, ... }:

                    {
                        boot.extraModulePackages = [ self.packages.${system}.default ]; 
                        boot.kernelModules = [ "stregkernel" ];

                        virtualisation.vmVariant.virtualisation.graphics = false;
                        virtualisation.vmVariant.virtualisation.forwardPorts = [
                            { from = "host"; host.port = 8080; guest.port = 80; }
                            { from = "host"; host.port = 8081; guest.port = 8080; }
                        ];
                        environment.systemPackages = with pkgs; [
                            python312
                        ];
                        networking.firewall.enable = false;
                    
                        users.users.root.password = "1234";
                        system.stateVersion = "nixos-24.11";

                        stregsystemet = {
                            enable = true;
                            port = 8080;
                            testData = {
                                enable = true;
                            };
                            debug.debug = true;

                        };
                    })
                ];
            }).config.system.build.vm;
            insert = (pkgs.writeScriptBin "insert" ''
                #!${pkgs.bash}/bin/bash
                sudo insmod ${self.packages.${system}.default}/lib/modules/${pkgs.linux.dev.modDirVersion}/misc/stregkernel.ko
            '');
            remove = (pkgs.writeScriptBin "remove" ''
                #!${pkgs.bash}/bin/bash
                sudo rmmod stregkernel
            '');
        };
    };
}
