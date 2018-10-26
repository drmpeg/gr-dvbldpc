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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "ldpc_decoder_fb_impl.h"
#include <cmath>
#include <iostream>
#include <algorithm>

constexpr int DVB_S2_TABLE_B1::DEG[];
constexpr int DVB_S2_TABLE_B1::LEN[];
constexpr int DVB_S2_TABLE_B1::POS[];

constexpr int DVB_S2_TABLE_B2::DEG[];
constexpr int DVB_S2_TABLE_B2::LEN[];
constexpr int DVB_S2_TABLE_B2::POS[];

constexpr int DVB_S2_TABLE_B3::DEG[];
constexpr int DVB_S2_TABLE_B3::LEN[];
constexpr int DVB_S2_TABLE_B3::POS[];

constexpr int DVB_S2_TABLE_B4::DEG[];
constexpr int DVB_S2_TABLE_B4::LEN[];
constexpr int DVB_S2_TABLE_B4::POS[];

constexpr int DVB_S2_TABLE_B5::DEG[];
constexpr int DVB_S2_TABLE_B5::LEN[];
constexpr int DVB_S2_TABLE_B5::POS[];

constexpr int DVB_S2_TABLE_B6::DEG[];
constexpr int DVB_S2_TABLE_B6::LEN[];
constexpr int DVB_S2_TABLE_B6::POS[];

constexpr int DVB_S2_TABLE_B7::DEG[];
constexpr int DVB_S2_TABLE_B7::LEN[];
constexpr int DVB_S2_TABLE_B7::POS[];

constexpr int DVB_S2_TABLE_B8::DEG[];
constexpr int DVB_S2_TABLE_B8::LEN[];
constexpr int DVB_S2_TABLE_B8::POS[];

constexpr int DVB_S2_TABLE_B9::DEG[];
constexpr int DVB_S2_TABLE_B9::LEN[];
constexpr int DVB_S2_TABLE_B9::POS[];

constexpr int DVB_S2_TABLE_B10::DEG[];
constexpr int DVB_S2_TABLE_B10::LEN[];
constexpr int DVB_S2_TABLE_B10::POS[];

constexpr int DVB_S2_TABLE_B11::DEG[];
constexpr int DVB_S2_TABLE_B11::LEN[];
constexpr int DVB_S2_TABLE_B11::POS[];

constexpr int DVB_S2_TABLE_C1::DEG[];
constexpr int DVB_S2_TABLE_C1::LEN[];
constexpr int DVB_S2_TABLE_C1::POS[];

constexpr int DVB_S2_TABLE_C2::DEG[];
constexpr int DVB_S2_TABLE_C2::LEN[];
constexpr int DVB_S2_TABLE_C2::POS[];

constexpr int DVB_S2_TABLE_C3::DEG[];
constexpr int DVB_S2_TABLE_C3::LEN[];
constexpr int DVB_S2_TABLE_C3::POS[];

constexpr int DVB_S2_TABLE_C4::DEG[];
constexpr int DVB_S2_TABLE_C4::LEN[];
constexpr int DVB_S2_TABLE_C4::POS[];

constexpr int DVB_S2_TABLE_C5::DEG[];
constexpr int DVB_S2_TABLE_C5::LEN[];
constexpr int DVB_S2_TABLE_C5::POS[];

constexpr int DVB_S2_TABLE_C6::DEG[];
constexpr int DVB_S2_TABLE_C6::LEN[];
constexpr int DVB_S2_TABLE_C6::POS[];

constexpr int DVB_S2_TABLE_C7::DEG[];
constexpr int DVB_S2_TABLE_C7::LEN[];
constexpr int DVB_S2_TABLE_C7::POS[];

constexpr int DVB_S2_TABLE_C8::DEG[];
constexpr int DVB_S2_TABLE_C8::LEN[];
constexpr int DVB_S2_TABLE_C8::POS[];

constexpr int DVB_S2_TABLE_C9::DEG[];
constexpr int DVB_S2_TABLE_C9::LEN[];
constexpr int DVB_S2_TABLE_C9::POS[];

constexpr int DVB_S2_TABLE_C10::DEG[];
constexpr int DVB_S2_TABLE_C10::LEN[];
constexpr int DVB_S2_TABLE_C10::POS[];

