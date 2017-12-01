#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Video Transmitter
# Generated: Thu Jun 15 10:55:03 2017
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import time
import wx

import socket

class video_transmitter(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Video Transmitter")

        ##################################################
        # Variables
        ##################################################
        self.signal = signal = 30000
        self.samp_rate = samp_rate = 1000000
        self.rfgain = rfgain = 25
        self.freq = freq = 1700000000

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=freq,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title='FFT Plot',
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(freq, 0)
        self.uhd_usrp_sink_0.set_gain(80, 0)
        self.uhd_usrp_sink_0.set_antenna('TX/RX', 0)
        self.uhd_usrp_sink_0.set_bandwidth(1000000, 0)
        _signal_sizer = wx.BoxSizer(wx.VERTICAL)
        self._signal_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_signal_sizer,
        	value=self.signal,
        	callback=self.set_signal,
        	label='Signal',
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._signal_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_signal_sizer,
        	value=self.signal,
        	callback=self.set_signal,
        	minimum=0,
        	maximum=32000,
        	num_steps=100,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_signal_sizer)
        _rfgain_sizer = wx.BoxSizer(wx.VERTICAL)
        self._rfgain_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_rfgain_sizer,
        	value=self.rfgain,
        	callback=self.set_rfgain,
        	label='RF',
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._rfgain_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_rfgain_sizer,
        	value=self.rfgain,
        	callback=self.set_rfgain,
        	minimum=0,
        	maximum=50,
        	num_steps=100,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_rfgain_sizer)
        self.digital_gmsk_mod_0 = digital.gmsk_mod(
        	samples_per_symbol=2,
        	bt=0.35,
        	verbose=False,
        	log=False,
        )
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((5000, ))
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, '/home/droy/Documents/GNURadio_Files/video1.ts', False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, '/home/droy/Desktop/video1.avi', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blks2_packet_encoder_0 = grc_blks2.packet_mod_b(grc_blks2.packet_encoder(
        		samples_per_symbol=2,
        		bits_per_symbol=1,
        		preamble='',
        		access_code='',
        		pad_for_usrp=True,
        	),
        	payload_length=0,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blks2_packet_encoder_0, 0), (self.digital_gmsk_mod_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blks2_packet_encoder_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.digital_gmsk_mod_0, 0), (self.blocks_multiply_const_vxx_0, 0))

    def get_signal(self):
        return self.signal

    def set_signal(self, signal):
        self.signal = signal
        self._signal_slider.set_value(self.signal)
        self._signal_text_box.set_value(self.signal)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)

    def get_rfgain(self):
        return self.rfgain

    def set_rfgain(self, rfgain):
        self.rfgain = rfgain
        self._rfgain_slider.set_value(self.rfgain)
        self._rfgain_text_box.set_value(self.rfgain)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.wxgui_fftsink2_0.set_baseband_freq(self.freq)
        self.uhd_usrp_sink_0.set_center_freq(self.freq, 0)


def main(top_block_cls=video_transmitter, options=None):

    print ("Print 1")
    tb = top_block_cls()
    print ("Print 2")
    tb.Start(True)
    print ("Print 3")
    #time.sleep(5)
    #print ("Sleeping")
    #tb.get_freq()
    #tb.Stop(True)
    print ("Freq Before: %f" % tb.get_freq())
    #time.sleep(5)
    #tb.set_freq(1750000000)

########################################################
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
    port = 9999
    # connection to hostname on the port.
    s.connect(('10.251.72.161', port))                               
    print ("Print 4")
    # Receive no more than 1024 bytes
    #tm = s.recv(1024)
    num= 2.4
    init_freq=tb.get_freq()
    while num > 0:   
	buf = ''
	sep = ' '
	while sep not in buf:
		buf += s.recv(1024)
	num = float(buf)
	print ("Number received %f" % num)
	if int(num) == 0:
		break 
	tb.set_freq(num*1000000000)
	print ("Freq After: %f" % tb.get_freq())
#	print ("Frequency is: %f" get_freq())
	                                 
    print ("Print 5")
    s.close()



########################################################
    print ("Freq After Finally: %f" % tb.get_freq())
    tb.Wait()
    print ("Print 4")

if __name__ == '__main__':
    main()
