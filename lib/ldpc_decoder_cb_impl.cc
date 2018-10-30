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
#include "ldpc_decoder_cb_impl.h"
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

constexpr int DVB_S2X_TABLE_B1::DEG[];
constexpr int DVB_S2X_TABLE_B1::LEN[];
constexpr int DVB_S2X_TABLE_B1::POS[];

constexpr int DVB_S2X_TABLE_B2::DEG[];
constexpr int DVB_S2X_TABLE_B2::LEN[];
constexpr int DVB_S2X_TABLE_B2::POS[];

constexpr int DVB_S2X_TABLE_B3::DEG[];
constexpr int DVB_S2X_TABLE_B3::LEN[];
constexpr int DVB_S2X_TABLE_B3::POS[];

constexpr int DVB_S2X_TABLE_B4::DEG[];
constexpr int DVB_S2X_TABLE_B4::LEN[];
constexpr int DVB_S2X_TABLE_B4::POS[];

constexpr int DVB_S2X_TABLE_B5::DEG[];
constexpr int DVB_S2X_TABLE_B5::LEN[];
constexpr int DVB_S2X_TABLE_B5::POS[];

constexpr int DVB_S2X_TABLE_B6::DEG[];
constexpr int DVB_S2X_TABLE_B6::LEN[];
constexpr int DVB_S2X_TABLE_B6::POS[];

constexpr int DVB_S2X_TABLE_B7::DEG[];
constexpr int DVB_S2X_TABLE_B7::LEN[];
constexpr int DVB_S2X_TABLE_B7::POS[];

constexpr int DVB_S2X_TABLE_B8::DEG[];
constexpr int DVB_S2X_TABLE_B8::LEN[];
constexpr int DVB_S2X_TABLE_B8::POS[];

constexpr int DVB_S2X_TABLE_B9::DEG[];
constexpr int DVB_S2X_TABLE_B9::LEN[];
constexpr int DVB_S2X_TABLE_B9::POS[];

constexpr int DVB_S2X_TABLE_B10::DEG[];
constexpr int DVB_S2X_TABLE_B10::LEN[];
constexpr int DVB_S2X_TABLE_B10::POS[];

constexpr int DVB_S2X_TABLE_B11::DEG[];
constexpr int DVB_S2X_TABLE_B11::LEN[];
constexpr int DVB_S2X_TABLE_B11::POS[];

constexpr int DVB_S2X_TABLE_B12::DEG[];
constexpr int DVB_S2X_TABLE_B12::LEN[];
constexpr int DVB_S2X_TABLE_B12::POS[];

constexpr int DVB_S2X_TABLE_B13::DEG[];
constexpr int DVB_S2X_TABLE_B13::LEN[];
constexpr int DVB_S2X_TABLE_B13::POS[];

constexpr int DVB_S2X_TABLE_B14::DEG[];
constexpr int DVB_S2X_TABLE_B14::LEN[];
constexpr int DVB_S2X_TABLE_B14::POS[];

constexpr int DVB_S2X_TABLE_B15::DEG[];
constexpr int DVB_S2X_TABLE_B15::LEN[];
constexpr int DVB_S2X_TABLE_B15::POS[];

constexpr int DVB_S2X_TABLE_B16::DEG[];
constexpr int DVB_S2X_TABLE_B16::LEN[];
constexpr int DVB_S2X_TABLE_B16::POS[];

constexpr int DVB_S2X_TABLE_B17::DEG[];
constexpr int DVB_S2X_TABLE_B17::LEN[];
constexpr int DVB_S2X_TABLE_B17::POS[];

constexpr int DVB_S2X_TABLE_B18::DEG[];
constexpr int DVB_S2X_TABLE_B18::LEN[];
constexpr int DVB_S2X_TABLE_B18::POS[];

constexpr int DVB_S2X_TABLE_B19::DEG[];
constexpr int DVB_S2X_TABLE_B19::LEN[];
constexpr int DVB_S2X_TABLE_B19::POS[];

constexpr int DVB_S2X_TABLE_B20::DEG[];
constexpr int DVB_S2X_TABLE_B20::LEN[];
constexpr int DVB_S2X_TABLE_B20::POS[];

constexpr int DVB_S2X_TABLE_B21::DEG[];
constexpr int DVB_S2X_TABLE_B21::LEN[];
constexpr int DVB_S2X_TABLE_B21::POS[];

constexpr int DVB_S2X_TABLE_B22::DEG[];
constexpr int DVB_S2X_TABLE_B22::LEN[];
constexpr int DVB_S2X_TABLE_B22::POS[];

