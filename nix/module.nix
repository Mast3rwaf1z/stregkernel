{ stdenv, kernel }:

stdenv.mkDerivation {
    pname = "stregkernel";
    version = "latest";
    name = "stregkernel";
    src = ../.;
    nativeBuildInputs = kernel.moduleBuildDependencies;
    KERNEL = kernel.dev;
    KERNEL_VERSION = kernel.modDirVersion;
    installPhase = ''
        install -D stregkernel.ko $out/lib/modules/${kernel.modDirVersion}/misc/stregkernel.ko 
    '';
}
