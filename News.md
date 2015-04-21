# June 7, 2013 Version 1.5 released #
See the ReleaseNotes page for details.

# October 5, 2011 Version 1.4 released #
See the ReleaseNotes page for details.

# December 2, 2010 Version 1.3 released #
The release notes for this version now have their own ReleaseNotes wiki page.

# April 5, 2010 Version 1.2 released #

QuickFAST version 1.2 is now available.

This version includes the InterpretApplication example program which replaces the old InterpretFAST program.  InterpretApplication interacts with QuickFAST at a higher level than InterpretFAST did.  This makes it easier to configure an application for the most common uses of the QuickFAST decoder.

InterpretFAST can still be used as an example of how to use the more basic objects within QuickFAST.

The source code tree was reorganized to support this new level.   A new Application directory contains the DecoderConnection and DecoderConfiguration classes that are intended to support all of the typical ways for an application to use the decoder.

The publication of Don Bush's Middleware News Brief article,  [Using QuickFAST and OpenDDS for a Low Latency Market Data Feed](http://mnb.ociweb.com/mnb/MiddlewareNewsBrief-201001.html|), inspired a lot of interest in QuickFAST.   Because Don's work was based on the trunk of the QuickFAST repository rather than on the 1.1 release people have had some trouble reproducing his work.  Version 1.2 will work with the code in Don's article.

There are a number of bug fixes included in this version.  In particular Issues #37 through #41 have been resolved. More information about these issues is available under the Issues tab.

To get this version, select the Downloads tab and choose one of the following files:
|quickfast\_lnx\_src\_1\_2.tar.gz|Linux Source Distribution, V1.2|
|:-------------------------------|:------------------------------|
|quickfast\_win\_src\_1\_2.zip|Windows Source Distribution, V1.2|
|quickfast\_win\_bin\_vc9\_b1400\_x301\_1\_2.zip|Windows Binary Distribution, V1.2, <br> built with VC9, Boost 1.40.0, and Xerces 3.0.1 </tbody></table>

Thanks for your continued interest in QuickFAST.<br>
<br>
<h1>March 10, 2010 .NET is back! #
Anyone who has tried to use QuickFAST in a .NET application recently knows that the .NET support was "turned off" due to some substantial changes in the underlying C++ FAST engine.   The good news is that the decoder side of these changes is now complete.   Since the decoder is stable (and much faster), I have reenabled the .NET support based on the up-to-date C++ code.

