#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <tuple>

using namespace sw::redis;
using namespace std;

uint64_t getMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


int main(){

    int message_count = 0;
    unsigned int microseconds = 10000;


    try{
        ConnectionOptions connection_options;
        connection_options.host = "192.168.99.113";      
        connection_options.port = 32770;             
        connection_options.db = 0;
        //Connection pool
        ConnectionPoolOptions pool_options;
        pool_options.size = 5;


        //For each master node, maintains a connection pool of size 3.
        RedisCluster cluster(connection_options, pool_options); 

        auto msg_value = "Message number : " ;
        auto pub = cluster.publish("test", "test_message");
        string time_stamp;

        
        //message_count <= 10000000
        while(true){
            time_stamp = std::to_string(getMs());
            cluster.publish("test", time_stamp);
            message_count++;
        }


    }catch(const Error &e){
        cout << "Connection failed ..." << endl;
    }
}

