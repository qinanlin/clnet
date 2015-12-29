#ifndef _SHAREMEM_H_
#define _SHAREMEM_H_

#include "CommMisc.h"
#include <sys/shm.h>

template <class Type>
class ShareMem
{
public:
	ShareMem();
	~ShareMem();
	bool Shm_init(int, int);
	bool Shm_del();
public:
	Type *m_ShmMem;
};
#endif

