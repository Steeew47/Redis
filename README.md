# RedisTest Java & C++

- W Redisie opóźnienia praktycznie nie występują (średnio 0,2ms)
- Redis może działać w 4 trybach
    - Asynchronous
    - Synchronous
    - Reactive
    - Pub/Sub (z tego korzystamy, analogicznie jak w kafce)
- Analogicznie jak w kafce tworzymy dwie instancje, jedna generuje wiadomości a druga je odczytuje(wystarczy zasubskrybować ten sam kanał)


### Ustawienie klastra

Podczas testowania właściwości Redis skorzystaliśmy z dwóch różnych sposobów uruchomienia klastra:
- Wykorzystując wcześniej przygotowany [obraz docker'a](https://github.com/Grokzen/docker-redis-cluster) z wbudowanym klastrem.
- Wykorzystując oficjalny [obraz](https://hub.docker.com/_/redis) Redis i samodzielną konfiguracje klastra.

Wykonując samodzielną konfigurację kontenerów posłużono się [plikiem yml](http://gitlab/softswitch/redistest/blob/master/Docker-compose/docker-compose.yml) dla Docker-compose'a. Warto zaznaczyć, że dla każdego kontenera zkorzystano z osobnego pliku konfiguracyjnego.

Przykładowy plik konfiguracyjny `redis.conf`

```
port <port number>
cluster-enabled yes
cluster-config-file nodes.conf
cluster-node-timeout 5000
appendonly yes
```
gdzie port należy podać inny numer portu dla każdego kontenera.

Zgodnie z [dokumentacją](https://redis.io/topics/cluster-tutorial) klastra, aby zepewnić działanie konterów należy podłączyć je do sieci w trybie  `host`
>  In order to make Docker compatible with Redis Cluster you need to use the host networking mode of Docker.





### Awaryjność

- Aby w podstawowy sposób móc korzystać z bezawaryjności Redisa tworzymy clouster składający się z 6 nodów (3 mastery i 3 slave'y)
- Jeśli wysyłamy dane na Mastera A to Master B i C też je otrzymują
- Jeśli Master A nagle "zniknie" Master B i C autoryzują Slave'a przypisanego do Mastera A i ten Slave go zastępuje
- Jeśli jakis Master nie ma Slave'a to Redis mu go przypisuje 

### Przydatne Linki

- [doker-image-readyToUse](https://github.com/Grokzen/docker-redis-cluster)
- [java-library](https://github.com/lettuce-io/lettuce-core)
- [c++-library](https://github.com/sewenew/redis-plus-plus)
    - [c-library-required](https://github.com/redis/hiredis)
