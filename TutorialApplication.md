

# Introduction #
There are several example applications included with QuickFAST to illustrate different ways in which QuickFAST can be used.   The TutorialApplication described here is intended to help new QuickFAST users get started quickly.  You can think of this as the QuickFAST equivalent of a HelloWorld program.

This Wiki page is an introduction to the TutorialApplication.  The best way to use this page is to download QuickFAST; open the TutorialApplication in an editor; and examine the actual source code while you read the description of what the source code is doing.

# Files #
The source files for the TutorialApplication are in `../src/Examples/TutorialApplication`.
The files of interest are:
  * `main.cpp`
  * `TutorialApplication.h`
  * `TutorialApplication.cpp`

In addition to these files, the `MessageInterpreter` object defined in  `../src/Examples/Examples/MessageInterpreter.*` is used.  The `MessageInterpreter` will not be described here because this tutorial focuses on how to receive decoded FAST data, not what to do with it once you have it.

# main.cpp #

The main function in this program uses a pattern you will find in all of the QuickFAST example programs. It:

  * Creates an _Application_ object.
  * Initializes that object using command line parameters.
  * Runs the object.
  * Finishes the object to allow final clean-up.

In this case, the object is a TutorialApplication which will be discussed next.

# TutorialApplication #

`TutorialApplicaton.h` simply declares TutorialApplication.  The methods needed by main are declared here.  Two member objects, a DecoderConfiguration and a DecoderConnection work together to provide access to QuickFAST.

The essence of the application is in `TutorialApplication.cpp`.

## `TutorialApplication::init(int argc, char *argv[])` ##
This method captures two parameters from the command line into the DecoderConfiguration.  If there are not exactly two parameters, it complains and returns false, causing main to exit without attempting to run the application.

Assuming the command line options are present, the init method sets additional DecoderConfiguration options.   These configuration options describe the incoming FAST data and how it will be received.

The diagram below will help you understand the meaning of some of these options.

## `TutorialApplication::run()` ##
The run method starts the actual decoding process.

It creates a `Example::MessageInterpreter` object.  This object is part of the application, not part of the core of QuickFAST.  It is one example of what can be done with the incoming decoded data.  For this example it is used to receive messages and display them in a human readable format.

Now the TutorialApplication creates `Codecs::GenericMessageBuilder` to serve as a `ValueMessageBuilder`.  The `MessageInterpreter` is attached to this `GenericMessageBuilder` as consumer of the decoded messages.


Next the TutorialApplication uses the DecoderConfiguration and message builder to initialize the DecoderConnection .

Once the DecoderConnection is configured, the TutorialApplication calls the run() method on the connection.  QuickFAST will run until it reaches a stopping condition (in this case, end of file on the input file) then return.

## `TutorialApplication::fini()` ##
For this application, no cleanup is necessary.

# A Picture to Make Things Clearer #

This diagram should help explain the meaning of some of the options set in the DecoderConfiguration

![http://images.quickfast.org/QuicFASTArchitectureOverview.png](http://images.quickfast.org/QuicFASTArchitectureOverview.png)

The green boxes (abstract classes) are roles that need to be filled for QuickFAST to accept incoming data and deliver it to the decoder.   The yellow boxes (concrete objects) are actors that can fill the various roles.  The inheritance triangles indicate which actors are available to fill each role.


# Running the Tutorial #

Ah! Now we have a problem. I'm still trying to find test data and templates that I can redistribute without violating agreements with the source of the data.

Assuming that this problem was solved, or that you have acquired sample templates and data from your source of FAST encoded data, this program can by run by typing the following command line:

```
  TutorialApplication templates.xml RawFASTData.data
```

The templates from `templates.xml` will be used to decode FAST messages read from `RawFASTData.data`, and the resulting decoded messages will be displayed on the standard output device.