import os
import sys
import yaml
import zipfile


class TextColors:
    BOLD = '\033[1m'
    RED = '\033[91m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    RESET = '\033[0m'


def find_solutions_tex_files(yaml_data, base_path="", in_solutions=False):
    """Recursively collect .tex files under any 'solutions' directory in the yaml."""
    found = []
    for item in yaml_data:
        if isinstance(item, dict):
            for directory, files in item.items():
                new_base = os.path.join(base_path, directory)
                new_in_solutions = in_solutions or (directory == "solutions")
                found += find_solutions_tex_files(files or [], new_base, new_in_solutions)
        else:
            if in_solutions and item.endswith(".tex"):
                found.append(os.path.join(base_path, item))
    return found


def find_yaml_path(zf):
    """Find lab-files.yaml inside the zip, ignoring __MACOSX entries."""
    for name in zf.namelist():
        if name.endswith("lab-files.yaml") and not name.startswith("__MACOSX"):
            return name
    return None


def add_blank_solutions(zip_file):
    with zipfile.ZipFile(zip_file, 'r') as zf:
        yaml_path = find_yaml_path(zf)
        if not yaml_path:
            print(f"{TextColors.BOLD}{TextColors.RED}Could not find lab-files.yaml in the zip.{TextColors.RESET}")
            sys.exit(1)

        print(f"Found yaml: {TextColors.BLUE}{yaml_path}{TextColors.RESET}")

        try:
            yaml_data = yaml.safe_load(zf.open(yaml_path))
        except Exception as e:
            print(f"{TextColors.BOLD}{TextColors.RED}Could not parse {yaml_path}: {e}{TextColors.RESET}")
            sys.exit(1)

        existing = set(zf.namelist())

    # base_path is the folder that contains lab-files.yaml
    base_path = os.path.dirname(yaml_path)

    solution_files = find_solutions_tex_files(yaml_data, base_path=base_path)

    if not solution_files:
        print(f"{TextColors.YELLOW}No .tex files found under 'solutions' in {yaml_path}.{TextColors.RESET}")
        return

    to_add = [f for f in solution_files if f not in existing]
    already_present = [f for f in solution_files if f in existing]

    if already_present:
        print(f"\n{TextColors.BLUE}Already present (skipped):{TextColors.RESET}")
        for f in already_present:
            print(f"  {f}")

    if not to_add:
        print(f"\n{TextColors.BOLD}{TextColors.GREEN}All solution .tex files already in zip — nothing to do.{TextColors.RESET}")
        return

    with zipfile.ZipFile(zip_file, 'a') as zf:
        for path in to_add:
            zf.writestr(path, "")
            print(f"{TextColors.GREEN}Added blank:{TextColors.RESET}  {path}")

    print(f"\n{TextColors.BOLD}{TextColors.GREEN}Done — added {len(to_add)} blank .tex file(s) to {zip_file}.{TextColors.RESET}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python AddTex.py <zip_file>")
        sys.exit(1)

    zip_file = sys.argv[1]
    if not os.path.isfile(zip_file):
        print(f"{TextColors.BOLD}{TextColors.RED}File not found: {zip_file}{TextColors.RESET}")
        sys.exit(1)

    add_blank_solutions(zip_file)