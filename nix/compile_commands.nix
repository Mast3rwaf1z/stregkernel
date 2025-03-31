{ pkgs ? import <nixpkgs> {} }: pkgs.writeScriptBin "mk_compile_commands" ''
    #!${pkgs.bash}/bin/bash
    ${pkgs.python312}/bin/python ${pkgs.linux.dev}/lib/modules/${pkgs.linux.dev.modDirVersion}/source/scripts/clang-tools/gen_compile_commands.py
''
