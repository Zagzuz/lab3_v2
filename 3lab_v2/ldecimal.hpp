#pragma once

#include "digit.hpp"

#include <list>
#include <string>
#include <algorithm>
#include <iterator>

class ldecimal
{
	std::list<digit> value;
	enum sign_t : bool { plus, minus } sign;

public:
	ldecimal() : sign(plus) {}
	ldecimal(digit);
	ldecimal(int64_t);
	ldecimal(const std::string&);
	ldecimal(ldecimal&&) = default;
	ldecimal(const ldecimal& ld) : value(ld.value), sign(ld.sign) {}

	ldecimal& operator= (const ldecimal& ld);

	void divide_by_10();
	void multiply_by_10();

	ldecimal& operator+= (digit);
	ldecimal& operator-= (digit);

	ldecimal operator~ () const;
	ldecimal& to_ones_complement();

	friend ldecimal operator+ (ldecimal, ldecimal);
	friend ldecimal operator- (ldecimal, ldecimal);

	friend bool operator> (const ldecimal&, const ldecimal&);
	friend bool operator< (const ldecimal&, const ldecimal&);
	friend bool operator== (const ldecimal&, const ldecimal&);
	friend bool operator!= (const ldecimal&, const ldecimal&);

	friend std::istream& operator>> (std::istream&, ldecimal&);
	friend std::ostream& operator<< (std::ostream&, const ldecimal&);
	
};

ldecimal operator"" _ld(uint64_t);