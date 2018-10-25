/* -*- c++ -*- */

#define DVBLDPC_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dvbldpc_swig_doc.i"

%{
#include "dvbldpc/dvb_config.h"
#include "dvbldpc/ldpc_decoder_fb.h"
%}


%include "dvbldpc/dvb_config.h"
%include "dvbldpc/ldpc_decoder_fb.h"
GR_SWIG_BLOCK_MAGIC2(dvbldpc, ldpc_decoder_fb);
