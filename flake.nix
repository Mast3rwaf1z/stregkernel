{
    inputs.nixpkgs.url = "nixpkgs/nixos-24.11";
    outputs = { self, nixpkgs, ... }: let
        system = "x86_64-linux";
        pkgs = import nixpkgs { inherit system; };
        
    in {
        packages.${system} = {
            default = pkgs.callPackage ./nix/module.nix { kernel = pkgs.linux.dev; };
            vm = (nixpkgs.lib.nixosSystem {
                inherit system;
                modules = [({ ... }:

                {
                    boot.extraModulePackages = [ self.packages.${system}.default ]; 

                    virtualisation.vmVariant.virtualisation.graphics = false;
                    
                    users.users.root.password = "1234";
                    system.stateVersion = "nixos-24.11";
                })];
            }).config.system.build.vm;
        };
    };
}
