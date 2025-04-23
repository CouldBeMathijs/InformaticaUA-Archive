import pathlib
import filecmp

from quetzal.app import App


def test():
    run_case("voorbeeld1")
    run_case("voorbeeld2")


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
        app = App()
        resultaten = app.run(script)

        # schrijf de log bestanden weg naar de output directory
        for tijdstip, log in resultaten.items():
            with open(case_directory / f"log{tijdstip}.html", "w") as file:
                file.write(log)

    # compare all output log files in the case directory with the log files in the model directory
    output_log_names = [f.name for f in case_directory.glob("*.html")]
    matching_files, _, _ = filecmp.cmpfiles(
        case_directory,
        model_directory,
        output_log_names,
        shallow=False,
    )

    # check if all output log files match
    assert len(matching_files) == len(output_log_names), "Output is niet correct"
