{
        description = "A C++ project";

        inputs = {
                # Get the systems from the flake's utility functions
                nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
                flake-utils.url = "github:numtide/flake-utils";
        };

        outputs = { self, nixpkgs, flake-utils }:
                # Use flake-utils to iterate over common systems
                flake-utils.lib.eachDefaultSystem 
                (system: # <--- system variable is now passed in
                        let
                                pkgs = import nixpkgs {
                                        inherit system;
                                        config.allowUnfree = true;
                                };

                                compiler = pkgs.clang;
                                sfml-pkgs = with pkgs; [ compiler cmake valgrind doxygen ];

                                cpp-env = pkgs.mkShell {
                                        buildInputs = sfml-pkgs;
                                        shellHook = ''
                                        echo "Entering C++ development environment for ${system}."
                                        echo "Please launch your IDE from within this terminal to make sure all dependencies are met"
                                        '';
                                };
                        in {
                                devShells = {
                                        default = cpp-env;
                                };
                        }
                );
}
