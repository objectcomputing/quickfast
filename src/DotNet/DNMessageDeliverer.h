// Copyright (c) 2009, 2010 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once
#include <DotNet/DNMessageBuilder.h>
#include <DotNet/DNMessage.h>

namespace QuickFAST
{
  namespace DotNet
  {
    /// An implemntation class
    class ImplFieldSetBuilder;

    public ref class DNMessageDeliverer
      : public DNMessageBuilder
    {
    public:
      DNMessageDeliverer();
      virtual ~DNMessageDeliverer();
      !DNMessageDeliverer();
      virtual Messages::ValueMessageBuilder & builder()override;

      /// @brief Delagate to handle incoming messages
      /// @returns true if decoding should continue. False to stop decoder
      delegate bool MessageReceiver(DNMessageDeliverer ^ builder, DNMessage ^ message);

      /// @brief the event to signal incoming messages
      ///
      /// Usage:
      ///   myMessageBuilder.MessageReceived +=
      ///       new DNMessageDeliverer.MessageReceiver(myMessageHandler);
      ///  myMessageHandler will be called with a reference to myMessageBuilder
      ///    properties on the DNMessageDeliverer may be used to retrieve the fields
      ///    of the incoming message.
      event MessageReceiver ^ MessageReceived;

      ///@brief Delegate to handle log messages
      delegate bool LogHandler(
        DNMessageDeliverer ^ builder,
        unsigned short logLevel,
        System::String ^ reason);

      /// @brief event to signal a message is available to be logged
      event LogHandler ^ LogMessage;

      /// @brief set the lowest log message of interest
      void setLogLimit(unsigned short logLimit);

      ///@brief Delegate to handle all types of errors from QuickFAST
      delegate bool ErrorHandler(
        DNMessageDeliverer ^ builder,
        System::String ^ reason);

      /// @brief the event to signal communications errors.
      ///
      /// Usage:
      ///  myMessageBuilder.CommunicationError +=
      ///     new DNMessageDeliverer.ErrorHandler(myErrorHandler);
      ///  myErrorHandler will be called with a String describing the
      ///  problem.  In most cases decoding will continue with the next
      ///  message after the one causeing the error.
      event ErrorHandler ^ CommunicationError;

      /// @brief the event to signal decoding errors.
      ///
      /// Usage:
      ///  myMessageBuilder.DecodingError +=
      ///     new DNMessageDeliverer.ErrorHandler(myErrorHandler);
      ///  myErrorHandler will be called with a String describing the
      ///  problem.  In most cases decoding will continue with the next
      ///  message after the one causeing the error.
      event ErrorHandler ^ DecodingError;

      ///@brief Delegate handle receiver status changes
      delegate void ReceiverStatusChange(DNMessageDeliverer ^ builder);

      /// @brief the event to signal start of multicast communication
      ///
      /// Usage:
      ///  myMessageBuilder.ReciverStarted +=
      ///     new DNMessageDeliverer.ReceiverStatusChange(myStatusHandler);
      ///  myStatusHandler will be called when the multicast builder
      /// is started..
      event ReceiverStatusChange ^ ReceiverStarted;

      /// @brief the event to signal end of multicast communication
      ///
      /// Usage:
      ///  myMessageBuilder.ReceiverStopped +=
      ///     new DNMessageDeliverer.ReceiverStatusChange(myStatusHandler);
      ///  myStatusHandler will be called when the multicast builder
      /// is stopped..
      event ReceiverStatusChange ^ ReceiverStopped;

      // semi-private: for internal use only
      // managed classes don't support friends
      bool signalMessageArrived(DNMessage ^ message);
      bool signalLogMessage(unsigned short level, System::String ^ message);
      bool signalCommunicationError(System::String ^ message);
      bool signalDecodingError(System::String ^ message);
      void signalReceiverStarted();
      void signalReceiverStopped();

    private:
      void release();

    private:
      ImplFieldSetBuilder * fieldSetBuilder_;
    };
  }
}

