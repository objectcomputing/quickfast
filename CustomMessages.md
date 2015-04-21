# History #
When I first started working on QuickFAST, I designed a general purpose Message class similar to the ones in QuickFIX, QuickFIX/J, and OpenFAST.  A Message is a collection of  Fields.  Each field can hold any of the FAST-defined data types.  You can retrieve a field from a message if you know its FieldIdentity (basically its name) and once you have it, you can determine the type of data contained in the field (if you don't already know it) and the actual value stored in the field.

Functionally, this was a good approach.  The Message object could hold any structure that was expressible in a FAST message.  Looking at performance, however, told another story.  The performance wasn't even close to what I had hoped for.  I profiled the code and the results consistently pointed to the Field and FieldIdentity objects as sources of delay.

Over time I was able to optimize these objects to produce an order of magnitude performance improvement (from 425 microseconds decoding latency to 34 microseconds).  However at that point I ran out of optimization tricks.

At about the same time, I started doing some serious performance testing on the .NET wrappers and discovered that the generic collection of "any" fields approach did not play well at all in the .NET world.   There were way too many transitions between managed and unmanaged code -- each of which incurred a large performance price.

Again there were some tweaks that produced incremental performance improvements in the .NET world, but after trying every simple fix I could think of the .NET latency was still over 80 microseconds.

# The New Approach Evolves #
In an attempt to get the performance levels up to what I hoped for -- and up to what was needed to handle the typical volume in market data feeds -- I abandoned the idea of a one-size-fits-all Message object.  Instead I decided to design a set of message objects specifically to handle the data feed I was working with (in this case ArcaBook.)  This was not a trival change.   Every object in the system that interacted with the Message object had to be examined to determine what "services" it needed from the Message.

What I discovered was that the Message object played three roles.

  * In its first role, it was accepting decoded fields from the decoder and storing the field identity and value.
  * In the second role it was acting as a source of field values for the encoder.
  * The third role was the interaction between the message and the application that was using the QuickFAST library.  This role could potentially involve accepting field values into messages that would eventually be encoded, delivering field values for messages that had been received and decoded, and possibly other application-dependent functions such as storing data for extended periods of time.

I decided to create abstract interfaces for the decoder-target role, the encoder-source role, and to leave the application role as uncommitted as possible.

The first phase of this redesign was to create the interface to be used in the decoder-target role.  As I worked on this I realized that the decoder "knew" too much about the message.  If more functionality was moved out of the decoder and into the message object, the design became much simpler.

As one example, when the original decoder encountered a sequence, it would create a sequence object, then add each sequence entry to the sequence object as it was decoded.  At the end of the sequence, the decoder would then add the sequence object as a field into the message.  Handling the sequence this way meant that the decoder was enforcing its own view of how a sequence should be managed onto the message object -- thereby limiting the possibilities for the message to optimize sequence handling to suit the characteristics of a particular data feed and application.

To avoid this, the decoder was changed so that it just notified the message object about the sequence by calling newly defined methods: startSequence, startSequenceEntry, endSequenceEntry, and endSequence.   This allowed the message object to decide what was the appropriate thing to do with the decoded sequence entries.

At this point the decoder was becoming much cleaner and was interacting with  the message-under-construction through a much more intuitive interface.  Unfortunately the message object itself was becoming more bloated.   This is a problem because I anticipated not just one message object but a whole family of message objects corresponding to the messages delivered by a particular data feed.

The solution to this was to abandon the idea that a message should be able to "build itself" during the decoding process.  Instead, I renamed the interface used by the decoder to MessageBuilder.   Supplying the decoder with a MessageBuilder to do the work of building the message rather than just a Message to receive the decoded fields gave the application a great deal more freedom in determining how the decoded data should be used.   A further refinement at this point was to to allow the application to switch MessageBuilders on-the-fly.  That allows, for example, the application to provide a special MessageBuilder to be used while it is decoding a sequence entry or a group.   Of course if the application doesn't want to switch MessageBuilders, it can easily have one MessageBuilder do everything.  In fact, it can even keep the earlier approach in which a Message builds itself by simply having the Message object implement the MessageBuilder interface.

Given this approach to handling a sequence, it seemed obvious that there should be beginGroup and endGroup methods.  Not quite so obvious was the need for beginMessage and endMessage methods, but it turns out these allow even more simplification.  In fact, the endMessage method turned out to be an appropriate place for the MessageBuilder to pass the newly decoded message on to the MessageConsumer object.   This meant the decoder no longer needed to know about the MessageConsumer -- an important simplification that let the decoder focus single-mindedly on decoding the message and passing the necessary information to the MessageBuilder.

# Gratifying Results #
Using these decoder changes and message objects designed specifically to hold ArcaBook messages produced some surprising, and very welcome, results.   In a .NET environment, the latency cost of decoding FAST encoded ArcaBook messages has been consistently between two and three microseconds -- a far cry from the 420+ microsecond performance of the original version of QuickFAST.

The original Generic Message format is still available and still supported. It's a good way to get started with a new feed, but the decoding latency is still going to be in the tens or hundreds of microsecond range until the work is done to adapt QuickFAST to the particular data feed being handled.   Fortunately the tools to do this adaptation are in place.

# [Revision 208](https://code.google.com/p/quickfast/source/detail?r=208) in Subversion #

For [revision 208](https://code.google.com/p/quickfast/source/detail?r=208), I focused on implementing the above ideas in the decoder (the encoder changes can come later.)  The following objects are new in [revision 201](https://code.google.com/p/quickfast/source/detail?r=201) of QuickFAST.

  * MessageBuilder: an interface to be used by the decoder to build a message from incoming, decoded data.
  * GenericMessageBuilder: an implementation of MessageBuilder that creates the "classic" QuickFAST Message object.
  * MessageAccessor: an interface to be used by the encoder to retrieve information from a message while encoding it.
  * SingleMessageConsumer: a consumer to be used by the QuickFAST unit tests.  Not recommended for general purpose work.

Most of the classes used by the decoder were also touched by this change.

Unfortunately it wasn't feasible to make the revised interface to the decoder backward compatible with the previous version, so applications that use the low-level decoder directly will need some changes.   I was able to preserve the interface of SynchronousDecoder and MulticastDecoder so applications that interact at this level should be OK.