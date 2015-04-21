Questions



&lt;hr/&gt;



## I'm having trouble building my application when I include QuickFAST headers! ##

There are two issues that may be causing you problems.

QuickFAST assumes that the $QUICKFAST\_HOME/src directory is on the include path.

> | $QUICKFAST\_HOME refers to the directory where QuickFAST is installed.|
|:----------------------------------------------------------------------|

Also, QuickFAST is designed to work in an environment where precompiled headers are used. This means many of the QuickFAST header files assume that a common set of headers is already available.  If you include the individual QuickFAST headers directly you are likely to run into undefined symbols.

To resolve this most applications should set $QUICKFAST\_HOME/src as an include directory and include <Application/QuickFAST.h> before including any other QuickFAST headers.   Applications that use precompiled headers should include this header in their precompiled header file.

In fact because the QuickFAST.h header includes the most commonly used QuickFAST headers, it is likely that this will be the only QuickFAST-related include needed by applications that depend on QuickFAST.

## When is it beneficial to use multithreading to process feeds in QuickFAST? ##

Ideally a single thread should be able to handle the load from a single feed.  If you application is handling multiple feeds then there may be an advantage having a separate thread for each feed.  In this case you should avoid as much as possible sharing the data between the threads (at least avoid sharing mutable data) to avoid the need to synchronize access.  Synchronization can be a very expensive operation that will adversely affect your performance.
That being said, for many feeds the market data entry (one entry in the sequence of market data entries) is often a good unit to process.

## I want to use QuickFAST library in my multithreaded program.  Can I use a single Decoder with two threads? ##

The Codecs::Decoder cannot be safely used by more than one thread simultaneously.   It also cannot be used to decode more than one stream of FAST encoded data. 

This is intentional.  A thread-safe decoder would be very slow.  A decoder that attempted to decode messages from multiple sources would produce incorrect results due to the existence of the dictionary.

To use QuickFAST properly 
  * Create a separate instance of Codecs::SynchronousDecoder for each incoming source of FAST-encoded messages.
  * Ensure that at most one thread can be executing code within that decoder at any point in time.

To see examples, take a look at Application::DecoderConnection and Application::DecoderConfiguration.

Assuming you have a single process accepting data from two separate FAST feeds. You should create two decoder objects, one for each feed.

You should ensure that no more than one thread is executing code within each of these decoders at any point in time.   That is two threads can be running, each one in a separate decoder.  QuickFAST can be using additional threads to accept incoming packets and add them to the queue waiting to be decoded, but it cannot use more than two to do the actual decoding.

From a hardware point of view, if you have four cores in your system two of them can be in the decoders at the same time (one per decoder) and the other two cores are available for other tasks.  If no other tasks are available, the extra cores will be idle because they can't help with the decoding process.

If you have more feeds you can have more decoders, hence more active threads and cores, and the DecoderConnection object with an appropriate Receiver configured will handle all of this for you.

## If I receive data on the same feed from TWO sources how many decoders should I create - one or two? ##

Assuming you arbitrate the incoming messages to be sure that you decode each incoming packet once then you should send all of the packets to the same decoder.  The data from one feed should be processed in order -- not only because the FAST protocol expects it, but also because market data has an inherent order.  

## Is it important to pass packets from a feed in sequence order to a Decoder? ##

Yes. It is important that the packets be handled in order. Even if you are resetting the dictionary on every packet which many sources require, you still want to see the market data in the correct order so that your client has an accurate representation of the market book and trade history.

## How do I use QuickFAST to receive data from multiple multicast IPs? ##

There is built-in support for receiving multiple multicast streams in QuickFAST, InterpretApplication which is located in src/Examples/InterpretApplication.cpp.

For each connection you wish to support:
  * Create an Application::DecoderSettings object and populate it (call this settings)
  * Create a message builder to accept the decoded information.  (call this builder)
    * (GenericMessageBuilder will do, or derive your own from ValueMessageBuilder)
  * Create an Application::DecoderConnection object (call this connection)
    *  Call  connection.configure(builder, settings);
end for-each-connection.

