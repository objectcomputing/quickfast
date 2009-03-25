using System;
using System.Collections.Generic;
using System.Text;
using QuickFASTDotNet.Messages;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class MessageProcessor
        {
            private ulong messageCount_ = 0;

            public MessageProcessor()
            {
                messageCount_ = 0;
            }

            public ulong getMesssageCount()
            {
                return messageCount_;
            }

            public bool MessageReceived(Message message)
            {
                messageCount_++;
                return true;
            }

        }
    }
}
