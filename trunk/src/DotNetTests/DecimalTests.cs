// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

using NUnit.Framework;
using NUnit.Framework.SyntaxHelpers;

namespace QuickFASTDotNetTests
{
    [TestFixture]
    [Category("Functionality Tests")]
    public class DecimalTests
    {
        [Test]
        public void TestConstruction1()
        {
            new QuickFASTDotNet.Decimal();
        }

        [Test]
        public void TestConstruction2()
        {
            int mantissa = 25;
            sbyte exponent = 21;

            QuickFASTDotNet.Decimal fpDecimal = new QuickFASTDotNet.Decimal(mantissa, exponent);

            Assert.That(mantissa, Is.EqualTo(fpDecimal.Mantissa));
            Assert.That(exponent, Is.EqualTo(fpDecimal.Exponent));
        }

        [Test]
        [Ignore("Ignore until Equals() is implemented")]
        public void TestEquality()
        {

        }
    }
}
