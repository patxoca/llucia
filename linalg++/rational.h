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
//
//    La versió original simplificava (reduce()) la fracció resultant de cada
//  operació. Açò suposa un cost computacional molt elevat. Per accelerar els
//  càlculs s'ha eliminat l'autosimplificació.
//
//    La simplificació, a més d'eliminar els factors comuns entre el numerador
//  i el denominador, normalitzava certes fraccions (0/n -> 0/1, n/0 -> 0/0)
//  i, sobretot, garantía que en les fraccions negatives el signe aparegués en
//  el numerador. Açò últim és necessari per garantir la correció de la suma i
//  la resta. Aquestes comprovacions s'han mogut al constructor.
//
//    En cas de modificar el codi els mètodes que reben una fracció poden
//  assumir que el signe està en el numerador i han de garantir que en el
//  resutat que produeixen el signe també està en el numerador.
//
//    Pel que fa a la simplificació pròpiament dita cal fer-la manualment quan
//  es consideri adient, per exemple al acabar un càlcul complex. Cal tindre
//  en compte però que si no es simplifica, depenent dels valors i les
//  operacions, es poden produir desbordaments, obtenint resultat errònis.
//
//    Com cas especial, al fer E/S amb fraccions utilitzant l'operador << la
//  fraccio es mostra simplificada. Com que la E/S és costosa de per sí aquest
//  overhead es considera acceptable.
//
//=============================================================================
template< class TYPE = int, TYPE L = 127 > class Rational {

protected:

  TYPE numerator;
  TYPE denominator;

  void reduce_maybe() {
      TYPE abs_num = numerator >= 0 ? numerator : -numerator;
      //TYPE abs_den = denominator >= 0 ? denominator : -denominator;
      if ((abs_num >= L) || (denominator >= L)) {
          reduce();
      }
  }

  TYPE binary_gcd(TYPE u, TYPE v) {
      int shift;
      TYPE tmp;

      if (u < 0) u = -u;
      if (v < 0) v = -v;

      if (u == 0) return v;
      if (v == 0) return u;

      for (shift = 0; ((u | v) & 1) == 0; ++shift) {
          u >>= 1;
          v >>= 1;
      }

      do {
          while ((u & 1) == 0) {
              u >>= 1;
          }
          while ((v & 1) == 0) {
              v >>= 1;
          }
          if (u >= v) {
              u = (u - v) >> 1;
          } else {
              tmp = u;
              u = (v - u) >> 1;
              v = tmp;
          }
      } while (u != 0);

      return v << shift;
  }

public:

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

  //=======================================================================
  // Set functions.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Set the numerator.
  //-----------------------------------------------------------------------
  void setNumerator( TYPE const & value ) {
      numerator = value;
  }

  //-----------------------------------------------------------------------
  // Set the denominator.
  //-----------------------------------------------------------------------
  void setDenominator ( TYPE const & value ) {
      denominator = value;
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

      return *this;
  }

  //=======================================================================
  // Multiplication.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Multiplication by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator * ( TYPE const & value ) const {
      Rational r(numerator * value, denominator);
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Multiplication by rational.
  //-----------------------------------------------------------------------
  Rational const operator * ( Rational const & value ) const {
      Rational r(
          numerator * value.numerator,
          denominator * value.denominator
          );
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Multiply self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator *= ( Rational const & value ) {
      numerator *= value.numerator;
      denominator *= value.denominator;
      reduce_maybe();
      return *this;
  }

  //-----------------------------------------------------------------------
  // Multiply self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator *= ( TYPE const & value ) {
      numerator *= value;
      reduce_maybe();
      return *this;
  }

  //=======================================================================
  // Division.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Divide by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator / ( TYPE const & value ) const {
      Rational r(numerator, denominator * value);
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Divide by rational.
  //-----------------------------------------------------------------------
  Rational const operator / ( Rational const & value ) const {
      Rational r(
          numerator   * value.denominator,
          denominator * value.numerator
          );
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Divide self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator /= ( Rational const & value ) {
      if (value.numerator < 0) {
          numerator *= -value.denominator;
          denominator *= -value.numerator;
      } else {
          numerator *= value.denominator;
          denominator *= value.numerator;
      }
      reduce_maybe();
      return *this;
  }

  //-----------------------------------------------------------------------
  // Divide self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator /= ( TYPE const & value ) {
      if (value < 0) {
          numerator = -numerator;
          denominator *= -value;
      } else {
          denominator *= value;
      }
      reduce_maybe();
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

      Rational r( newNumerator, newDenominator );
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Add by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator + ( TYPE const & value ) const {
      Rational r( numerator + (value * denominator) , denominator );
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Add self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator += ( Rational const & value ) {
      TYPE newNumerator   = numerator;
      TYPE newDenominator = denominator;
      TYPE otherNumerator = value.numerator;

      if ( denominator != value.denominator ) {
          newNumerator   *= value.denominator;
          newDenominator *= value.denominator;
          otherNumerator *= denominator;
      }

      newNumerator += otherNumerator;

      numerator = newNumerator;
      denominator = newDenominator;
      reduce_maybe();

      return *this;
  }

  //-----------------------------------------------------------------------
  // Add self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator += ( TYPE const & value ) {
      numerator += value * denominator;
      reduce_maybe();

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

      Rational r( newNumerator, newDenominator );
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Subtract by integer-type.
  //-----------------------------------------------------------------------
  Rational const operator - ( TYPE const & value ) const {
      Rational r(
          numerator - (value * denominator) ,
          denominator
          );
      r.reduce_maybe();
      return r;
  }

  //-----------------------------------------------------------------------
  // Subtract self by rational.
  //-----------------------------------------------------------------------
  Rational const & operator -= ( Rational const & value ) {
      TYPE newNumerator   = numerator;
      TYPE newDenominator = denominator;
      TYPE otherNumerator = value.numerator;

      if ( denominator != value.denominator ) {
          newNumerator   *= value.denominator;
          newDenominator *= value.denominator;
          otherNumerator *= denominator;
      }

      newNumerator -= otherNumerator;

      numerator =newNumerator;
      denominator = newDenominator;
      reduce_maybe();

      return *this;
  }

  //-----------------------------------------------------------------------
  // Subtract self by integer-type.
  //-----------------------------------------------------------------------
  Rational const & operator -= ( TYPE const & value ) {
      numerator -= value * denominator;
      reduce_maybe();
      return *this;
  }

  //=======================================================================
  // Logic operators.
  //=======================================================================

  //-----------------------------------------------------------------------
  // Equality operator test between self and rational.
  //-----------------------------------------------------------------------
  bool operator == ( Rational const & value ) const {
      return ( numerator * value.denominator == denominator * value.numerator );
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
      return ( numerator == value * denominator );
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
  {
      if (denominatorParameter == 0) {
          numerator = 0;
          denominator = 0;
      } else {
          if (numeratorParameter == 0) {
              numerator = 0;
              denominator = 1;
          } else if (denominatorParameter < 0) {
              numerator = -numeratorParameter;
              denominator = -denominatorParameter;
          } else {
              numerator = numeratorParameter;
              denominator = denominatorParameter;
          }
      }
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
  : numerator( 0 ), denominator( 1 ) {}

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
template< class TYPE, TYPE L >
Rational< TYPE , L > operator - ( Rational< TYPE, L > const & value ) {
    return Rational< TYPE, L >( -value.getNumerator(), value.getDenominator() );
}

//-----------------------------------------------------------------------------
// Valor absolut per un racional
//-----------------------------------------------------------------------------
template< class TYPE, TYPE L >
Rational< TYPE, L > abs ( Rational< TYPE, L > const & value ) {
    return Rational< TYPE, L >( abs(value.getNumerator()), abs(value.getDenominator()) );
}

//-----------------------------------------------------------------------------
// Stream output operator for rational number--turns rational into a string.
//-----------------------------------------------------------------------------
template< class TYPE, TYPE L >
std::ostream & operator << ( std::ostream & stream, Rational< TYPE, L > const & value ) {
    Rational< TYPE, L > value2 = Rational< TYPE, L >(value);
    TYPE numerator;
    TYPE denominator;

    // Simplifiquem una copia de la fraccio. L'E/S és lenta de per sí, ens
    // podem permetre la sobrecàrrega de simplificar.
    //value2.reduce();
    numerator   = value2.getNumerator();
    denominator = value2.getDenominator();

    if ( 0 == denominator )
        // Divide-by-zero error
        stream << "NaN";
    else if ( 1 == denominator )
        // Whole number
        stream << numerator;
    else
        // Just a fraction.
        stream << numerator << "/" << denominator;

    return stream;
}

#endif // _RATIONAL_H_
