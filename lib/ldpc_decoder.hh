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

#ifndef LDPC_HH
#define LDPC_HH

#include "exclusive_reduce.hh"

template <typename TYPE>
struct MinSumCAlgorithm
{
  static TYPE correction_factor(TYPE a, TYPE b)
  {
    if (1) {
      TYPE c = 0.5;
      TYPE apb = std::abs(a + b);
      TYPE amb = std::abs(a - b);
      if (apb < TYPE(2) && amb > TYPE(2) * apb)
        return c;
      if (amb < TYPE(2) && apb > TYPE(2) * amb)
        return -c;
      return 0;
    }
    return std::log(TYPE(1)+std::exp(-std::abs(a+b))) - std::log(TYPE(1)+std::exp(-std::abs(a-b)));
  }
  static TYPE min(TYPE a, TYPE b)
  {
    TYPE m = std::min(std::abs(a), std::abs(b));
    TYPE x = (a < TYPE(0)) != (b < TYPE(0)) ? -m : m;
    x += correction_factor(a, b);
    return x;
  }
  static void finalp(TYPE *links, int cnt)
  {
    TYPE tmp[cnt];
    CODE::exclusive_reduce(links, tmp, cnt, min);
    for (int i = 0; i < cnt; ++i)
      links[i] = tmp[i];
  }
  static TYPE add(TYPE a, TYPE b)
  {
    return a + b;
  }
};

template <typename TYPE>
struct LDPCInterface
{
  virtual int code_len() = 0;
  virtual int data_len() = 0;
  virtual void decode(TYPE *, TYPE *) = 0;
  virtual ~LDPCInterface() = default;
};

template <typename TABLE, typename TYPE>
class LDPC : public LDPCInterface<TYPE>
{
  static const int M = TABLE::M;
  static const int N = TABLE::N;
  static const int K = TABLE::K;
  static const int R = N-K;
  static const int q = R/M;
  static const int CNL = TABLE::LINKS_MAX_CN;
  int acc_pos[TABLE::DEG_MAX];
  const int *row_ptr;
  int bit_deg;
  int grp_cnt;
  int grp_num;
  int grp_len;
  TYPE bnl[TABLE::LINKS_TOTAL];
  TYPE bnv[N];
  TYPE cnl[R * CNL];
  int cnv[R];
  int cnc[R];
  MinSumCAlgorithm<TYPE> alg;

  int signum(TYPE v)
  {
    return (v > TYPE(0)) - (v < TYPE(0));
  }

  void next_group()
  {
    if (grp_cnt >= grp_len) {
      grp_len = TABLE::LEN[grp_num];
      bit_deg = TABLE::DEG[grp_num];
      grp_cnt = 0;
      ++grp_num;
    }
    for (int i = 0; i < bit_deg; ++i)
      acc_pos[i] = row_ptr[i];
    row_ptr += bit_deg;
    ++grp_cnt;
  }

  void next_bit()
  {
    for (int i = 0; i < bit_deg; ++i)
      acc_pos[i] += q;
    for (int i = 0; i < bit_deg; ++i)
      acc_pos[i] %= R;
  }

  void first_group()
  {
    grp_num = 0;
    grp_len = 0;
    grp_cnt = 0;
    row_ptr = TABLE::POS;
    next_group();
  }

  void bit_node_init(TYPE *data, TYPE *parity)
  {
    for (int i = 0; i < R; ++i)
      bnv[i] = parity[i];
    for (int i = 0; i < K; ++i)
      bnv[i+R] = data[i];

    TYPE *bl = bnl;
    for (int i = 0; i < R-1; ++i) {
      *bl++ = parity[i];
      *bl++ = parity[i];
    }
    *bl++ = parity[R-1];
    first_group();
    for (int j = 0; j < K; j += M) {
      for (int m = 0; m < M; ++m) {
        for (int n = 0; n < bit_deg; ++n)
          *bl++ = data[j+m];
        next_bit();
      }
      next_group();
    }
  }

  void check_node_update()
  {
    cnv[0] = signum(bnv[0]);
    for (int i = 1; i < R; ++i)
      cnv[i] = signum(bnv[i-1]) * signum(bnv[i]);

    TYPE *bl = bnl;
    cnl[0] = *bl++;
    cnc[0] = 1;
    for (int i = 1; i < R; ++i) {
      cnl[CNL*i] = *bl++;
      cnl[CNL*i+1] = *bl++;
      cnc[i] = 2;
    }
    first_group();
    for (int j = 0; j < K; j += M) {
      for (int m = 0; m < M; ++m) {
        for (int n = 0; n < bit_deg; ++n) {
          int i = acc_pos[n];
          cnv[i] *= signum(bnv[j+m+R]);
          cnl[CNL*i+cnc[i]++] = *bl++;
        }
        next_bit();
      }
      next_group();
    }
    for (int i = 0; i < R; ++i)
      alg.finalp(cnl+CNL*i, cnc[i]);
  }

  void bit_node_update()
  {
    bnv[0] += cnl[0] + cnl[CNL];
    for (int i = 1; i < R-1; ++i)
      bnv[i] += cnl[CNL*i+1] + cnl[CNL*(i+1)];
    bnv[R-1] += cnl[CNL*(R-1)+1];

    TYPE *bl = bnl;
    cnc[0] = 1;
    for (int i = 1; i < R; ++i)
      cnc[i] = 2;
    *bl++ += cnl[CNL];
    *bl++ += cnl[0];
    for (int i = 1; i < R-1; ++i) {
      *bl++ += cnl[CNL*(i+1)];
      *bl++ += cnl[CNL*i+1];
    }
    ++bl; // alone
    first_group();
    for (int j = 0; j < K; j += M) {
      for (int m = 0; m < M; ++m) {
        TYPE inp[bit_deg];
        for (int n = 0; n < bit_deg; ++n) {
          int i = acc_pos[n];
          inp[n] = cnl[CNL*i+cnc[i]++];
        }
        TYPE out[bit_deg];
        CODE::exclusive_reduce(inp, out, bit_deg, alg.add);
        bnv[j+m+R] += out[0] + inp[0];
        for (int n = 0; n < bit_deg; ++n)
          *bl++ += out[n];
        next_bit();
      }
      next_group();
    }
    if (0) {
      static int count;
      std::cout << count++;
      for (int i = 0; i < N; ++i)
        std::cout << " " << std::min<TYPE>(std::max<TYPE>(bnv[i], -1000000), 1000000);
      std::cout << std::endl;
    }
  }

  bool hard_decision()
  {
    for (int i = 0; i < R; ++i)
      if (cnv[i] <= 0)
        return true;
    return false;
  }

  void update_user(TYPE *data, TYPE *parity)
  {
    for (int i = 0; i < R; ++i)
      parity[i] = bnv[i];
    for (int i = 0; i < K; ++i)
      data[i] = bnv[i+R];
  }

public:

  int code_len()
  {
    return N;
  }

  int data_len()
  {
    return K;
  }

  void decode(TYPE *data, TYPE *parity)
  {
    bit_node_init(data, parity);
    check_node_update();
    int count = 0, trials = 50;
    while (count < trials && hard_decision()) {
      bit_node_update();
      check_node_update();
      bit_node_init(data, parity);
      ++count;
    }
    bit_node_update();
    update_user(data, parity);
  }
};

#endif
