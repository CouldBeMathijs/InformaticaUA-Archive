from tests.utils import run_case


def test_basis():
    assert run_case("opdracht3_basis"), "Output is niet correct voor basis voorbeeld"

def test_volledig():
    assert run_case("opdracht3_volledig"), "Output is niet correct voor volledig voorbeeld"