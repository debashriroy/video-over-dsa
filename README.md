# video-over-dsa
In this project, HD videos can be streamed over contested RF environment implementing adaptive encoding and channel selection, leveraging the concept of Dynamic Spectrum Access.

Equipment Needed:

1. Webcamera with 720p resolution
2. 2 USRP B210s for each PU or SU transmitter-receiver pair 
3. 1 USRP B210 for spectrum sensing
4. Encough computing resources for operating as video transmitter, receiver, channel selector, and feedback server


Software Prerequisite:

1. Install "gstreamer-0.10" package
2. Install "gtk+-3.0" library
3. Install gnuradio and uhd: Please refer to "GNURadio_Installation.txt"

Instructions: 

Transmitter Side:
1. Create a pipe for video source: "mkfifo video1.ts"
2. Compile working.c: "gcc working.c -o working `pkg-config --cflags --libs gstreamer-0.10` -lm"

Receiver Side:
1. Create a pipe for received video: "mkfifo video2.ts"
2. Compile encoder feedback: "gcc `pkg-config --cflags gtk+-3.0` channel_model_reset.c -o channel_model_reset `pkg-config --libs gtk+-3.0` -lm"

Spectrum Sensor Side:
1. Run specturm sensor: "./usrp_spectrum_sense.py -A RX2 -g 70.0 -b 1000 902000000 926000000"


To Run a Demo:

1. Run encoder feedback: "./channel_model_reset" - Receiver

2.	Run channel feedback: "python channel_server_DSA.py" or "python channel_serve_non_DSA.py" - Receiver


3.	Run video receiver: "python video_receiver_DSA.py" or  "python video_receiver_non_DSA.py" – Receiver

4.	Run video display: "mplayer /path-to-file/video2.ts" – Receiver

5.	Run video transmitter: "python video_transmitter_DSA.py" or "python video_transmitter_non_DSA.py" - Transmitter

6.	Run video source: "python working.py" –Transmitter

7.	Run video display: "gst-launch -v playbin uri=file:///path-to-file/video1.ts" – Transmitter 
