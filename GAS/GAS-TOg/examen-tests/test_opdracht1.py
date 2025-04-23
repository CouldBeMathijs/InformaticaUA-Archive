from quetzal.app import App
from gas.examen.stack import Stack
from gas.examen.MyQueue import MyQueue
from tests.utils import run_case

def extract_stack(stack):
    for attribute in stack.__dict__.values():
        if attribute.__class__ == Stack:
            return attribute

    if Stack in stack.__class__.__bases__:
        return stack

    return None

def test_stack():
    app = App()
    stack = app.get_internals()["werknemer_stack"]

    assert extract_stack(stack) is not None, "De werknemer stack gebruikt intern niet de stack van gas.examen.Stack"

    # empty en push
    assert stack.isEmpty() == True, "isEmpty geeft niet True terug bij een lege stack"
    assert stack.push("test") == True, "de push geeft geen True terug"
    assert not stack.isEmpty(), "na een push geeft isEmpty True terug"

    # gettop
    assert stack.getTop() == ("test", True), "De getTop() methode van de werknemer stack werkt niet correct"

    # pop
    assert stack.pop() == ("test", True),"De pop() methode van de werknemer stack werken niet correct"
    assert stack.isEmpty() == True,"De pop() methode van de werknemer stack werken niet correct"
    assert stack.pop() == (None, False),"De pop() methode van de werknemer stack werken niet correct"

    # gettop
    assert stack.getTop() == (None, False),"De getTop() methode van de werknemer stack werkt niet correct"

    # werking
    assert stack.push("test2") == True,"De werknemer stack werkt niet correct"
    assert stack.push("test3") == True,"De werknemer stack werkt niet correct"
    assert stack.push("test4") == True,"De werknemer stack werkt niet correct"
    assert stack.getTop() == ("test4", True),"De werknemer stack werkt niet correct"
    assert stack.pop() == ("test4", True),"De werknemer stack werkt niet correct"
    assert stack.getTop() == ("test3", True),"De werknemer stack werkt niet correct"
    assert stack.pop() == ("test3", True),"De werknemer stack werkt niet correct"
    assert stack.getTop() == ("test2", True),"De werknemer stack werkt niet correct"
    assert stack.pop() == ("test2", True),"De werknemer stack werkt niet correct"
    assert stack.isEmpty(),"De werknemer stack werkt niet correct"
    assert stack.getTop() == (None, False),"De werknemer stack werkt niet correct"
    assert stack.pop() == (None, False),"De werknemer stack werkt niet correct"


def test_queue():
    app = App()
    queue = app.get_internals()["bestelling_queue"]
    assert (
        queue.__class__ == MyQueue
    ), "De bestelling queue is niet de queue van gas.examen.MyQueue"


def test_app():
    assert run_case("opdracht1"), "Het verwerken van de voorbeeld invoer werkt niet meer correct"
