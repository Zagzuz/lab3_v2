#include "ldecimal.hpp"

bool operator> (const ldecimal &lhs, const ldecimal &rhs)
{
	if (lhs.sign == ldecimal::plus && rhs.sign == ldecimal::minus) return true;
	if (lhs.sign == ldecimal::minus && rhs.sign == ldecimal::plus) return false;

	if (lhs.value.size() > rhs.value.size()) return true;
	if (lhs.value.size() < rhs.value.size()) return false;

	for (auto lhsIt = lhs.value.cbegin(), rhsIt = rhs.value.cbegin();
		lhsIt != lhs.value.cend();
		++lhsIt, ++rhsIt)
	{
		if (*lhsIt > *rhsIt) 
			return true;
	}

	return false;
}

bool operator< (const ldecimal &lhs, const ldecimal &rhs)
{
	if (lhs.sign == ldecimal::plus && rhs.sign == ldecimal::minus) return false;
	if (lhs.sign == ldecimal::minus && rhs.sign == ldecimal::plus) return true;

	if (lhs.value.size() > rhs.value.size()) return false;
	if (lhs.value.size() < rhs.value.size()) return true;

	for (auto lhsIt = lhs.value.cbegin(), rhsIt = rhs.value.cbegin();
		lhsIt != lhs.value.cend();
		++lhsIt, ++rhsIt)
	{
		if (*lhsIt < *rhsIt)
			return true;
	}

	return false;
}

bool operator== (const ldecimal &lhs, const ldecimal &rhs)
{
	if (lhs.value.size() != rhs.value.size() || lhs.sign != rhs.sign)
		return false;

	for (auto lhsIt = lhs.value.cbegin(), rhsIt = rhs.value.cbegin();
		lhsIt != lhs.value.cend();
		++lhsIt, ++rhsIt)
	{
		if (*lhsIt != *rhsIt)
			return false;
	}

	return true;
}

bool operator!= (const ldecimal& lhs, const ldecimal& rhs)
{
	if (lhs.value.size() != rhs.value.size() || lhs.sign != rhs.sign)
		return true;

	for (auto lhsIt = lhs.value.cbegin(), rhsIt = rhs.value.cbegin();
		lhsIt != lhs.value.cend();
		++lhsIt, ++rhsIt)
	{
		if (*lhsIt != *rhsIt)
			return true;
	}

	return false;
}

ldecimal operator+ (ldecimal lhs, ldecimal rhs)
{
	if (lhs.value.empty()) return rhs;
	if (rhs.value.empty()) return lhs;

	std::size_t maxlen = std::max(lhs.value.size(), rhs.value.size());
	if (lhs.sign == rhs.sign) ++maxlen;

	ldecimal::sign_t lhs_sign = lhs.sign, rhs_sign = rhs.sign, res_sign;
	lhs.sign = rhs.sign = ldecimal::plus;
	res_sign = (lhs > rhs ? lhs_sign : rhs_sign);

	while (lhs.value.size() < maxlen) lhs.value.push_front(0);
	while (rhs.value.size() < maxlen) rhs.value.push_front(0);

	lhs.sign = lhs_sign;
	rhs.sign = rhs_sign;
	lhs.to_ones_complement();
	rhs.to_ones_complement();

	ldecimal res;
	res.sign = res_sign;
	digit cash = 0;

	for (auto lhsIt = lhs.value.rbegin(), rhsIt = rhs.value.rbegin();
		lhsIt != lhs.value.rend(); 
		++lhsIt, ++rhsIt)
	{
		digit sum = *lhsIt + *rhsIt + cash;

		cash = (static_cast<int>(*lhsIt) + static_cast<int>(*rhsIt) +
			static_cast<int>(cash)) / 10;

		res.value.push_front(sum);
	}

	if (res_sign == ldecimal::minus) res.to_ones_complement();

	while (res.value.empty() == false && res.value.front() == 0)
		res.value.pop_front();

	return res;
}

ldecimal operator-(ldecimal lhs, ldecimal rhs)
{
	if (lhs.value.empty()) return rhs;
	if (rhs.value.empty()) return lhs;

	rhs.sign = (rhs.sign == ldecimal::plus ? ldecimal::minus : ldecimal::plus);

	std::size_t maxlen = std::max(lhs.value.size(), rhs.value.size());
	if (lhs.sign == rhs.sign) ++maxlen;

	ldecimal::sign_t lhs_sign = lhs.sign, rhs_sign = rhs.sign, res_sign;
	lhs.sign = rhs.sign = ldecimal::plus;
	res_sign = (lhs > rhs ? lhs_sign : rhs_sign);

	while (lhs.value.size() < maxlen) lhs.value.push_front(0);
	while (rhs.value.size() < maxlen) rhs.value.push_front(0);

	lhs.sign = lhs_sign;
	rhs.sign = rhs_sign;
	lhs.to_ones_complement();
	rhs.to_ones_complement();

	ldecimal res;
	res.sign = res_sign;
	digit cash = 0;

	for (auto lhsIt = lhs.value.rbegin(), rhsIt = rhs.value.rbegin();
		lhsIt != lhs.value.rend();
		++lhsIt, ++rhsIt)
	{
		digit sum = *lhsIt + *rhsIt + cash;

		cash = (static_cast<int>(*lhsIt) + static_cast<int>(*rhsIt) +
			static_cast<int>(cash)) / 10;

		res.value.push_front(sum);
	}

	if (res_sign == ldecimal::minus) res.to_ones_complement();

	while (res.value.empty() == false && res.value.front() == 0)
		res.value.pop_front();

	return res;
}

