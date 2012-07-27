
BOOST_DIR = /boost/boost_1_49_0
BOOST_LIBDIR = /boost/boost_1_49_0

!exists("$$(BOOST_DIR)"){
    BOOST_DIR = /boost/boost_1_48_0
    BOOST_LIBDIR = /boost/boost_1_48_0
} 
BOOST_LIBS = 

MUPARSER_DIR=muparser_v2_2_2

!exists("$$(MUPARSER_DIR)"){
    MUPARSER_DIR = /muparser/muparser_v2_2_2
} 
LIBS += -L"$$(MUPARSER_DIR)/lib -lmuparser32

INSTALLDIR = ../../windows


