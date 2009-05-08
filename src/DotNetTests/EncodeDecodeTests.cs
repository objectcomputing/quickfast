// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
using NUnit.Framework;
using NUnit.Framework.SyntaxHelpers;
using System.IO;
using QuickFASTDotNet.Codecs;
using QuickFASTDotNet.Messages;

namespace QuickFASTDotNetTests
{
    [TestFixture]
    [Category("Functionality Tests")]
    public class EncodeDecodeTests
    {
        [Test]
        [Description("Test decoding.")]
        public void TestDecoding()
        {
            using(FileStream templateStream = new FileStream("..\\TestData\\templates.xml", FileMode.Open),
                dataStream = new FileStream("..\\TestData\\fixfast.dat", FileMode.Open))
            {
                Decoder decoder = new Decoder(TemplateRegistry.Parse(templateStream), dataStream);
                Message message = decoder.Decode();
            }
        }


        [Test]
        [Description("Test decode and encode round trip.")]
        public void TestRoundTrip()
        {
            using (FileStream templateStream = new FileStream("..\\TestData\\templates.xml", FileMode.Open),
                dataStream = new FileStream("..\\TestData\\fixfast.dat", FileMode.Open))
            {

                TemplateRegistry templateRegistry = TemplateRegistry.Parse(templateStream);
                Decoder decoder = new Decoder(templateRegistry, dataStream);

                Message message = decoder.Decode();
                string outputFile = "..\\TestData\\testoutput.dat";
                if (File.Exists(outputFile))
                {
                    File.Delete(outputFile);
                }

                using (FileStream outStream = new FileStream(outputFile, FileMode.CreateNew))
                {
                    Encoder encoder = new Encoder(templateRegistry, outStream);
                    encoder.Encode(32, message);
                }
            }
        }
    }
}
