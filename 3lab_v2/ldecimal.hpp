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

	void divide_by_10();
	void multiply_by_10();

	ldecimal operator~ () const;
	ldecimal& to_ones_complement();

	ldecimal& operator+= (digit);
	ldecimal& operator-= (digit);
	friend ldecimal operator+ (ldecimal, ldecimal);
	friend ldecimal operator- (ldecimal, ldecimal);

	friend bool operator> (const ldecimal&, const ldecimal&);
	friend bool operator< (const ldecimal&, const ldecimal&);
	friend bool operator== (const ldecimal&, const ldecimal&);
	friend bool operator!= (const ldecimal&, const ldecimal&);

	friend std::istream& operator>> (std::istream&, ldecimal&);
	friend std::ostream& operator<< (std::ostream&, const ldecimal&);
	
	ldecimal& operator= (const ldecimal& ld) { value = ld.value; sign = ld.sign; return *this; }
};

ldecimal operator"" _ld(uint64_t);