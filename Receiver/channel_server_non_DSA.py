# server.py 
import socket                                         
import time

# create a socket object
serversocket = socket.socket(
	        socket.AF_INET, socket.SOCK_STREAM) 

# get local machine name
#host = socket.gethostname()                           

port = 9999                                           

# bind to the port
serversocket.bind(("0.0.0.0", port))                                  

# queue up to 5 requests
serversocket.listen(5)                                           

#while True:
    # establish a connection
clientsocket,addr = serversocket.accept()      

print("Got a connection from %s" % str(addr))
currentTime = time.ctime(time.time()) + "\r\n"
   # clientsocket.send(currentTime.encode('ascii'))

print ("ENTER THE FREQUENCY (MHz) AND CHANNEL BANDWIDTH(MHz) IN WHICH YOU WANT TO TRANSMIT\n") 
print ("ENTER 0 WHEN YOU WANT TO STOP\n") 

while True:
	num = float(raw_input("Enter Frequency (MHz): "))
	sep = ' '
	val = str(num) + sep # sep = ' ' or sep = `\n`
	clientsocket.send(val)
	if num == 0:
		break
clientsocket.close()
