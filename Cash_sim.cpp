#include  <iostream>
#include  <iomanip>
#include  <string>
#include  <math.h>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)
#define		Line_size		(8)
#define		Num_lines		(CACHE_SIZE/Line_size)
int cache[Num_lines][3];	//chace array 


enum cacheResType { MISS = 0, HIT = 1 };

// The following implements a random number generator 
unsigned int m_w = 0xABCCAB55;    // must not be zero, nor 0x464fffff 
unsigned int m_z = 0x05786902;    // must not be zero, nor 0x9068ffff 
unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  // 32-bit result 
}

unsigned int memGenH()
{
	static unsigned int addr = 0;
	return (addr += 4) % (DRAM_SIZE);
}

unsigned int memGenA()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE / 4);
}

unsigned int memGenB()
{
	static unsigned int addr = 0;
	return  rand_() % (64 * 1024);
}

unsigned int memGenC()
{
	static unsigned int a1 = 0, a0 = 0;
	a0++;
	if (a0 == 512) { a1++; a0 = 0; }
	if (a1 == 128) a1 = 0;
	return(a1 + a0 * 128);
}

unsigned int memGenD()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 4);
}

unsigned int memGenE()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 16);
}

unsigned int memGenF()
{
	static unsigned int addr = 0;
	return (addr += 64) % (128 * 1024);
}



// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{

	unsigned int offset = log2(Line_size);
	unsigned int index_size = log2((CACHE_SIZE / Line_size));
	unsigned int shift = pow(2, index_size);
	unsigned int index = (addr / Line_size) % shift;
	unsigned int tag = addr / (shift * Line_size);

	if (cache[index][1])
	{
		if (cache[index][2] == tag)
		{
			return HIT;
		}
		else
		{
			cache[index][2] = tag;
			return MISS;
		}

	}
	else
	{
		cache[index][1] = 1;
		cache[index][2] = tag;
		return MISS;
	}
}


string msg[2] = { "Miss","Hit" };

#define		NO_OF_Iterations	100	// CHange to 1,000,000
int main()
{
	unsigned int hit = 0;
	cacheResType r;

	unsigned int addr;
	cout << "Direct Mapped Cache Simulator\n";

	for (int i = 0; i < Num_lines; i++)
	{
		cache[i][1] = 0;
	}

	for (int inst = 0; inst < NO_OF_Iterations; inst++)
	{
		addr = memGenH();
		r = cacheSimDM(addr);
		if (r == HIT) hit++;
		cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "Hit ratio = " << dec << (100 * hit / NO_OF_Iterations) << endl;

	system("pause");
	return 0;
}