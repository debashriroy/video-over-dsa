# Adaptive Video Encoding and Dynamic Channel Access for Real-time Streaming over SDRs
In this project, HD videos can be streamed over contested RF environment implementing adaptive encoding and channel selection, leveraging the concept of Dynamic Spectrum Access. This repository was used to implement the testbed in the linked paper: https://ieeexplore.ieee.org/abstract/document/8710795

## Cite This paper
To use this repository, please refer to our paper: 

 `D. Roy, T. Mukherjee, M. Chatterjee and E. Pasiliao, "Adaptive Video Encoding and Dynamic Channel Access for Real-time Streaming over SDRs," 2018 IEEE 37th International Performance Computing and Communications Conference (IPCCC), 2018, pp. 1-9, doi: 10.1109/PCCC.2018.8710795.`
 
## Equipment Needed:

1. Webcamera with 720p resolution
2. Two USRP B210s for each PU or SU transmitter-receiver pair 
3. One USRP B210 for spectrum sensing
4. Encough computing resources for operating as video transmitter, receiver, channel selector, and feedback server


## Software Prerequisite:

1. Install "gstreamer-0.10" package
2. Install "gtk+-3.0" library
3. Install gnuradio and uhd: Please refer to "GNURadio_Installation.txt"

## Instructions: 

### Transmitter Side:
1. Create a pipe for video source: "mkfifo video1.ts"
2. Compile working.c: "gcc working.c -o working `pkg-config --cflags --libs gstreamer-0.10` -lm"

### Receiver Side:
1. Create a pipe for received video: "mkfifo video2.ts"
2. Compile encoder feedback: "gcc `pkg-config --cflags gtk+-3.0` channel_model_reset.c -o channel_model_reset `pkg-config --libs gtk+-3.0` -lm"

### Spectrum Sensor Side:
1. Run specturm sensor: "./usrp_spectrum_sense.py -A RX2 -g 70.0 -b 1000 902000000 926000000"


### To Run a Demo:

1. Run encoder feedback: "./channel_model_reset" - Receiver

2.	Run channel feedback: "python channel_server_DSA.py" or "python channel_serve_non_DSA.py" - Receiver


3.	Run video receiver: "python video_receiver_DSA.py" or  "python video_receiver_non_DSA.py" – Receiver

4.	Run video display: "mplayer /path-to-file/video2.ts" – Receiver

5.	Run video transmitter: "python video_transmitter_DSA.py" or "python video_transmitter_non_DSA.py" - Transmitter

6.	Run video source: "python working.py" –Transmitter

7.	Run video display: "gst-launch -v playbin uri=file:///path-to-file/video1.ts" – Transmitter 
