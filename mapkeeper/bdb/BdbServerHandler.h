/*
 * Copyright 2012 Yahoo! Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <boost/thread.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <db_cxx.h>
#include "Bdb.h"
#include "MapKeeper.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace mapkeeper;
using boost::shared_ptr;

class BdbServerHandler : virtual public MapKeeperIf {
public:
    BdbServerHandler();
    int init(const std::string& homeDir, 
             uint32_t pageSizeKb, uint32_t numRetries,
             uint32_t keyBufferSizeBytes, uint32_t valueBufferSizeBytes,
             uint32_t checkpointFrequencyMs, uint32_t checkpointMinChangeKb);
    ResponseCode::type ping();
    ResponseCode::type addMap(const std::string& databaseName);
    ResponseCode::type dropMap(const std::string& databaseName);
    void listMaps(StringListResponse& _return);
    void scan(RecordListResponse& _return, const std::string& databaseName, const ScanOrder::type order, 
            const std::string& startKey, const bool startKeyIncluded,
            const std::string& endKey, const bool endKeyIncluded,
            const int32_t maxRecords, const int32_t maxBytes);
    void get(BinaryResponse& _return, const std::string& databaseName, const std::string& recordName);
    ResponseCode::type put(const std::string& databaseName, const std::string& recordName, const std::string& recordBody);
    ResponseCode::type insert(const std::string& databaseName, const std::string& recordName, const std::string& recordBody);
    ResponseCode::type insertMany(const std::string& databaseName, const std::vector<Record> & records);
    ResponseCode::type update(const std::string& databaseName, const std::string& recordName, const std::string& recordBody);
    ResponseCode::type remove(const std::string& databaseName, const std::string& recordName);

private:
    void checkpoint(uint32_t checkpointFrequencyMs, uint32_t checkpointMinChangeKb);
    void initEnv(const std::string& homeDir);
    static void bdbMessageCallback(const DbEnv *dbenv, const char *errpfx, const char *msg);
    boost::shared_ptr<DbEnv> env_;
    boost::ptr_map<std::string, Bdb> maps_;
    boost::shared_mutex mutex_; // protect maps_
    boost::scoped_ptr<boost::thread> checkpointer_;
    uint32_t keyBufferSizeBytes_;
    uint32_t valueBufferSizeBytes_;
    static std::string DBNAME_PREFIX;
};
