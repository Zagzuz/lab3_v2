#include "digit.hpp"

digit::digit(int v)
{
	v = abs(v);
	value = (v > 9 ? v % 10 : v);
}

digit::digit(char c)
{
	if (c < '0' || c > '9') value = 0;
	else value = c - '0';
}

digit digit::operator~() const
{
	return digit(9 - value);
}

digit& digit::to_ones_complement()
{
	this->value = 9 - this->value;
	return *this;
}

digit& digit::operator++ ()
{
	value = (value == 9 ? 0 : value + 1);
	return *this;
}

digit digit::operator++ (int)
{
	int8_t old_value = this->value;
	++*this;
	return digit(old_value);
}

digit& digit::operator-- ()
{
	value = (value == 0 ? 9 : value - 1);
	return *this;
}

digit digit::operator-- (int)
{
	int8_t old_value = this->value;
	--*this;
	return digit(old_value);
}

digit& digit::operator+= (const digit d)
{
	this->value += d.value;
	this->value %= 10;
	return *this;
}

digit& digit::operator-= (const digit d)
{
	this->value -= d.value;
	this->value = abs(this->value);
	return *this;
}

digit operator+ (const digit lhs, const digit rhs)
{
	return digit
	(
		static_cast<int8_t>(lhs.value + rhs.value)
	);
}

digit operator-(const digit lhs, const digit rhs)
{
	return digit
	(
		static_cast<int8_t>(lhs.value + rhs.value)
	);
}

std::istream& operator>> (std::istream& is, digit &d)
{
	for (int val;;)
	{
		is >> val;
		if (is.eof())
		{
			d = digit();
			return is;
		}
		if (is.bad() || is.fail())
		{
			is.clear();
			is.ignore(is.rdbuf()->in_avail());
		}
		else if (val >= 0 || val <= 9)
		{
			d.value = val;
			return is;
		}
	}
}

std::ostream& operator<< (std::ostream& os, const digit d)
{
	os << static_cast<int>(d.value);
	return os;
}

digit operator""_dgt(uint64_t value)
{
	return digit
	(
		static_cast<int8_t>(value > 9 ? value % 10 : value)
	);
}

digit operator""_dgt(char c)
{
	return digit(c);
}
