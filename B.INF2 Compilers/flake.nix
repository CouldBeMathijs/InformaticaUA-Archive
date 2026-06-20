{
  description = "LLVM Mips Compiler - UAntwerpen";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        generate-parser = pkgs.writeShellScriptBin "generate-parser" ''
          set -euo pipefail

          cd src/parser/grammars

          ${pkgs.antlr4}/bin/antlr4 \
            -Dlanguage=Python3 \
            Grammar.g4 \
            -o . \
            -visitor \
            -no-listener

          echo "Parser generated in src/parser/grammars/"
        '';
      in
      {
        packages.generate-parser = generate-parser;

        apps.generate-parser = {
          type = "app";
          program = "${generate-parser}/bin/generate-parser";
        };

        apps.default = self.apps.${system}.generate-parser;

        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            antlr4
            llvmPackages_22.clang # Bypasses the Nix wrapper to allow clean cross-compilation
            llvmPackages_22.llvm
            gcc # As a reference
            generate-parser
            graphviz # Generate flow charts
            python3
            python3Packages.antlr4-python3-runtime # ANTLR Python integration
            python3Packages.graphviz # Graphviz Python integration
            python3Packages.llvmlite # LLVM integration
            python3Packages.anybadge
            xspim # MARS Emulator
          ];

          # Stops Nix from injecting host-specific hardening options into compile steps
          hardeningDisable = [ "all" ];

          shellHook = ''
            export PYTHONPATH="$PYTHONPATH:$(pwd)/src"
          '';
        };
      }
    );
}
