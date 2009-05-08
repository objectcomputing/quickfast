using NUnit.Framework;
using NUnit.Framework.SyntaxHelpers;
using System.IO;
using QuickFASTDotNet.Messages;
using QuickFASTDotNet;

namespace QuickFASTDotNetTests
{
    [TestFixture]
    public class MessageTests
    {

#if QUICKFAST_DOTNET_ENCODING
        [Test]
        public void BuildMessage()
        {
            QuickFASTDotNet.Messages.FieldIdentity identity = new QuickFASTDotNet.Messages.FieldIdentity();
            identity.LocalName = "Test ascii field";

            QuickFASTDotNet.Messages.AsciStringField testAsciiField = new QuickFASTDotNet.Messages.AsciStringField("Some ascii value.");

            QuickFASTDotNet.Messages.Message message = new QuickFASTDotNet.Messages.Message();

            message.AddField(identity, testAsciiField);

            QuickFASTDotNet.Messages.Field field = message["Test ascii field"].Value;
            QuickFASTDotNet.Messages.ByteVectorField testField = new QuickFASTDotNet.Messages.ByteVectorField(null);
            Assert.That(field, Is.EqualTo(testAsciiField));
        }

        [Test]
        public void TestBuildMessage()
        {
            //Build a message here.            

            //Build the field sets that will be included in the sequence.
            FieldSet fieldSet = new FieldSet();

            fieldSet.AddField(MDUpdateAction, new UInt32Field(2));
            fieldSet.AddField(MDPriceLevel, new UInt32Field(1));
            fieldSet.AddField(MDEntryType, new AsciStringField("0"));
            fieldSet.AddField(MDEntryTime, new UInt32Field(181204000));
            fieldSet.AddField(SecurityIDSource, new UInt32Field(8)); 
            fieldSet.AddField(SecurityID, new UInt32Field(625223));
            fieldSet.AddField(RptSeq, new UInt32Field(278271));
            fieldSet.AddField(MDEntryPx, new DecimalField(new Decimal(196875, -5)));
            fieldSet.AddField(MDEntrySize, new Int32Field(20));
            fieldSet.AddField(NumberOfOrders, new UInt32Field(1)); 
            fieldSet.AddField(TradingSessionID, new AsciStringField("2"));

            //Add the first FieldSet.
            Sequence sequence = new Sequence();
            sequence.Add(fieldSet);

            //Build the second FieldSet
            fieldSet = new FieldSet();

            fieldSet.AddField(MDUpdateAction, new UInt32Field(0));
            fieldSet.AddField(MDPriceLevel, new UInt32Field(3));
            fieldSet.AddField(MDEntryType, new AsciStringField("0"));
            fieldSet.AddField(MDEntryTime, new UInt32Field(181204000));
            fieldSet.AddField(SecurityIDSource, new UInt32Field(8));
            fieldSet.AddField(SecurityID, new UInt32Field(625223));
            fieldSet.AddField(RptSeq, new UInt32Field(278272));
            fieldSet.AddField(MDEntryPx, new DecimalField(new Decimal(196875, -6)));
            fieldSet.AddField(MDEntrySize, new Int32Field(20));
            fieldSet.AddField(NumberOfOrders, new UInt32Field(1));
            fieldSet.AddField(TradingSessionID, new AsciStringField("2"));

            //Add the second FieldSet
            sequence.Add(fieldSet);

            //Build the main message body.
            Message message = new Message();
            message.AddField(ApplVerID, new AsciStringField("8"));
            message.AddField(MessageType, new AsciStringField("X"));
            message.AddField(SenderCompID, new AsciStringField("CME"));
            message.AddField(MsgSeqNum, new UInt32Field(12883180));
            message.AddField(SendingTime, new Int64Field(20081017181204616));
            message.AddField(TradeDate, new UInt32Field(20081017));
            message.AddField(MDEntries, new SequenceField(sequence));
        }
#endif

        private void SetupIdentities()
        {

        }


        //Main message identities
        private FieldIdentity ApplVerID = new FieldIdentity("", "ApplVerID", "1128");
        private FieldIdentity MessageType = new FieldIdentity("", "MessageType", "35");
        private FieldIdentity SenderCompID = new FieldIdentity("", "SenderCompID", "49");
        private FieldIdentity MsgSeqNum = new FieldIdentity("", "MsgSeqNum", "34");
        private FieldIdentity SendingTime = new FieldIdentity("", "SendingTime", "52");
        private FieldIdentity TradeDate = new FieldIdentity("", "TradeDate", "75");
        private FieldIdentity MDEntries = new FieldIdentity("MDEntries");

        //Entries identities
        private FieldIdentity MDUpdateAction = new FieldIdentity("", "MDUpdateAction", "279");
        private FieldIdentity MDPriceLevel = new FieldIdentity("", "MDPriceLevel", "1023");
        private FieldIdentity MDEntryType = new FieldIdentity("", "MDEntryType", "269");
        private FieldIdentity MDEntryTime = new FieldIdentity("", "MDEntryTime", "273");
        private FieldIdentity SecurityIDSource = new FieldIdentity("", "SecurityIDSource", "22");
        private FieldIdentity SecurityID = new FieldIdentity("", "SecurityID", "48");
        private FieldIdentity RptSeq = new FieldIdentity("", "RptSeq", "83");
        private FieldIdentity MDEntryPx = new FieldIdentity("", "MDEntryPx", "270");
        private FieldIdentity MDEntrySize = new FieldIdentity("", "MDEntrySize", "271");
        private FieldIdentity NumberOfOrders = new FieldIdentity("", "NumberOfOrders", "346");
        private FieldIdentity TradingSessionID = new FieldIdentity("", "TradingSessionID", "336");
    }
}
