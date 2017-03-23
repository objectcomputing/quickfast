# Command file to set QuickFAST environment
# QuickFAST depends on MPC V 3.6 or later. (http://www.ociweb.com/products/mpc)
# QuickFAST depends on BOOST V 1.36.0 or later. (http://www.boost.org/)
# QuickFAST depends on Xerces V3.0 or later. (http://xerces.apache.org/xerces-c/)
# Customize this file by setting variables to suit your environment
SOURCE="${BASH_SOURCE[0]}"
SOURCE_DIR=`dirname $SOURCE`
$READLINK --version >/dev/null 2>/dev/null
if (( $? != 0 )); then
    echo "readlink does not exist or it does not support --version"
    echo "maybe it is not GNU readlink but BSD"
    echo "trying with greadlink..."
    READLINK='greadlink'
fi
$READLINK --version >/dev/null 2>/dev/null
if (( $? != 0 )); then
    echo "greadlink does not exist or an error occurred"
    UNAME=`uname`
    if [[ $UNAME == "Darwin" ]]; then
        echo "You are running on a Mac OSX system."
        echo "Consider installing homebrew."
        echo "Then install coreutils."
        echo "# brew install coreutils"
    fi
else
    echo "$READLINK found at `which $READLINK`."
fi
$READLINK -f $SOURCE_DIR
if (( $? != 0 )); then
    echo "trying exporting QUICKFAST_ROOT by pwd."
    export QUICKFAST_ROOT=`pwd`
    echo "QUICKFAST_ROOT = $QUICKFAST_ROOT"
else
    export QUICKFAST_ROOT=`$READLINK -f $SOURCE_DIR`
fi

if test "$MPC_ROOT" = ""
then
  export MPC_ROOT=$ACE_ROOT/MPC
fi

if test "$BOOST_ROOT" = ""
then
  export BOOST_ROOT=~/boost/boost_1_38_0
fi

if test "$BOOST_ROOT_LIB" = ""
then
  export BOOST_ROOT_LIB=$BOOST_ROOT/lib
fi

if test "$BOOST_VERSION" = ""
then
  export BOOST_VERSION=boost-1_38
fi

if test "$XERCES_ROOT" = ""
then
  export XERCES_ROOT=~/xerces/xerces-c-3.0.1
fi

if test "$XERCES_LIBPATH" = ""
then
  export XERCES_LIBPATH=$XERCES_ROOT/src/.libs
fi

if test "$XERCES_LIBNAME" = ""
then
  export XERCES_LIBNAME=xerces-c-3.0
fi

if test "$XERCES_INCLUDE" = ""
then
  export XERCES_INCLUDE=$XERCES_ROOT/src
fi

export PATH=$QUICKFAST_ROOT/bin:$MPC_ROOT:$PATH
export LD_LIBRARY_PATH=$XERCESLIB:$QUICKFAST_ROOT/lib:$BOOST_ROOT_LIB:$LD_LIBRARY_PATH

