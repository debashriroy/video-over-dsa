// Server side C/C++ program to take a number from scale and send it to a client through a socket
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <gtk/gtk.h>
#include<math.h>

#define PI (3.141592653589793)
#define PORT 8080

int server_fd, new_socket=0, valread;

//int number_to_send = 0; // Put your value
int converted_number;

// Randomly Pick a value between pathloss_sd range 
double rand_sd;
int enc_1, enc_2, enc_3, enc_4, enc_5, enc_6, enc_7, enc_8, enc_9, enc_10;
float f_1, f_2, f_3, f_4, f_5, f_6, f_7, f_8, f_9, f_10;
int bitrate;
double pathloss_sd;
double rand_normal (double mu, double sigma);

void button_clicked(GtkWidget *widget, gpointer data) {
 
	//int val=1;
	/***Starting of Channel to Source Mapping*******/ 
	rand_sd = rand_normal(0, pathloss_sd);
	printf("Random value: %f\n", rand_sd);
	//Mapping the channel to source coding
	if (rand_sd>=(f_1*pathloss_sd)){
		bitrate = enc_1;
		//printf("Entering 1: %f\n", f_1*pathloss_sd);
	}
	if (rand_sd < f_1*pathloss_sd && rand_sd >= f_2*pathloss_sd){
		bitrate = enc_2;
		//printf("Entering 2: %f %f\n", f_1*pathloss_sd, f_2*pathloss_sd);
	}if (rand_sd < f_2*pathloss_sd && rand_sd >= f_3*pathloss_sd){
		bitrate = enc_3;
		//printf("Entering 3\n");
	}if (rand_sd < f_3*pathloss_sd && rand_sd >= f_4*pathloss_sd){
		bitrate = enc_4;
		//printf("Entering 4\n");
	}if (rand_sd < f_4*pathloss_sd && rand_sd >= 0.0){
		bitrate = enc_5;
		//printf("Entering 5\n");
	}if (rand_sd < 0.0 && rand_sd >= -(f_5*pathloss_sd)){
		bitrate = enc_6;
		//printf("Entering 6\n");
	}if (rand_sd < -(f_5*pathloss_sd) && rand_sd >= -(f_6*pathloss_sd)){
		bitrate = enc_7;
		//printf("Entering 7\n");
	}if (rand_sd < -(f_6*pathloss_sd) && rand_sd >= -(f_7*pathloss_sd)){
		bitrate = enc_8;
		//printf("Entering 8\n");
	}if (rand_sd < -(f_7*pathloss_sd) && rand_sd >= -(f_8*pathloss_sd)){
		bitrate = enc_9;
		//printf("Entering 9\n");
	}if (rand_sd <= -(f_1*pathloss_sd)){
		bitrate = enc_10;
		//printf("Entering 10: %f\n", (f_1*pathloss_sd));
	}
	printf("The bitrate to be sent: %d\n",bitrate);
	/***Ending of Channel to Source Mapping*******/
	converted_number = htonl((int)bitrate);
	send(new_socket, &converted_number, sizeof(converted_number),0);


};

void value_changed(GtkRange *range, gpointer win) {
    
   gdouble val = gtk_range_get_value(range);
 //  g_print("Print:%.f\n",val);
   gchar *str = g_strdup_printf("%d", (int)val);    
   gtk_label_set_text(GTK_LABEL(win), str);
   
   g_free(str);
};

//Random number generation based on Normal Distribution
double rand_normal (double mu, double sigma)
{
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }
 
  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
 
  return (mu + sigma * (double) X1);
}

// Random number generation within a range: including min and max
int randInRange(int min, int max)
{
  return min + (int) (rand() % (max - min + 1));
};

