# Requirements:

## Berkeley DB

Download the source code from http://www.oracle.com/technetwork/database/berkeleydb/downloads/index.html

    tar xfvz db-5.2.28.tar.gz
    cd db-5.2.28/build_unix
    ../dist/configure --prefix=/usr/local --enable-cxx
    make
    sudo make install