On any one of the connections:  Call  connection.receiver().runThreads specifying how many threads you want to use and whether you want to use the main thread. It's OK to call runThreads on all the connections.  The threads are going into a common pool.

## How can I log the input received by the decoder? ##

The DataSource which is the base class for any object that provides input to the decoder includes the concept of an echo stream.  All input passed to the decoder will also be written to the echo stream.  You can choose to echo raw data, or a hex representation of the data.  You can also mark message and/or field boundaries in the echo stream.

Echo stream support is enabled by calling DataSource::setEcho().

The InterpretApplication example program demonstrates the echo stream support

## Does QuickFAST work with CME's FIX/FAST Market Data? ##

One of the goals of this project is to support FIX/FAST.  With that in mind quite a bit of testing has been done using the CME templates, and sample test data encoded using those templates.

Full support for FIX/FAST requires considerable more effort than simply accepting and decoding multicast FAST messages.  QuickFAST will remain focused on decoding FAST messages and delivering them to the application for subsequent processing.  QuickFAST provides a solid foundation on which a full featured FIX/FAST client can be built.

## Does QuickFAST work with CME's FIX/FAST 2.0 Market Data? ##

Background:  CME has announced a revision to their FAST-based protocol for distributing market data.  They are calling this FIX/FAST 2.0.  This naming has caused some confusion.  FIX/FAST 2.0 is based on FAST version 1.1.  They have added additional information to each packet (the preamble), but the actual FAST encoded data is unchanged.

Because QuickFAST supports the FAST specification version 1.1, it should support CME's FIX/FAST 2.0 without any major problems.

Support has recently been added for handling the FIX/FAST 2.0 preamble.  Documentation is still pretty sketchy.  See src\Codecs\HeaderAnalyzer and FixedSizeHeaderAnalyzer to get started.

Update:  Some users on the QuickFAST users' mailing list have reported success handling CME's FIX/FAST 2.0 data.  Check the mailing list archives or ask for details on the list.


## Does QuickFAST work with Arca? ##

The ArcaBook data feeds can be described as FAST-like than true FAST.  Arca developed their encoding scheme before the FAST Specification was available, and it departs in several ways from the published standard.   Particulars include compressing part of the message but leaving the remainder of the message in clear text; using field types that are not defined in the FAST standard; using fixed presence map bits rather than consuming the presence map bits as needed by the fields; and sending the fields in a different order from the presence map bits to which they correspond.

QuickFAST does not handle Arca data "out-of-the-box" but it contains hooks to allow Arca support to be added on.  At least one QuickFAST user has been live, receiving ARCA data, since mid-summer 2009.  Feel free to contact us at OCI ([mailto:sales@ociweb.com](mailto:sales@ociweb.com)) for more details.

## Does QuickFAST support version 1.2 of the FAST specification ? ##

Not yet. Version 1.2 of the specification is still being developed.  We expect to add support when the specification is complete. See the previous comment about sponsoring QuickFAST development.

## Why doesn't QuickFAST support dynamic templates? ##

Dynamic templates (not the same as dynamic template exchange!) are "included" templates selected by sending a template ID down the wire.  That is, in the middle of decoding a message, a `<templateRef>` tag says:  "Use the next thing on the wire as a persistence map and template ID of another template.  Switch to decoding using that template. When you reach the end of that template, return to this point in the original template."

When QuickFAST was first developed I could not find any exchanges that were sending out dynamic templates. This is a relatively expensive feature to support, so the first version of QuickFAST omitted it.   I have a completed design to add support for dynamic templates, but unless someone needs it and is willing to fund the effort, it will be on hold for a while.

Please note that QuickFAST <b>does</b> support static template references.  If the `<templateRef>` tag includes a `name="templatename"` attribute then the named template will be used by the decoder at this point in the stream.

