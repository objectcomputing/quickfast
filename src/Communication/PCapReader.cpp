// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#include <Common/QuickFASTPch.h>
#include "PCapReader.h"
#ifdef _WIN32
#include <Winsock2.h>
#else
#include <netinet/in.h>
#endif

using namespace QuickFAST;
using namespace Communication;

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
  enum linkType
  {
    DLT_EN10MB = 1, /* Ethernet (10Mb) */
    DLT_EN3MB = 2, /* Experimental Ethernet (3Mb) */
    DLT_AX25 = 3, /* Amateur Radio AX.25 */
    DLT_PRONET = 4, /* Proteon ProNET Token Ring */
    DLT_CHAOS = 5, /* Chaos */
    DLT_IEEE802 = 6, /* IEEE 802 Networks */
    DLT_ARCNET = 7, /* ARCNET */
    DLT_SLIP = 8, /* Serial Line IP */
    DLT_PPP = 9, /* Point-to-point Protocol */
    DLT_FDDI = 10, 	/* FDDI */
    DLT_LINUX_SLL = 113, /* Linux cooked sockets */
    DLT_NULL = 0 /* no link-layer encapsulation */
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

  struct pcap_pkthdr32 {
    uint32 tv_sec;
    uint32 tv_usec;
    uint32 caplen;	/* length of portion present */
    uint32 len;	/* length this packet (off wire) */
  };

  struct pcap_pkthdr64 {
    uint64 tv_sec;
    uint64 tv_usec;
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

  struct linuxCookedCaptureHeader{
    uint16 packetType;
    uint16 linkAddressType;
    uchar filler; // cooked Capture Headers do not get properly byte swapped (????)
    uchar linkAddressLength;
    uchar linkAddress[8]; // link address
    uint16 protocol;  /* s/b 0x8 for IP */
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

}

PCapReader::PCapReader()
: fileSize_(0)
, pos_(0)
, ok_(false)
, usetv32_(false)
, usetv64_(false)
, linktype_(DLT_NULL)
, swap(false)
, verbose_(false)
{
}

bool
PCapReader::open(const char * filename, std::ostream * dumpFile)
{
  ok_ = true;
  FILE * file = fopen(filename, "rb");
  ok_ = file != 0;
  if(ok_)
  {
    fseek(file, 0, SEEK_END);
    fileSize_ = ftell(file);
    buffer_.reset(new unsigned char[fileSize_]);
    fseek(file, 0, SEEK_SET);
    unsigned char * rawBuffer = buffer_.get();
    size_t byteCount = fread(rawBuffer, 1, fileSize_, file);
    fclose(file);
    ok_ = byteCount == fileSize_;
    if(dumpFile != 0)
    {
      *dumpFile << std::hex << std::setfill('0') <<std::endl;
      for(size_t pos = 0; pos < byteCount;)
      {
        *dumpFile << std::setw(4) << pos << ' ';
        size_t end = pos + 16;
        for(;pos < end && pos < byteCount; ++pos)
        {
          *dumpFile << ' ' << std::setw(2) << (short) rawBuffer[pos];
        }
        *dumpFile << std::endl;
      }
      *dumpFile << std::dec;
    }
  }
  if(ok_)
  {
    ok_ = rewind();
  }
  return ok_;
}

bool
PCapReader::rewind()
{
  ok_ = true;
  pos_ = 0;

  //////////////////////////
  // Process the file header
  if(fileSize_ - pos_ < sizeof(pcap_file_header))
  {
    std::cerr << "Invalid pcap file: no header." << std::endl;
    ok_ = false;
  }
  if(ok_)
  {
    pcap_file_header * fileHeader = reinterpret_cast<pcap_file_header *>(buffer_.get() + pos_);
    pos_ += sizeof(pcap_file_header);

    if(fileHeader->magic != nativeMagic && fileHeader->magic != swappedMagic)
    {
      std::cerr << "Invalid pcap file: missing magic." << std::endl;
      ok_ = false;
    }
    if(ok_)
    {
      swap.setSwap(fileHeader->magic == swappedMagic);
      assert(swap(fileHeader->magic) == nativeMagic);
      if(verbose_)
      {
        std::cout << "PCapReader: Setting swap to : " << (fileHeader->magic == swappedMagic) << std::endl;
      }
    }
    linktype_ = swap(fileHeader->linktype);
  }
  return ok_;
}

bool
PCapReader::good()const
{
  return ok_;
}

bool
PCapReader::read(const unsigned char *& buffer, size_t & size)
{
  if(ok_)
  {
    ok_ = false;
    size_t skipped = 0;
    size_t minBytes = sizeof(pcap_pkthdr) + sizeof(ip_header) + sizeof(udp_header);
    if(usetv32_)
    {
      minBytes = sizeof(pcap_pkthdr32) + sizeof(ip_header) + sizeof(udp_header);
    }
    if(usetv64_)
    {
      minBytes = sizeof(pcap_pkthdr64) + sizeof(ip_header) + sizeof(udp_header);
    }

    if(verbose_)
    {
      std::cout << "PCapReader: Starting read position: " << pos_ << " file size: " << fileSize_
                << " minimum packet size: " << minBytes << std::endl;
      if(usetv32_)
      {
        std::cout << "PCapReader: reading from 32 bit packet capture" << std::endl;
      }
      else if(usetv64_)
      {
        std::cout << "PCapReader: reading from 64 bit packet capture" << std::endl;
      }
      else
      {
        std::cout << "PCapReader: reading from unidentified packet capture" << std::endl;
      }
    }

    while(!ok_ && (pos_ + minBytes < fileSize_))
    {
      ////////////////////////////
      // process the packet header
      size_t headerPos = pos_;
      size_t datalen = 0;
      size_t expectlen = 0;
      bool truncate = false;
//      unsigned int dataLinkType = 0;

      if(usetv32_)
      {
        pcap_pkthdr32 * packetHeader = reinterpret_cast<pcap_pkthdr32 *>(buffer_.get() + pos_);
        pos_ += sizeof(pcap_pkthdr32);
        datalen = swap(packetHeader->caplen);
        expectlen = swap(packetHeader->len);
        truncate = (packetHeader->caplen != packetHeader->len);
      }
      else if(usetv64_)
      {
        pcap_pkthdr64 * packetHeader = reinterpret_cast<pcap_pkthdr64 *>(buffer_.get() + pos_);
        pos_ += sizeof(pcap_pkthdr64);
        datalen = swap(packetHeader->caplen);
        expectlen = swap(packetHeader->len);
        truncate = (packetHeader->caplen != packetHeader->len);
      }
      else
      {
        pcap_pkthdr * packetHeader = reinterpret_cast<pcap_pkthdr *>(buffer_.get() + pos_);
        pos_ += sizeof(pcap_pkthdr);
        datalen = swap(packetHeader->caplen);
        expectlen = swap(packetHeader->len);
        truncate = (packetHeader->caplen != packetHeader->len);
      }
      if(verbose_)
      {
        std::cout << "PCapReader: after header position: " << pos_ << " data length: " << datalen;
      }
      if(truncate)
      {
        pos_ += datalen;
        skipped+= 1;
        if(verbose_)
        {
          std::cout << "  Truncated. received 0x"  << std::hex << datalen
                    << " expected 0x" << expectlen << std::dec << std::endl;
        }
      }
      else
      {
        if(verbose_)
        {
          std::cout << std::endl;
        }
        bool found = false;
        switch(linktype_)
        {
        case DLT_EN10MB:
          {
            if(verbose_)
            {
              std::cout << "PCapReader: Ethernet packet." << std::endl;
            }
            pos_ += sizeof(ethernetIIHeader);
            datalen -= sizeof(ethernetIIHeader);
            found = true;
            break;
          }
        case DLT_LINUX_SLL:
          {
            if(verbose_)
            {
              std::cout << "PCapReader: Linux cooked socket packet." << std::endl;
            }
            pos_ += sizeof(linuxCookedCaptureHeader);
            datalen -= sizeof(linuxCookedCaptureHeader);
            found = true;
            break;
          }
        default:
          {
            if(verbose_)
            {
              std::cout << "PCapReader: Other type of packet.  Checking for IP protocol flag." << std::endl;
            }
            // HACK!look for the IP protocol flag to mark the end of the the link layer header
            static unsigned short IPProtocol = 0x0008;
            while(!found && datalen > 2)
            {
              unsigned short protocol = *(unsigned short *)(buffer_.get() + pos_);
              if(swap(protocol) == IPProtocol)
              {
                found = true;
                pos_ += 2;
                datalen -= 2;
              }
              else
              {
                pos_ += 1;
                datalen -= 1;
              }
            }
            break;
          }
        }
        if(found)
        {
          ip_header * ipHeader = reinterpret_cast<ip_header *>(buffer_.get() + pos_);
          // IP header contains its own length expressed in 4 byte units.
          size_t ipLen = (ipHeader->ver_ihl & 0xF) * 4;
          pos_ += ipLen;
          datalen -= ipLen;
          udp_header * udpHeader = reinterpret_cast<udp_header*>(buffer_.get() + pos_);
          pos_ += sizeof(udp_header);
          datalen -= sizeof(udp_header);

          // udplen includes udp header + cargo
          // udplen is stored in network byte order
          size_t udplen = ntohs(udpHeader->len);

          buffer = buffer_.get() + pos_;
          // trust the udp header for actual cargo size
          // but continue to trust pos_ and datalen (originates in pcap header) for position in buffer.
          size = udplen - sizeof(udp_header);
          if(verbose_)
          {
            std::cout << "PCapReader: " << headerPos << ": " << pos_ << ' ' << size
              << "=== 0x" << std::hex  << headerPos << ": 0x" << pos_ << " 0x" << size << std::dec << std::endl;
          }
          ok_ = true;
          pos_ += datalen;
        }
        else
        {
          if(verbose_)
          {
            std::cout << "PCapReader: could not find packet. Skipping " << datalen << " bytes." << std::endl;
          }
          pos_ += datalen;
          skipped += 1;
        }

      }
    }
    if(skipped != 0)
    {
      std::cerr << "Warning: ignoring " << skipped << " truncated packets." << std::endl;
    }
  }
  return ok_;
}

void
PCapReader::setVerbose(bool verbose)
{
  verbose_ = verbose;
}

void
PCapReader::seek(size_t address)
{
  pos_ = address;
}

