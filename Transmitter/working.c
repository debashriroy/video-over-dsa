#include <gst/gst.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//HEADER FILES FOR CREATING SOCKET
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#define IP_ADDR "10.173.215.176"

int main(int argc, char *argv[]) {
 	GstElement *pipeline, *source, *sink, *mux;
 	GstElement *encoder, *csp_filter, *timeoverlay, *textoverlay;
	GstBus *bus;
	GstMessage *msg;
	GstStateChangeReturn ret;
	GstCaps *caps;
	int initial_bitrate;
	int new_bitrate;
	int new_framerate;
	int time_interval;
	char *file_sink_location;

	clock_t start, end;
	double cpu_time_used;

	//VARIABLES TO FOR HANDLING CLIENT SOCKET CONNECTION
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	int received_int = 0;
	int converted_int=0;

	/* Initialize GStreamer */
	gst_init (&argc, &argv);

	/*Setting new bitrate */
	if(argc!=3){
		printf("Give input in format:%s (INT)framerate filename\n", argv[0]);
		return -1;	
	}

	initial_bitrate=512; //Setting initial bitrate
	new_bitrate = 512; //Setting new bitrate
	new_framerate= atoi(argv[1]); //Setting new framerate
	//time_interval=atoi(argv[2]); //Setting new time interval
	file_sink_location = argv[2]; // Setting File Sink Location path

	/* Create the elements */
	/* Camera video stream comes from a Video4Linux driver */
	source = gst_element_factory_make ("v4l2src", "source");

	/* A Caps Filter is assigned */
	csp_filter = gst_element_factory_make("ffmpegcolorspace", "csp_filter");
	//  textoverly=gst_element_factory_make ("videotestsrc", "source");
	//  timeoverly=gst_element_factory_make ("videotestsrc", "source");

	/* Timeoverlay Element is assigned */
	//timeoverlay = gst_element_factory_make ("timeoverlay", "timeoverlay");
	
	/* Textoverlay Element is assigned */
	textoverlay = gst_element_factory_make ("textoverlay", "textoverlay");
	
	/* A H.264 encoder */
	encoder = gst_element_factory_make ("x264enc", "encoder");
	//printf("Print1\n");

	/* A MPEG-TS Multiplexer */
	mux = gst_element_factory_make("mpegtsmux", "muxer");

	/* A RAW-AVI Multiplexer */
	//mux = gst_element_factory_make("avimux", "muxer");
	//printf("Print2\n");

	/* A dummy sink for the video stream */
	sink = gst_element_factory_make ("filesink", "sink");

	/* Create the empty pipeline */
	pipeline = gst_pipeline_new ("test-pipeline");

	/* Check that elements are correctly initialized */
	if (!pipeline || !source || !csp_filter || !textoverlay || !encoder || !mux || !sink) {
		g_printerr ("Not all elements could be created.\n");
		if(!mux)
			g_printerr("DVB-Mux not created\n");
		return -1;
	}
	//printf("Print3\n");

	/* Build the pipeline */
	gst_bin_add_many (GST_BIN (pipeline), source, csp_filter, textoverlay, encoder, mux, sink, NULL);

	/* Specify what kind of video is wanted from the camera */
	caps = gst_caps_new_simple("video/x-raw-yuv",
			"width", G_TYPE_INT, 640,
			"height", G_TYPE_INT, 480,
			"framerate", GST_TYPE_FRACTION, 25, 1,
			NULL);
			
	//printf("Print4\n");
	/* Link the camera source and colorspace filter using capabilities specified */
	if(!gst_element_link_filtered(source, csp_filter, caps)){
		//printf("Print4.5\n");		
      		 return FALSE;
    	}
    	gst_caps_unref(caps);
    	//printf("Print5\n");
	//   gst_element_link(source, csp_filter);
	//   gst_element_link_many(encoder, mux, sink);

	/* Start of linking all elements together */
	if(gst_element_link(csp_filter, textoverlay)!=TRUE){
 		g_printerr ("Elements could not be linked.\n");
    		gst_object_unref (pipeline);
   		 return -1;
	}
	/*if(gst_element_link(timeoverlay, textoverlay)!=TRUE){
 		g_printerr ("Elements could not be linked.\n");
    		gst_object_unref (pipeline);
   		 return -1;
	}*/
	if(gst_element_link(textoverlay, encoder)!=TRUE){
 		g_printerr ("Elements could not be linked.\n");
    		gst_object_unref (pipeline);
   		 return -1;
	}

	if(gst_element_link(encoder, mux)!=TRUE){
	 	g_printerr ("Elements could not be linked.\n");
	    	gst_object_unref (pipeline);
	   	 return -1;
	}

	if(gst_element_link(mux, sink)!=TRUE){
	 	g_printerr ("Elements could not be linked.\n");
	    	gst_object_unref (pipeline);
	   	 return -1;
	}
	/* End of linking all elements together */
  
	/*
	  if(gst_element_link_many(csp_filter, encoder, mux, sink)!=TRUE){
		g_printerr ("Elements could not be linked.\n");
	   	gst_object_unref (pipeline);
	   	 return -1;
	  }*/

	//printf("Print6\n");
	/* Modify the properties of some elements*/
	// g_object_set (source, "pattern", 0, NULL);
	g_object_set (encoder, "bitrate", initial_bitrate, NULL);

	//Setting the Timeoverlay paramters
	//g_object_set (timeoverlay, "halign", "right", NULL);
       // g_object_set (timeoverlay, "valign", "bottom", NULL);
	//g_object_set (timeoverlay, "shaded-background", "true", NULL);
	
	//Setting the Textoverlay paramters
    	g_object_set(G_OBJECT(textoverlay), "text", "Video 640x480 25fps", NULL);
	g_object_set (textoverlay, "halignment", "left", NULL);
        g_object_set (textoverlay, "valignment", "bottom", NULL);
	g_object_set (textoverlay, "shaded-background", "true", NULL);

	//g_object_set (sink, "location", "/home/droy/Documents/GNURadio_Files/video1.ts", NULL);
	//g_object_set (sink, "location", strcat(file_sink_location, ".ts"), NULL);
	g_object_set (sink, "location", file_sink_location, NULL);
	g_object_set (source, "device", "/dev/video0", NULL); // Taking video source from web camera

	//printf("Print7\n");

	/* Getting the time before starting to play */
	//start = clock();
//	start = gettimeofday();

	/* Start playing */
	ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref (pipeline);
		return -1;
	}
	printf("Print8\n");
 
	/* Giving some time duration */
	//sleep(15);

	//HANDLING THE CLIENT SIDE SOCKET CONNECTION

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
       		printf("\n Socket creation error \n");
        	return -1;
   	}
  
    	memset(&serv_addr, '0', sizeof(serv_addr));
  
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(PORT);
      
    	// Convert IPv4 and IPv6 addresses from text to binary form
    	if(inet_pton(AF_INET, IP_ADDR, &serv_addr.sin_addr)<=0)     {
       		printf("\nInvalid address/ Address not supported \n");
       		return -1;
    	}
  
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
       		printf("\nConnection Failed \n");
       		return -1;
   	}
    	send(sock , hello , strlen(hello) , 0 );
   	printf("Hello message sent\n");

	do{
   		valread = read(sock, &received_int, sizeof(received_int));
		printf("Received int = %d\n", ntohl(received_int));
		if(ntohl(received_int)==0){
				printf("STOPPED STREAMING\n");
				return 0;
		}
    		if (valread > 0) {
			printf("Received int = %d\n", ntohl(received_int));
			new_bitrate= ntohl(received_int);
			//SETTING BITRATES ACCORDING TO RECEIVED CHANNEL CONDITION
			/*if(converted_int==1) 
     				 new_bitrate=50;
         		else if(converted_int==2) 
     				 new_bitrate=100;
         		else if(converted_int==3) 
     				 new_bitrate=150;
         		else if(converted_int==4) 
     				 new_bitrate=200;
         		else if(converted_int==5) 
     				 new_bitrate=250;
         		else if(converted_int==6) 
     				 new_bitrate=300;
         		else if(converted_int==7) 
     				 new_bitrate=350;
         		else if(converted_int==8) 
     				 new_bitrate=400;
         		else if(converted_int==9) 
     				 new_bitrate=450;
         		else if(converted_int==10) 
     				 new_bitrate=512;*/
      
			printf("Bitrate changed from %d to %d.\n",initial_bitrate, new_bitrate);
			if(initial_bitrate!=new_bitrate){
				initial_bitrate=new_bitrate;
				printf("Bitrate Changed.\n");
				/* Pause playing */
				gst_element_set_state (pipeline, GST_STATE_PAUSED); 

				/* Modify the properties of some elements*/
				g_object_set (encoder, "bitrate", initial_bitrate, NULL); // Setting the bitrate of encoder
				//g_object_set (caps, "framerate", new_framerate, 1, NULL); // Setting the framerate of caps filter


				/* Again Start playing */
				ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
				if (ret == GST_STATE_CHANGE_FAILURE) {
					g_printerr ("Unable to set the pipeline to the playing state.\n");
					gst_object_unref (pipeline);
					return -1;
				}
			}else{
				printf("Bitrate does not change.\n");
			}
   		 }
    		else {
			// Handling erros here
			printf ("STOPPED STREAMING\n");
			//continue;
			return 0;
   		     }
	}while(ntohl(received_int)>0);



	/*
	printf("PLEASE ENTER BITRATE YOU WANT TO SEE\n");
	printf("PLEASE ENTER 0 WHEN YOU LIKE TO STOP STREAMING\n");
	printf("Enter bitrate:");
	scanf("%d", &new_bitrate);
	do{
		if(new_bitrate==0)
			return;
		if (abs(initial_bitrate-new_bitrate)>20){
			printf("Bitrate changed from %d to %d.\n",initial_bitrate, new_bitrate);		
			initial_bitrate=new_bitrate;
			//Pause playing
			gst_element_set_state (pipeline, GST_STATE_PAUSED); 

			//Modify the properties of some elements
			g_object_set (encoder, "bitrate", initial_bitrate, NULL); // Setting the bitrate of encoder
			//g_object_set (caps, "framerate", new_framerate, 1, NULL); // Setting the framerate of caps filter


			//Again Start playing
			ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
			if (ret == GST_STATE_CHANGE_FAILURE) {
				g_printerr ("Unable to set the pipeline to the playing state.\n");
				gst_object_unref (pipeline);
				return -1;
			}
		}else{
			printf("Bitrate NOT changed from %d to %d.\n",initial_bitrate, new_bitrate);
		}
		printf("\nEnter bitrate:");
		scanf("%d", &new_bitrate);

	}while(abs(initial_bitrate-new_bitrate)>0);
	*/

		
	/* Wait until error or EOS */
	bus = gst_element_get_bus (pipeline);
	msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
	
	/* Calculate time at the end */
	//end = clock();
	//end = gettimeofday();
	//cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	//cpu_time_used = ((double) (end - start));
	//printf("Time elapsed: %f seconds\n", cpu_time_used);
	

	/* Parse message */
	if (msg != NULL) {
	    GError *err;
	    gchar *debug_info;

 		   switch (GST_MESSAGE_TYPE (msg)) {
  			case GST_MESSAGE_ERROR:
       				 gst_message_parse_error (msg, &err, &debug_info);
       				 g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
   				 g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
       				 g_clear_error (&err);
        			 g_free (debug_info);
       				 break;
      			case GST_MESSAGE_EOS:
       				 g_print ("End-Of-Stream reached.\n");
        			 break;
     			default:
     			/* We should not reach here because we only asked for ERRORs and EOS */
     				g_printerr ("Unexpected message received.\n");
      				break;
		}
	gst_message_unref (msg);
	}

	

	/* Free resources */
	gst_object_unref (bus);
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (pipeline);
	return 0;
}
