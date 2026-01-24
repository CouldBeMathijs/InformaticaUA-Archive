from math import sqrt
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def print(self):
        print((self.x, self.y))
    def reflect_x(self):
        return Point(self.x, -self.y)
    def distance(self, other):
        return sqrt((self.x - other.x)**2 + (self.y - other.y)**2)
    def compute_line_to(self, other):
        a = (other.y - self.y)/(other.x - self.x)
        b = self.y - a*self.x
        return (a, b)