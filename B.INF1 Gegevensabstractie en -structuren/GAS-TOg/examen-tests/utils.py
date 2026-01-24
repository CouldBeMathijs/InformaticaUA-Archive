import pathlib
import filecmp

from quetzal.app import App


INPUT_BASE_PATH = pathlib.Path("input")
OUTPUT_BASE_PATH = pathlib.Path("output")
MODEL_BASE_PATH = pathlib.Path("model")


def run_case(case_name):
    # get paths
    case_directory = OUTPUT_BASE_PATH / case_name
    case_directory.mkdir(parents=True, exist_ok=True)
    case_path = (INPUT_BASE_PATH / case_name).with_suffix(".txt")
    model_directory = MODEL_BASE_PATH / case_name

    # run the case (outputs log files to the case directory)
    with open(case_path) as file:
        script = file.read()

        # voer het script uit
        try:
            app = App()
            resultaten = app.run(script)
        except:
            raise AssertionError("Er ging iets mis bij het uitvoeren van het script")

        # schrijf de log bestanden weg naar de output directory
        for tijdstip, log in resultaten.items():
            with open(case_directory / f"log{tijdstip}.html", "w") as file:
                file.write(log)

    # compare all output log files in the case directory with the log files in the model directory
    log_names = [f.name for f in model_directory.glob("*.html")]
    matching_files, _, _ = filecmp.cmpfiles(
        case_directory,
        model_directory,
        log_names,
        shallow=False,
    )

    # check if all output log files match
    return len(matching_files) == len(log_names)