#ifndef MEMORYBANK_H_
#define MEMORYBANK_H_
template <class T>
class MemoryBank
{
	//singleton pointer
	static MemoryBank<T>* pSingle;
	//helper class for list of memory blocks
	struct NextInMemBank
	{
	public:
		NextInMemBank* next;
	};
	//pointer to memory list first element
	NextInMemBank *bank;
	MemoryBank(int exp, int elem):bank(0),expSize(exp), elemSize(elem)
	{
		expandMemBank();
	}
public:
	//number of elements to expand bank if bank->next == 0
	int expSize;
	//size of each element of memory  = size of MapBlock::data in bytes
	int elemSize;
	void expandMemBank()
	{
		//
		if (!bank)
		{
			 bank = reinterpret_cast<NextInMemBank *> (new char[elemSize]);
			 bank->next = 0;
		}
		NextInMemBank * runner = bank;
		//go to last elem of list
		while (runner->next) runner = bank->next;
		for(int i = 0; i< expSize; i++)
		{
			runner->next = reinterpret_cast<NextInMemBank *> (new char[elemSize]);
			runner = runner->next;
		}
		runner->next = 0;
	}
	void* newFromMemBank()
	{
		if(!bank) expandMemBank();
		NextInMemBank* head = bank;
		bank = head->next;
		return head;
	}
	void deleteToMemBank(void *doomed)
	{
		NextInMemBank* head = reinterpret_cast<NextInMemBank*> (doomed);
		head->next = bank;
		bank = head;
	}
	static MemoryBank* getBank(int exp, int elem)
	{
		if (pSingle) return pSingle;
		return pSingle = new MemoryBank(exp,elem);
	}
	static MemoryBank *getBank()
	{
		return pSingle;
	}
	~MemoryBank()
	{
		while(bank)
		{
			NextInMemBank* del = bank;
			bank = bank->next;
			delete [] del;
		}
	}

};

#endif /*MEMORYBANK_H_*/
