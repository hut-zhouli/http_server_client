#include<stdio.h>
#include<string.h>
#include<event2/buffer.h>
#include<event2/bufferevent.h>
#include<event2/listener.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>

void read_cb(struct bufferevent *bv,void *arg)
{
    char buf[1024]={0};
    bufferevent_read(bv,buf,sizeof(buf));
    printf("%s\n",buf);
}

void write_cb(struct bufferevent *bv,void *arg)
{
    printf("send data is success!\n");
}

void event_cb(struct bufferevent *bv,short events,void *arg)
{
    
}

void event_callback(evutil_socket_t fd,short events,void *arg)
{
    char buf[1024]={0};
    struct bufferevent * bev = (struct bufferevent *)arg;
    int len = read(fd,buf,sizeof(buf));
    if(len==-1||len==0)
    {
        perror("read error!");
    }
    printf("%s\n",buf);

    //char buffer[1024]="hello world!my name is youweihao!";
    bufferevent_write(bev,buf,sizeof(buf));
}


int main()
{
    struct event_base *base = event_base_new();
    int fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    struct bufferevent * bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_socket_connect(bev,(struct sockaddr *)&addr,sizeof(addr));
    bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);

    struct event *ev = event_new(base,STDIN_FILENO,EV_READ|EV_PERSIST,event_callback,(void *)bev);
    event_add(ev,NULL);
    bufferevent_enable(bev,EV_READ);
    event_base_dispatch(base);
    event_base_free(base);
    event_free(ev);
    bufferevent_free(bev);
    return 0;
}