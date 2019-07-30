
#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <string>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>




using namespace sw::redis;
using namespace std;
using namespace std::chrono;


int m_receive = 0;
bool initialize = false;
int start = 0;




void print_message(std::string channel, std::string msg){
    cout << "received message " << m_receive <<" from channel: " << channel;
    long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long t_start = atol(msg.c_str());
    cout <<" # Latency: "<< now - t_start << " [ms]" << endl;
    m_receive ++;
}

uint64_t getMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


int main(){


    try{
        ConnectionOptions connection_options;
        connection_options.host = "192.168.99.112";
        connection_options.port = 32768;
        connection_options.socket_timeout = std::chrono::milliseconds(200);


        //Connection pool
        ConnectionPoolOptions pool_options;
        pool_options.size = 3;

        //For each master node, maintains a connection pool of size 3.
        RedisCluster cluster(connection_options, pool_options);

        // Create a Subscriber.
        auto sub = cluster.subscriber();

        sub.on_message(print_message);


        // Subscribe to channels and patterns.
        sub.subscribe("test");

        

        // Consume messages in a loop
        while (true) {
            try {
                if(!initialize){
                    start = getMs();
                    initialize = true;
                }
                sub.consume();
                m_receive++;
            } catch (const Error &err) {
                cout << "Message consume error";
                //cluster2 = std::move(cluster);
            }
        }
        
    }catch(const Error &e){
        cout << "Connection failed ..." << endl;

    }
}