constexpr int DVB_S2X_TABLE_B23::DEG[];
constexpr int DVB_S2X_TABLE_B23::LEN[];
constexpr int DVB_S2X_TABLE_B23::POS[];

constexpr int DVB_S2X_TABLE_B24::DEG[];
constexpr int DVB_S2X_TABLE_B24::LEN[];
constexpr int DVB_S2X_TABLE_B24::POS[];

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

constexpr int DVB_S2X_TABLE_C1::DEG[];
constexpr int DVB_S2X_TABLE_C1::LEN[];
constexpr int DVB_S2X_TABLE_C1::POS[];

constexpr int DVB_S2X_TABLE_C2::DEG[];
constexpr int DVB_S2X_TABLE_C2::LEN[];
constexpr int DVB_S2X_TABLE_C2::POS[];

constexpr int DVB_S2X_TABLE_C3::DEG[];
constexpr int DVB_S2X_TABLE_C3::LEN[];
constexpr int DVB_S2X_TABLE_C3::POS[];

constexpr int DVB_S2X_TABLE_C4::DEG[];
constexpr int DVB_S2X_TABLE_C4::LEN[];
constexpr int DVB_S2X_TABLE_C4::POS[];

constexpr int DVB_S2X_TABLE_C5::DEG[];
constexpr int DVB_S2X_TABLE_C5::LEN[];
constexpr int DVB_S2X_TABLE_C5::POS[];

constexpr int DVB_S2X_TABLE_C6::DEG[];
constexpr int DVB_S2X_TABLE_C6::LEN[];
constexpr int DVB_S2X_TABLE_C6::POS[];

constexpr int DVB_S2X_TABLE_C7::DEG[];
constexpr int DVB_S2X_TABLE_C7::LEN[];
constexpr int DVB_S2X_TABLE_C7::POS[];

constexpr int DVB_S2X_TABLE_C8::DEG[];
constexpr int DVB_S2X_TABLE_C8::LEN[];
constexpr int DVB_S2X_TABLE_C8::POS[];

constexpr int DVB_S2X_TABLE_C9::DEG[];
constexpr int DVB_S2X_TABLE_C9::LEN[];
constexpr int DVB_S2X_TABLE_C9::POS[];

constexpr int DVB_S2X_TABLE_C10::DEG[];
constexpr int DVB_S2X_TABLE_C10::LEN[];
constexpr int DVB_S2X_TABLE_C10::POS[];

namespace gr {
  namespace dvbldpc {

    ldpc_decoder_cb::sptr
    ldpc_decoder_cb::make(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation, dvb_outputmode_t outputmode)
    {
      return gnuradio::get_initial_sptr
        (new ldpc_decoder_cb_impl(standard, framesize, rate, constellation, outputmode));
    }

