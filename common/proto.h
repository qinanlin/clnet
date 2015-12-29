#ifndef PROTO_H
#define PROTO_H

#define PROTO_MSG_LENTH 2
#define MAX_MSG_LENTH 65536

#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
	unsigned short uiLen;
	char *pBuf;
}ProtoUnit;

#ifdef __cplusplus
}
#endif

#endif