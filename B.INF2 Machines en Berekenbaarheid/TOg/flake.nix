{
  description = "The Flake for the TOg for the Machines & Berekenbaarheid course at the university of Antwerp";

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
        pkgs = import nixpkgs {
          inherit system;
        };

        compiler = pkgs.clang;
        sfml_2 = pkgs.sfml_2;

        # Packages needed for development shell
        dev-pkgs = with pkgs; [
          compiler
          cmake
          gdb
          sfml_2
          valgrind
          graphviz
          curl
          nlohmann_json
        ];

        cppGamePackage = pkgs.stdenv.mkDerivation {
          pname = "mbtog";
          version = "0.1.0";

          src = ./.; # Source code is in the current flake directory

          buildInputs = [ sfml_2 ];

          nativeBuildInputs = [
            pkgs.cmake
            pkgs.clang
          ];

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
            "-DSFML_DIR=${sfml_2}/lib/cmake/SFML"
          ];

          installPhase = ''
            mkdir -p $out
            cp mbtog $out/
          '';

        };

        cpp-env = pkgs.mkShell {
          buildInputs = dev-pkgs;
          shellHook = ''
            export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath [ sfml_2 ]}:$LD_LIBRARY_PATH
            export CMAKE_PREFIX_PATH=${sfml_2}:$CMAKE_PREFIX_PATH

            # Manual fix for SFML 2.6 CMake targets
            export SFML_DIR=${sfml_2}/lib/cmake/SFML

            echo "Entering C++ development environment."
            echo "Run 'cmake . && make' to build, or 'nix build' outside this shell."
            echo "Please run your IDE from within this env to make sure all dependencies are present."
          '';
        };
      in
      {
        packages = {
          default = cppGamePackage;
          sfml-cpp-project = cppGamePackage;
        };

        devShells = {
          default = cpp-env;
        };

        apps.default = {
          type = "app";
          program = "${cppGamePackage}/mbtog";
        };
      }
    );
}
