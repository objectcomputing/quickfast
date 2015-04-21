
# General Requirements #
In addition to the source code downloaded from Google Code, you will need:

  * BOOST V 1.36.0 or later. (http://www.boost.org/)
| **NOTE:** a conflict has shown up between boost versions 1.41 and later, and .NET. If you are building QuickFASTDotNet you should be using a boost version between 1.36 and 1.40.  I hope to remove this limitation in a future release of QuickFAST. |
|:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

| Boost needs to be built using the --layout=versioned option. |
|:-------------------------------------------------------------|

  * Xerces V3.0 or later. (http://xerces.apache.org/xerces-c/)
  * MPC V 3.6 or later. (http://www.ociweb.com/products/mpc)
  * Perl to run MPC. Active State Perl (for MPC) V5.x or later is strongly recommended.  (http://www.activestate.com/)

## Windows Specific Requirements ##
  * Visual Studio 2008 (VC9) should be installed.   The express version should be sufficient to build the C++ code although this is not tested. The Professional Edition will be needed to support .NET because it mixes C++ and C# code.
  * Support has been added for Visual Studio 2010 (VC10) but this is still preliminary.  If you want to use VC10, be sure to install the most recent version of MPC.

## .NET Specific Requirements ##
| **Important notice** the encoder side of the .NET support in QuickFAST is presently disabled.  This is due to some significant improvements that are being made in the encoder.  As soon as these encoder changes are stable, the .NET support will be reenabled.  If .NET encoding is important to you you can speed this work along by sponsoring it. |
|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

See the above note about boost versions (1.36 - 1.40 are OK)

## Linux Specific Requirements ##
  * gcc version 4.1.1
  * gmake 3.80
Earlier versions of gcc and gmake may work, but these are the versions used to test QuickFAST.

# Building QuickFAST #
These instructions assume you have checked the QuickFAST project out into a directory named QuickFAST.  `.../QuickFAST` means whatever path information is necessary to get to this directory.

## For native C++ on Windows ##
  * Make QuickFAST your current working directory:
> `  > cd ...\QuickFAST `
  * Edit the setup.cmd file with your choice of text editor:
> `  > notepad setup.cmd `
  * Set the following values in setup.cmd to point to the directories in which the corresponding tools are installed:
    * MPC\_ROOT
    * XERCESROOT
    * BOOST\_ROOT
`[you may set these as environment variables if you prefer, but you should still run setup.cmd] `

  * Save the edited file, then run it:
> `  > setup.cmd `
  * If this is successful, you can now run MPC to generate Visual Studio Solution and Project files.  The "m.cmd" file included in the QuickFAST download does this:
> `  > m `

You are now ready to load the newly-generated solution into Visual Studio.  If Visual Studio is registered to handle .sln files, you can run from the command line by typing:

> `  > QuickFAST.sln `

| IMPORTANT: If you start Visual Studio from the Start menu rather than from the command line prompt, it will not have the environment variables defined by the setup.cmd command.  Because of this your build is likely to fail in odd ways.  If you would prefer to start from the menu rather than from the command line, you should define the necessary environment variables in your system's control panel.   Details are left as an exercise for the reader. |
|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

The "Build|Rebuild Solution" menu choice will build the QuickFAST library -- ready to be linked with your application.

For Windows builds, all compiled output files are placed in the following directories: Output/Debug and Output/Release. These directories correspond to debug and release project configurations respectively.

## For Windows.NET ##
  * Edit the file QuickFAST.features in the ...QuickFAST directory with the text editor of your choice:
    * To enable building the .NET library change
> > > ` dotnet=0 ` to ` dotnet=1 `
    * To enable the .NET unit tests when you have NUnit installed change
> > > ` nunit=0 ` to ` nunit=1 `

Then follow the above instructions for building QuickFAST/C++ on Windows

For .NET builds, the QuickFastDotNet assembly is placed in the appropriate output directory.  This assembly can be referenced in your .NET applications.

## For C++ on Linux ##

| [Chad Cravens has created an excellent blog entry describing the process of building QuickFAST on linux. Click here to see it.](http://www.street-tech.com/2011/10/12/building-quickfast-from-source-on-linux/)|
|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|



  * Move to the directory where you checked out QuickFAST

> > `> cd .../QuickFAST`
  * edit the setup.sh file:
> > `> vim setup.sh`
  * Correct exports in the file for
    * MPC\_ROOT
    * BOOST\_ROOT
    * BOOST\_VERSION  (should look like boost\_1\_NN)
    * BOOST\_CFG      (should look like gccXX\_mt)
    * XERCES\_ROOT
    * XERCES\_LIBNAME
  * for non-standard installations these exports may also need to be set:
    * BOOST\_ROOT\_LIB
    * XERCES\_LIBPATH
    * XERCES\_INCLUDE
  * Source the setup.sh file to incorporate variables into your environment:
> > `> . ./setup.sh`
  * Run MPC to generate Makefiles using the m.sh script:
> > `> ./m.sh`
  * Make the project:
> > `> make`

# Where to Go From Here #
## Run the Testing Application ##
Run the QuickFASTTest application to verify that QuickFAST built correctly.

## Internal Documentation ##
A good way to get familiar with QuickFAST is to generate and read the Doxygen documentation.

Doxygen is downloadable from here: http://www.stack.nl/~dimitri/doxygen/

Once it is installed and on your path you can generate the documentation by:
```
    > cd ...QuickFAST/src
    > doxygen 
```

This will generate documentation in both HTML and LaTeX.  Both versions contain the same information.
  * HTML documentation is in .../QuickFAST/Doc/html/index.html
> > This web page is viewable with any web browser, and
  * LaTeX documentation is in .../QuickFAST/Doc/latex.
> > The LaTeX documentation needs to be further processed by LaTeX which is available here:   http://www.latex-project.org/

## OCI Commercial Support ##
OCI provides commercial support for QuickFAST. Contact [sales@ociweb.com](mailto:sales@ociweb.com) for details.

## OCI Training ##
OCI offers a training course that covers the basics of FAST and guides you through creating applications that take full advantage of QuickFAST.  You can find more information about OCI's training here: http://www.ociweb.com/training/

## OCI Consulting ##
OCI also has a team of experienced developers who can work with you to create a solution custom tailored to your particular needs.  In addition to QuickFAST expertise (obviously!) we have experience in a wide variety of other technologies that are extremely valuable to those wishing to get started quickly and build a solid foundation for future growth.  Information about OCI's consulting services is available here: http://www.ociweb.com/consulting/

## Tutorial Application ##
QuickFAST includes a number of Example programs including a TutorialApplication designed to get you started quickly.

## The QuickFAST Users Mailing List ##
And of course, you can always find help from the QuickFAST developers and from experienced QuickFAST users by subscribing to [the QuickFAST Users mailing list](http://groups.google.com/group/quickfast_users)