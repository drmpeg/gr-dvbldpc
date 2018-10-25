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

typedef DVB_S2_TABLE_B4 TABLE;
constexpr int TABLE::DEG[];
constexpr int TABLE::LEN[];
constexpr int TABLE::POS[];

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
            break;
          case C1_3:
            break;
          case C2_5:
            break;
          case C1_2:
            break;
          case C3_5:
            break;
          case C2_3:
            break;
          case C3_4:
            break;
          case C4_5:
            break;
          case C5_6:
            break;
          case C8_9:
            break;
          case C9_10:
            break;
          default:
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        frame_size = FRAME_SIZE_SHORT;
        switch (rate) {
          case C1_4:
            break;
          case C1_3:
            break;
          case C2_5:
            break;
          case C1_2:
            break;
          case C3_5:
            break;
          case C2_3:
            break;
          case C3_4:
            break;
          case C4_5:
            break;
          case C5_6:
            break;
          case C8_9:
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
      LDPC<TABLE, float> ldpc;
      float code[TABLE::N];

      for (int i = 0; i < noutput_items; i += frame_size) {
        for (int j = 0; j < TABLE::N; j++) {
          code[j] = 2 * (*in++ / (sigma * sigma));
        }
        ldpc.decode(code, code + TABLE::K);
        for (int j = 0; j < TABLE::N; j++) {
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

