def leap_year(year):
    """
    :param year: het meegegeven jaar
    :return: True als schrikkeljaar, False als geen schrikkeljaar
    """
    return year % 4 == 0 and year % 100 != 0 or year % 400 == 0
def days_in_month(month, year):
    """
    :param month: meegegeven maand
    :param year: meegegeven jaar
    :return: Het aantal dagen in de maand
    """
    if month in [1, 3, 5, 7, 8, 10, 12]:
        return 31
    if month in [4, 6, 9, 11]:
        return 30
    if leap_year(year) and month == 2:
        return 29
    else:
        return 28
def next_month(month, year):
    """
    :param month: meegegeven maand
    :param year: meegegeven jaar
    :return: als de maand december is, ga dan door naar januari volgend jaar, anders ga naar volgende maand
    """
    if month == 12:
        return (1, year + 1)
    else:
        return (month + 1, year)
class Date:
    def __init__(self, day, month, year):
        self.year = year
        self.month = month
        self.day = day
    def __str__(self):
        """
        Zorg ervoor dat de datum wordt weergegeven als dd-mm-yyyy
        """
        daystring = str(self.day).zfill(2)
        monthstring = str(self.month).zfill(2)
        return f"{daystring}-{monthstring}-{self.year}"
    def comes_before(self, other):
        """
        check steeds of het Most Significant deeltje groter of kleiner is, en als ze gelijk zijn,
        ga dan door met de 2de MS, en zo verder. Als ze exact gelijk zijn, is het ook False
        """
        if self.year < other.year:
            return True
        elif self.year > other.year:
            return False
        if self.month < other.month:
            return True
        elif self.month > other.month:
            return False
        if self.day < other.day:
            return True
        else:
            return False
    def add_days(self, n):
        """Voeg n dagen toe aan de huidige dag"""
        self.day += n

        while self.day > days_in_month(self.month, self.year):
            self.day -= days_in_month(self.month, self.year)
            self.month += 1
            if self.month > 12:
                self.month = 1
                self.year += 1
    def get_days_to(self, other):
        """
        Deze methode gebruikt tijdelijke kopieën van self en other om aan te kunnen passen, het gebruikt ook een teller nod
        (Number Of Days) om bij te houden hoeveel keer de while loop al uitgevoerd is.
        """
        temp_self = Date(self.day, self.month, self.year)
        temp_other = Date(other.day, other.month, other.year)
        nod = 0

        while temp_self.comes_before(temp_other):
            nod += 1
            temp_self.add_days(1)

        while temp_other.comes_before(temp_self):
            nod -= 1
            temp_other.add_days(1)

        return nod