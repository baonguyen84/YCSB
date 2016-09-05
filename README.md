YCSB with modified POM


Install YCSB + LevelDB

*** Install all dependencies:
$sudo apt-get install libboost-dev libboost-test-dev libboost-program-options-dev libboost-system-dev libboost-filesystem-dev libevent-dev automake libtool flex bison pkg-config g++ libssl-dev libleveldb-dev

1. Install libevent

    wget http://monkey.org/~provos/libevent-2.0.12-stable.tar.gz
    tar xfvz libevent-2.0.12-stable.tar.gz
    cd libevent-2.0.12-stable
    ./configure --prefix=/usr/local
    make && sudo make install

2. Install boost
	Require verion > 1.54 for Thrift 0.9.3 and above
    wget http://superb-sea2.dl.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz
    tar xfvz boost_1_60_0.tar.gz
    cd boost_1_60_0
    ./bootstrap.sh --prefix=/usr/local
    sudo ./b2 install 

2. Install thrift

    wget http://archive.apache.org/dist/thrift/0.9.3/thrift-0.9.3.tar.gz
    tar xfvz thrift-0.9.3.tar.gz
    cd thrift-0.9.3
    ./configure --prefix=/usr/local

4. Install mapkeeper:

   git clone git://github.com/m1ch1/mapkeeper.git
   export MKROOT=`pwd`/mapkeeper
   cd $MKROOT/thrift
   make

5. Install LevelDB
   
   cd leveldb
   make
   cp libleveldb.a /usr/local/lib
   cp -r include/leveldb /usr/local/include

6. Download YCSB 
	Use latest verion of YCSB 
	git clone git://github.com/brianfrankcooper/YCSB.git
	cd YCSB
	mvn clean package 

7. Run YCSB

   cd mapkeeper/leveldb
   sudo ./mapkeeper_leveldb -d /tmp/data
   
   /* go to YCSB directory */
   cd ycsb-0.4.0
   ./bin/ycsb load mapkeeper -p mapkeeper.port=9090 -P workloads/workloada -threads 4
   ./bin/ycsb run mapkeeper -p mapkeeper.port=9090 -P workloads/workloada -threads 4

# Mapkeeper 

Mapkeeper binding doesn't get built by default because there is no
Maven repository for it. To build the binding:

1. follow "https://github.com/m1ch1/mapkeeper/wiki/Getting-Started" to Generate Thrift MapKeeper Binding
   a.  install libevent
   b.  install boost
   c.  install thrift
   d.  generate mapkeeper binding

2. After generating mapkeeper binding, go the "mapkeeper/thrift/gen-java/target", and 
   cp mapkeeper-1.1-SNAPSHOT.jar mapkeeper-1.0.jar 
   mvn install:install-file -Dfile=mapkeeper-1.0.jar -DgroupId=com.yahoo.mapkeeper -DartifactId=mapkeeper -Dversion=1.0 -Dpackaging=jar

3. Modify "YCSB/mapkeeper/pom.xml" as follows:
    <dependency>
           <groupId>com.yahoo.mapkeeper</groupId>
           <artifactId>mapkeeper</artifactId>
           <version>1.0</version>
    </dependency>
    <dependency>
        <groupId>org.apache.thrift</groupId>
        <artifactId>libthrift</artifactId>
        <version>0.8.0</version>
    </dependency>


4. Uncomment "<module>mapkeeper</module>" and "<mapkeeper.version>1.0</mapkeeper.version>" in "YCSB/pom.xml"

5. mvn clean package