    /*
     * The private constructor
     */
    ldpc_decoder_cb_impl::ldpc_decoder_cb_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation, dvb_outputmode_t outputmode)
      : gr::block("ldpc_decoder_cb",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
      if (framesize == FECFRAME_NORMAL) {
        frame_size = FRAME_SIZE_NORMAL;
        switch (rate) {
          case C1_4:
            nbch = 16200;
            ldpc = new LDPC<DVB_S2_TABLE_B1, float>();
            break;
          case C1_3:
            nbch = 21600;
            ldpc = new LDPC<DVB_S2_TABLE_B2, float>();
            break;
          case C2_5:
            nbch = 25920;
            ldpc = new LDPC<DVB_S2_TABLE_B3, float>();
            break;
          case C1_2:
            nbch = 32400;
            ldpc = new LDPC<DVB_S2_TABLE_B4, float>();
            break;
          case C3_5:
            nbch = 38880;
            ldpc = new LDPC<DVB_S2_TABLE_B5, float>();
            break;
          case C2_3:
            nbch = 43200;
            ldpc = new LDPC<DVB_S2_TABLE_B6, float>();
            break;
          case C3_4:
            nbch = 48600;
            ldpc = new LDPC<DVB_S2_TABLE_B7, float>();
            break;
          case C4_5:
            nbch = 51840;
            ldpc = new LDPC<DVB_S2_TABLE_B8, float>();
            break;
          case C5_6:
            nbch = 54000;
            ldpc = new LDPC<DVB_S2_TABLE_B9, float>();
            break;
          case C8_9:
            nbch = 57600;
            ldpc = new LDPC<DVB_S2_TABLE_B10, float>();
            break;
          case C9_10:
            nbch = 58320;
            ldpc = new LDPC<DVB_S2_TABLE_B11, float>();
            break;
          case C2_9_VLSNR:
            nbch = 14400;
            ldpc = new LDPC<DVB_S2X_TABLE_B1, float>();
            break;
          case C13_45:
            nbch = 18720;
            ldpc = new LDPC<DVB_S2X_TABLE_B2, float>();
            break;
          case C9_20:
            nbch = 29160;
            ldpc = new LDPC<DVB_S2X_TABLE_B3, float>();
            break;
          case C90_180:
            nbch = 32400;
            ldpc = new LDPC<DVB_S2X_TABLE_B11, float>();
            break;
          case C96_180:
            nbch = 34560;
            ldpc = new LDPC<DVB_S2X_TABLE_B12, float>();
            break;
          case C11_20:
            nbch = 35640;
            ldpc = new LDPC<DVB_S2X_TABLE_B4, float>();
            break;
          case C100_180:
            nbch = 36000;
            ldpc = new LDPC<DVB_S2X_TABLE_B13, float>();
            break;
          case C104_180:
            nbch = 37440;
            ldpc = new LDPC<DVB_S2X_TABLE_B14, float>();
            break;
          case C26_45:
            nbch = 37440;
            ldpc = new LDPC<DVB_S2X_TABLE_B5, float>();
            break;
          case C18_30:
            nbch = 38880;
            ldpc = new LDPC<DVB_S2X_TABLE_B22, float>();
            break;
          case C28_45:
            nbch = 40320;
            ldpc = new LDPC<DVB_S2X_TABLE_B6, float>();
            break;
          case C23_36:
            nbch = 41400;
            ldpc = new LDPC<DVB_S2X_TABLE_B7, float>();
            break;
          case C116_180:
            nbch = 41760;
            ldpc = new LDPC<DVB_S2X_TABLE_B15, float>();
            break;
          case C20_30:
            nbch = 43200;
            ldpc = new LDPC<DVB_S2X_TABLE_B23, float>();
            break;
          case C124_180:
            nbch = 44640;
            ldpc = new LDPC<DVB_S2X_TABLE_B16, float>();
            break;
          case C25_36:
            nbch = 45000;
            ldpc = new LDPC<DVB_S2X_TABLE_B8, float>();
            break;
          case C128_180:
            nbch = 46080;
            ldpc = new LDPC<DVB_S2X_TABLE_B17, float>();
            break;
          case C13_18:
            nbch = 46800;
            ldpc = new LDPC<DVB_S2X_TABLE_B9, float>();
            break;
          case C132_180:
            nbch = 47520;
            ldpc = new LDPC<DVB_S2X_TABLE_B18, float>();
            break;
          case C22_30:
            nbch = 47520;
            ldpc = new LDPC<DVB_S2X_TABLE_B24, float>();
            break;
          case C135_180:
            nbch = 48600;
            ldpc = new LDPC<DVB_S2X_TABLE_B19, float>();
            break;
          case C140_180:
            nbch = 50400;
            ldpc = new LDPC<DVB_S2X_TABLE_B20, float>();
            break;
          case C7_9:
            nbch = 50400;
            ldpc = new LDPC<DVB_S2X_TABLE_B10, float>();
            break;
          case C154_180:
            nbch = 55440;
            ldpc = new LDPC<DVB_S2X_TABLE_B21, float>();
            break;
          default:
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        frame_size = FRAME_SIZE_SHORT;
        switch (rate) {
          case C1_4:
            nbch = 3240;
            ldpc = new LDPC<DVB_S2_TABLE_C1, float>();
            break;
          case C1_3:
            nbch = 5400;
            ldpc = new LDPC<DVB_S2_TABLE_C2, float>();
            break;
          case C2_5:
            nbch = 6480;
            ldpc = new LDPC<DVB_S2_TABLE_C3, float>();
            break;
          case C1_2:
            nbch = 7200;
            ldpc = new LDPC<DVB_S2_TABLE_C4, float>();
            break;
          case C3_5:
            nbch = 9720;
            ldpc = new LDPC<DVB_S2_TABLE_C5, float>();
            break;
          case C2_3:
            nbch = 10800;
            ldpc = new LDPC<DVB_S2_TABLE_C6, float>();
            break;
          case C3_4:
            nbch = 11880;
            ldpc = new LDPC<DVB_S2_TABLE_C7, float>();
            break;
          case C4_5:
            nbch = 12600;
            ldpc = new LDPC<DVB_S2_TABLE_C8, float>();
            break;
          case C5_6:
            nbch = 13320;
            ldpc = new LDPC<DVB_S2_TABLE_C9, float>();
            break;
          case C8_9:
            nbch = 14400;
            ldpc = new LDPC<DVB_S2_TABLE_C10, float>();
            break;
          case C11_45:
            nbch = 3960;
            ldpc = new LDPC<DVB_S2X_TABLE_C1, float>();
            break;
          case C4_15:
            nbch = 4320;
            ldpc = new LDPC<DVB_S2X_TABLE_C2, float>();
            break;
          case C14_45:
            nbch = 5040;
            ldpc = new LDPC<DVB_S2X_TABLE_C3, float>();
            break;
          case C7_15:
            nbch = 7560;
            ldpc = new LDPC<DVB_S2X_TABLE_C4, float>();
            break;
          case C8_15:
            nbch = 8640;
            ldpc = new LDPC<DVB_S2X_TABLE_C5, float>();
            break;
          case C26_45:
            nbch = 9360;
            ldpc = new LDPC<DVB_S2X_TABLE_C6, float>();
            break;
          case C32_45:
            nbch = 11520;
            ldpc = new LDPC<DVB_S2X_TABLE_C7, float>();
            break;
          case C1_5_VLSNR_SF2:
            nbch = 2680;
            ldpc = new LDPC<DVB_S2_TABLE_C1, float>();
            break;
          case C11_45_VLSNR_SF2:
            nbch = 3960;
            ldpc = new LDPC<DVB_S2X_TABLE_C1, float>();
            break;
          case C1_5_VLSNR:
            nbch = 3240;
            ldpc = new LDPC<DVB_S2_TABLE_C1, float>();
            break;
          case C4_15_VLSNR:
            nbch = 4320;
            ldpc = new LDPC<DVB_S2X_TABLE_C2, float>();
            break;
          case C1_3_VLSNR:
            nbch = 5400;
            ldpc = new LDPC<DVB_S2_TABLE_C2, float>();
            break;
          default:
            nbch = 0;
            break;
        }
      }
      else {
        frame_size = FRAME_SIZE_MEDIUM;
        switch (rate) {
          case C1_5_MEDIUM:
            nbch = 5840;
            ldpc = new LDPC<DVB_S2X_TABLE_C8, float>();
            break;
          case C11_45_MEDIUM:
            nbch = 7920;
            ldpc = new LDPC<DVB_S2X_TABLE_C9, float>();
            break;
          case C1_3_MEDIUM:
            nbch = 10800;
            ldpc = new LDPC<DVB_S2X_TABLE_C10, float>();
            break;
          default:
            nbch = 0;
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
      frame = 0;
      code_rate = rate;
      dvb_standard = standard;
      output_mode = outputmode;
      if (outputmode == OM_MESSAGE) {
        set_output_multiple(nbch);
      }
      else {
         set_output_multiple(frame_size);
      }
    }

    /*
     * Our virtual destructor.
     */
    ldpc_decoder_cb_impl::~ldpc_decoder_cb_impl()
    {
      delete ldpc;
    }

    void
    ldpc_decoder_cb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      if (output_mode == OM_MESSAGE) {
        ninput_items_required[0] = (noutput_items / nbch) * (frame_size / mod->bits());
      }
      else {
        ninput_items_required[0] = noutput_items / mod->bits();
      }
    }

    int
    ldpc_decoder_cb_impl::general_work (int noutput_items,
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
      int trials = 50;
      int consumed = 0;
      int output_size = output_mode ? nbch : ldpc->code_len();

      for (int i = 0; i < noutput_items; i += output_size) {
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
        for (int j = 0; j < SYMBOLS; j++) {
          mod->soft(code + (j * mod->bits()), in[j], precision);
        }
        int count = ldpc->decode(code, code + ldpc->data_len(), trials);
        if (count < 0) {
          printf("frame = %d, snr = %.2f, LDPC decoder failed at converging to a code word.\n", frame, snr);
        }
        else {
          printf("frame = %d, snr = %.2f, trials = %d\n", frame, snr, trials - count);
        }
        frame++;
        for (int j = 0; j < output_size; j++) {
          if (code[j] >= 0) {
            *out++ = 0;
          }
          else {
            *out++ = 1;
          }
        }
        in += frame_size / mod->bits();
        consumed += frame_size / mod->bits();
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dvbldpc */
} /* namespace gr */

