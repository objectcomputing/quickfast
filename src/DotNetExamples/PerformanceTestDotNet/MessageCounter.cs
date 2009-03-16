using System;
using System.Collections.Generic;
using System.Text;
using QuickFASTDotNet.Messages;

namespace QuickFAST
{
    namespace Examples
    {
        class MessageCounter
        {
            private ulong messageCount_ = 0;

            public MessageCounter()
            {
                messageCount_ = 0;
            }

            public ulong  getMesssageCount ()
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
