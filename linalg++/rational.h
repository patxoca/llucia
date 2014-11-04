/*****************************************************************************/
/* Name: rational.h                                                          */
/* Uses: Class for basic rational number arithmetic.                         */
/* Date: 4/19/2011                                                           */
/* Author: Andrew Que <http://www.DrQue.net/>                                */
/* Revisions:                                                                */
/*   2011/04/19 - QUE - Creation.                                            */
/*   2014/11/04 - ARV - Canvis variats                                       */
/*                                                                           */
/* License:                                                                  */
/*   This program is free software: you can redistribute it and/or modify    */
/*   it under the terms of the GNU General Public License as published by    */
/*   the Free Software Foundation, either version 3 of the License, or       */
/*   (at your option) any later version.                                     */
/*                                                                           */
/*   This program is distributed in the hope that it will be useful,         */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/*   GNU General Public License for more details.                            */
/*                                                                           */
/*   You should have received a copy of the GNU General Public License       */
/*   along with this program.  If not, see <http://www.gnu.org/licenses/>.   */
/*                                                                           */
/*                     (C) Copyright 2011 by Andrew Que                      */
/*                           http://www.DrQue.net/                           */
/*****************************************************************************/
#ifndef _RATIONAL_H_
#define _RATIONAL_H_

#include <cstdlib>  // For 'abs' on integer.
#include <ostream>  // For 'std::ostream'.
#include <limits>

