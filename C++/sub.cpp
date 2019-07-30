
#include <sw/redis++/redis++.h>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <iterator>




using namespace sw::redis;
using namespace std;
using namespace std::chrono;


string channel = "test";

int m_receive = 0;
bool initialize = false;
int start = 0;

ConnectionOptions connection_options[6];




void print_message(std::string channel, std::string msg){
    cout << "received message " << m_receive <<" from channel: " << channel;
    long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long t_start = atol(msg.c_str());
    cout <<" # Latency: "<< now - t_start << " [ms]" << endl;
}

uint64_t getMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}



bool connection_init(){

    connection_options[0].host = "192.168.99.111";
    connection_options[0].port = 32768; 

    connection_options[1].host = "192.168.99.111";
    connection_options[1].port = 32770;

    connection_options[2].host = "192.168.99.112";
    connection_options[2].port = 32768;

    connection_options[3].host = "192.168.99.112";
    connection_options[3].port = 32770;

    connection_options[4].host = "192.168.99.113";
    connection_options[4].port = 32768;

    connection_options[5].host = "192.168.99.113";
    connection_options[5].port = 32770;

    // /RedisCluster cluster[6] = {RedisCluster(connection_options[0]),RedisCluster(connection_options[1]),RedisCluster(connection_options[2]),RedisCluster(connection_options[3]),RedisCluster(connection_options[4]),RedisCluster(connection_options[5])};


    /*
    
    try{
        RedisCluster cluster0(connection_options[0]);
        return true;

    }catch(const Error &e){
        return false;
    }

    try{
        RedisCluster cluster1(connection_options[1]);
        return true;

    }catch(const Error &e){
        return false;
    }

    try{
        RedisCluster cluster2(connection_options[2]);
        return true;

    }catch(const Error &e){
        return false;
    }

    try{
        RedisCluster cluster3(connection_options[3]);
        return true;

    }catch(const Error &e){
        return false;
    }

    try{
        RedisCluster cluster4(connection_options[4]);
        return true;

    }catch(const Error &e){
        return false;
    }

    try{
        RedisCluster cluster5(connection_options[5]);
        return true;

    }catch(const Error &e){
        return false;
    }
    
     */
    

}

/*

void connection_check(){
    for(int i = 0; i <= 5; i++){
        if(connection_init(i)){
        cout << "Connection with host : " << connection_options[i].host << " | port: " << connection_options[i].port << " : OK" << endl; 
    }else{
        cout << "Connection with host : " << connection_options[i].host << " | port: " << connection_options[i].port << " : FAILED" << endl; 
    }
}
}

 */




int main(){

    connection_init();



    try{
        RedisCluster cluster2(connection_options[2]);
        //connection_options[2].host = "192.168.99.112";
        //connection_options[2].port = 32768;

        //vector<vector<vector<OptionalString>>> result;
        auto r = cluster2.command("CLUSTER","SLOTS");
        std::vector<OptionalString> result;
        reply::to_array(*r, std::back_inserter(result));
        
        //cluster2.command("CLUSTER", "SLOTS", std::back_inserter(result));

        //result = cluster2.command<vector<vector<vector<OptionalString>>>>("CLUSTER", "SLOTS");
    }
    catch(const ReplyError &e){
        cout << "Cluster slots error" << endl;

    }


    try{
        RedisCluster cluster(connection_options[2]);
        auto sub = cluster.subscriber();
        sub.on_message(print_message);
        sub.subscribe("test");

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
            }
        }

    }
    catch(const Error &e){
        cout << "lost";
    }
}

