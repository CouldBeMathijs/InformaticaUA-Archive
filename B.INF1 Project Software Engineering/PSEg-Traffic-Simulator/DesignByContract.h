//============================================================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer, modified by Kasper Engelen, modified by CouldBeMathijs
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Declarations for design by contract in C++
//============================================================================

#ifndef DESIGNBYCONTRACT_H
#define DESIGNBYCONTRACT_H

#include <string>

// For pre condities, something ought to be before the code is ran
#define REQUIRE(assertion, what) \
if (!(assertion)) { \
throw std::logic_error(std::string("REQUIRE failed: ") + (what) + " in " + __FILE__ + " at line " + std::to_string(__LINE__)); \
}

// For post conditions, something must be after the code is done
#define ENSURE(assertion, what) \
if (!(assertion)) { \
throw std::logic_error(std::string("ENSURE failed: ") + (what) + " in " + __FILE__ + " at line " + std::to_string(__LINE__)); \
}

#endif