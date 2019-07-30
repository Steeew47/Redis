import io.lettuce.core.*;
import io.lettuce.core.api.StatefulRedisConnection;
import io.lettuce.core.pubsub.RedisPubSubAdapter;
import io.lettuce.core.pubsub.RedisPubSubListener;
import io.lettuce.core.pubsub.StatefulRedisPubSubConnection;
import io.lettuce.core.pubsub.api.sync.RedisPubSubCommands;
import reactor.util.function.Tuple2;
import reactor.util.function.Tuples;


import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;

import static java.lang.Integer.parseInt;


public class main {
    public static void main(String[] args) throws ExecutionException, InterruptedException {

        final long[] avg = {0};
        final int messages= 10000;

        //RedisClient client = RedisClient.create("redis://172.19.0.2:7000");

        RedisClient redisClient = RedisClient.create(RedisURI.create("172.19.0.2", 7001));
        StatefulRedisPubSubConnection<String, String> connection = redisClient.connectPubSub();

        //RedisListener
        RedisPubSubListener<String, String> listener = new RedisPubSubAdapter<String, String>() {

            @Override
            public void message(String channel, String message) {

                long ts = System.currentTimeMillis()- Long.valueOf(message);
                avg[0]+=ts;
                System.out.println(String.format("Channel: %s, Message: %s, Delay: %d", channel, message, ts));
            }
        };

        connection.addListener(listener);
        RedisPubSubCommands<String, String> sync = connection.sync();
        sync.subscribe("channel");

        //RedisSender
        StatefulRedisConnection<String, String> sender = redisClient.connect();

        for(int i=0; i < messages; i++){
            sender.sync().publish("channel", Long.toString(System.currentTimeMillis()));
        }

        Thread.sleep(1000);

        System.out.printf("Average delay: %d", avg[0]/messages);

    }


}
