
BOOST_DIR = /boost/boost_1_49_0
BOOST_LIBDIR = /boost/boost_1_49_0

!exists("$$(BOOST_DIR)"){
    BOOST_DIR = /boost/boost_1_48_0
    BOOST_LIBDIR = /boost/boost_1_48_0
} 
BOOST_LIBS = "$${BOOST_DIR}/libs"

MUPARSER_DIR=muparser_v2_2_2

!exists("$$(MUPARSER_DIR)"){
    MUPARSER_DIR = /muparser/muparser_v2_2_2
} 

INSTALLDIR = ../../windows


