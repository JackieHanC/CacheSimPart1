#include "stdio.h"
#include "cache.h"
#include "memory.h"
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments\n");
        printf("Usage: ./sim pathOfTraceFile\n");
        return 0;
    }
    Memory m;
    Cache l1;
    l1.SetLower(&m);

    StorageStats s;
    s.access_time = 0; 
    s.access_counter = 0;
    m.SetStats(s);
    l1.SetStats(s);

    StorageLatency ml;
    ml.bus_latency = 6;
    ml.hit_latency = 100;
    m.SetLatency(ml);

    StorageLatency ll;
    ll.bus_latency = 3;
    ll.hit_latency = 10;
    l1.SetLatency(ll);


    // set the cache config
    CacheConfig_ config;

    config.size = 1 MB;

    config.associativity = 2;

    config.set_num = 128;


    config.write_through = 1;
    config.write_allocate = 0;

    l1.SetConfig(config);



    int hit, time;
    char content[64];
// #ifdef DEBUG
//     printf("HandleRequest 1\n");
// #endif 
//     l1.HandleRequest(0, 0, 1, content, hit, time);
//     printf("Request access time: %dns\n", time);

// #ifdef DEBUG
//     printf("HandleRequest 2\n");
// #endif
//     l1.HandleRequest(1024, 0, 1, content, hit, time);
//     printf("Request access time: %dns\n", time);
    ifstream trace_file(argv[1]);

    char c;
    uint64_t addr;
    int read;
    while(trace_file >> c >> addr) {
        //read = (c == 'r' ? 1:0);
        if (c=='r')
        {
            read = 1;
        }
        else
            read = 0;
        printf("\n%c  %x\n", c, addr);

        l1.HandleRequest(addr, 0, read, content, hit, time);

        
        printf("Request access time: %dns\n", time);
    }


    l1.GetStats(s);
    printf("Total L1 access time: %dns\n", s.access_time);
    printf("L1 access count is %d\n",s.access_counter);
    printf("L1 miss num is %d\n",s.miss_num );
    printf("L1 miss rate %.2f%%\n", s.miss_num/(float)s.access_counter * 100);
    m.GetStats(s);
    printf("Total Memory access time: %dns\n", s.access_time);
    return 0;
}
