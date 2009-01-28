# Copyright (c) 2009, Object Computing, Inc.
# All rights reserved.
# See the file license.txt for licensing information.

""" Reads an "echo file" written by InterpretFAST and uses the message boundary
    information to add FAST encoded block sizes.
    Usage:  insertBlocks.py FASTfile ECHOfile OUTfile
    
    OUTFile will contain a copy of FASTfile with block lengths inserted before each message.
    Use case:
    The following command was used to prepare an echo (or index) file containing message boundaries
     InterpretFAST -r -o nul -t templates.xml -f fixfast.dat -e fixfast.index -enone
    This python script was run to create a new data file containing FAST encoded block counts.     
      insertBlocks.py fixfast.dat fixfast.index fixfast.tcp
    The FileToTcp program was used to deliver the data to a test application via a
    TCP/IP socket. 
      FileToTcp -f fixfast.tcp
    """
import sys
import getopt
import os
import re
import array

stopBit = 0x80
dataBits = 0x7F
dataShift = 7

def encodeUInt(out, value):
  """ FAST encode an unsigned integer into a output file """
  buffer = array.array("B")
  byte = stopBit
  while value != 0 or byte != 0:
    byte |= (value & dataBits)
    buffer.append(byte)
    value >>= dataShift
    byte = 0
  buffer.reverse()
  buffer.tofile(out)
  
  
def main(argv = None):
  if(argv == None):
    argv = sys.argv
  
  if(len(argv) != 4):
     print "Usage:  insertBlocks.py FASTfile ECHOfile OUTfile"
     return -1
  
  # FAST encoded file
  fast = file(argv[1], "rb")
  #message headers
  info = file(argv[2], "r")
  #output file
  out = file(argv[3], "wb")

  message = re.compile(r"\*\*\*MESSAGE \@([0-9a-fA-F]*)\*\*\*")
  endoffile = re.compile(r"\*\*\* End of data \@([0-9a-fA-F]*)\*\*\*")
  start = 0
  for line in info.readlines() :
    msg = message.match(line)
    if(msg):
      end = long(msg.group(1), 16)
      if(end != 0):
        count = end - start
        encodeUInt(out, count)
        buffer = array.array('B')
        buffer.fromfile(fast, count)
        buffer.tofile(out)
      start = end
    else:
      eof = endoffile.match(line)
      if(eof):
        end = long(eof.group(1), 16)
        count = end - start
        encodeUInt(out, count)
        out.write(fast.read(count))
        start = end

if __name__ == "__main__":
  sys.exit(main(sys.argv))
