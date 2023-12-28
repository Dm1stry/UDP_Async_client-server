// Copyright 2022 Mikhail Oborotov, Ivan Borisenkov, Elveg Khunshaev
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LNL_H
#define LNL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <climits>
#include <string>

// Opening namespace "LNL" (short for "LongNumberLib")
namespace LNL {


/**
 * Structure for restoring fuctors of a LongInt
 * 
 * 
*/
struct factor_t;

/**
 * Structure for quotient and remainder for LongInt div() function.
 * 
 * Stores quotient and remainder after division of two numbers.
 * 
 * @see div
 */
struct lidiv_t;

/**
 * Structure for _isPowerOfTen() function.
 * 
 * Stores whether the number is a power of ten
 * and if it is then the power itself.
 * 
 * @see _isPowerOfTen
 */
struct isPowerOfTen_t;

/**
 * Implementation of arithmetic for big numbers.
 *
 * Stores a large number and allows you to construct big number, 
 * perform any arithmetic operations with it and output it to console.
 *
 */
class LongInt {
private:
	static std::vector<int> primes;
	static void addPrime(int num);
	static void fillPrimes();

	static const int PRIMES_MAX = 100000;
	static const int PRIMES_STEP = 6;

	std::vector<int> _digits; // Number as a vector of digits
	bool _positive; // Is number positive?
	void _checkLeadingZeroes(); // Remove leading zeroes
	void _checkDigitOverflow(); // Correct digits if they are beyond 9 or below 0
	void _checkNegativeZero(); // Correct -0 to 0
	static int _compare(const LongInt& firstNum, const LongInt& secondNum); // Compare numbers
	static LongInt _sumAux(const LongInt& biggerNum, const LongInt& smallerNum); // Auxiliary function
	static LongInt _diffAux(const LongInt& biggerNum, const LongInt& smallerNum); // Auxiliary function
	static LongInt _multAux(const LongInt& firstNum, const LongInt& secondNum); // Auxiliary function
	static LongInt _karatsubaAlg(const LongInt& firstNum, const LongInt& secondNum); // Karatsuba algorithm
	static isPowerOfTen_t _isPowerOfTen(const LongInt& number); // Is number a power of 10?
public:
	LongInt(); // Default constructor
	LongInt(signed short input); // Constructor for short
	LongInt(unsigned short input); // Constructor for unsigned short
	LongInt(signed int input); // Constructor for int
	LongInt(unsigned int input); // Constructor for unsigned int
	LongInt(signed long input); // Constructor for long
	LongInt(unsigned long input); // Constructor for unsigned long
	LongInt(signed long long input); // Constructor for long long
	LongInt(unsigned long long input); // Constructor for unsigned long long
	LongInt(const std::string& input); // Constructor for string
	std::string toString() const; // Get number value as a string
	LongInt abs() const; // Get absolute number value
	long long size() const; // Get number size
	long long length() const; // Get number size
	bool isEven() const; // Is number even?
	bool isOdd() const; // Is number odd?
	bool isZero() const; // Is number equal to 0?
	bool isOne() const; // Is number equal to 1?
	bool isPositive() const; // Is number positive?
	bool isNegative() const; // Is number negative?
	friend std::string toString(const LongInt& number); // Get number value as a string
	friend LongInt abs(const LongInt& number); // Get absolute number value
	friend long long size(const LongInt& number); // Get number size
	friend long long length(const LongInt& number); // Get number size
	friend bool isEven(const LongInt& number); // Is number even?
	friend bool isOdd(const LongInt& number); // Is number odd?
	friend bool isZero(const LongInt& number); // Is number equal to 0?
	friend bool isOne(const LongInt& number); // Is number equal to 1?
	friend bool isPositive(const LongInt& number); // Is number positive?
	friend bool isNegative(const LongInt& number); // Is number negative?
	friend std::istream& operator >>(std::istream& in, LongInt& number); // Input stream operator
	friend std::ostream& operator <<(std::ostream& out, const LongInt& number); // Output stream operator
	LongInt& operator =(signed short input); // Set number value with short
	LongInt& operator =(unsigned short input); // Set number value with unsigned short
	LongInt& operator =(signed int input); // Set number value with int
	LongInt& operator =(unsigned int input); // Set number value with unsigned int
	LongInt& operator =(signed long input); // Set number value with long
	LongInt& operator =(unsigned long input); // Set number value with unsigned long
	LongInt& operator =(signed long long input); // Set number value with long long
	LongInt& operator =(unsigned long long input); // Set number value with unsigned long long
	LongInt& operator =(const std::string& input); // Set number value with string
	LongInt& operator =(const LongInt& input); // Set number value with LongInt
	bool operator ==(const LongInt& secondNum) const; // Is this number equal to 2nd?
	bool operator !=(const LongInt& secondNum) const; // Is this number not equal to 2nd?
	bool operator >(const LongInt& secondNum) const; // Is this number greater than 2nd?
	bool operator >=(const LongInt& secondNum) const; // Is this number greater or equal to 2nd?
	bool operator <(const LongInt& secondNum) const; // Is this number less than 2nd?
	bool operator <=(const LongInt& secondNum) const; // Is this number less or equal to 2nd?
	friend LongInt max(const LongInt& firstNum, const LongInt& secondNum); // Return max number
	friend LongInt min(const LongInt& firstNum, const LongInt& secondNum); // Return min number
	LongInt operator +(const LongInt& secondNum) const; // Sum of 2 numbers
	LongInt operator -(const LongInt& secondNum) const; // Difference of 2 numbers
	LongInt operator *(const LongInt& secondNum) const; // Product of 2 numbers
	LongInt operator /(const LongInt& secondNum) const; // Quotient of 2 numbers division
	LongInt operator %(const LongInt& secondNum) const; // Remainder of 2 numbers division
	LongInt& operator +=(const LongInt& secondNum); // Add another number
	LongInt& operator -=(const LongInt& secondNum); // Subtract another number
	LongInt& operator *=(const LongInt& secondNum); // Multiply by another number
	LongInt& operator /=(const LongInt& secondNum); // Divide by another number
	LongInt& operator %=(const LongInt& secondNum); // Remainder of division by another number
	LongInt operator +(); // Return this number (unary plus)
	LongInt& operator ++(); // Add 1 to the number (prefix)
	LongInt operator ++(int); // Add 1 to the number (postfix)
	LongInt operator -(); // Return opposite sign number (unary minus)
	LongInt& operator --(); // Subtract 1 from the number (prefix)
	LongInt operator --(int); // Subtract 1 to the number (postfix)
	LongInt operator >> (LongInt number); // Logical right-shift operator 
	LongInt operator << (LongInt number); // Logical left-shift operator 
	friend LongInt pow(const LongInt& firstNum, const LongInt& secondNum); // firstNum to the power of secondNum
	friend lidiv_t div(const LongInt& firstNum, const LongInt& secondNum); // Division
	friend LongInt mod(const LongInt& firstNum, const LongInt& secondNum); // Modulo
	friend LongInt gcd(const LongInt& firstNum, const LongInt& secondNum); // Greatest common divisor
	friend LongInt lcm(const LongInt& firstNum, const LongInt& secondNum); // Least common multiple
	friend LongInt factorial(const LongInt& number); // Factorial of a number
	friend LongInt random(); // Generates random LongInt number
	friend LongInt random(const LongInt& left, const LongInt& right); // Generates random LongInt in some range
	friend LongInt random(const long long size); // Generates random LongInt number with specified size
	bool isMillerRabinPrime() const; // Miller-Rabin primality test
	factor_t factor() const; // Returns 2 factors of number
	friend LongInt randomPrime(long long size); // Generates random prime for encryption algorithms
	friend LongInt modPowTwo(const LongInt& li, const long long pow, const LongInt& modular); // Modular exponentiation by 2 to some power
	friend LongInt modPow(const LongInt& li, LongInt pow, const LongInt& modular); // Modular exponentiation
	friend void shiftEncrypt(LongInt& li, long long key); // Caesar's cipher encryption
	friend void shiftDecrypt(LongInt& li, long long key); // Caesar's cipher decryption
};
} // Closing namespace "LNL" (short for "LongNumberLib")

#endif // LNL_H