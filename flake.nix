{
  description = "litex dep flake for aarch64-darwin";
  # TODO: make this multiplatform

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    inherit (nixpkgs) lib;
    system = "aarch64-darwin";
    overlay = import ./overlay.nix;

    pkgs = import nixpkgs {
      inherit system;
      overlays = [overlay];
    };

    deps = with pkgs; [
      git
      autoconf
      # automake
      # mill
      dtc
      boost
      # cmake
      ninja
      pkgsCross.riscv64-embedded.buildPackages.gcc
      # pkgsCross.riscv64-embedded.buildPackages.gdb
      openocd
      # spike

      ### litex dependencies ###
      # python312
      uv
      expat
      fakeroot
      perl
      json_c
      meson
      libevent
      libmpc
      verilator
      yosys
      trellis
      nextpnr

      qemu
      spike
    ];
  in {
    legacyPackages.${system} = pkgs;
    devShells.${system}.default = pkgs.mkShell.override {stdenv = pkgs.clangStdenv;} {
      name = "rkbmt-mac";
      buildInputs = deps;
      env = {
        RV64_TOOLCHAIN_ROOT = "${pkgs.pkgsCross.riscv64-embedded.buildPackages.gcc}";
      };
    };
  };

  overlays.default = overlay;
}