std::istream& operator>> (std::istream& is, ldecimal& ld)
{
	ld.value.clear();
	ld.sign = ldecimal::plus;

	std::string strnum;
	is >> strnum;

	if (is.eof())
	{
		ld.value.push_back(0);
		return is;
	}
	
	if (is.bad() || is.fail())
	{
		is.clear();
		is.ignore(is.rdbuf()->in_avail());
		ld.value.push_back(0);
		return is;
	}
	
	bool non_zero_found = false;

	for (const auto& ch : strnum)
	{
		if (&ch == &strnum[0] && strnum.length() > 1
			&& (ch == '-' || ch == '+'))
		{
			if (ch == '-') ld.sign = ldecimal::minus;
			continue;
		}

		if (ch == '0' && non_zero_found == false) continue;
		else if (ch >= '0' && ch <= '9')
		{
			non_zero_found = true;
			ld.value.push_back(ch - '0');
		}
		else
		{
			ld.value.clear();
			ld.value.push_back(0);
			break;
		}
	}

	return is;
}

std::ostream& operator<< (std::ostream &os, const ldecimal &ld)
{
	if (ld.sign == ldecimal::minus) os << '-';
	if (ld.value.empty()) os << '0';
	else std::for_each(ld.value.cbegin(), ld.value.cend(), [&os](digit d) { os << d; });
	return os;
}

ldecimal operator""_ld(uint64_t v)
{
	while (v > INT64_MAX) v /= 10;
	return ldecimal(static_cast<int64_t>(v));
}

ldecimal& ldecimal::to_ones_complement()
{
	if (sign == plus) return *this;
	for (auto& dgt : value) 
		dgt.to_ones_complement();
	*this -= 1;	// since *this is negative
	return *this;
}

ldecimal::ldecimal(digit d) : sign(plus)
{
	value.push_back(d);
}

ldecimal::ldecimal(int64_t v) : sign(v < 0 ? minus : plus)
{
	for (; v; v /= 10)
		value.push_front(
			digit(
				static_cast<int>(v % 10)
			)
		);
}

ldecimal::ldecimal(const std::string &strnum) : ldecimal()
{
	if (strnum.empty() == true)
		std::cerr << "[ERROR] empty string found, "
			"number value is set to '+0'.\n";

	for (const auto& ch : strnum)
	{
		if (&ch == &strnum[0] && strnum.length() > 1 
			&& (ch == '-' || ch == '+'))
		{
			sign = minus;
			continue;
		}

		if (ch >= '0' && ch <= '9') value.push_back(ch - '0');
		else
		{
			std::cerr << "[ERROR] non-digit character found, "
				"number value is set to '+0'.\n";
			value.clear();
			value.push_back(0);
			break;
		}
	}
}

void ldecimal::divide_by_10()
{
	if (value.size() > 1) 
		value.pop_back();
}

void ldecimal::multiply_by_10()
{
	if (value.size() != 0)
		value.push_back(0);
}

ldecimal ldecimal::operator~ () const
{
	if (sign == plus) return *this;
	ldecimal ld(*this);
	for (const auto dgt : value)
		ld.value.push_front(~dgt);
	return ld;
}

ldecimal& ldecimal::operator+= (digit rhs_dgt)
{
	if (value.empty())
	{
		value.push_back(rhs_dgt);
		return *this;
	}

	if (sign == minus && value.size() >= 1 && value.back() >= rhs_dgt ||
		value.size() == 1 && value.back() < rhs_dgt)
	{
		value.back() -= rhs_dgt;
		return *this;
	}
	else if (sign == minus && value.size() > 1 && value.back() < rhs_dgt)
	{
		*std::prev(value.end(), 2) -= 1;
		value.back() += digit(10 - static_cast<int>(rhs_dgt));
		return *this;
	}

	digit cash = 0;

	for (auto lhs_dgt_it = value.rbegin(); lhs_dgt_it != value.rend(); ++lhs_dgt_it)
	{
		digit sum = *lhs_dgt_it + cash +
			(lhs_dgt_it == value.rbegin() ? rhs_dgt : 0);

		cash = (static_cast<int>(*lhs_dgt_it) + static_cast<int>(cash) +
			static_cast<int>(lhs_dgt_it == value.rbegin() ? rhs_dgt : 0)) / 10;

		*lhs_dgt_it = sum;
	}

	if (cash != 0) value.push_front(cash);

	return *this;
}

ldecimal& ldecimal::operator-= (digit rhs_dgt)
{
	if (value.empty())
	{
		value.push_back(rhs_dgt);
		return *this;
	}

	if (sign == plus && value.size() >= 1 && value.back() >= rhs_dgt ||
		value.size() == 1 && value.back() < rhs_dgt)
	{
		value.back() -= rhs_dgt;
		return *this;
	}
	else if (sign == plus && value.size() > 1 && value.back() < rhs_dgt)
	{
		*std::prev(value.end(), 2) -= 1;
		value.back() += digit(10 - static_cast<int>(rhs_dgt));
		return *this;
	}

	digit cash = 0;

	for (auto lhs_dgt_it = value.rbegin(); lhs_dgt_it != value.rend(); ++lhs_dgt_it)
	{
		digit sum = *lhs_dgt_it + cash +
			(lhs_dgt_it == value.rbegin() ? rhs_dgt : 0);

		cash = (static_cast<int>(*lhs_dgt_it) + static_cast<int>(cash) +
			static_cast<int>(lhs_dgt_it == value.rbegin() ? rhs_dgt : 0)) / 10;

		*lhs_dgt_it = sum;
	}

	if (cash != 0) value.push_front(cash);

	return *this;
}

ldecimal& ldecimal::operator= (const ldecimal& ld)
{
	if (&*this != &ld)
	{
		value = ld.value;
		sign = ld.sign;
	}

	return *this;
}
