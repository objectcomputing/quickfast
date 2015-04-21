# Disclaimer #
_added June 18, 2009, Rev 168; trunk_

In my description of the test below I tried to be clear that the performance test numbers should not be considered absolute numbers.  It would be a mistake to look at these numbers and think, "Oh, my system is going to be able to process X messages per second with a latency of Y."   To illustrate this point, here is the results of running a similar test against a different set of data on the same machine.

```
Decoded 30545 messages, 6500 Packets in 317  milliseconds. [0.010 msec/message. = 96356.467 messages/second]
```

This data is from Arca -- it is much simpler than the CME data I used in the tests below. The test was written in C# so it uses the .NET wrappers.

I used the CME data for the tests below because it thoroughly exercises QuickFAST, but I wouldn't want to discourage anyone from trying QuickFAST because they think it can't handle the load.

# Performance Test Results #

Because the primary motivation for FAST is to handle large amounts of data with minimal latency, performance results are an important part of the project status.  One of the example applications, PerformanceTest, is designed to measure QuickFAST performance.

Because latency is difficult to measure directly, PerformanceTest reads all the test data into memory, then measures the time it takes to process the data in a single-threaded application.  From these numbers, it calculates approximate throughput and latency information.

The following tests were all run on the same system with the same test data.
**THESE NUMBERS WERE PRODUCED ON A RELATIVELY SLOW COMPUTER**  I am getting reports from users of much higher performance rates. Different data and/or different computers will produce different results so compare these numbers to each other, but don't use them as absolute results.  Your mileage will vary.

### January 16, 2009;  trunk ###
```
Decoded 9999 messages in 4250 milliseconds. [0.425 msec/message. = 2352.706 messages/second]
```

### January 20, 2009;  trunk ###
```
Decoded 9999 messages in 1875 milliseconds. [0.188 msec/message. = 5332.800 messages/second]
```

### January 23, 2009;  trunk ###
```
Decoded 9999 messages in 937 milliseconds. [0.094 msec/message. = 10671.291 messages/second]
```

### February 6, 2009;  trunk ###
```
Decoded 9999 messages in 750 milliseconds. [0.075 msec/message. = 13332.000 messages/second]
```

### February 20, 2009; Rev 51; trunk ###
```
Decoded 9999 messages in 468 milliseconds. [0.047 msec/message. = 21365.385 messages/second]
```

### February 20, 2009; Rev 51; FieldIdentityPtr branch ###
Note: this branch will be merged into the trunk when some .NET build issues are resolved.
```
Decoded 9999 messages in 390 milliseconds. [0.039 msec/message. = 25638.462 messages/second]
```

### February 24, 2009; Rev 61; trunk ###
```
Decoded 9999 messages in 343 milliseonds. [0.034 msec/message. = 29151.603 messages/second]
```

### January 7, 2010, rev 282, ValueClass branch ###
Preparing for V1.1.  Note that latency times are now expressed in microseconds rather than milliseconds in order to retain the same level of precision at these lower latencies.
```
Decoded 9999 messages in 187 milliseonds. [18.702 usec/message. = 53470.588 messages/second]
      Fields: 415997 -> 0.450 usec/field. = 2224582.888 fields/second]
      Sequence Entries: 32372 -> 5.777 usec/entry. = 173112.299 entries/second]
```

### January 18, 2010 rev 302 ###
```
Decoded 9999 messages in 140 milliseonds. [14.001 usec/message. = 71421.429 messages/second]
      Fields: 415997 -> 0.337 usec/field. = 2971407.143 fields/second]
      Sequence Entries: 32372 -> 4.325 usec/entry. = 231228.571 entries/second]
```


## For comparison ##
[OpenFAST](http://www.openfast.org/), an open source Java implementation of FAST has a similar test.  When it is run on the same system with the same test data it produces the following output.

```
Decoded 9999 messages in 422 milliseconds.
Average decode time per message: 42 microseconds
```