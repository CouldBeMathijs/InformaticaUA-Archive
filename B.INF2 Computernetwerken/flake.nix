{
  description = "CLI tools to zip and validate lab submissions";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
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

        # 1. Updated Python validation script with explicit exit codes
        check-zip-script =
          pkgs.writers.writePython3Bin "check-zip"
            {
              libraries = [ pkgs.python3Packages.pyyaml ];
              doCheck = false;
            }
            ''
              import os
              import sys
              import yaml
              import zipfile
              import hashlib

              class TextColors:
                  BOLD = '\033[1m'
                  RED = '\033[91m'
                  GREEN = '\033[32m'
                  YELLOW = '\033[33m'
                  BLUE = '\033[34m'
                  RESET = '\033[0m'

              def check_zip_contents(yaml_data, zip_file, base_path=""):
                  result = True
                  with zipfile.ZipFile(zip_file, 'r') as zip:
                      zip_contents = set(zip.namelist())
                      for item in yaml_data:
                          if isinstance(item, dict):
                              for directory, files in item.items():
                                  new_base_path = os.path.join(base_path, directory)
                                  result = check_zip_contents(files, zip_file, new_base_path) and result
                          else:
                              full_path = os.path.join(base_path, item)
                              if full_path not in zip_contents:
                                  result = False
                                  print(f"{full_path} {TextColors.BOLD}{TextColors.RED}does not exist.{TextColors.RESET}")
                              else:
                                  with zip.open(full_path) as file_in_zip:
                                      if (not file_in_zip.read(1)):
                                          result = False
                                          print(f"{full_path} {TextColors.BOLD}{TextColors.RED}is empty.{TextColors.RESET}")
                  return result

              def calculate_md5(zip_file_path, file_inside_zip):
                  with zipfile.ZipFile(zip_file_path, 'r') as zip:
                      with zip.open(file_inside_zip) as file:
                          md5 = hashlib.md5()
                          while chunk := file.read(8192):
                              md5.update(chunk)
                  return md5.hexdigest()

              if __name__ == "__main__":
                  if len(sys.argv) != 3:
                      print("Usage: python check_zip.py <group number> <zip_file>")
                      sys.exit(1)

                  group_id, zip_file = sys.argv[1], sys.argv[2]
                  try:
                      with zipfile.ZipFile(zip_file, 'r') as zip:
                          yaml_data = yaml.safe_load(zip.open("Group"+group_id+"/lab-files.yaml"))
                  except Exception:
                      print(f"Unable to check zip file: {TextColors.BOLD}{TextColors.RED}Group{group_id}/lab-files.yaml could not be read!{TextColors.RESET}")
                      sys.exit(1)

                  ok = check_zip_contents(yaml_data, zip_file, "Group"+group_id)
                  md5 = calculate_md5(zip_file, "Group"+group_id+"/groupid.tex")
                  
                  if (md5 == "aa34daa9f73511943006d9043ced8992"):
                      ok = False
                      print(f"Group{group_id}/groupid.tex {TextColors.BOLD}{TextColors.YELLOW}has not been modified.{TextColors.RESET}")
                  
                  if (ok):
                      print(f"Zip file check result: {TextColors.BOLD}{TextColors.GREEN}PASS{TextColors.RESET}")
                      sys.exit(0)
                  else:
                      print(f"Zip file check result: {TextColors.BOLD}{TextColors.RED}FAIL{TextColors.RESET}")
                      sys.exit(1) # CRITICAL: Ensure shell knows it failed
            '';

        # 2. Shell command remains the same, now relying on the fixed exit code
        zip-and-check = pkgs.writeShellApplication {
          name = "zip-and-check";
          runtimeInputs = [
            check-zip-script
            pkgs.zip
          ];
          text = ''
            shopt -s nullglob
            groups=(Group*/)

            if [ ''${#groups[@]} -eq 0 ]; then
              echo "No folder starting with 'Group' found in the current directory."
              exit 1
            fi

            for dir_path in "''${groups[@]}"; do
              dir=''${dir_path%/}
              group_id=''${dir#Group}
              zip_name="$dir.zip"

              echo "Zipping $dir into $zip_name..."
              zip -q -r "$zip_name" "$dir"

              echo "Running check-zip for Group $group_id..."
              
              if ! check-zip "$group_id" "$zip_name"; then
                echo "Validation failed for $zip_name. Removing invalid zip file."
                rm "$zip_name"
              fi
              echo "-----------------------------------"
            done
          '';
        };
      in
      {
        packages.default = zip-and-check;
        devShells.default = pkgs.mkShell {
          buildInputs = [
            check-zip-script
            zip-and-check
          ];
        };
      }
    );
}
