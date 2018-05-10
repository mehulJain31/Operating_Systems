
# Mehul Jain 1001229017
#lab 4

import socket
import os
import sys

sys.path.append('/Users/mehuljain/Desktop/pyaes-master')

def server(): # initialize socket

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	s.bind(('127.0.0.1', 8000)) # binding to localhost

	s.listen(1);

	print 'Listening to connetion 8000'

	conn, addr = s.accept() # storing the address of the client in conn and addr variables

	print 'Client was connected', addr

	while True:

		command = conn.recv(1024); # command input
		
		if 'exit' in command: # if exit is typed
			s.close()
			break

		elif 'upload' in command: # upload to server	
			upload(conn, command.split(' ')[1]);
			
		elif 'download' in command: # sending file to the client
			download(conn, command);


#code for decryption
def decryption(encryped):
	message = []
	password = 'key'
	for i, c in enumerate(encryped):
		c_key = ord(password[i % len(password)])
		encrypt = ord(c)
		message.append(chr((encrypt - c_key) % 127))
	return ''.join(message)


def decrypting(file): # temp file for storing the data from the encrypted file to decrypted file
	
	fr = open('./Server/' + file, 'r')
	fw = open('./Server/' + 'cache~' + file, 'w')

	line = fr.read(1024);
	while line != '':
		fw.write(decryption(line))
		line = fr.read(1024)

	fw.close()
	fr.close()

def downloading(conn,command):  # decrypt function, calls decryptFileCOntents to decrypt

	file = command.split(" ")[1];
	path = './Server/' + file;

	#decrypting the file.
	decrypting(file)
	
	f = open('./Server/' + 'cache~' + file, 'rb') # reading and sending

	packet = f.read(1024)
	
	while packet != '':
		# print packet
		conn.send(packet)
		packet = f.read(1024)
			
	conn.send('DONE');
	f.close()


def download(conn, command): # send the requested file , calls downloadhelper

	file = command.split(' ')[1]
	path = './Server/' + file;
	if os.path.exists(path):
		print 'Found'
		conn.send('FOUND')
		downloading(conn,command)
		os.remove('./Server/cache~' + file);

	else:
		print 'Could not find file'
		conn.send('Unable to find out the file');


def upload(conn, filename): # upload to server helper
	
	f = open('./Server/' + filename,'wrb') # open file for write

	while True:
		bits = conn.recv(1024)

		if 'Unable to find out the file' in bits:
			print 'Did not find the file'
			break
		if bits.endswith('DONE'):
			print 'Transfer done successfully '
			f.write(bits[:-4])
			f.close()
			break
		f.write(bits)

def main():
	server(); 
if(__name__ == '__main__'):
	main();