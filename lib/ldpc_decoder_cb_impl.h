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

#ifndef INCLUDED_DVBLDPC_LDPC_DECODER_CB_IMPL_H
#define INCLUDED_DVBLDPC_LDPC_DECODER_CB_IMPL_H

#include <dvbldpc/ldpc_decoder_cb.h>
#include "psk.hh"
#include "qam.hh"
#include "ldpc_decoder.hh"
#include "dvb_s2_tables.hh"

namespace gr {
  namespace dvbldpc {

    class ldpc_decoder_cb_impl : public ldpc_decoder_cb
    {
     private:
      unsigned int frame_size;
      unsigned int code_rate;
      unsigned int dvb_standard;
      LDPCInterface<float> *ldpc;
      Modulation<gr_complex> *mod;

     public:
      ldpc_decoder_cb_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation);
      ~ldpc_decoder_cb_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace dvbldpc
} // namespace gr

#endif /* INCLUDED_DVBLDPC_LDPC_DECODER_CB_IMPL_H */

