#ifndef __PERFTIMER_H__
#define __PERFTIMER_H__

#include "p2Defs.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //__PERFTIMER_H__