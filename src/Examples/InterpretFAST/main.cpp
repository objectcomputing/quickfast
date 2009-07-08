// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include <Examples/ExamplesPch.h>
#if 0
#include <InterpretFAST/InterpretFAST.h>

using namespace QuickFAST;
using namespace Examples;

int main(int argc, char* argv[])
{
  int result = -1;
  InterpretFAST application;
  if(application.init(argc, argv))
  {
    result = application.run();
    application.fini();
  }
  return result;
}
#else
#include <boost/asio.hpp>
/*
            IPAddress listenInterface = IPAddress.Parse("10.10.9.56");
            IPEndPoint listenChannel = new IPEndPoint(IPAddress.Parse("224.1.2.133"), 13014);

            IPEndPoint listenEndpoint = new IPEndPoint(listenInterface, listenChannel.Port); //.NET-Windows
            //IPEndPoint listenEndpoint = listenChannel; //Mono-Linux
            EndPoint remoteEP = (EndPoint)new IPEndPoint(IPAddress.Any, 0);

            Socket listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            listenSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            listenSocket.Bind(listenEndpoint);
            listenSocket.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.MulticastInterface, listenInterface.GetAddressBytes());
            listenSocket.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, new MulticastOption(listenChannel.Address, listenInterface));
            Trace.WriteLine(string.Format("Multicast Listener started for channel {0} on interface {1}", listenChannel, listenInterface));

            byte[] buffer = new byte[2048];

            while (true)
            {
                int bytes = listenSocket.ReceiveFrom(buffer, 0, buffer.Length, SocketFlags.None, ref remoteEP);
                byte b = buffer[0];
                Console.Write(bytes);
            }
        }

*/

namespace
{
//  const char * listenIP = "0.0.0.0";
const char * listenIP = "10.10.9.56";
//  const char * listenIP = "10.201.200.116";  // At OCI
  const unsigned short listenPort = 13014;
//  const unsigned short listenPort = 30001;
  const char * multicastIP = "224.1.2.133";
//  const char * multicastIP = "239.255.0.1";

}

class Worker
{
public:
  Worker()
    : stopping_(false)
    , loop_(0)
    , loopLimit_(1000)
    , ioService_()
    , socket_(ioService_)
  {
  }

  void run()
  {
#if 10
    try
    {
      boost::asio::ip::address listenInterface(boost::asio::ip::address::from_string(listenIP));
      boost::asio::ip::address listenChannel(boost::asio::ip::address::from_string(multicastIP));
      boost::asio::ip::udp::endpoint listenEndpoint(listenInterface, listenPort);

      socket_.open(listenEndpoint.protocol());
      socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
      socket_.bind(listenEndpoint);

      boost::asio::ip::multicast::join_group joinRequest(listenChannel.to_v4(), listenInterface.to_v4());
      socket_.set_option(joinRequest);
      std::cout << "Listening on " << socket_.local_endpoint().address().to_string()
        << ':' << socket_.local_endpoint().port()
        << " for multicast group " << listenChannel.to_string()
        << std::endl;

      // receive the first 5 packets in-line just to prove we can.
      for(size_t x = 0; x < 5; ++x)
      {
        size_t bytes = socket_.receive_from(
          boost::asio::buffer(buffer_, sizeof(buffer_)),
          senderEndpoint_);
        std::cout << bytes << ' ';
      }
      std::cout << std::endl << "Switch to asynchronous." << std::endl;
      startReceive();
      ioService_.run();
    }
    catch (const std::exception & ex)
    {
      std::cout << std::endl << "Caught: " << ex.what() << std::endl;
    }
#else
#define MSGBUFSIZE 1600
#define HELLO_PORT 13014
#define HELLO_GROUP "224.1.2.133"
#define HELLO_INTERFACE "10.10.9.56"

     sockaddr_in addr;
     SOCKET fd;
     int nbytes,addrlen;
     ip_mreq mreq;
     char msgbuf[MSGBUFSIZE];

     const char yes = 1;            /*** MODIFICATION TO ORIGINAL */

     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
  	  perror("socket");
      exit(1);
     }

/**** MODIFICATION TO ORIGINAL */
    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
       perror("Reusing ADDR failed");
       exit(1);
       }
/*** END OF MODIFICATION TO ORIGINAL */

     /* set up destination address */
     memset(&addr,0,sizeof(addr));
     inet_pton(AF_INET, HELLO_INTERFACE, &addr.sin_addr);
     addr.sin_family=AF_INET;
//     addr.sin_addr.s_addr=htonl(INADDR_ANY);
     addr.sin_port=htons(HELLO_PORT);

     /* bind to receive address */
     if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
  	  perror("bind");
      exit(1);
     }

     /* use setsockopt() to request that the kernel join a multicast group */
     mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);
     mreq.imr_interface.s_addr=inet_addr(HELLO_INTERFACE);
     if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP, reinterpret_cast<const char *>(&mreq),sizeof(mreq)) < 0) {
      perror("setsockopt");
      exit(1);
     }

     /* now just enter a read-print loop */
     while (1) {
  	    addrlen=sizeof(addr);
        if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0,
                 (struct sockaddr *) &addr,&addrlen)) < 0) {
             perror("recvfrom");
             exit(1);
        }
        std::cout << ' ' << nbytes << std::endl;
     }
#endif
  }
private:
    void handleReceive(
    const boost::system::error_code& error,
    size_t bytesReceived)
  {
    if(stopping_)
    {
      return;
    }
    if (!error)
    {
      ++loop_;
      try
      {
        std::cout << ' ' << bytesReceived;
        if(loop_ > loopLimit_ && loopLimit_ != 0)
        {
          stopping_ = true;
          socket_.cancel();
        }
        else
        {
          startReceive();
        }
      }
      catch (const std::exception &ex)
      {
        stopping_ = true;
        socket_.cancel();
        std::cout << "Exception: " << ex.what() << std::endl;
      }
    }
    else
    {
      std::cout << "Communication error " << error.message() << std::endl;
      stopping_ = true;
      socket_.cancel();
    }
  }

  void startReceive()
  {
    socket_.async_receive_from(
      boost::asio::buffer(buffer_, sizeof(buffer_)),
      senderEndpoint_,
        boost::bind(&Worker::handleReceive,
          this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)
        );

  }


private:
  bool stopping_;
  size_t loop_;
  size_t loopLimit_;
  boost::asio::io_service ioService_;
  boost::asio::ip::udp::socket socket_;
  char buffer_[2048];
  boost::asio::ip::udp::endpoint senderEndpoint_;

};




int main(int argc, char * argv[])
{
  int result = -1;
  Worker worker;
  worker.run();
  return result;
}

#endif
