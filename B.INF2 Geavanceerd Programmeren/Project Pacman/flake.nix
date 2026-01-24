{
        description = "The Pacman Project for the Advanced Programming course at the University of Antwerp";

        inputs = {
                nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
                flake-utils.url = "github:numtide/flake-utils";
        };

        outputs = { self, nixpkgs, flake-utils }:
                flake-utils.lib.eachDefaultSystem (system:
                        let
                                pkgs = import nixpkgs {
                                        inherit system;
                                };

                                compiler = pkgs.clang;
                                sfml_2 = pkgs.sfml_2;

                                sfml-dev-pkgs = with pkgs; [
                                        compiler
                                        cmake
                                        meson
                                        ninja
                                        pkg-config
                                        gdb
                                        sfml_2
                                        valgrind
                                        clang-uml
                                ];

                                mkPacmanDerivation = { optimization, debug, version }: pkgs.stdenv.mkDerivation {
                                        pname = "pacman";
                                        inherit version;
                                        src = ./.;

                                        nativeBuildInputs = with pkgs; [
                                                meson
                                                ninja
                                                pkg-config
                                                compiler
                                                makeWrapper
                                        ];

                                        buildInputs = [ sfml_2 ];

                                        mesonFlags = [
                                                "-Doptimization=${optimization}"
                                                "-Ddebug=${if debug then "true" else "false"}"
                                        ];

                                        # We install the binary and assets into a subfolder in 'share', 
                                        # then wrap the binary to ensure it runs from that directory.
                                        installPhase = ''
            runHook preInstall

            mkdir -p $out/bin
            mkdir -p $out/share/pacman

            # Find the compiled binary and move it to our share folder
            find . -name pacman -type f -executable -exec cp {} $out/share/pacman/ \;

            # Copy assets to the same folder so the relative path "assets/..." works
            if [ -d "$src/assets" ]; then
              cp -r $src/assets $out/share/pacman/
            fi

            # Create a wrapper script in $out/bin/pacman
            # This script 'cd's into the share folder before running the actual game
            makeWrapper $out/share/pacman/pacman $out/bin/pacman \
              --run "cd $out/share/pacman"

            runHook postInstall
                                        '';
                                };

                                cppGamePackage = mkPacmanDerivation {
                                        optimization = "3";
                                        debug = false;
                                        version = "0.1.0-release";
                                };

                                cppGamePackageDebug = mkPacmanDerivation {
                                        optimization = "0";
                                        debug = true;
                                        version = "0.1.0-debug";
                                };

                        in {
                                packages = {
                                        default = cppGamePackage;
                                        pacman-release = cppGamePackage;
                                        pacman-debug = cppGamePackageDebug;
                                };

                                devShells.default = pkgs.mkShell {
                                        buildInputs = sfml-dev-pkgs;

                                        shellHook = ''
            export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath [ sfml_2 ]}:$LD_LIBRARY_PATH
            export CMAKE_PREFIX_PATH=${sfml_2}:$CMAKE_PREFIX_PATH
            export SFML_DIR=${sfml_2}/lib/cmake/SFML

            echo "--------------------------------------------------------"
            echo "Pacman Dev Environment (Meson + CMake + Clang)"
            echo "--------------------------------------------------------"
            echo "To build with Meson:"
            echo "  meson setup build && meson compile -C build"
            echo ""
            echo "To build with CMake:"
            echo "  cmake -B build-cmake . && cmake --build build-cmake"
            echo "--------------------------------------------------------"
                                        '';
                                };

                                apps.default = {
                                        type = "app";
                                        program = "${self.packages.${system}.default}/bin/pacman";
                                };
                        }
                );
}