template <typename TYPE>
LDPCInterface<TYPE> *create_decoder(char prefix, int number)
{
  if (prefix == 'B') {
    switch (number) {
    case 1:
      return new LDPC<DVB_S2_TABLE_B1, TYPE>();
    case 2:
      return new LDPC<DVB_S2_TABLE_B2, TYPE>();
    case 3:
      return new LDPC<DVB_S2_TABLE_B3, TYPE>();
    case 4:
      return new LDPC<DVB_S2_TABLE_B4, TYPE>();
    case 5:
      return new LDPC<DVB_S2_TABLE_B5, TYPE>();
    case 6:
      return new LDPC<DVB_S2_TABLE_B6, TYPE>();
    case 7:
      return new LDPC<DVB_S2_TABLE_B7, TYPE>();
    case 8:
      return new LDPC<DVB_S2_TABLE_B8, TYPE>();
    case 9:
      return new LDPC<DVB_S2_TABLE_B9, TYPE>();
    case 10:
      return new LDPC<DVB_S2_TABLE_B10, TYPE>();
    case 11:
      return new LDPC<DVB_S2_TABLE_B11, TYPE>();
    }
  }
  if (prefix == 'C') {
    switch (number) {
    case 1:
      return new LDPC<DVB_S2_TABLE_C1, TYPE>();
    case 2:
      return new LDPC<DVB_S2_TABLE_C2, TYPE>();
    case 3:
      return new LDPC<DVB_S2_TABLE_C3, TYPE>();
    case 4:
      return new LDPC<DVB_S2_TABLE_C4, TYPE>();
    case 5:
      return new LDPC<DVB_S2_TABLE_C5, TYPE>();
    case 6:
      return new LDPC<DVB_S2_TABLE_C6, TYPE>();
    case 7:
      return new LDPC<DVB_S2_TABLE_C7, TYPE>();
    case 8:
      return new LDPC<DVB_S2_TABLE_C8, TYPE>();
    case 9:
      return new LDPC<DVB_S2_TABLE_C9, TYPE>();
    case 10:
      return new LDPC<DVB_S2_TABLE_C10, TYPE>();
    }
  }
  return 0;
}

namespace gr {
  namespace dvbldpc {

    ldpc_decoder_fb::sptr
    ldpc_decoder_fb::make(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, float snr)
    {
      return gnuradio::get_initial_sptr
        (new ldpc_decoder_fb_impl(standard, framesize, rate, snr));
    }

    /*
     * The private constructor
     */
    ldpc_decoder_fb_impl::ldpc_decoder_fb_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, float snr)
      : gr::block("ldpc_decoder_fb",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
      if (framesize == FECFRAME_NORMAL) {
        frame_size = FRAME_SIZE_NORMAL;
        switch (rate) {
          case C1_4:
            ldpc = create_decoder<float>('B', 1);
            break;
          case C1_3:
            ldpc = create_decoder<float>('B', 2);
            break;
          case C2_5:
            ldpc = create_decoder<float>('B', 3);
            break;
          case C1_2:
            ldpc = create_decoder<float>('B', 4);
            break;
          case C3_5:
            ldpc = create_decoder<float>('B', 5);
            break;
          case C2_3:
            ldpc = create_decoder<float>('B', 6);
            break;
          case C3_4:
            ldpc = create_decoder<float>('B', 7);
            break;
          case C4_5:
            ldpc = create_decoder<float>('B', 8);
            break;
          case C5_6:
            ldpc = create_decoder<float>('B', 9);
            break;
          case C8_9:
            ldpc = create_decoder<float>('B', 10);
            break;
          case C9_10:
            ldpc = create_decoder<float>('B', 11);
            break;
          default:
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        frame_size = FRAME_SIZE_SHORT;
        switch (rate) {
          case C1_4:
            ldpc = create_decoder<float>('C', 1);
            break;
          case C1_3:
            ldpc = create_decoder<float>('C', 2);
            break;
          case C2_5:
            ldpc = create_decoder<float>('C', 3);
            break;
          case C1_2:
            ldpc = create_decoder<float>('C', 4);
            break;
          case C3_5:
            ldpc = create_decoder<float>('C', 5);
            break;
          case C2_3:
            ldpc = create_decoder<float>('C', 6);
            break;
          case C3_4:
            ldpc = create_decoder<float>('C', 7);
            break;
          case C4_5:
            ldpc = create_decoder<float>('C', 8);
            break;
          case C5_6:
            ldpc = create_decoder<float>('C', 9);
            break;
          case C8_9:
            ldpc = create_decoder<float>('C', 10);
            break;
          default:
            break;
        }
      }
      code_rate = rate;
      dvb_standard = standard;
      sigma = std::sqrt(1 / std::pow(10, snr / 10));
      set_output_multiple(frame_size);
    }

    /*
     * Our virtual destructor.
     */
    ldpc_decoder_fb_impl::~ldpc_decoder_fb_impl()
    {
      delete ldpc;
    }

    void
    ldpc_decoder_fb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    ldpc_decoder_fb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      float code[ldpc->code_len()];

      for (int i = 0; i < noutput_items; i += frame_size) {
        for (int j = 0; j < ldpc->code_len(); j++) {
          code[j] = 2 * (*in++ / (sigma * sigma));
        }
        ldpc->decode(code, code + ldpc->data_len());
        for (int j = 0; j < ldpc->code_len(); j++) {
          if (code[j] >= 0) {
            *out++ = 0;
          }
          else {
            *out++ = 1;
          }
        }
      }
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dvbldpc */
} /* namespace gr */