[Revision 369](https://code.google.com/p/quickfast/source/detail?r=369) in the repository now fully supports decoding FAST messages for .NET applications. This includes decoding messages received via multicast, TCP/IP, raw FAST data files, or PCAP format data files.  These various data sources were not fully supported by the previous .NET implementation.

The InterpretFastDotNet and PerformanceTestDotNet applications are examples of how the .NET support in QuickFAST works.

FAST encoding support for .NET is still a work in progress, however.  Let me know if you have an urgent need to encode messages from a .NET application.

This .NET decoding support will be included in the next official release of QuickFAST.  If you would like to base your development work on a stable, released version of QuickFAST rather than on a revision pulled from the source code repository let me know that, too -- particularly if you or your company are willing to sponsor the time necessary to package and test a release.


# January 22, 2010 Version 1.1 Released. #
Version 1.1 of QuickFAST is now available on [the Downloads page](http://code.google.com/p/quickfast/downloads/list).   Three packaged versions are available:  Windows source, Windows binary build with Visual Studio 9, and Linux source.

Recent work has produced some significant improvements in the decoding performance of QuickFAST, and all reported issues have been resolved to produce this stable release.  These changes have required a change to the API used by an application to receive the decoded version as described in the news item below.  Of course the old API is still supported but applications that use it will not benefit from some of the performance improvements.

Instructions for using source releases are available on [the Getting Started page](GettingStarted.md) in the Wiki.

The Windows binary package contains the results of the build (DLLs and EXEs) in the Output/Release directory, and source code documentation in the /Doc/html.

# January 2010: Yet Another Refactoring Produces Yet Another Significant Performance Improvement. #

The first versions of QuickFAST followed the lead of other similar message-passing protocol implementations like QuickFIX and OpenFAST.   Messages we delivered to the application in the form of a generic Message object designed to represent any possible incoming message.  This technique was very useful in getting a correct implementation of the protocol and developing example applications that used QuickFAST.

Unfortunately this approach is not ideal when message decoding latency is critical – which pretty much includes all deployments of QuickFAST.

Profiling showed that the processes of creating the generic message format inside QuickFAST and pulling the data out of that generic message format in the application code was by far the slowest part of the decoding process.   This led to the first refactoring of QuickFAST last summer.

During this first refactoring, the decoder was changed to pass each individual field to the application rather than waiting until a complete message was assembled.   Additional calls notified the application of significant structural features of the message.  The application was notified at the beginning and end of each message, each sequence, each sequence entry, etc. This let the application be aware not only of the value of the data fields, but the structure of the message.  The application could choose to interpret this structural information however it saw fit.  In particular this opened up the possibility that the application could begin to process the information that appeared early in the message even before the remainder of the message had been decoded.

All in all skipping the intermediate generic message stage resulted in a significant reduction in decoding latency.  The version with this refactoring incorporated formed the basis for release 1.0.

However, there was still room for improvement.   In order to create a generic message, QuickFAST had been built around the concept of a data type called a Field that could store any specific data type defined by the FAST standard.  Because the total number of fields in a message could not be determined ahead of time, and because QuickFAST guaranteed the field would remain available as long as the application needed it, these field objects had to be dynamically allocated and freed.

Whereas the original code created a collection of these Field objects (the QuickFAST message) and passed the entire collection, the new code was passing each Field object to the application immediately after it was created.  These field objects were still heap-allocated as needed.   Profiling of the revised code by a QuickFAST user, carlosz (thanks Carlos), revealed that allocating and freeing these Field objects had a major performance cost.  I tried caching and reusing field objects to avoid allocation and deallocation time, but that turned out to be not much of an improvement.  The Field object had to go.

Unfortunately the Field object was a central concept in the initial QuickFAST design.  In addition to serving as a value container when passing decoded data to the application, the Field object had proved to be handy for internal use inside QuickFAST.  Without the Field object, QuickFAST needed type-specific methods to pass data around.  In places where a single method had formerly served to pass data around inside QuickFAST, without the Field object there separate methods would be needed for signed and unsigned integers, decimal numbers, and the strings of various types.  The code was much more cluttered.

Fortunately the code was also much faster!   Also the benefits of having a single data type to pass around and ultimately to pass to the application were somewhat outweighed by the requirements with the application code to extract the specific type of data from the Field object before using it.  With type-specific calls the application code to process a field turns out to be simpler.  Trading more complexity inside QuickFAST for reduced complexity in the application code is a bonus of this change.

At the time I am writing this, January 7, 2010, the new code is still in a branch named ValueClass in the subversion repository.  Because this is such a pervasive change, I am being extra careful with the testing process before I commit these changes to the trunk.

To fully take advantage of these latest changes will require code changes at the application layer.  The application must be changed to accept “raw” data values rather than a Field containing the data being delivered.   For backwards compatibility, however, the old application interface will still be supported by a layer that translates the new method calls into to the old ones.

Watch this space in the next few days for version 1.1.0 that will include these changes.

# November 2009: Version 1.0 #

QuickFAST is growing up.  Now that it has been deployed at several sites and is being used in production, I decided it was time for a version numbering system.  This will let people work with known versions rather than just the latest pull from the source archive.

I tagged the subversion repository: https://quickfast.googlecode.com/svn/tags/V1_0
and created downloadable source and binary packages --  see the Downloads tag above.

For new development, I still recommend the tip of the repository.  It's reasonably stable -- it gets built and tested every night.  However, when you go into production you should probably choose a well defined version to make it easier in case you need help.  In fact if you want all the latest repository changes packaged in a known state you might considering sponsoring a release -- paying for the time necessary to package and label the a stable version of QuickFAST.

Edited: 11/06  Linux source is download is available.  I decided not to do a Linux binary because there are too many flavors of Linux to cover -- and besides, you Linux guys like to build-your-own (grin).

---

# July 2009: Major Refactoring in Progress #

On the [performance page](PerformancePage.md) I documented the improvements that have been made in QuickFAST performance over time.   Starting with the initial version in which decoding latency was 425 microseconds, there were a series of incremental improvements that reduced this latency to 34 microseconds.  At this point I hit a wall.  Several attempts to squeeze a little better performance from QuickFAST produced little if any improvement.

Fortunately I found a way around the problem.  Using a different approach to capture the decoded message and make it available to the application produced some remarkable results.   The latest performance numbers I generated with this new approach showed decoding latency times of 2 to 3 microseconds.  If all the machine was doing was decoding FAST messages it could handle more than 300,000 messages a second.

The downside of this change is the interface to the Codecs::Decoder object has changed.  I did my best to make the higher-level support, Codecs::SynchronousDecoder and Codecs::MulticastDecoder forward compatible, but applications that are using Codecs::Decoder directly will need to be reworked.

Also to take full advantage of this new technique requires some custom coding for every data source.   QuickFAST provides the foundation for this custom coding, but applications that wish to see the lowest possible latency numbers (the 2 to 3 microsecond numbers mentioned above) will need exchange-specific and possibly application-specific adapters built on top of the support provided by QuickFAST.  More details about this message-handling customization is available on [this custom message page (under construction).](CustomMessages.md)