/* -*- c++ -*- */
/* 
 * Copyright 2018 Ahmet Inan, Ron Economos.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef PSK_HH
#define PSK_HH

#include "modulation.hh"
#include <gnuradio/math.h>

template <int NUM, typename TYPE>
struct PhaseShiftKeying;

template <typename TYPE>
struct PhaseShiftKeying<2, TYPE> : public Modulation<TYPE>
{
  static const int NUM = 2;
  static const int BITS = 1;
  typedef TYPE complex_type;
  typedef typename TYPE::value_type value_type;

  static constexpr value_type DIST = 2;

  int bits()
  {
    return BITS;
  }

  void hard(value_type *b, complex_type c, int stride = 1)
  {
    b[0*stride] = c.real() < value_type(0) ? value_type(-1) : value_type(1);
  }

  void soft(value_type *b, complex_type c, value_type precision, int stride = 1)
  {
    b[0*stride] = DIST * precision * c.real();
  }

  complex_type map(value_type *b, int stride = 1)
  {
    return complex_type(b[0*stride], 0);
  }
};

template <typename TYPE>
struct PhaseShiftKeying<4, TYPE> : public Modulation<TYPE>
{
  static const int NUM = 4;
  static const int BITS = 2;
  typedef TYPE complex_type;
  typedef typename TYPE::value_type value_type;

  // 1/sqrt(2)
  static constexpr value_type rcp_sqrt_2 = 0.70710678118654752440;
  static constexpr value_type DIST = 2 * rcp_sqrt_2;

  int bits()
  {
    return BITS;
  }

  void hard(value_type *b, complex_type c, int stride = 1)
  {
    b[0*stride] = c.real() < value_type(0) ? value_type(-1) : value_type(1);
    b[1*stride] = c.imag() < value_type(0) ? value_type(-1) : value_type(1);
  }

  void soft(value_type *b, complex_type c, value_type precision, int stride = 1)
  {
    b[0*stride] = DIST * precision * c.real();
    b[1*stride] = DIST * precision * c.imag();
  }

  complex_type map(value_type *b, int stride = 1)
  {
    return rcp_sqrt_2 * complex_type(b[0*stride], b[1*stride]);
  }
};

template <typename TYPE>
struct PhaseShiftKeying<8, TYPE> : public Modulation<TYPE>
{
  static const int NUM = 8;
  static const int BITS = 3;
  typedef TYPE complex_type;
  typedef typename TYPE::value_type value_type;

  // c(a(1)/2)
  static constexpr value_type cos_pi_8 = 0.92387953251128675613;
  // s(a(1)/2)
  static constexpr value_type sin_pi_8 = 0.38268343236508977173;
  // 1/sqrt(2)
  static constexpr value_type rcp_sqrt_2 = 0.70710678118654752440;

  static constexpr value_type DIST = 2 * sin_pi_8;

  gr_complex rot = (gr_complex)(std::exp(gr_complexd(0.0, -M_PI / 8)));

  int bits()
  {
    return BITS;
  }

  void hard(value_type *b, complex_type c, int stride = 1)
  {
    c *= rot;
    b[1*stride] = c.real() < value_type(0) ? value_type(-1) : value_type(1);
    b[2*stride] = c.imag() < value_type(0) ? value_type(-1) : value_type(1);
    b[0*stride] = std::abs(c.real()) < std::abs(c.imag()) ? value_type(-1) : value_type(1);
  }

  void soft(value_type *b, complex_type c, value_type precision, int stride = 1)
  {
    c *= rot;
    b[1*stride] = DIST * precision * c.real();
    b[2*stride] = DIST * precision * c.imag();
    b[0*stride] = DIST * precision * rcp_sqrt_2 * (std::abs(c.real()) - std::abs(c.imag()));
  }

  complex_type map(value_type *b, int stride = 1)
  {
    value_type real = cos_pi_8;
    value_type imag = sin_pi_8;
    if (b[0*stride] < value_type(0))
      std::swap(real, imag);
    return complex_type(real * b[1*stride], imag * b[2*stride]);
  }
};

#endif

