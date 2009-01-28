// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
using NUnit.Framework;
using NUnit.Framework.SyntaxHelpers;
using System.IO;

namespace QuickFASTDotNetTests
{
    [TestFixture]
    [Category("Prerequisites")]
	public class PrerequisitTests
	{
        [Test]
        [Description("Verify templates.xml can be found.")]
        public void VerifyTemplateFileExists()
        {
            Assert.That(File.Exists("..\\TestData\\templates.xml"));
        }

        [Test]
        [Description("Verify marketdata.out can be found.")]
        public void VerifyMarketDataFileExists()
        {
            Assert.That(File.Exists("..\\TestData\\marketdata.out"));
        }

        [Test]
        [Description("Verify fixfast.dat can be found.")]
        public void VerifyFixFastFileExists()
        {
            Assert.That(File.Exists("..\\TestData\\fixfast.dat"));
        }   
    }
}
