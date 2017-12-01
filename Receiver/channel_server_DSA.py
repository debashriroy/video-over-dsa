# server.py 
import socket                                         
import time

# create a socket object
trans_serversocket = socket.socket(
	        socket.AF_INET, socket.SOCK_STREAM) 
recv_serversocket = socket.socket(
	        socket.AF_INET, socket.SOCK_STREAM) 

# get local machine name
#host = socket.gethostname()                           

trans_port = 9999 
recv_port = 9998                                          

# bind to the port
trans_serversocket.bind(("0.0.0.0", trans_port))                                  
recv_serversocket.bind(("0.0.0.0", recv_port))

# queue up to 5 requests
trans_serversocket.listen(5) 
recv_serversocket.listen(5)                                       

#while True:
    # establish a connection
trans_clientsocket,trans_addr = trans_serversocket.accept()  
recv_clientsocket,recv_addr = recv_serversocket.accept()    

print("Got a connection from %s" % str(trans_addr))
print("Got a connection from %s" % str(recv_addr))
currentTime = time.ctime(time.time()) + "\r\n"
   # clientsocket.send(currentTime.encode('ascii'))

print ("ENTER THE FREQUENCY (MHz) AND CHANNEL BANDWIDTH(MHz) IN WHICH YOU WANT TO TRANSMIT\n") 
print ("ENTER 0 FOR BOTH WHEN YOU WANT TO STOP\n") 

while True:
	freq = float(raw_input("Enter Frequncy (MHz): "))
	channel_bw = float(raw_input("Enter Channel Bandwidth (MHz): "))
	sep = ' '
	val = str(freq) + sep + str(channel_bw) + sep # sep = ' ' or sep = `\n`
	trans_clientsocket.send(val)
	recv_clientsocket.send(val)
	if freq == 0:
		break
trans_clientsocket.close()
recv_clientsocket.close()