//=============================================================================
// Rational number class.
//
// Background:
//    Rational numbers have the form of a/b, where 'a' is the numerator and
//  'b' is the denominator.  Using operations such as multiplication and
//  division by integer or rational numbers always results in an other rational
//  number.  In some cases, rational numbers may be more exact then floating
//  point values.  For example, 1/3 is approx 0.3333, but not exactly.  This
//  class will allow for the exact representation of rational values, which may
//  be useful for lengthy mathematical processes where error can add up, or
//  when an exact rational result is desired.
//
// Implementation:
//    This class is a template and requires a base type.  In most cases, this
//  will be 'int'.  However, unsigned and longs may also be useful depending
//  on the range and scale of the numbers worked with.
//    All basic arithmetic operators have been implemented (addition,
//  subtraction, multiplication, and division).  The assign operation, equal,
//  and not-equal logic operators are available.  Therefore this class may be
//  syntactically used like any number type.
//
// Notes:
//    This class has no overflow check, and is limited by the template type.
//  Problems can occur when adding or subtracting, as the values must be
//  multiplied up before adding can be done.
//=============================================================================
template< class TYPE = int>
	class Rational {
  protected:
  TYPE numerator;
  TYPE denominator;

  //-----------------------------------------------------------------------
  // Use:
  //   Put the rational into a standard form.  Reduce as much as possible.
  //
  // Standards:
  //   Zero is of the form 0/1.
  //   Divide-by-zero error will result in 0/0.
  //   All whole numbers will be in the form n/1.
  //   a/b will always be reduced.
  //-----------------------------------------------------------------------
  void reduce() {
	  if ( 0 == denominator ) {
		  // Divide-by-zero
          numerator = 0;
	  } else if ( 0 == numerator ) {
		  // Zero
		  denominator = 1;
	  } else if ( 0 == ( numerator % denominator ) ) {
		  // Whole number?
		  numerator   /= denominator;
		  denominator  = 1;
	  } else {
		  // Find Greatest Common Divisor (GCD) to reduce.
		  TYPE valueA = numerator;
		  TYPE valueB = denominator;
		  while ( valueA != 0 ) {
			  TYPE hold = valueA;
			  valueA = valueB % valueA;
			  valueB = hold;
		  }

		  // Reduce by GCD (thus, making the new GCF=1).
		  numerator   /= valueB;
		  denominator /= valueB;
	  }

	  // Keep the sign on the numerator.
	  if ( denominator < 0 ) {
          numerator   = -numerator;
          denominator = -denominator;
	  }

  } // reduce

  public:

  //=======================================================================
  // Set functions.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Set the numerator.
  // NOTE: Do not call this followed by setDenominator--fraction is
  // reduced.
  //-----------------------------------------------------------------------
  void setNumerator( TYPE const & value ) {
	  numerator = value;
	  reduce();
  }

  //-----------------------------------------------------------------------
  // Set the denominator.
  // NOTE: Do not call this followed by setNumerator--fraction is
  // reduced.  Example:
  //    Rational< int > value( 1, 3 );
  //    value.setNumerator( 3 )    // <- Rational will be 1/1
  //    value.setDenominator( 8 ); // <- Rational will not be 1/8, not 3/8
  //-----------------------------------------------------------------------
  void setDenominator ( TYPE const & value ) {
	  denominator = value;
	  reduce();
  }

  //-----------------------------------------------------------------------
  // Set rational to whole integer-type number.
  //-----------------------------------------------------------------------
  Rational const & operator = ( TYPE const & value ) {
	  numerator   = value;
	  denominator = 1;

	  return *this;
  }

  //-----------------------------------------------------------------------
  // Set rational to an other rational.
  //-----------------------------------------------------------------------
  Rational const & operator = ( Rational const & value ) {
	  numerator   = value.numerator;
	  denominator = value.denominator;
	  reduce();

	  return *this;
  }

  //=======================================================================
  // Multiplication.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Multiplication by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator * ( TYPE const & value ) const {
	  return Rational(numerator * value, denominator);
  }

  //-----------------------------------------------------------------------
  // Multiplication by rational.
  //-----------------------------------------------------------------------
  Rational const operator * ( Rational const & value ) const {
	  return Rational(
		  numerator * value.numerator,
		  denominator * value.denominator
		  );
  }

  //-----------------------------------------------------------------------
  // Multiply self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator *= ( Rational const & value ) {
	  *this = *this * value;

	  return *this;
  }

  //-----------------------------------------------------------------------
  // Multiply self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator *= ( TYPE const & value ) {
	  *this = *this * value;

	  return *this;
  }

  //=======================================================================
  // Division.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Divide by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator / ( TYPE const & value ) const {
	  return Rational(numerator, denominator * value);
  }

  //-----------------------------------------------------------------------
  // Divide by rational.
  //-----------------------------------------------------------------------
  Rational const operator / ( Rational const & value ) const {
	  return Rational(
		  numerator   * value.denominator,
		  denominator * value.numerator
          );
  }

  //-----------------------------------------------------------------------
  // Divide self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator /= ( Rational const & value ) {
	  *this = *this / value;

	  return *this;
  }

  //-----------------------------------------------------------------------
  // Divide self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator /= ( TYPE const & value ) {
	  *this = *this / value;

	  return *this;
  }

  //=======================================================================
  // Addition.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Add by rational.
  //-----------------------------------------------------------------------
  Rational const operator + ( Rational const & value ) const {
	  TYPE newNumerator   = numerator;
	  TYPE newDenominator = denominator;
	  TYPE otherNumerator = value.numerator;

	  if ( denominator != value.denominator ) {
          newNumerator   *= value.denominator;
          newDenominator *= value.denominator;
          otherNumerator *= denominator;
	  }

	  newNumerator += otherNumerator;

	  return Rational( newNumerator, newDenominator );
  }

  //-----------------------------------------------------------------------
  // Add by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator + ( TYPE const & value ) const {
	  return Rational( numerator + (value * denominator) , denominator );
  }

  //-----------------------------------------------------------------------
  // Add self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator += ( Rational const & value ) {
	  *this = *this + value;

	  return *this;
  }

  //-----------------------------------------------------------------------
  // Add self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator += ( TYPE const & value ) {
	  *this = *this + value;

	  return *this;
  }

  //=======================================================================
  // Subtractions.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Subtract by rational.
  //-----------------------------------------------------------------------
  Rational const operator - ( Rational const & value ) const {
	  TYPE newNumerator   = numerator;
	  TYPE newDenominator = denominator;
	  TYPE otherNumerator = value.numerator;

	  if ( denominator != value.denominator ) {
          newNumerator   *= value.denominator;
          newDenominator *= value.denominator;
          otherNumerator *= denominator;
	  }

	  newNumerator -= otherNumerator;

	  return Rational( newNumerator, newDenominator );
  }

  //-----------------------------------------------------------------------
  // Subtract by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator - ( TYPE const & value ) const {
	  return Rational(
		  numerator - (value * denominator) ,
		  denominator
          );
  }

  //-----------------------------------------------------------------------
  // Subtract self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator -= ( Rational const & value ) {
	  *this = *this - value;

	  return *this;
  }

  //-----------------------------------------------------------------------
  // Subtract self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator -= ( TYPE const & value ) {
	  *this = *this - value;

	  return *this;
  }

  //=======================================================================
  // Logic operators.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Equality operator test between self and rational.
  //-----------------------------------------------------------------------
  bool operator == ( Rational const & value ) const {
	  return ( ( numerator == value.numerator )
			   && ( denominator == value.denominator ) );
  }

  //-----------------------------------------------------------------------
  // Not equal operator test between self and rational.
  //-----------------------------------------------------------------------
  bool operator != ( Rational const & value ) const {
	  return !( *this == value );
  }

  //-----------------------------------------------------------------------
  // Equality operator test between self and integer-type.
  //-----------------------------------------------------------------------
  bool operator == ( TYPE const & value ) const {
	  return ( ( numerator == value )
			   && ( denominator == 1 ) );
  }

  //-----------------------------------------------------------------------
  // Not equal operator test between self and integer-type.
  //-----------------------------------------------------------------------
  bool operator != ( int const value ) const {
	  return !( *this == value );
  }

  //-----------------------------------------------------------------------
  // Greater than operator.
  //-----------------------------------------------------------------------
  bool operator > ( TYPE const & value ) const {
	  return ( numerator > ( value * denominator ) );
  }

  //-----------------------------------------------------------------------
  // Less than operator.
  //-----------------------------------------------------------------------
  bool operator < ( TYPE const & value ) const {
	  return ( numerator < ( value * denominator ) );
  }

  //-----------------------------------------------------------------------
  // Greater than operator.
  //-----------------------------------------------------------------------
  bool operator > ( Rational const & value ) const {
	  TYPE thisNumerator   = numerator;
	  TYPE otherNumerator  = value.numerator;

	  // If there isn't a common denominator...
	  if ( denominator != value.denominator ) {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
	  }

	  return ( thisNumerator > otherNumerator );
  }

  //-----------------------------------------------------------------------
  // Less than rational operator.
  //-----------------------------------------------------------------------
  bool operator < ( Rational const & value ) const {
	  TYPE thisNumerator   = numerator;
	  TYPE otherNumerator  = value.numerator;

	  // If there isn't a common denominator...
	  if ( denominator != value.denominator ) {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
	  }

	  return ( thisNumerator < otherNumerator );
  }

  //-----------------------------------------------------------------------
  // Greater or equal-to operator.
  //-----------------------------------------------------------------------
  bool operator >= ( TYPE const & value ) const {
	  return ( numerator >= ( value * denominator ) );
  }

  //-----------------------------------------------------------------------
  // Less or equal-to operator.
  //-----------------------------------------------------------------------
  bool operator <= ( TYPE const & value ) const {
	  return ( numerator <= ( value * denominator ) );
  }

  //-----------------------------------------------------------------------
  // Greater pr equal-to rational operator.
  //-----------------------------------------------------------------------
  bool operator >= ( Rational const & value ) const {
	  TYPE thisNumerator   = numerator;
	  TYPE otherNumerator  = value.numerator;

	  // If there isn't a common denominator...
	  if ( denominator != value.denominator ) {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
	  }

	  return ( thisNumerator >= otherNumerator );
  }

  //-----------------------------------------------------------------------
  // Less or equal-to operator.
  //-----------------------------------------------------------------------
  bool operator <= ( Rational const & value ) const {
	  TYPE thisNumerator   = numerator;
	  TYPE otherNumerator  = value.numerator;

	  // If there isn't a common denominator...
	  if ( denominator != value.denominator ) {
          // Create common denominator, and adjust numerators
          thisNumerator  *= value.denominator;
          otherNumerator *= denominator;
	  }

	  return ( thisNumerator <= otherNumerator );
  }

  //=======================================================================
  // Constructors.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Constructor from whole number.
  //-----------------------------------------------------------------------
  Rational ( TYPE const & value )
  : numerator( value ), denominator( 1 ) {}

  //-----------------------------------------------------------------------
  // Constructor two integers, numerator and denominator.
  //-----------------------------------------------------------------------
  Rational ( TYPE const & numeratorParameter, TYPE const & denominatorParameter )
  : numerator( numeratorParameter ), denominator( denominatorParameter )
  {
	  reduce();
  }

  //-----------------------------------------------------------------------
  // Constructor from rational.
  //-----------------------------------------------------------------------
  Rational ( Rational const & value ) {
	  *this = value;
  }

  //-----------------------------------------------------------------------
  // Empty constructor.
  //-----------------------------------------------------------------------
  Rational()
  :
  numerator( 0 ), denominator( 1 ) {}

  //=======================================================================
  // Get functions.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Return numerator.
  //-----------------------------------------------------------------------
  TYPE const & getNumerator() const {
	  return numerator;
  }

  //-----------------------------------------------------------------------
  // Return denominator.
  //-----------------------------------------------------------------------
  TYPE const & getDenominator() const {
	  return denominator;
  }

};

