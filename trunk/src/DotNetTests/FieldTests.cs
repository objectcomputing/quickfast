// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
using NUnit.Framework;
using NUnit.Framework.SyntaxHelpers;
using QuickFASTDotNet;
using QuickFASTDotNet.Messages;
using System.Text;

namespace QuickFASTDotNetTests
{
    [TestFixture]
    [Category("Functionality Tests")]
    public class FieldTests
    {
        [Test]
        public void TestInt32FieldContruction()
        {
            int value = 287;
            Int32Field field = new Int32Field(287);

            Assert.That(field.Value, Is.EqualTo(value));
        }


        [Test]
        public void TestInt64FieldContruction()
        {
            long value = 287287;
            Int64Field field = new Int64Field(value);

            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        public void TestUInt32FieldContruction()
        {
            uint value = 287;
            UInt32Field field = new UInt32Field(value);

            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        public void TestUInt64FieldContruction()
        {
            ulong value = 287;
            UInt64Field field = new UInt64Field(value);

            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        public void TestDecimalFieldContruction()
        {
            Decimal value = new Decimal(234, 20);
            DecimalField field = new DecimalField(value);
            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        public void TestAsciFieldConstruction()
        {
            string value = "QuickFASTDotNet construction test";
            QuickFASTDotNet.Messages.AsciStringField field = new QuickFASTDotNet.Messages.AsciStringField(value);

            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        public void TestUnicodeStringFieldContruction()
        {
            string value = "QuickFASTDotNet construction test";
            UnicodeStringField field = new UnicodeStringField(value);

            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        public void TestByteVectorFieldContruction()
        {
            byte[] value = Encoding.ASCII.GetBytes("QuickFASTDotNet construction test.");
            ByteVectorField field = new ByteVectorField(value);
   
            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        [Ignore("Ignore until there is a way to create an instance of the FielsSet")]
        public void TestGroupFieldContruction()
        {
            FieldSet value = new FieldSet();
            GroupField field = new GroupField(value);
            Assert.That(field.Value, Is.EqualTo(value));
        }

        [Test]
        [Ignore("Ignore until EqualTo() is implemented")]
        public void TestSequenceFieldContruction()
        {
            FieldSet fieldSet = new FieldSet();
            Sequence value = new Sequence();
            value.Add(fieldSet);

            SequenceField field = new SequenceField(value);
            Assert.That(field.Value, Is.EqualTo(value));
        }
    }
}
