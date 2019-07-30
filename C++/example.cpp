#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <string>

using namespace sw::redis;
using namespace std;
using namespace std::chrono;

int main(int set_key, char** argv){

	ConnectionOptions connection_options;
        connection_options.host = "127.0.0.1";
        connection_options.port = 7001;         

        //Connection pool
        ConnectionPoolOptions pool_options;
        pool_options.size = 3;

        //For each master node, maintains a connection pool of size 3.
        RedisCluster cluster(connection_options, pool_options);


        if(set_key == 1){
        	for(int i = 0; i <= 10000; i++){
        	cluster.set("key"+std::to_string(i), "message"+std::to_string(i));
        	}
        }

        
        for(int i = 0; i <= 100000; i++){
        	auto val = cluster.get("key"+std::to_string(i));
        	if(val){
        		cout << *val << endl;
        	}
        	else{
        		cout << "key doesn't exist" << endl;
        		break;
        	}
        }
}
