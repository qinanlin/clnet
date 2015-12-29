#include "sharemem.h"
#include <sys/shm.h>

template<class Type>
ShareMem<Type>::ShareMem()
{
}

template<class Type>
ShareMem<Type>::~ShareMem()
{
	Shm_del();
}

template<class Type>
bool ShareMem<Type>::Shm_init(int iKey, int iFlag)
{
	void *shm_mem = NULL;
	int shm_id;
	
	//iFlag = 0666 | IPC_CREAT;
	
	shm_id = shmget(iKey, sizeof(Type), iFlag);
	if(-1 == shm_id)
	{
		//printf("shmget error\n");
		return false;
	}
	
	shm_mem = shmat(shm_id, NULL, 0);
	if((void*)-1 == shm_mem)
	{
		//printf("shmat error\n");
		return false;
	}
	
	//printf("attach memory....\n");
	m_ShmMem = (Type*)shm_mem;
	
	
	return true;
}

template <class Type>
bool ShareMem<Type>::Shm_del()
{
	if(shmdt(m_ShmMem) == -1)
	{
		return false;
	}
	return true;
}