int main(int argc, char *argv[])
{
    
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

	

	GtkWidget *window;
 	GtkWidget *halign;
 	GtkWidget *hbox;
 	//GtkWidget *hscale;
 	//GtkWidget *label;
 	GtkWidget *btn;
      
	//Creating the Horizontal Scale
	gtk_init(&argc, &argv);

 	 window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 	 gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
 	 gtk_window_set_default_size(GTK_WINDOW(window), 200, 150);
 	 gtk_container_set_border_width(GTK_CONTAINER(window), 10);
 	 gtk_window_set_title(GTK_WINDOW(window), "Network Channel Condition");
  
 	 hbox = gtk_hbox_new(FALSE, 20);


	  btn = gtk_button_new_with_label("Reset");
	  gtk_widget_set_size_request(btn, 70, 30);

	  gtk_box_pack_start(GTK_BOX(hbox), btn, FALSE, FALSE, 0);

	  halign = gtk_alignment_new(0, 0, 0, 0);
	  gtk_container_add(GTK_CONTAINER(halign), hbox);
	  gtk_container_add(GTK_CONTAINER(window), halign);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );

	//Event Call from gtk window

	//***************Starting of Channel Modeling***********************/
	//Variables for modeling
	float frequency = 915; // In MHz
	float light_speed = 300; // X 10^6 meter/second
	float wavelen; // In meter 
	wavelen = light_speed/frequency; 

	float antenna_dim; // In meter
	//float wavelen; // In meter
	float antenna_far_field; // Generally 1-10 meter indoor, 10-100 meter outdoor
	antenna_far_field = (2 * antenna_dim * antenna_dim)/wavelen;
	antenna_far_field = 1; // In meter

	float K_val;
	K_val= pow((wavelen/(4*PI*antenna_far_field)),2);

	float dist; // In meter 
	printf("Enter the distance (meter):");
	scanf ("%f", &dist);

	//float ref_dist;
	float exponent = 1.7; // Generally between 1.6-1.8

	//Friis Freespace Pathloss Modeling
	float transmit_antenna_gain = 80.0;
	float received_antenna_gain = 70.0;
	int L_value= 1;
	float freespace_pathloss = 10 * log((transmit_antenna_gain*received_antenna_gain*wavelen*wavelen)/(16*PI*PI*dist*dist));
	printf("Friis Freespace Pathloass: %f dB\n", freespace_pathloss);

	// Simplified Pathloss Modeling
	float pathloss_mean = 10 * log(K_val) + 10 * exponent * log(antenna_far_field / dist);

	pathloss_sd = 2.6134; // Variance is 6.83 in LOS indoor,  7.85 OBS indoor, 7.44 ALL indoor for 900 MHz frequency 

	//rand_num = drand();
	

	//int i;
	//for(i=0; i<50; i++)
	//printf("Rand: %f\n", r2());
	//	printf("TEST: %f\n", ((float)randInRange(sd_min, sd_max)/ (float) 10000));


	printf("Pathloass Mean: %f dB\n", pathloss_mean);
	//printf("Total Pathloass: %f dB\n", (pathloss_mean + rand_sd));

	//Generating minimum bitrate and maximum bitrate from encoder specification and channel condition for a particular distance
	int enc_standard_min = 800;
	int enc_standard_max = 2048;
	int enc_min, enc_max;

	if(dist<=1.0){
		enc_min = enc_standard_min;
		enc_max = enc_standard_max;
	}
	else{
		enc_min = ceil(enc_standard_min*(10 * log(K_val) + 10 * exponent * log(antenna_far_field))/(10 * log(K_val) + 10 * exponent * log(antenna_far_field / dist)));
		enc_max = ceil(enc_standard_max*(10 * log(K_val) + 10 * exponent * log(antenna_far_field))/(10 * log(K_val) + 10 * exponent * log(antenna_far_field / dist)));
	}

	printf("The maximum encoding bitrate is: %d for distance %f meter in H.264 codec\n", enc_max, dist);
	printf("The minimum encoding bitrate is: %d for distance %f meter in H.264 codec\n", enc_min, dist);

	// Getting bitrate value to send as feedback from generated standard deviation
	bitrate = enc_min + (enc_max-enc_min)/2; // Default value of bitrate is 512 kbps
	printf("The default bitrate vaule is: %d\n", bitrate);
	// The channel to source coding mapping is done in L lavels
	int no_of_levels = 10;
	//Our considered pdf is Gaussian (The channel is Rician fading channel), so it is symetric
	//The fractions
	f_1= 1.2;
	f_2= 0.8;
	f_3= 0.45;
	f_4= 0.2;
	f_5=f_4;
	f_6=f_3;
	f_7=f_2;
	f_8=f_1;

	//Calculating the encoding bitrates
	enc_1= enc_min;
	enc_2 = enc_1 + ceil((enc_max-enc_min)*f_1/(no_of_levels-2));
	enc_3 = enc_2 + ceil((enc_max-enc_min)*f_2/(no_of_levels-2));
	enc_4 = enc_3 + ceil((enc_max-enc_min)*f_3/(no_of_levels-2));
	enc_5 = enc_4 + ceil((enc_max-enc_min)*f_4/(no_of_levels-2));
	enc_6 = enc_5 + ceil((enc_max-enc_min)*f_5/(no_of_levels-2));
	enc_7 = enc_6 + ceil((enc_max-enc_min)*f_6/(no_of_levels-2));
	enc_8 = enc_7 + ceil((enc_max-enc_min)*f_7/(no_of_levels-2));
	enc_9 = enc_8 + ceil((enc_max-enc_min)*f_8/(no_of_levels-2));
	enc_10= enc_max;

	printf("The encoding rate levels are: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", enc_1, enc_2, enc_3, enc_4, enc_5, enc_6, enc_7, enc_8, enc_9, enc_10);
	
	//***************Ending of Channel Modeling***********************/

	if(g_signal_connect(btn, "clicked",G_CALLBACK(button_clicked), NULL)>0){
		printf ("Sending data to client.\n");
	//	GtkRange *scale = &hscale;
		
	}

	g_signal_connect(window, "destroy",G_CALLBACK(gtk_main_quit), NULL);
      
	//g_signal_connect(hscale, "value-changed",G_CALLBACK(value_changed), label); 
 
	//Showing the gtk Window
	gtk_widget_show_all(window);

 	gtk_main();

	send(new_socket, &converted_number, sizeof(converted_number),0);
  
    return 0;
}
