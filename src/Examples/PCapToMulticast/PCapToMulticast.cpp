// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Examples/ExamplesPch.h>
#include "PcapToMulticast.h"
#include <Examples/StopWatch.h>
#include <Common/Types.h>
using namespace QuickFAST;
using namespace Examples;

namespace {
  void waitForEnter()
  {
    std::cout << "Hit Enter to continue:" << std::flush;
    char c = std::cin.get();
    if(c == 'q')
    {
      exit(1);
    }
  }
}

PcapToMulticast::PcapToMulticast()
: portNumber_(30001)
, sendAddress_("239.255.0.1")
, sendCount_(1)
, sendMicroseconds_(500)
, burst_(1)
, pauseEveryPass_(false)
, pauseEveryMessage_(false)
, verbose_(false)
, dataFile_(0)
, socket_(ioService_)
, strand_(ioService_)
, timer_(ioService_)
, bufferSize_(0)
, nPass_(0)
, nMsg_(0)
, totalMessageCount_(0)
{
}

PcapToMulticast::~PcapToMulticast()
{
}


bool
PcapToMulticast::init(int argc, char * argv[])
{
  commandArgParser_.addHandler(this);
  return commandArgParser_.parse(argc, argv);
}

int
PcapToMulticast::parseSingleArg(int argc, char * argv[])
{
  int consumed = 0;
  std::string opt(argv[0]);
  try
  {
    if(opt == "-f")
    {
      dataFileName_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-p" && argc > 1)
    {
      portNumber_ = boost::lexical_cast<unsigned short>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-r" && argc > 1)
    {
      size_t mps = boost::lexical_cast<size_t>(argv[1]);
      if(mps > 0)
      {
        sendMicroseconds_ = 1000000/mps;
        consumed = 2;
      }
      else
      {
        sendMicroseconds_ = 0;
        consumed = 2;
      }
    }
    else if(opt == "-b" && argc > 1)
    {
      burst_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-c" && argc > 1)
    {
      sendCount_ = boost::lexical_cast<size_t>(argv[1]);
      consumed = 2;
    }
    else if(opt == "-a" && argc > 1)
    {
      sendAddress_ = argv[1];
      consumed = 2;
    }
    else if(opt == "-pausemessage")
    {
      pauseEveryMessage_ = true;
      consumed = 1;
    }
    else if(opt == "-pausepass")
    {
      pauseEveryPass_ = true;
      consumed = 1;
    }
    else if(opt == "-v")
    {
      verbose_ = !verbose_;
      consumed = 1;
    }
  }
  catch (std::exception & ex)
  {
    std::cerr << ex.what() << " while interpreting " << opt << std::endl;
    consumed = 0;
  }
  return consumed;
}

void
PcapToMulticast::usage(std::ostream & out) const
{
  out << "  -a dotted_ip  : Multicast send address (default is 239.255.0.1)" << std::endl;
  out << "  -p port       : Multicast port number (default 30001)" << std::endl;
  out << "  -f datafile   : File containing PCAP format capture of a multicast feed." << std::endl;
  out << "  -r burst/sec  : Rate at which to send bursts of messages expressed as bursts per second (default = 2000)" << std::endl;
  out << "                : zero means send continuously." << std::endl;
  out << "  -b msg/burst  : Messages per burst(default = 1)" << std::endl;
  out << "  -c count      : How many times to send the file (passes)" << std::endl;
  out << "                  (default 1; 0 means forever.)" << std::endl;
  out << "  -pausemessage : Wait for 'Enter' before every message." << std::endl;
  out << "  -pausepass    : Wait for 'Enter' before every pass." << std::endl;
  out << "  -v            : Noise to the console while it runs" << std::endl;
}

bool
PcapToMulticast::applyArgs()
{
  bool ok = true;
  try
  {
    if(dataFileName_.empty())
    {
      ok = false;
      std::cerr << "ERROR: -f [datafile] option is required." << std::endl;
      commandArgParser_.usage(std::cerr);
    }
    if(ok)
    {
      dataFile_ = fopen(dataFileName_.c_str(), "rb");
      if(!dataFile_)
      {
        ok = false;
        std::cerr << "ERROR: Can't open file: "
          << dataFileName_
          << std::endl;
      }
    }

    ok = ok && preparsePcapFile();

    multicastAddress_ = boost::asio::ip::address::from_string(sendAddress_);
    endpoint_ = boost::asio::ip::udp::endpoint(multicastAddress_, portNumber_);
    socket_.open(endpoint_.protocol());
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    ok = false;
  }
  return ok;
}
namespace
{
#pragma pack(push)
#pragma pack(1)
  /*
   * To avoid dependancies on additional libraries, I have included the
   * following structure definitions with the relevant copyright information.
   * Original information is available at:
   *
   * http://www.winpcap.org/devel.htm
   * Modifications were made to use C++ rather than C syntax for structures
   * (i.e. no typedef) and to use data types defined in QuickFAST.
   *
   */

  /*
   * Copyright (c) 1993, 1994, 1995, 1996, 1997
   *	The Regents of the University of California.  All rights reserved.
   *
   * Redistribution and use in source and binary forms, with or without
   * modification, are permitted provided that the following conditions
   * are met:
   * 1. Redistributions of source code must retain the above copyright
   *    notice, this list of conditions and the following disclaimer.
   * 2. Redistributions in binary form must reproduce the above copyright
   *    notice, this list of conditions and the following disclaimer in the
   *    documentation and/or other materials provided with the distribution.
   * 3. All advertising materials mentioning features or use of this software
   *    must display the following acknowledgement:
   *	This product includes software developed by the Computer Systems
   *	Engineering Group at Lawrence Berkeley Laboratory.
   * 4. Neither the name of the University nor of the Laboratory may be used
   *    to endorse or promote products derived from this software without
   *    specific prior written permission.
   *
   * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
   * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
   * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   * SUCH DAMAGE.
   *
   */

  /*
   * The first record in the file contains saved values for some
   * of the flags used in the printout phases of tcpdump.
   * Many fields here are 32 bit ints so compilers won't insert unwanted
   * padding; these files need to be interchangeable across architectures.
   */
  struct pcap_file_header {
    uint32 magic;
    uint16 version_major;
    uint16 version_minor;
    uint32 thiszone;	/* gmt to local correction */
    uint32 sigfigs;	/* accuracy of timestamps */
    uint32 snaplen;	/* max length saved portion of each pkt */
    uint32 linktype;	/* data link type (LINKTYPE_*) */
  };

  /*
   * Generic per-packet information, as supplied by libpcap.
   *
   * The time stamp can and should be a "struct timeval", regardless of
   * whether your system supports 32-bit tv_sec in "struct timeval",
   * 64-bit tv_sec in "struct timeval", or both if it supports both 32-bit
   * and 64-bit applications.  The on-disk format of savefiles uses 32-bit
   * tv_sec (and tv_usec); this structure is irrelevant to that.  32-bit
   * and 64-bit versions of libpcap, even if they're on the same platform,
   * should supply the appropriate version of "struct timeval", even if
   * that's not what the underlying packet capture mechanism supplies.
   */
  struct pcap_pkthdr {
    struct timeval ts;	/* time stamp */
    uint32 caplen;	/* length of portion present */
    uint32 len;	/* length this packet (off wire) */
  };

  /*
   * Copyright (c) 1999 - 2005 NetGroup, Politecnico di Torino (Italy)
   * Copyright (c) 2005 - 2006 CACE Technologies, Davis (California)
   * All rights reserved.
   *
   * Redistribution and use in source and binary forms, with or without
   * modification, are permitted provided that the following conditions
   * are met:
   *
   * 1. Redistributions of source code must retain the above copyright
   * notice, this list of conditions and the following disclaimer.
   * 2. Redistributions in binary form must reproduce the above copyright
   * notice, this list of conditions and the following disclaimer in the
   * documentation and/or other materials provided with the distribution.
   * 3. Neither the name of the Politecnico di Torino, CACE Technologies
   * nor the names of its contributors may be used to endorse or promote
   * products derived from this software without specific prior written
   * permission.
   *
   * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   */

  /* 4 bytes IP address */
  struct ip_address{
      uchar byte1;
      uchar byte2;
      uchar byte3;
      uchar byte4;
  };

  struct ethernetIIHeader{
    uchar dst_mac[6];
    uchar src_mac[6];
    uchar ether_type[2];
  };

  /* IPv4 header */
  struct ip_header{
      uchar  ver_ihl;           // Version (4 bits) + Internet header length (4 bits)
      uchar  tos;               // Type of service
      uint16 tlen;              // Total length
      uint16 identification;    // Identification
      uint16 flags_fo;          // Flags (3 bits) + Fragment offset (13 bits)
      uchar  ttl;               // Time to live
      uchar  proto;             // Protocol
      uint16 crc;               // Header checksum
      ip_address  saddr;        // Source address
      ip_address  daddr;        // Destination address
      uint32   op_pad;          // Option + Padding
  };

  /* UDP header*/
  struct udp_header{
      uint16 sport;             // Source port
      uint16 dport;             // Destination port
      uint16 len;               // Datagram length
      uint16 crc;               // Checksum
  };
  /* end of libpcap headers */

  static const uint32 nativeMagic = 0xa1b2c3d4;
  static const uint32 swappedMagic = 0xd4c3b2a1;
#pragma pack(pop)
  class ByteSwapper
  {
  public:
    ByteSwapper(bool swap)
      : swap_(swap)
    {
    }

    uint16 operator()(uint16 v) const
    {
      if(swap_)
      {
        return (v >> 8) | (v << 8);
      }
      return v;
    }

    int16 operator()(int16 v) const
    {
      if(swap_)
      {
        return ((v >> 8) & 0xFF) | (v << 8);
      }
      return v;
    }

    uint32 operator()(uint32 v) const
    {
      if(swap_)
      {
        return ((v >> 24) & 0x000000FF)
             | ((v >> 8)  & 0x0000FF00)
             | ((v << 8)  & 0x00FF0000)
             | ((v << 24) & 0xFF000000)
             ;
      }
      return v;
    }

    int32 operator()(int32 v) const
    {
      if(swap_)
      {
        return ((v >> 24) & 0x000000FF)
             | ((v >> 8)  & 0x0000FF00)
             | ((v << 8)  & 0x00FF0000)
             | ((v << 24) & 0xFF000000)
             ;
      }
      return v;
    }

  private:
    bool swap_;
  };
}
bool
PcapToMulticast::preparsePcapFile()
{
  bool ok = true;
  fseek(dataFile_, 0, SEEK_END);
  size_t fileLength = ftell(dataFile_);
  boost::scoped_array<char> buffer(new char[fileLength]);
  fseek(dataFile_, 0, SEEK_SET);
  fread(buffer.get(), 1, fileLength, dataFile_);
  size_t pos = 0;

  //////////////////////////
  // Process the file header
  if(fileLength - pos < sizeof(pcap_file_header))
  {
    std::cerr << "Invalid pcap file: no header." << std::endl;
    return false;
  }

  pcap_file_header * fileHeader = reinterpret_cast<pcap_file_header *>(buffer.get() + pos);
  pos += sizeof(pcap_file_header);

  if(fileHeader->magic != nativeMagic && fileHeader->magic != swappedMagic)
  {
    std::cerr << "Invalid pcap file: missing magic." << std::endl;
    return false;
  }
  ByteSwapper swap(fileHeader->magic == swappedMagic);
  assert(swap(fileHeader->magic) == nativeMagic);

  size_t skipped = 0;

  while (pos + sizeof(pcap_pkthdr) + sizeof(ip_header) + sizeof(udp_header) < fileLength)
  {
    ////////////////////////////
    // process the packet header
    pcap_pkthdr * packetHeader = reinterpret_cast<pcap_pkthdr *>(buffer.get() + pos);
    pos += sizeof(pcap_pkthdr);
    size_t datalen = swap(packetHeader->caplen);
    if(packetHeader->caplen != packetHeader->len)
    {
      pos += datalen;
      skipped+= 1;
    }
    else
    {
      // skip the other headers
      pos += sizeof(ethernetIIHeader);
      datalen -= sizeof(ethernetIIHeader);

      ip_header * ipHeader = reinterpret_cast<ip_header *>(buffer.get() + pos);
      // IP header contains its own length expressed in 4 byte units.
      size_t ipLen = (ipHeader->ver_ihl & 0xF) * 4;
      pos += ipLen;
      datalen -= ipLen;
      pos += sizeof(udp_header);
      datalen -= sizeof(udp_header);
      if(datalen > 0)
      {
          messageIndex_.push_back(MessagePosition(pos, datalen));
      }
      pos += datalen;
    }
  }


  bufferSize_ = 0;
  for(size_t nMsg = 0; nMsg < messageIndex_.size(); ++nMsg)
  {
    const MessagePosition & position = messageIndex_[nMsg];
    if(position.second > bufferSize_)
    {
      bufferSize_ = position.second;
    }
  }

  buffer_.reset(new unsigned char[bufferSize_]);
  if(skipped != 0)
  {
    std::cerr << "Warning: ignoring " << skipped << " truncated packets." << std::endl;
  }
  return ok;
}

int
PcapToMulticast::run()
{
  try
  {
    if(verbose_)
    {
      std::cout << " Configuring multicast: " << multicastAddress_ << '|' << sendAddress_ << ':' << portNumber_ << std::endl;
      std::cout << "Sending " << messageIndex_.size() << " messages. "
        << "Largest is " << bufferSize_ << " bytes." << std::endl;
    }

    strand_.dispatch(
        strand_.wrap(boost::bind(&PcapToMulticast::sendBurst, this)));
    StopWatch lapse;
    this->ioService_.run();
    unsigned long sendLapse = lapse.freeze();
    std::cout << "sent "
      << totalMessageCount_
      << " messages in "
      << std::fixed << std::setprecision(3)
      << sendLapse
      << " milliseonds. [";
    std::cout << std::fixed << std::setprecision(3)
      << double(sendLapse)/double(totalMessageCount_) << " msec/message. = "
      << std::fixed << std::setprecision(0)
      << 1000. * double(totalMessageCount_)/double(sendLapse) << " message/second.]"
      << std::endl;

#ifdef _WIN32
    // On WIN32 if sender closes the socket before a localhost receiver
    // reads all data, any unread data is thrown away.
    // This ugly sleep lets the dust settle.
    Sleep(1000);
#endif // _WIN32
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

void
PcapToMulticast::sendBurst()
{
  try
  {
    // set the next timeout
    if(sendMicroseconds_ != 0)
    {
      timer_.expires_from_now(boost::posix_time::microseconds(sendMicroseconds_));
      timer_.async_wait(
        strand_.wrap(boost::bind(&PcapToMulticast::sendBurst, this))
        );
    }

    for(size_t nBurstMsg = 0; nBurstMsg < burst_; ++nBurstMsg)
    {
      if(nMsg_ >= messageIndex_.size())
      {
        // completed a pass;  count it and see if we should stop or pause
        nPass_ += 1;
        if(nPass_ >= sendCount_ && sendCount_ != 0)
        {
          ioService_.stop();
          return;
        }
        if(verbose_)
        {
          std::cout << "Begin pass #" << nPass_ << " of " << sendCount_ << std::endl;
        }
        if(pauseEveryPass_)
        {
          waitForEnter();
        }
        nMsg_ = 0;
      }

      // then send this message
      const MessagePosition & position = messageIndex_[nMsg_];
      nMsg_ += 1;
      totalMessageCount_ += 1;
      size_t messageStart = position.first;
      size_t messageLength = position.second;
      fseek(dataFile_,  messageStart, SEEK_SET);
      assert(messageLength <= bufferSize_);
      size_t bytesRead = fread(buffer_.get(), 1, messageLength, dataFile_);
      assert (bytesRead == messageLength);

      if(verbose_)
      {
        std::cout << "Send message #" << nMsg_ + 1 << " of " << messageIndex_.size() << std::endl;
        std::cout << "         to: " << multicastAddress_ << '|' << sendAddress_ << ':' << portNumber_ << std::endl;

        std::cout << "Msg:";
        for(size_t nByte = 0; /*nByte < 10 && */ nByte < bytesRead; ++nByte)
        {
          if(nByte % 16 == 0) std::cout << std::endl;
          std::cout << ' ' << std::hex << std::setw(2)<< std::setfill('0') << (unsigned short (buffer_[nByte]) & 0xFF) << std::dec;
        }
        std::cout << std::endl;
      }

      if(pauseEveryMessage_)
      {
        waitForEnter();
      }
      socket_.send_to(boost::asio::buffer(buffer_.get(), messageLength), endpoint_);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    ioService_.stop();
  }
}


void
PcapToMulticast::fini()
{
}

