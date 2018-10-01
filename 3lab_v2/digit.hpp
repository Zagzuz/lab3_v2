#pragma once

#include <iostream>

class digit final
{
	int8_t value;
public:
	digit() : value(0) {}
	digit(int v);
	digit(char c);
	digit& operator++ ();
	digit operator++ (int);
	digit& operator-- ();
	digit operator-- (int);
	digit operator~() const;
	digit& to_ones_complement();
	digit& operator+= (const digit);
	digit& operator-= (const digit);
	friend digit operator+ (const digit, const digit);
	friend digit operator- (const digit, const digit);
	friend std::istream& operator>> (std::istream&, digit&);
	explicit operator int() { return static_cast<int>(value); }
	friend std::ostream& operator<< (std::ostream&, const digit);
	friend bool operator> (const digit lhs, const digit rhs) { return lhs.value > rhs.value; }
	friend bool operator< (const digit lhs, const digit rhs) { return lhs.value < rhs.value; }
	friend bool operator>= (const digit lhs, const digit rhs) { return lhs.value >= rhs.value; }
	friend bool operator<= (const digit lhs, const digit rhs) { return lhs.value <= rhs.value; }
	friend bool operator== (const digit lhs, const digit rhs) { return lhs.value == rhs.value; }
	friend bool operator!= (const digit lhs, const digit rhs) { return lhs.value != rhs.value; }
};

digit operator""_dgt(uint64_t);
digit operator""_dgt(char);