## Does QuickFAST support SCP? ##
The Session Control Protocol (SCP) is a layer on top of FAST that provides support for managing FAST Sessions (i.e. connections) and Dynamic Template Exchange (sending FAST encoded templates "down-the-wire" to update the templates being used in the decoder.

This specification is still at the Draft stage.  There are three levels of compliance described in the draft SCP document.  QuickFAST is at compliance level 1 -- the simplest level.

I have completed a design for full SCP support but until the specification is finalized, or someone funds an implementation of the existing draft specification this will remain on hold.

In practical terms, this means QuickFAST does support the reset attribute of templates, but does not yet support dynamic template exchange.

As part of the SCP support QuickFAST now automatically recognizes the _magic number_ template ID 120 and uses it as an indicator that the decoder should be reset.  Other than reseting the decoder this template will be ignored.  In particular QuickFAST will **not** deliver an empty message to the application.

If you explicitly define a template with an ID of 120, this will override the implicit interpretation of template ID 120.  Your explicit definition of this template will be used, and the presence or absence of the reset="yes" attribute will determine whether the decoder will be reset.  Note that if you explicitly define template id="100" as an empty template, your application will receive an empty message.

## Why doesn't QuickFAST reset the decoder when it receives a template id of 120? The SCP document says that `<template id="120">` is a reset message. ##

Not exactly.   The SCP 1.1 document actually says a reset message looks like:
```
 <template name="Reset" scp:reset="yes" id="120"> <typeRef name="Reset"/> </template>
```
_(Note QuickFIX will accept the reset attribute either with our without the scp: namespace qualifier.)_

The document clarifies this in section 4.3 by saying

> NOTE: It is the reset property of the template used to encode the message rather than the Reset message itself that triggers the reset.

Unfortunately at least one exchange I know of has misread this and added this template definition to their XML file:
```
 <template name="FASTReset" id="120"/>
```

This explicit definition of the reset message actually prevents QuickFAST from resetting the decoder because I don't want to break pre-SCP templates that use template id 120 for other purposes.

If you are having this problem, either remove the definition of template 120 or add a reset="yes" to the definition.  Please also notify the source of the template file that their usage is non-standard.  If they question this, refer to section 4.3 and Appendix 2.3  of the SCP document which can be downloaded from this page: http://www.fixprotocol.org/fast

## Why can't I run the performance test?  Why can't I run the example programs? ##
Many of the example programs included with QuickFAST require a set of templates and sample FAST-encoded data to run.  We have these at OCI where we use them to test QuickFAST.  Unfortunately the test files we use are protected by non-disclosure agreements or other legal restrictions so we can't publish them.

If you are considering using QuickFAST as a trader, the simplest thing for you to do is to contact the counterparties with whom you will be communicating and ask for test templates, data file, or possibly test data feeds.

If you work for a source of market data and would like to be sure that your potential customers have a readily available open source package to support receiving your data, please consider contributing your templates and sample data.

## Can I build QuickFAST on MacOSX? ##
As mentioned on the Project Home page QuickFAST is regularly built and tested on Windows (Visual Studio) and Linux (gcc).  It is quite likely to run well on many other platforms.   Even though we have Macs available in our build farm here at OCI, we cannot commit the resources to set up a test and run it regularly unless we find one or more sponsors to help fund the work.


That being said, if you would like to build QuickFAST on MacOSX, follow the instructions for building for Linux.  When you run MPC, try this command:
```
  $MPC_ROOT/mwc.pl -type make -value_template platforms=macosx QuickFAST.mwc
```

Note: this requires MPC [Revision 1714](https://code.google.com/p/quickfast/source/detail?r=1714) created 2/16/2010.

Remember, though, if you plan to depend on QuickFAST on MacOSX it would be a prudent thing for you to open a support contract with OCI ([mailto:sales@ociweb.com](mailto:sales@ociweb.com)).  With OCI's support, you will be billed only for the time we actually spend supporting you.

## Can I build QuickFAST on Solaris ##
As mentioned in the previous FAQ entry, the only two platforms on which QuickFAST is regularly tested are Windows (Visual Studio) and Linux (gcc).  However, it is likely that QuickFAST can be built and run on Solaris.

You have two compiler choices on Solaris.  You may use the gcc compiler, or you may use the SunCC compiler.  If you want to to use gcc, just follow the instructions for building on Linux.

To use the SunCC compiler, follow the Linux instructions except use the following command to run MPC:

```
  $MPC_ROOT/mwc.pl -type make -value_template compilers=SunCC QuickFAST.mwc
```

We do have Solaris machines -- both X86 and Sparc in our build farm, so if Solaris support is important for you, please consider sponsoring a build and/or opening a support contract with OCI.

## Setup says BOOST\_ROOT is set incorrectly, or I'm getting link errors looking for the boost libraries. ##
This problem often occurs after boost is built from source rather than being installed from a binary distribution.   If you discover that the libraries are in the boost/stage directory but not in boost/lib that means you haven't run the last step of the boost build process.

Try typing

`bjam install`

in the boost root directory.  This should fix the problem.

The current (Boost 1.50) default on Windows is to not build DLLs, but QuickFAST needs a few.  After the regular build steps (which will produce the static LIBs, run):

`b2 link=shared toolset=msvc-10.0 address-model=64 --with-thread --with-test install`

After adjusting the the specifics (MSVC version, 32/64 bit) for your own environment.

## What happened to InterpretFAST and MulticastInterpreter? ##
These two example programs were developed early in the life of QuickFAST to interpret
FAST encoded data read from a file (!InterpretFAST) or received via multicast (MulticastInterpreter).

The new program InterpretApplication interacts with QuickFAST at a higher level and completely replaces the functionality of both of these programs.  InterpretApplication also adds quite a few additional features.

New QuickFAST users who found the old programs rather than InterpretApplication were working much to hard using the lower level interfaces to QuickFAST.  They also did not have access to some of the newer features of QuickFAST.   To eliminate the
resulting confusion, I removed the old programs.

There are some differences in the command line options between the the old programs and InterpretApplication.  The following table shows the command line changes that are necessary to substitute InterpretApplication for one of the older programs:

|Replace |With this InterpretApplication option|
|:-------|:------------------------------------|
|-f filename|-file filename|
|-o output\_file|>>output\_file|
|-head count|-limit count|
|-r|-reset|
|-s|-strict|
|-vp, -vd, -vx|-vo filename|
|-b size|-buffersize size|
|-# count|-buffers count|
|-l ip|-mlisten ip|
|-m ip <br>-p port<table><thead><th>-multicast ip:port</th></thead><tbody></tbody></table>

And remember that the -? option provides a complete list of the available command line options.<br>
<br>
The svn repository was tagged before this change was made so the final versions of InterpretFAST and MulticastInterpreter can be retrieved using the tag: //quickfast.googlecode.com/svn/tags/beforeRetiringExamples<br>
<br>
<h2>Which message builder should I use?</h2>
Write your own implementation of ValueMessageBuilder.  See the next question.<br>
<br>
<h2>What do the ValueMessageBuilder methods do?</h2>
Most QuickFAST-based applications will implement their own message builder derived from Messages::ValueMessageBuilder<br>
There are two good example messages builders that you can use as examples:  Messages::MessageBuilder creates a general purpose structure (Messages::Message) that contains the contents of the message, and Examples::ValueToFix writes the contents of the message as a FIX message.<br>
<br>
Note that you rarely want to have the message as a FIX message in a "real" application.  Probably the only reason to do it would be if you were writing an adapter that received FAST-encoded data and sent it on to an existing application that expected FIX messages -- this would be slow, defeating the purpose of FAST.<br>
<br>
You can find out more about implementing ValueMessageBuilder <a href='ValueMessageBuilder.md'>here.</a>

<h2>Why am I receiving messages related to .NET</h2>

If you are seeing messages about "QuickFastDotNet.dll or one of dependent components can't be loaded" and do not plan to use .NET, you can disable the creation of .NET projects by editing the QuickFAST.features file and regenerating the project files.<br>
<br>
Find the lines in "QuickFAST.features"<br>
<br>
dotnet=1<br>
<blockquote>and<br>
dotnetapp=1</blockquote>

Change the '1' to a '0' for each of them and then regenerate the project files with m.cmd or m.sh, or manually perform the project creation using mwc.pl  (see <a href='http://downloads.ociweb.com/MPC/docs/html/MakeProjectCreator.html'>http://downloads.ociweb.com/MPC/docs/html/MakeProjectCreator.html</a> for more information on MPC).