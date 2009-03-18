# Command file to set QuickFAST environment
# QuickFAST depends on MPC V 3.6 or later. (http://www.ociweb.com/products/mpc)
# QuickFAST depends on BOOST V 1.36.0 or later. (http://www.boost.org/)
# QuickFAST depends on Xerces V3.0 or later. (http://xerces.apache.org/xerces-c/)
# Customize this file by setting variables to suit your environment
export QUICKFAST_ROOT=`pwd`

export MPC_ROOT=~/MPC

export BOOST_ROOT=~/boost/boost_1_38_0
export BOOST_ROOT_LIB=$BOOST_ROOT/lib
export BOOST_CFG=-gcc41-mt
export BOOST_VERSION=boost-1_38
export XERCES_ROOT=~/xerces/xerces-c-3.0.1
export XERCES_LIBPATH=$XERCES_ROOT/src/.libs
export XERCES_LIBNAME=xerces-c-3.0
export XERCES_INCLUDE=$XERCES_ROOT/src


export PATH=$MPC_ROOT:$PATH
export LD_LIBRARY_PATH=$XERCES_LIBPATH:$QUICKFAST_ROOT/lib:$BOOST_ROOT_LIB:$LD_LIBRARY_PATH
