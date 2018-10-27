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
#include <stdio.h>
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

namespace gr {
  namespace dvbldpc {

    ldpc_decoder_fb::sptr
    ldpc_decoder_fb::make(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation)
    {
      return gnuradio::get_initial_sptr
        (new ldpc_decoder_fb_impl(standard, framesize, rate, constellation));
    }

    /*
     * The private constructor
     */
    ldpc_decoder_fb_impl::ldpc_decoder_fb_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation)
      : gr::block("ldpc_decoder_fb",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
      if (framesize == FECFRAME_NORMAL) {
        frame_size = FRAME_SIZE_NORMAL;
        switch (rate) {
          case C1_4:
            ldpc = new LDPC<DVB_S2_TABLE_B1, float>();
            break;
          case C1_3:
            ldpc = new LDPC<DVB_S2_TABLE_B2, float>();
            break;
          case C2_5:
            ldpc = new LDPC<DVB_S2_TABLE_B3, float>();
            break;
          case C1_2:
            ldpc = new LDPC<DVB_S2_TABLE_B4, float>();
            break;
          case C3_5:
            ldpc = new LDPC<DVB_S2_TABLE_B5, float>();
            break;
          case C2_3:
            ldpc = new LDPC<DVB_S2_TABLE_B6, float>();
            break;
          case C3_4:
            ldpc = new LDPC<DVB_S2_TABLE_B7, float>();
            break;
          case C4_5:
            ldpc = new LDPC<DVB_S2_TABLE_B8, float>();
            break;
          case C5_6:
            ldpc = new LDPC<DVB_S2_TABLE_B9, float>();
            break;
          case C8_9:
            ldpc = new LDPC<DVB_S2_TABLE_B10, float>();
            break;
          case C9_10:
            ldpc = new LDPC<DVB_S2_TABLE_B11, float>();
            break;
          default:
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        frame_size = FRAME_SIZE_SHORT;
        switch (rate) {
          case C1_4:
            ldpc = new LDPC<DVB_S2_TABLE_C1, float>();
            break;
          case C1_3:
            ldpc = new LDPC<DVB_S2_TABLE_C2, float>();
            break;
          case C2_5:
            ldpc = new LDPC<DVB_S2_TABLE_C3, float>();
            break;
          case C1_2:
            ldpc = new LDPC<DVB_S2_TABLE_C4, float>();
            break;
          case C3_5:
            ldpc = new LDPC<DVB_S2_TABLE_C5, float>();
            break;
          case C2_3:
            ldpc = new LDPC<DVB_S2_TABLE_C6, float>();
            break;
          case C3_4:
            ldpc = new LDPC<DVB_S2_TABLE_C7, float>();
            break;
          case C4_5:
            ldpc = new LDPC<DVB_S2_TABLE_C8, float>();
            break;
          case C5_6:
            ldpc = new LDPC<DVB_S2_TABLE_C9, float>();
            break;
          case C8_9:
            ldpc = new LDPC<DVB_S2_TABLE_C10, float>();
            break;
          default:
            break;
        }
      }
      switch (constellation) {
        case MOD_QPSK:
          mod = new PhaseShiftKeying<4, gr_complex>();
          break;
        case MOD_16QAM:
          mod = new QuadratureAmplitudeModulation<16, gr_complex>();
          break;
        case MOD_64QAM:
          mod = new QuadratureAmplitudeModulation<64, gr_complex>();
          break;
        case MOD_256QAM:
          mod = new QuadratureAmplitudeModulation<256, gr_complex>();
          break;
        default:
          break;
      }
      code_rate = rate;
      dvb_standard = standard;
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
      ninput_items_required[0] = noutput_items / mod->bits();
    }

    int
    ldpc_decoder_fb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      float code[ldpc->code_len()];
      float tmp[mod->bits()];
      float sp, np, sigma, precision, snr;
      gr_complex s, e;
      const int SYMBOLS = ldpc->code_len() / mod->bits();

      for (int i = 0; i < noutput_items; i += frame_size) {
        sp = 0;
        np = 0;
        for (int j = 0; j < SYMBOLS; j++) {
          mod->hard(tmp, in[j]);
          s = mod->map(tmp);
          e = in[j] - s;
          sp += std::norm(s);
          np += std::norm(e);
        }
        if (!(np > 0)) {
          np = 1e-12;
        }
        snr = 10 * std::log10(sp / np);
        sigma = std::sqrt(np / (2 * sp));
        precision = 1 / (sigma * sigma);
        printf("sigma = %f, precision = %f, snr = %f\n", sigma, precision, snr);
        for (int j = 0; j < SYMBOLS; j++) {
          mod->soft(code + (j * 2), in[j], precision);
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
        in += frame_size / mod->bits();
      }
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items / mod->bits());

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dvbldpc */
} /* namespace gr */

