# From the QuickFAST mailing list #

On 8/3/2011 10:22 PM, Paulo R. Panhoto wrote:

Hello, everybody

I am new to QuickFAST and I happen to like things under /usr
directory. In this case, I wrote a small script to create an include
directory wherever I want.

When I run this command line:
src> sudo ./mkinc /usr/local/include/QuickFAST

the outcome is this directory tree, with the respective header files
(.h) in it:
```
/usr/local/include/QuickFAST/
/usr/local/include/QuickFAST/Common
/usr/local/include/QuickFAST/DotNet
/usr/local/include/QuickFAST/Tests
/usr/local/include/QuickFAST/Examples
/usr/local/include/QuickFAST/Examples/FileToMulticast
/usr/local/include/QuickFAST/Examples/FileToTCP
/usr/local/include/QuickFAST/Examples/PCapToMulticast
/usr/local/include/QuickFAST/Examples/TutorialApplication
/usr/local/include/QuickFAST/Examples/InterpretApplication
/usr/local/include/QuickFAST/Examples/Examples
/usr/local/include/QuickFAST/Examples/PerformanceTest
/usr/local/include/QuickFAST/Codecs
/usr/local/include/QuickFAST/Messages
/usr/local/include/QuickFAST/Application
/usr/local/include/QuickFAST/Communication
```

Just thought this might be useful. The script itself is listed below:
```
#!/bin/bash

if [ $# -ne 1 ]
then
echo "usage: $0 <include-directory>"

  exit 1
fi

mkdir $1
if [ $? -ne 0  ]
then
  echo "mkdir failed"
  exit 2
fi

HEADERS=`find . -name *.h`
for FILE in $HEADERS
do
  NAME=${FILE##*/}
  DIR=${FILE:0:${#FILE} - ${#NAME}}
  mkdir -p $1/$DIR
  cp $FILE $1/$DIR$NAME
done

exit 0
#end
```