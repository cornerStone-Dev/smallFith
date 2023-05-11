#include "../localTypes.h"


static void *zalloc_internal(u32 nByte)
{
	Chunk **cursor = &s.chunks;
	Chunk *target = 0;
	while (*cursor)
	{
		if ((*cursor)->size == nByte)
		{
			target = *cursor;
			*cursor = (*cursor)->next;
			break;
		}
		cursor = &(*cursor)->next;
	}
	if (target == 0)
	{
		s.heapCursor -= nByte;
		target = (void*)s.heapCursor;
	}

	// set memory to zero using DMA
	//~ setZeroWait();
	//~ setZero(target, nByte);
	target->size = nByte;
	u32 *mem = target->data;
	u32 *end = (u32*)((u32)mem + nByte - 8);
	do {
		mem[0] = 0;
		mem[1] = 0;
		mem += 2;
	} while (mem < end);
	return target->data;
}


/*
** Free an outstanding memory allocation.
*/
static void free_internal(void *pOld)
{
	u8 *raw = pOld;
	Chunk *target = (void*)(raw-8);
	target->next = s.chunks;
	s.chunks = target;
}

/*e*/
void free(void *pOld)/*p;*/
{
	if (pOld==0) { return; }
	asm("CPSID i");  // disable interrupts
	//~ os_takeSpinLock(LOCK_MEMORY_ALLOC);
	free_internal(pOld);
	//~ os_giveSpinLock(LOCK_MEMORY_ALLOC);
	asm("CPSIE i"); // enable interrupts
}

/*e*/
void *zalloc(u32 nByte)/*p;*/
{
	// if nByte is 0 -> return 0 to be consistent with realloc
	if (nByte==0) { return 0; }
	asm("CPSID i"); // disable interrupts
	//~ os_takeSpinLock(LOCK_MEMORY_ALLOC);
	void *memory = zalloc_internal((nByte + 15) & -8);
	//~ os_giveSpinLock(LOCK_MEMORY_ALLOC);
	asm("CPSIE i"); // enable interrupts
	return memory;
}

/*
* Flexible memory allocation function.
* 1. If pPrior is 0 -> call zalloc.
* 2. If nBytes is 0 -> call free. 
* 3. If nBytes > oldSize we get a larger fresh allocation and copy data into it
* 4. If nBytes <= oldSize/2 we get a smaller allocation and copy into it.
* 5. Otherwise we return pPrior 
*/

#if 0

/*e*/
void *realloc(void *pPrior, u32 nBytes)/*p;*/
{
	u32   oldSize, copyAmount;
	void *newMemory;
	if (pPrior == 0)
	{
		return zalloc(nBytes);
	}
	if (nBytes == 0)
	{
		free(pPrior);
		return 0;
	}
	oldSize = memsys5Size(pPrior);
	if (nBytes>oldSize)
	{
		copyAmount = oldSize;
	} else if (nBytes <= (oldSize/2)) {
		copyAmount = nBytes;
	} else {
		return pPrior;
	}
	// standard realloc logic
	newMemory = zalloc(nBytes);
	if (newMemory)
	{
		os_takeSpinLock(LOCK_COPY_DMA);
		dmaWordForwardCopy(pPrior, newMemory, copyAmount);
		os_giveSpinLock(LOCK_COPY_DMA);
		//~ ut_memCopy(newMemory, pPrior, copyAmount);
		free(pPrior);
	}
	return newMemory;
}

/*e*/
void *allocStack(void)/*p;*/
{
	asm("CPSID i"); // disable interrupts
	os_takeSpinLock(LOCK_MEMORY_ALLOC);
	void *stack = mem.cache;
	mem.cache = zalloc_internal(512, 4);
	os_giveSpinLock(LOCK_MEMORY_ALLOC);
	asm("CPSIE i"); // enable interrupts
	return stack;
}

/*
** Initialize the memory allocator.
*/
/*e*/
void memSysInit(void)/*p;*/
{
	s32 ii;            /* Loop counter */
	u32 iOffset;       /* An offset into mem.aCtrl[] */
	
	mem.zPool = ((u8*)START_OF_REG_RAM); // start of memory
	//~ mem.nBlock = asmDiv(33, END_OF_REG_RAM - (u32)mem.zPool); // heapsize / (ATOM+1)
	mem.nBlock = (END_OF_REG_RAM - START_OF_REG_RAM) / 33;
	mem.aCtrl = &mem.zPool[mem.nBlock*ATOM_SIZE]; // start of control

	// initialize free lists to sentinal
	for(ii=0; ii<LOGMAX; ii++){
	mem.aiFreelist[ii] = &mem.sentinalNode;
	}
	// set sentinal free list to a value other than sentinal node
	mem.aiFreelist[LOGMAX] = (MemDLL*)&mem.aiFreelist[LOGMAX];
	// fill free lists with largest possible blocks of powers of 2
	iOffset = 0;
	for(ii=LOGMAX; ii>=0; ii--)
	{
		int nAlloc = (1<<ii);
		if( (iOffset+nAlloc)<=mem.nBlock ){
			mem.aCtrl[iOffset] = ii + CTRL_FREE;
			memsys5Link(GET_MEMDLL(iOffset), ii, mem.aiFreelist);
			iOffset += nAlloc;
		}
	}
	(void)allocStack();
	return;
}

/*e*/
void printMemStats(void)/*p;*/
{
	u32 totalMemory = mem.nBlock * 32;
	io_printi(totalMemory);
	io_prints(": bytes of memory under stewardship.\n");
	u32     memReady = 0;
	MemDLL *cursor   = 0;
	u32     memSize  = 32;
	for (u32 x = 0; x < LOGMAX; x++)
	{
		io_printi(memSize);
		io_prints(":");
		cursor = mem.aiFreelist[x];
		u32 nodeCount = 0;
		while (cursor != &mem.sentinalNode)
		{
			memReady += memSize;
			//~ io_prints("#");
			nodeCount++;
			cursor = cursor->next;
		}
		while (nodeCount > 0)
		{
			io_prints("#");
			nodeCount--;
		}
		io_prints("\n");
		memSize *= 2;
	}
	io_printi(totalMemory - memReady);
	io_printsn(": bytes of memory in use.");
	io_printi(memReady);
	io_printsn(": bytes of memory remaining.");
	io_printi(START_OF_REG_RAM - (u32)c.threadBase);
	io_printsn(": bytes of memory remaining in program data region.");
}

#endif
