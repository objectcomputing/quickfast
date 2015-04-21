## ValueMessageBuilder Interface ##
The ValueMessageBuilder interface is intended to be extremely flexible.  Because it is abstract you must implement all the methods, but you can leave many of the implementations empty -- only provide implementations for the ones you need.

Here is what you can expect if you are writing your own message builder derived from ValueMessageBuilder

At the beginning each incoming message you will receive a call to
  * startMessage()
    * initialize your object to build a new message

As each field is decoded in the message, you will receive a call to:
  * addValue()
    * capture the data for this field

When a group is encountered you will receive a call to
  * startGroup()
    * prepare to receive the fields that make up the group

After the group is complete you will receive a call to
  * endGroup()
    * go back to accepting fields for the parent that contains the group

When a sequence is encountered you will receive a call to:
  * startSequence()
    * prepare to receive sequence entries

then one or more calls to
  * startSequenceEntry()
    * prepare to receive the fields that make up this sequence entry.

After a sequence entry is complete you will receive a call to:
  * endSequenceEntry()
    * get ready for the next call which will be either startSequenceEntry or endSequence
    * Note: it may be possible for your application to process this sequence entry at this point.

After all entries in a sequence have been decoded you will receive a call to:
  * endSequence()
    * go back to accepting fields for the parent that contains the sequence.

After the entire message is decoded you will receive EITHER a a call to
  * endMessage()
    * process the message just received and get ready for the next startMessage

or a call to
  * ignoreMessage()
    * discard the (partial) message being received and get ready for the next startMessage
---
So to express this in another way, here's the sequence of calls you might expect to see:

  * startMessage (once per message)
    * addValue (zero or more times per message)
      * startGroup (zero or more times per message)
        * addValue(zero or more times per group)
      * endGroup (once per startGroup)
      * startSequence(zero or more times per message)
        * startSequenceEntry(zero or more times per sequence)
          * addValue(zero or more times per sequence entry)
        * endSequenceEntry(once per startSequenceEntry)
      * endSequence(once per startSequence)
    * endMessage (once per message)

ignoreMessage might occur at any time after a startMessage.

The start... methods return a reference to a new ValueMessageBuilder -- you can simply return `*this` or you can return a reference to a new ValueMessageBuilder that will handle the child data.

## Logger interface ##
In addition to accepting decoded messages, ValueMessageBuilder is also derived from Logger.   That means you need to implement the following methods as well:

```
virtual bool wantLog(unsigned short level);
```
  * indicate whether messages with this logging level are wanted.
  * logging level will be one of
    * Logger::QF\_LOG\_FATAL
    * Logger::QF\_LOG\_SERIOUS
    * Logger::QF\_LOG\_WARNING
    * Logger::QF\_LOG\_INFO
    * Logger::QF\_LOG\_VERBOSE
  * Returning `false` for unwanted levels can improve performance.

```
virtual bool logMessage(unsigned short level, 
   const std::string & logMessage);
```
  * capture the message in the log of your choice.

```
virtual bool reportDecodingError(const std::string & errorMessage);
```
  * capture the message in the log of your choice.
  * return to try to recover, or
  * throw an exception to stop decoding this message
    * Your application can catch the exception and try to recover by resetting the decoder and starting at the beginning of the next message (if it can find it!).

```
virtual bool reportCommunicationError(const std::string & errorMessage);
```
  * capture the message in the log of your choice.
  * throw an exception to stop decoding this message
    * your application can catch the exception and do whatever it feels is appropriate.  It probably needs to close the connection and open a new one.