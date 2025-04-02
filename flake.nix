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
                        environment.systemPackages = with pkgs; [
                            python312
                            # to test the stregsystem
                            lynx
                            # helpers
                            (pkgs.writeScriptBin "quickbuy" ''
                                echo "$@" > /dev/fklub/quickbuy
                            '')
                            (pkgs.writeScriptBin "flogo" ''
                                cat /dev/fklub/flogo
                            '')
                            (pkgs.writeScriptBin "set-setting" ''
                                echo $2 > /dev/fklub/settings/$1
                            '')
                            (pkgs.writeScriptBin "get-setting" ''
                                cat /dev/fklub/settings/$1
                            '')
                            (pkgs.writeScriptBin "stregsystemet" ''
                                ${pkgs.tmux}/bin/tmux new-session -d
                                ${pkgs.tmux}/bin/tmux split-window -h "dmesg --follow"
                                ${pkgs.tmux}/bin/tmux attach
                            '')
                        ];
                        networking.firewall.enable = false;
                    
                        users.users.root.password = "1234";
                        services.getty.autologinUser = "root";
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
            mkCompileCommands = pkgs.callPackage ./nix/compile_commands.nix {};
        };
        devShells.${system}.default = pkgs.mkShellNoCC {
            packages = [
                pkgs.linux.dev
            ] ++ pkgs.linux.dev.moduleBuildDependencies;
        };
    };
}
