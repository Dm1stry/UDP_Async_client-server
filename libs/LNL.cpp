#include "LNL.h"

// Opening namespace "LNL" (short for "LongNumberLib")
namespace LNL {

std::vector<int> LongInt::primes;

void LongInt::addPrime(int num) {
	bool isPrime = true;
	for (const int& prime : primes) {
		if (num % prime == 0 || num == prime) {
			isPrime = false;
		}
	}

	if (isPrime) {
		primes.push_back(num);
	}
}

void LongInt::fillPrimes() {
	if (primes.size() != 0) {
		return;
	}

	primes.push_back(2);
	primes.push_back(3);
	//(6k+i) mod 6
	for (int i = 5; i < PRIMES_MAX; i+= PRIMES_STEP) {
		addPrime(i);
		addPrime(i + 2);
	}
}

// Structure for restoring factors
struct factor_t {
	LongInt first;
	LongInt second;
};

// Structure for quotient and remainder for LongInt div() function
struct lidiv_t {
	LongInt quot;
	LongInt rem;
};

// Structure for _isPowerOfTen() function
struct isPowerOfTen_t {
	bool isPowerOfTen;
	long long power;
};

// Chesks if LongInt number have leading zeroes and remove them
void LongInt::_checkLeadingZeroes() {
	for (long long i = size() - 1; i > 0; i--) {
		if (_digits[i] == 0) {
			_digits.pop_back();
		} else {
			break;
		}
	}
}

// Correct LongInt number's digits if they are beyond 9 or below 0
void LongInt::_checkDigitOverflow() {
	for (long long i = 0; i < size() - 1; i++) {
		while (_digits[i] > 9) {
			_digits[i] -= 10;
			_digits[i + 1] += 1;
		}
		while (_digits[i] < 0) {
			_digits[i] += 10;
			_digits[i + 1] -= 1;
		}
	}
	if (_digits[size() - 1] > 9) {
		_digits[size() - 1] -= 10;
		_digits.push_back(1);
	}
	while (_digits[size() - 2] > 9) {
		_digits[size() - 2] -= 10;
		_digits[size() - 1] += 1;
	}
}

// Correct LongInt number if it is equal to -0 to 0
void LongInt::_checkNegativeZero() {
	if (isZero()) {
		_positive = true;
	}
}

/**
 * Compare two LongInt numbers (1st relative to 2nd).
 *
 * @param firstNum First number to be compared.
 * @param secondNum Second number to be compared.
 * @return 1 if 1st number is grater than 2nd.
 * 		   2 if numbers are equal.
 * 		  -1 if 1st number is less than 2nd.
 */
int LongInt::_compare(const LongInt& firstNum, const LongInt& secondNum) {
	// Signs: + -
	if (firstNum.isPositive() and secondNum.isNegative()) {
		return 1;
	}
	// Signs: - +
	if (firstNum.isNegative() and secondNum.isPositive()) {
		return -1;
	}
	// Signs: + +
	if (firstNum.isPositive() and secondNum.isPositive()) {
		if (firstNum.size() > secondNum.size()) {
			return 1;
		} else if (firstNum.size() < secondNum.size()) {
			return -1;
		} else {
			for (long long i = firstNum.size() - 1; i >= 0; i--) {
				if (firstNum._digits[i] > secondNum._digits[i]) {
					return 1;
				} else if (firstNum._digits[i] < secondNum._digits[i]) {
					return -1;
				}
			}
		}
	}
	// Signs: - -
	if (firstNum.isNegative() and secondNum.isNegative()) {
		if (firstNum.size() > secondNum.size()) {
			return -1;
		} else if (firstNum.size() < secondNum.size()) {
			return 1;
		} else {
			for (long long i = firstNum.size() - 1; i >= 0; i--) {
				if (firstNum._digits[i] > secondNum._digits[i]) {
					return -1;
				} else if (firstNum._digits[i] < secondNum._digits[i]) {
					return 1;
				}
			}
		}
	}
	return 0; // If numbers are equal
}

/**
 * Auxiliary function for sum().
 *
 * Sums 2 positive numbers (biggerNum >= smallerNum).
 * 
 * @param biggerNum First number for summation.
 * @param smallerNum Second number for summation.
 * @see sum
 */
LongInt LongInt::_sumAux(const LongInt& biggerNum, const LongInt& smallerNum) {
	LongInt result = biggerNum;
	for (long long i = 0; i < smallerNum.size(); i++) {
		result._digits[i] += smallerNum._digits[i];
	}
	result._checkDigitOverflow();
	return result;
}

/**
 * Auxiliary function for sum().
 *
 * Finds the difference of 2 positive numbers (biggerNum >= smallerNum).
 * 
 * @param biggerNum First number for subtraction.
 * @param smallerNum Second number for subtraction.
 * @see sum
 */
LongInt LongInt::_diffAux(const LongInt& biggerNum, const LongInt& smallerNum) {
	LongInt result = biggerNum;
	for (long long i = 0; i < smallerNum.size(); i++) {
		result._digits[i] -= smallerNum._digits[i];
	}
	result._checkDigitOverflow();
	result._checkLeadingZeroes();
	return result;
}

/**
 * Auxiliary function (default multiplication).
 *
 * Multiply two LontInt numbers.
 * 
 * @param firstNum First number for multiplication.
 * @param secondNum Second number for multiplication.
 * @return Member of LongInt class as a result of multiplication.
 */
LongInt LongInt::_multAux(const LongInt& firstNum, const LongInt& secondNum) {
	LongInt result;
	result._digits.resize(firstNum.size() + secondNum.size());
	for (long long i = 0; i < secondNum.size(); i++) {
		for (long long j = 0; j < firstNum.size(); j++) {
			result._digits[i + j] += secondNum._digits[i] * firstNum._digits[j];
		}
	}
	result._checkDigitOverflow();
	result._checkLeadingZeroes();
	return result;
}

/**
 * Implementation of Karatsuba algorithm for fast multiplication. 
 *
 * Implements Karatsuba algorithm for fast multiplication. 
 * Both params are absolute numbers.
 * 
 * @param firstNum First number for multiplication.
 * @param secondNum Second number for multiplication.
 * @return Member of LongInt class as a result of multiplication.
 */
LongInt LongInt::_karatsubaAlg(const LongInt& firstNum, const LongInt& secondNum) {
	// If it is faster to multiply as usual
	if ((firstNum.size() < 100) or (secondNum.size() < 100)) {
		return _multAux(firstNum, secondNum);
	}
	// Algorithm
	LongInt result;
	long long base = std::max(firstNum.size(), secondNum.size());
	long long halfBase = base / 2;
	LongInt A = firstNum / pow(LongInt(10), halfBase);
	LongInt B = firstNum % pow(LongInt(10), halfBase);
	LongInt C = secondNum / pow(LongInt(10), halfBase);
	LongInt D = secondNum % pow(LongInt(10), halfBase);
	LongInt AC = _karatsubaAlg(A, C);
	LongInt BD = _karatsubaAlg(B, D);
	LongInt ADplusBC = _karatsubaAlg(A + B, C + D) - AC - BD;
	result = AC * pow(LongInt(10), 2 * halfBase) + (ADplusBC * pow(LongInt(10), halfBase)) + BD;
	return result;
}

/**
 * Checks if number is power of 10.
 * 
 * @see isPowerOfTen_t
 * 
 * @param number Numbet to be checked.
 * @return Member of isPowerOfTen_t structure.
 */
isPowerOfTen_t LongInt::_isPowerOfTen(const LongInt& number) {
	bool isPowerOfTen = true;
	long long power = 0;
	for (long long i = 0; i < number.size() - 1; i++) {
		if (number._digits[i] != 0) {
			isPowerOfTen = false;
			break;
		} else {
			power++;
		}
	}
	if (number._digits[number.size() - 1] != 1) {
		isPowerOfTen = false;
	}
	return {isPowerOfTen, power};
}

// Default constructor
LongInt::LongInt() {
	_digits.push_back(0); // Number is set to 0 by default
	_positive = true;
}

// Constructor for short
LongInt::LongInt(signed short input) {
	*this = input;
}

// Constructor for unsigned short
LongInt::LongInt(unsigned short input) {
	*this = input;
}

// Constructor for int
LongInt::LongInt(signed int input) {
	*this = input;
}

// Constructor for unsigned int
LongInt::LongInt(unsigned int input) {
	*this = input;
}

// Constructor for long
LongInt::LongInt(signed long input) {
	*this = input;
}

// Constructor for unsigned long
LongInt::LongInt(unsigned long input) {
	*this = input;
}

// Constructor for long long
LongInt::LongInt(signed long long input) {
	*this = input;
}

// Constructor for unsigned long long
LongInt::LongInt(unsigned long long input) {
	*this = input;
}

// Constructor for string
LongInt::LongInt(const std::string& input) {
	*this = input;
}

// Get number as a string
std::string LongInt::toString() const {
	std::string output;
	for (const auto &digit: _digits) {
		output.push_back(digit + '0');
	}
	if (isNegative()) {
		output.push_back('-');
	}
	std::reverse(output.begin(), output.end());
	return output;
}

// Get absolute number value
LongInt LongInt::abs() const {
	LongInt result;
	result = *this;
	result._positive = true;
	return result;
}

// Get number size
long long LongInt::size() const {
	return _digits.size();
}

// Get number size, synonym to size()
long long LongInt::length() const {
	return _digits.size();
}

// Is number even?
bool LongInt::isEven() const {
	return (_digits[0] % 2 == 0);
}

// Is number odd?
bool LongInt::isOdd() const {
	return !isEven();
}

// Is number equal to 0?
bool LongInt::isZero() const {
	if ((size() == 1) and (_digits[0] == 0)) {
		return true;
	}
	return false;
}

// Is number equal to 1?
bool LongInt::isOne() const {
	if ((size() == 1) and (_digits[0] == 1) and _positive) {
		return true;
	}
	return false;
}

// Is number positive?
bool LongInt::isPositive() const {
	return _positive;
}

// Is number negative?
bool LongInt::isNegative() const {
	return !_positive;
}

// Get number value as a string
std::string toString(const LongInt& number) {
	return number.toString();
}

// Get absolute number value
LongInt abs(const LongInt& number) {
	return number.abs();
}

// Get number size
long long size(const LongInt& number) {
	return number.size();
}

// Get number size, synonym to size()
long long length(const LongInt& number) {
	return number.length();
}

// Is number even?
bool isEven(const LongInt& number) {
	return number.isEven();
}

// Is number odd?
bool isOdd(const LongInt& number) {
	return number.isOdd();
}

// Is number equal to 0?
bool isZero(const LongInt& number){
	return number.isZero();
}

// Is number equal to 1?
bool isOne(const LongInt& number) {
	return number.isOne();
}

// Is number positive?
bool isPositive(const LongInt& number) {
	return number.isPositive();
}

// Is number negative?
bool isNegative(const LongInt& number) {
	return number.isNegative();
}

// Input stream operator
std::istream& operator >>(std::istream& in, LongInt& number) {
	std::string input;
	in >> input;
	number = input;
	return in;
}

// Output stream operator
std::ostream& operator <<(std::ostream& out, const LongInt& number) {
	out << number.toString();
	return out;
}

// Set number value with short
LongInt& LongInt::operator =(signed short input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with unsigned short
LongInt& LongInt::operator =(unsigned short input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with int
LongInt& LongInt::operator =(signed int input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with unsigned int
LongInt& LongInt::operator =(unsigned int input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with long
LongInt& LongInt::operator =(signed long input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with unsigned long
LongInt& LongInt::operator =(unsigned long input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with long long
LongInt& LongInt::operator =(signed long long input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with unsigned long long
LongInt& LongInt::operator =(unsigned long long input) {
	*this = std::to_string(input);
	return *this;
}

// Set number value with string
LongInt& LongInt::operator =(const std::string& input) {
	_digits.erase(_digits.begin(), _digits.end());
	if (input.substr(0, 1) == "-") {
		_positive = false;
	} else {
		_positive = true;
	}
	for (long long i = 0; i < input.size(); i++) {
		if ((i == 0) and !_positive) {
			continue;
		}
		// In case of a non-digit character
		if ((input[i] < 48) or (input[i]  > 57)) {
			*this = "0";
			std::cout << "ERROR: A non-digit character \"" << input[i] ;
			std::cout << "\" is encountered when assigning a value of a LongInt with a string.";
			std::cout << " Value is set to 0 by default.\n";
			return *this;
		}
		_digits.push_back(input[i] - '0');
	}
	std::reverse(_digits.begin(), _digits.end());
	_checkLeadingZeroes();
	_checkNegativeZero();
	return *this;
}

// Set number value with LongInt
LongInt& LongInt::operator =(const LongInt& input) {
	_digits.erase(_digits.begin(), _digits.end());
	_digits = input._digits;
	_positive = input._positive;
	return *this;
}

// Is this number equal to 2nd?
bool LongInt::operator ==(const LongInt& secondNum) const {
	if ((this->isPositive() != secondNum.isPositive()) or (this->size() != secondNum.size())) {
		return false;
	}
	for (long long i = 0; i < this->size(); i++) {
		if (this->_digits[i] != secondNum._digits[i]) {
			return false;
		}
	}
	return true;
}

// Is this number not equal to 2nd?
bool LongInt::operator !=(const LongInt& secondNum) const {
	return !(*this == secondNum);
}

// Is this number greater than 2nd?
bool LongInt::operator >(const LongInt& secondNum) const {
	switch(LongInt::_compare(*this, secondNum)) {
		case 1: return true; // Greater
		case 0: return false; // Equal
		case -1: return false; // Less
	}
	return false; // Duplicating '0' case to avoid warning
}

// Is this number greater or equal to 2nd?
bool LongInt::operator >=(const LongInt& secondNum) const {
	switch(LongInt::_compare(*this, secondNum)) {
		case 1: return true; // Greater
		case 0: return true; // Equal
		case -1: return false; // Less
	}
	return true; // Duplicating '0' case to avoid warning
}

// Is this number less than 2nd?
bool LongInt::operator <(const LongInt& secondNum) const {
	switch(LongInt::_compare(*this, secondNum)) {
		case 1: return false; // Greater
		case 0: return false; // Equal
		case -1: return true; // Less
	}
	return false; // Duplicating '0' case to avoid warning
}

// Is this number less or equal to 2nd?
bool LongInt::operator <=(const LongInt& secondNum) const {
	switch(LongInt::_compare(*this, secondNum)) {
		case 1: return false; // Greater
		case 0: return true; // Equal
		case -1: return true; // Less
	}
	return true; // Duplicating '0' case to avoid warning
}

// Return max number
LongInt max(const LongInt& firstNum, const LongInt& secondNum) {
	if (firstNum >= secondNum) {
		return firstNum;
	}
	return secondNum;
}

// Return min number
LongInt min(const LongInt& firstNum, const LongInt& secondNum) {
	if (firstNum <= secondNum) {
		return firstNum;
	}
	return secondNum;
}

// Sum of 2 numbers
LongInt LongInt::operator +(const LongInt& secondNum) const {
	LongInt result;
	if (this->isPositive() == secondNum.isPositive()) {
		// If (+,+) or (-,-)
		if (*this >= secondNum) {
			result = _sumAux(*this, secondNum);
		} else {
			result = _sumAux(secondNum, *this);
		}
	} else if (this->isPositive() and secondNum.isNegative()) {
		// If (+,-)
		if (*this >= secondNum.abs()) {
			result = _diffAux(*this, secondNum.abs());
		} else {
			result = _diffAux(secondNum.abs(), *this);
			result._positive = false;
		}
	} else {
		// If (-,+)
		if (this->abs() > secondNum) {
			result = _diffAux(this->abs(), secondNum);
			result._positive = false;
		} else {
			result = _diffAux(secondNum, this->abs());
		}
	}
	result._checkNegativeZero();
	return result;
}

// Difference of 2 numbers
LongInt LongInt::operator -(const LongInt& secondNum) const {
	LongInt temp = secondNum;
	temp._positive = !temp.isPositive();
	return *this + temp;
}

// Product of 2 numbers
LongInt LongInt::operator *(const LongInt& secondNum) const {
	// Checking multiplication by zero
	if (this->isZero() or secondNum.isZero()) {
		return LongInt(0);
	}
	// Checking multiplication by 10 in some power
	isPowerOfTen_t thisInfo = _isPowerOfTen(*this);
	isPowerOfTen_t secondInfo = _isPowerOfTen(secondNum);
	// Algorithm
	LongInt result;
	if (thisInfo.isPowerOfTen) {
		// Multiplication if firstNum is a 10 in some power
		result._digits.resize(thisInfo.power);
		result._digits.insert(result._digits.end(), secondNum._digits.begin(), secondNum._digits.end());
		result._positive = (this->isPositive() == secondNum.isPositive());
		return result;
	} else if (secondInfo.isPowerOfTen) {
		// Multiplication if secondNum is a 10 in some power
		result._digits.resize(secondInfo.power);
		result._digits.insert(result._digits.end(), this->_digits.begin(), this->_digits.end());
		result._positive = (this->isPositive() == secondNum.isPositive());
		return result;
	} else {
		// General multiplication
		result = _karatsubaAlg(this->abs(), secondNum.abs());
		result._positive = (this->isPositive() == secondNum.isPositive());
		return result;
	}
	return result;
}

// Quotient of 2 numbers
LongInt LongInt::operator /(const LongInt& secondNum) const {
	lidiv_t temp = div(*this, secondNum);
	return temp.quot;
}

// Remainder of 2 numbers division
LongInt LongInt::operator %(const LongInt& secondNum) const {
	lidiv_t temp = div(*this, secondNum);
	return temp.rem;
}

// Add another number
LongInt& LongInt::operator +=(const LongInt& secondNum) {
	return *this = *this + secondNum;
}

// Subtract another number
LongInt& LongInt::operator -=(const LongInt& secondNum) {
	return *this = *this - secondNum;
}

// Multiply by another number
LongInt& LongInt::operator *=(const LongInt& secondNum) {
	return *this = *this * secondNum;
}

// Divide by another number
LongInt& LongInt::operator /=(const LongInt& secondNum) {
	return *this = *this / secondNum;
}

// Remainder of division by another number
LongInt& LongInt::operator %=(const LongInt& secondNum) {
	return *this = *this % secondNum;
}

// Return this number (unary plus)
LongInt LongInt::operator +() {
	return *this;
}

// Add 1 to the number (prefix)
LongInt& LongInt::operator ++() {
	return *this = *this + LongInt("1");
}

// Add 1 to the number (postfix)
LongInt LongInt::operator ++(int) {
	return *this = *this + LongInt("1");
}

// Return opposite sign number (unary minus)
LongInt LongInt::operator -() {
	LongInt result;
	result = *this;
	result._positive = !isPositive();
	result._checkNegativeZero();
	return result;
}

// Subtract 1 from the number (prefix)
LongInt& LongInt::operator --() {
	return *this = *this - LongInt("1");
}

// Subtract 1 to the number (postfix)
LongInt LongInt::operator --(int) {
	return *this = *this - LongInt("1");
}

// Logical right-shift operator 
LongInt LongInt::operator >>(LongInt number) {
	return *this = *this / pow(2, number);
}

// Logical left-shift operator 
LongInt LongInt::operator <<(LongInt number) {
	return *this = *this * pow(2, number);
}

/**
 * Calculate power of two LongInt numbers.
 *
 * @param firstNum Number to be raised.
 * @param secondNum The power to which we raise.
 * @return 'firstNum' to the power of 'secondNum'.
 */
LongInt pow(const LongInt& firstNum, const LongInt& secondNum) {
	// If to the power of negative number
	if (secondNum.isNegative()) {
		std::cout << "ERROR: Unable to raise LongInt to the power of negative number!\n";
		return LongInt(0);
	}
	LongInt result;
	// If in to the power of 0
	if (secondNum.isZero()) {
		return LongInt(1);
	}
	// If in to the power of 1
	if (secondNum.isOne()) {
		return firstNum;
	}
	// If 0 in some power
	if (firstNum.isZero()) {
		return LongInt(0);
	}
	// If 1 or -1 in some power
	if (isOne(abs(firstNum))) {
		result = firstNum;
		if (secondNum.isEven()) {
			result._positive = true;
		}
		return result;
	}
	// Checking if firstNum is a 10 to some power
	isPowerOfTen_t firstInfo = LongInt::_isPowerOfTen(firstNum);
	// Algorithm
	LongInt power = secondNum;
	if (firstInfo.isPowerOfTen) {
		// If firstNum is a 10 in some power
		result._digits.resize(firstInfo.power * std::stoi(toString(secondNum)));
		result._digits.push_back(1);
		result._positive = firstNum.isPositive();
		if (firstNum.isNegative() and power.isEven()) {
			result._positive = true;
		}
	} else {
		// General case pow()
		result = pow(firstNum * firstNum, power / 2);
		if (power.isOdd()) {
			result *= firstNum;
		}
	}
	return result;
}

/**
 * Calculate result of division of two LongInt numbers.
 *
 * @see lidiv_t
 * 
 * @param firstNum Number to be devided.
 * @param secondNum Number by which we divide.
 * @return Member of lidiv_t structure as a result of division.
 */
lidiv_t div(const LongInt& firstNum, const LongInt& secondNum) {
	lidiv_t result;
	// Checking division by zero
	if (secondNum == LongInt(0)) {
		std::cout << "ERROR: Division by zero!\n";
		return {LongInt(0), LongInt(0)};
	}
	// Checking division by one
	if (secondNum.abs() == LongInt(1)) {
		if (secondNum.isPositive()) {
			return {firstNum, LongInt(0)};
		} else {
			result.quot = firstNum;
			result.quot._positive = false;
			result.rem = LongInt(0);
			return result;
		}
	}
	// If the divisor is bigger than divident (in absolute values)
	if (secondNum.abs() > firstNum.abs()) {
		return {LongInt(0), firstNum};
	}
	// If the divisor is equal to divident (in absolute values)
	if (secondNum.abs() == firstNum.abs()) {
		if (firstNum.isPositive() == secondNum.isPositive()) {
			return {LongInt(1), LongInt(0)};
		} else {
			return {LongInt(-1), LongInt(0)};
		}
	}
	// Checking if divisor is a 10 to some power
	isPowerOfTen_t secondInfo = LongInt::_isPowerOfTen(secondNum);
	// Algorithm
	if (secondInfo.isPowerOfTen) {
		// Division if secondNum is a 10 in some power
		result.rem._digits = std::vector<int>(firstNum._digits.begin(), firstNum._digits.begin() + secondInfo.power);
		result.quot._digits = std::vector<int>(firstNum._digits.begin() + secondInfo.power, firstNum._digits.end());
		// Checking signs
		result.quot._positive = (firstNum.isPositive() == secondNum.isPositive());
		result.rem._positive = firstNum.isPositive();
		result.rem._checkLeadingZeroes();
		result.rem._checkNegativeZero();
	} else {
		// General division
		result.rem = firstNum.abs();
		LongInt divisor = secondNum.abs();
		long long sizeDiff = firstNum.size() - secondNum.size();
		while (sizeDiff >= 0) {
			LongInt remStep = divisor * pow(LongInt(10), sizeDiff);
			while (result.rem >= remStep) {
				result.quot += pow(LongInt(10), sizeDiff);
				result.rem -= remStep;
			}
			sizeDiff--;
		}
		result.quot._positive = (firstNum.isPositive() == secondNum.isPositive());
		result.rem._positive = firstNum.isPositive();
	}
	return result;
}

/**
 * Calculate remainder of division of two LongInt numbers.
 *
 * @param firstNum Number to be devided.
 * @param secondNum Number by which we divide.
 * @return Remainder of dividing 'firstNum' by 'secondNum'.
 */
LongInt mod(const LongInt& firstNum, const LongInt& secondNum) {
	LongInt result;
	// If modulo zero
	if (secondNum.isZero()) {
		std::cout << "ERROR: Modulo zero is an undefined value!\n";
		return result;
	}
	// If modulo of zero
	if (firstNum.isZero()) {
		return firstNum;
	}
	// If no need for division
	if (firstNum.abs() < secondNum.abs()) {
		result = firstNum;
		if (firstNum.isPositive() != secondNum.isPositive()) {
			result += secondNum;
		}
		return result;
	} else {
		// If firstNum is bigger or equal to secondNum
		lidiv_t divResults = div(firstNum, secondNum);
		if (firstNum.isPositive() != secondNum.isPositive()) {
			divResults.rem += secondNum;
		}
		return divResults.rem;
	}
	return result;
}

/**
 * Calculate greatest common divisor of two LongInt numbers.
 *
 * @param firstNum First number to calculate gcd of.
 * @param secondNum Second number to calculate gcd of.
 * @return Greatest common divisor of 'firstNum' and 'secondNum'.
 */
LongInt gcd(const LongInt& firstNum, const LongInt& secondNum) {
	LongInt first = firstNum;
	LongInt second = secondNum;
	first._positive = true;
	second._positive = true;
	// If first is zero
	if (first.isZero()) {
		if (second.isZero()) {
			std::cout << "ERROR: Unable to find the greatest common divisor as both numbers are zero!\n";
			return LongInt(0);
		}
		return second;
	}
	// If second is zero
	if (second.isZero()) {
		return first;
	}
	// Algorithm
	while (!first.isZero() and !second.isZero()) {
		if (first > second) {
			first %= second;
		} else {
			second %= first;
		}
	}
	return first + second;
}

/**
 * Calculate least common multiple of two LongInt numbers.
 *
 * @param firstNum First number to calculate lcm of.
 * @param secondNum Second number to calculate lcm of.
 * @return Least common multiple of 'firstNum' and 'secondNum'.
 */
LongInt lcm(const LongInt& firstNum, const LongInt& secondNum) {
	LongInt first = firstNum;
	LongInt second = secondNum;
	first._positive = true;
	second._positive = true;
	// If one of the numbers is zero
	if (first.isZero() or second.isZero()) {
		return LongInt(0);
	}
	return first * second / gcd(first, second);
}

/**
 * Calculate factoirial of LongInt number.
 *
 * @param number Number to generates its factorial.
 * @return factorial of 'number'.
 */
LongInt factorial(const LongInt& number) {
	LongInt result(1);
	for (LongInt i = 1; i <= number; i++) {
		result *= i;
	}
	return result;
}

/**
 * Generates random LongInt number.
 *
 * @return random generated member of LongInt class.
 */
LongInt random() {
	std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(10000, 100000);

	LongInt randomNumber(distrib(gen));

	randomNumber = pow(randomNumber, 10);
	randomNumber = randomNumber << 2;
	randomNumber = pow(randomNumber, 20);
	randomNumber = randomNumber >> 1;

	if (distrib(gen) % 2 == 1)
	{
		randomNumber *= -1;
	}

	return randomNumber;
}

// Generates random LongInt in some range
LongInt random(const LongInt& left, const LongInt& right) {
	std::random_device rd;
    std::mt19937 gen(rd());

	LongInt diff = right - left;

	if (diff < INT_MAX) {
		std::uniform_int_distribution<> distrib(0, std::stoi(diff.toString()));
		return left + distrib(gen);
	} else {
		std::uniform_int_distribution<> distrib(0, INT_MAX);
		int size = distrib(gen) % (diff.size() + 1);	//random size from 0 to diff.size()
		if (size == 0) {
			return left;
		}
		LongInt displacement;
		displacement._digits.resize(size);
		for (int i = 0; i < size; i++) {
			int randNum = distrib(gen);
			while (randNum > 0 && i < size) {
				displacement._digits[i++] = randNum % 10;
				randNum /= 10;
			}
		}
		if (displacement > diff) {
			int delDigits = distrib(gen) % (diff.size()) + 1;	//random size from 1 to diff.size() for deletion
			displacement._digits.resize(delDigits);
		}
		if (displacement.size() == 0) {
			return left;
		}

		displacement._checkLeadingZeroes();
		displacement._checkDigitOverflow();

		return left + displacement;
	}
}

// Generates random LongInt number with specified size
LongInt random(const long long size) {
	std::random_device rd;
    std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 1000);

	LongInt result;
	result._digits.resize(size);
	
	for (long long i = 0; i < size; i++) {
		result._digits[i] = distrib(gen) % 10;
	}

	while (result._digits[size - 1] == 0) {
		result._digits[size - 1] = distrib(gen) % 10;
	}

	return result;
}

// Miller-Rabin primality test
bool LongInt::isMillerRabinPrime() const {
	fillPrimes();

	//Check small primes
	for (const int& prime : primes) {
		if (*this % prime == 0 && *this != prime) {
			return false;
		} else if (*this == prime) {
			return true;
		}
	}

	//Miller-Rabin test
	// n-1 = 2^s * t => s and t
	LongInt n_1 = *this - 1;
	int s = -1;
	LongInt t;
	lidiv_t divRes = {n_1, 0};

	LongInt two(2);
	while (divRes.rem != 1) {
		t = divRes.quot;
		divRes = div(t, two);
		s++;
	}
	
	//find out recommended attempts (according to wiki) ~ log2(n) ~ log2(n - 1) if n >> 1
	int primeCheckAttempts = s; 
	LongInt tmp = t;
	do {
		divRes = div(tmp, two);
		tmp = divRes.quot;
		primeCheckAttempts++;
	} while(tmp != 0);

	//primeCheckAttemts tests
	for (int i = 0; i < primeCheckAttempts; i++) {
		//random
		LongInt a = random(2, *this - 2);
		LongInt x = modPow(a, t, *this);	//a^t mod n

		if (x == 1 || x == *this - 1) {
			continue;
		}

		for (int j = 0; j < s; j++) {
			x = (x * x) % *this;
			if (x == 1) {
				return false;
			}
			if (x == *this - 1) {
				break;
			}
		}
		if (x == *this - 1) {
			continue;
		}

		return false;
	}

	return true;
}

// Returns 2 factors of number
factor_t LongInt::factor() const {
	fillPrimes();

	for (const int& prime : primes) {
		if (*this % prime == 0) {
			return {prime, *this / prime};
		} else if (*this < (prime * prime)) {
			return {1, *this};
		}
	}

	LongInt delimeter(PRIMES_MAX);
	while ((delimeter * delimeter) <= *this) {
		if (*this % delimeter == 0) {
			return {delimeter, *this / delimeter};
		}

		if (delimeter % 2 == 0) {
			delimeter += 1;
			continue;
		}

		//delimeter % 6 = +-1
		if (delimeter % PRIMES_STEP == 1) {
			delimeter += 4;
			continue;
		}

		delimeter += 2;
	}

	return {1, *this};
}

// Generates random prime for encryption algorithms
LongInt randomPrime(long long size) {
	// generate random number with size
	LongInt li = random(size);

	//make it prime
	while (!li.isMillerRabinPrime()) {
		if (li % 2 == 0) {
			li++;	// if even make odd
			continue;
		}
		// check numbers 6k +- 1
		if (li % 6 == 1) {	//make li % 6 =  5;
			li += 4;
			continue;
		}

		li += 2;	// was li % 6 = 5 -> li % 6 = 1
	}

	return li;
}

// Modular exponentiation by 2 to some power
LongInt modPowTwo(const LongInt& li, const long long pow, const LongInt& modular) {
	LongInt result = li % modular;

	for (long long i = 0; i < pow; i++) {
		result = (result * result) % modular;
	}

	return result;
}

// Modular exponentiation
LongInt modPow(const LongInt& li, LongInt pow, const LongInt& modular) {
	if (li == 0 || modular == 0) {
		return 0;
	}

	LongInt two(2);
	lidiv_t powDecomposition = div(pow, two);
	pow = powDecomposition.quot;
	LongInt modPowTwo = li % modular;
	LongInt result = powDecomposition.rem == 1 ? modPowTwo : 1;
	while (pow != 0) {
		powDecomposition = div(pow, two);
		pow = powDecomposition.quot;
		modPowTwo = (modPowTwo * modPowTwo) % modular;
		if (powDecomposition.rem == 1) {
			result = (result * modPowTwo) % modular;
		}
	}

	return result;
}

// Caesar's cipher encryption
void shiftEncrypt(LongInt& li, long long key) {
	key %= 10;
	for (long long i = 0; i < li.size(); i++) {
		li._digits[i] = (li._digits[i] + key) % 10;
		if (li._digits[i] < 0) {
			li._digits[i] += 10;
		}
	}
}

// Caesar's cipher decryption
void shiftDecrypt(LongInt& li, long long key) {
	key %= 10;
	for (long long i = 0; i < li.size(); i++) {
		li._digits[i] = (li._digits[i] - key) % 10;
		if (li._digits[i] < 0) {
			li._digits[i] += 10;
		}
	}
}

} // Closing namespace "LNL" (short for "LongNumberLib")
