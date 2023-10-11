// timer.c
#include "../localTypes.h"

//~ typedef struct TimerMemMap {
	//~ volatile u32 timeWriteHigh;
	//~ volatile u32 timeWriteLow;
	//~ volatile u32 timeReadHigh;
	//~ volatile u32 timeReadLow;
	//~ volatile u32 alarm[4];
	//~ volatile u32 armed;
	//~ volatile u32 timeReadHighRaw;
	//~ volatile u32 timeReadLowRaw;
	//~ volatile u32 debugPause;
	//~ volatile u32 pause;
	//~ volatile u32 intr;
	//~ volatile u32 inte;
	//~ volatile u32 intf;
	//~ volatile u32 ints;
//~ } TimerMemMap;

typedef struct TimerData {
	u32 *nextProcess;
} TimerData;

typedef struct TimersInfo {
	u32       usageBitMap;
	TimerData alarm[4];
} TimersInfo;

static TimersInfo timerInfo;

/*e*/void
enableWatchDogTick(void)/*p;*/
{
	u32 *watchDogTickReg = (void*)0x4005802C;
	*watchDogTickReg = 12|(1<<9);
}

/*e*/void
timerInit(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	timer->intr = 0xFF;
	timer->inte = (1<<0)|(1<<1)|(1<<2)|(1<<3);
}

/*e*/
s32 timer_set(u32 selectedTimer, u32 micoseconds)/*p;*/
{
	if (micoseconds < 10) { micoseconds = 10; }
	u32 retVal = 0;
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentTime = timer->timeReadLowRaw;
	u32 targetTime = currentTime + micoseconds;
	timer->alarm[selectedTimer] = targetTime;
	return retVal;
}

/*e*/
void alarm1ISR(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
	// reset timer
	timer_set(0, 5*1000*1000);
	// frame start
	// turn on frame
	u32 *intSet = (void*)PPB_INTERRUPT_SET_PEND;
	*intSet = (1<<26);
}

/*e*/
void alarm2ISR(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
	// reset timer
	timer_set(1, 15*1000*1000);
	// frame start
	// turn on frame
	u32 *intSet = (void*)PPB_INTERRUPT_SET_PEND;
	*intSet = (1<<28);
}

/*e*/
void alarm3ISR(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
}

/*e*/
void alarm4ISR(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
}

/*
 * 1. frame int
 * 
*/

/*e*/
void minorFrame(void)/*p;*/
{
	u32 *intClear = (void*)PPB_INTERRUPT_CLEAR_PEND;
	*intClear = (1<<26);
	io_prints("\nMinor Frame Start!\n");
}

volatile u32 bCounter;

/*e*/
void bProgress(void)/*p;*/
{
	bCounter = 0;
	while(bCounter < 5*1024*1024)
	{
		bCounter++;
	}
	io_prints("*");
}

/*e*/
void Background(void)/*p;*/
{
	u32 *intClear = (void*)PPB_INTERRUPT_CLEAR_PEND;
	*intClear = (1<<28);
	io_prints("\nBackground Start!\n");
	for (u32 i = 0; i < 20; i++)
	{
		bProgress();
	}
	io_prints("\nBackground End!\n");
}
