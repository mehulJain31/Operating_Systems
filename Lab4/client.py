

# Mehul Jain 1001229017
# Lab 4

import socket
import os
import sys

sys.path.append('/Users/mehuljain/Desktop/pyaes-master')

def connect(): #connection function

  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  
  s.connect(('127.0.0.1', 8000))

  while True:
    
    command = raw_input("$ ");

    if command == 'exit': #exit command
      s.send(command);
      s.close()
      break;

    elif 'upload' in command: # upload command

      filenames = command.split(' '); # split the grouped files

      group = filenames[1].split(',')

      for filenames in group: # file access loop

        command = 'upload ' + filenames # seperating them

        transfer(s, command)
    
    elif 'download' in command: # download from server

      filenames = command.split(' ');
      group = filenames[1].split(',')
      
      for filenames in group:
        command = 'download '+ filenames
        s.send(command)
        confirmation = s.recv(1024)

        if 'FOUND' in confirmation:
          receive(s, command.split(' ')[1])
          
        else:
          print 'File not found on server'

def encryption(message): # encryption code
  encryptstring = []
  key = 'key'
  for i, c in enumerate(message):
    key_c = ord(key[i % len(key)])
    msg_c = ord(c)
    encryptstring.append(chr((msg_c + key_c) % 127))

  return ''.join(encryptstring)

def encrypting(file): # temp file to store the enrypted data and send to server
  
  fr = open('./Client/' + file, 'r')
  fw = open('./Client/' + 'cache~' + file, 'w')

  line = fr.read(1024);
  while line != '':
    fw.write(encryption(line))
    line = fr.read(1024)

  fw.close()
  fr.close()

def transferring(s,command):#gets the encrypted data, enrypts it and sends it to the server

  file = command.split(" ")[1];
  path = './Client/' + file;
  encrypting(file)
  
  f = open('./Client/' + 'cache~' + file, 'rb')
  
  packet = f.read(1024)
 
  while packet != '':
    s.send(packet)
    packet = f.read(1024)

  s.send('DONE');
 
  f.close()

def transfer(s,command): # upload to the server

  file = command.split(" ")[1];
  path = './Client/' + file;
  
  if os.path.exists(path):
    s.send(command)
    transferring(s,command)
    os.remove('./Client/cache~' + file);
  
  else:
    print 'Did not find file'
    s.send('Unable to find out the file');
  

def receive(s, filename): # download file from the server

  f = open('./Client/' + filename,'wb')

  while True:
    loop = s.recv(1024)
    if 'Unable to find out the file' in loop:
      print 'File not found'
      break
    
    if loop.endswith('DONE'):
      print 'Successfully transferred '
      f.write(loop[:-4])
      f.close()
      break
    
    f.write(loop)


def main():
  connect();
if(__name__ == '__main__'):
  main();