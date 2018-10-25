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


#ifndef INCLUDED_DVBLDPC_LDPC_DECODER_FB_H
#define INCLUDED_DVBLDPC_LDPC_DECODER_FB_H

#include <dvbldpc/api.h>
#include <gnuradio/block.h>
#include <dvbldpc/dvb_config.h>

namespace gr {
  namespace dvbldpc {

    /*!
     * \brief <+description of block+>
     * \ingroup dvbldpc
     *
     */
    class DVBLDPC_API ldpc_decoder_fb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<ldpc_decoder_fb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dvbldpc::ldpc_decoder_fb.
       *
       * To avoid accidental use of raw pointers, dvbldpc::ldpc_decoder_fb's
       * constructor is in a private implementation
       * class. dvbldpc::ldpc_decoder_fb::make is the public interface for
       * creating new instances.
       */
      static sptr make(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, float snr);
    };

  } // namespace dvbldpc
} // namespace gr

#endif /* INCLUDED_DVBLDPC_LDPC_DECODER_FB_H */

