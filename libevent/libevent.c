#include"libevent.h"
#define POST 8888

void read_cb(struct bufferevent *bv,void *arg)
{
    char buf[1024]={0};
    bufferevent_read(bv,buf,sizeof(buf));
    printf("%s\n",buf);

    char buffer[1024]="hello world!my name is zhouli!";
    bufferevent_write(bv,buffer,sizeof(buffer));
}

void write_cb(struct bufferevent *bv,void *arg)
{
    printf("send data is success!\n");
}

void event_cb(struct bufferevent *bv,short events,void *arg)
{
    
}

void listen_cb(struct evconnlistener *listen1,evutil_socket_t sock,struct sockaddr *addr,int len,void *ptr)
{
    struct event_base *base =(struct event_base *)ptr;
    struct bufferevent *bev = bufferevent_socket_new(base,sock,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);
    bufferevent_enable(bev,EV_READ);
}
int main()
{
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(POST);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    struct event_base *base = event_base_new();
    struct evconnlistener *bevlis = evconnlistener_new_bind(base,listen_cb,(void *)base,LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,-1,(struct sockaddr *)&addr,sizeof(addr)); 
    
  
    event_base_dispatch(base);
    event_base_free(base);
    evconnlistener_free(bevlis);
    
    return 0;
}