//-----------------------------------------------------------------------------
// Negative operator for a rational number.
//-----------------------------------------------------------------------------
template< class TYPE >
Rational< TYPE > operator - ( Rational< TYPE > const & value ) {
    return Rational< TYPE >( -value.getNumerator(), value.getDenominator() );
}

//-----------------------------------------------------------------------------
// Valor absolut per un racional
//-----------------------------------------------------------------------------
template< class TYPE >
Rational< TYPE > abs ( Rational< TYPE > const & value ) {
	return Rational< TYPE >( abs(value.getNumerator()), abs(value.getDenominator()) );
}

//-----------------------------------------------------------------------------
// Stream output operator for rational number--turns rational into a string.
//-----------------------------------------------------------------------------
template< class TYPE >
std::ostream & operator << ( std::ostream & stream, Rational< TYPE > const & value ) {
    // Get numerator and denominator from parameter.
    TYPE numerator   = value.getNumerator();
    TYPE denominator = value.getDenominator();

    if ( 0 == denominator )
		// Divide-by-zero error
		stream << "NaN";
    else if ( 1 == denominator )
		// Whole number
		stream << numerator;
	else if ( abs( numerator ) > denominator ) {
		// Whole number with fractional part
		TYPE whole;
		TYPE remainder;

		whole     = numerator / denominator;
		remainder = numerator % denominator;

		if ( remainder < 0 )
			remainder = -remainder;

		stream << whole << " " << remainder << "/" << denominator;
	} else
		// Just a fraction.
		stream << numerator << "/" << denominator;

    return stream;
}

#endif // _RATIONAL_H_
