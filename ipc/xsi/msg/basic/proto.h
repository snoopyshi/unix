#ifndef __PROTO_H
#define __PROTO_H


#define KEYPATH     "/etc/services"
#define KEYPROJ     'g'
#define NAMESIZE     1024

struct msg_st
{
    long mtype;
    char name[NAMESIZE];
    int math;
    int chinese;
};



#endif
