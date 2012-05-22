

BOOST_DIR = /boost/boost_1_49_0
BOOST_LIBDIR = /boost/boost_1_49_0
BOOST_LIBS =
MUPARSER_DIR=muparser_v2_2_2

!exists("$${MUPARSER_DIR}"){
    MUPARSER_DIR = /muparser/muparser_v2_2_2
LIBS += -L$${MUPARSER_DIR}/lib
}

!exists("$${BOOST_DIR}"){
BOOST_DIR = C:/boost/boost_1_49_0
BOOST_LIBDIR = "$${BOOST_DIR}"
BOOST_LIBS = "$${BOOST_DIR}/lib"
}

INSTALLDIR = ../../windows


