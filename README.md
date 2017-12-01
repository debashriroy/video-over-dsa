# video-over-dsa
Gstreamer Options:
gcc working.c -o working `pkg-config --cflags --libs gstreamer-0.10` -lm


USRP Spectrum Sensing Options:
./usrp_spectrum_sense.py -A RX2 -g 70.0 -b 1000 902000000 926000000


Video Server Compilation:  
gcc `pkg-config --cflags gtk+-3.0` channel_model_reset.c -o channel_model_reset `pkg-config --libs gtk+-3.0` -lm


To Run a Demo

1)	Run source feedback server (./channel_model_reset)- Receiver

2)	Run channel feedback server (python channel_server_DSA/non_DSA)- Receiver


3)	Run video receiver (python video_receiver_DSA.py or python video_receiver_non_DSA.py) – Receiver

4)	Run video display (mplayer) – Receiver

5)	Run video transmitter (python video_transmitter_DSA or python video_transmitter_non_DSA.py) - Transmitter

6)	Run video source: python working.py –Transmitter

7)	Run video display- gst bin – Transmitter 
