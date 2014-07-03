//
//  vnMemoryTracker.h
//  vnbase
//
//  Created by Wayne on 13-6-6.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnMemoryTracker_h
#define _vnMemoryTracker_h

#include "vnTypes.h"
#ifdef VN_MEMORY_TRACKER

#include "vnFastMutex.h"
#include <map>

_vn_begin

class _vn_base_api MemoryTracker {
public:
	static MemoryTracker & instance();

	~MemoryTracker();

	void recordAllocation(void *p, size_t size, const char *file = 0, int line = 0, int hint = 0);
	void recordDeallocation(void *p);
	
	size_t totalMemoryAllocated();
	void reportLeaks();
    
    void setAutoReportLeaks(bool auto_report);
    bool isAutoReportLeaks();
    
    void setReportFile(const c8 *name);
private:
	struct Allocation {
		size_t size;
		str8 file;
		int line;
		int hint;
		
		Allocation() : size(0), line(0), hint(0) {}
		Allocation(size_t s, const char *f, int l, int h ) : size(s), line(l), hint(h) { if (f) file = f; }
	};
	typedef std::map<void *, Allocation> AllocationMap;

	MemoryTracker();
	MemoryTracker(const MemoryTracker &);

	size_t m_totalAllocations;
	AllocationMap m_allocations;
	FastMutex m_lock;
    bool m_auto_report;
    str8 m_report_file;
};

_vn_end

#endif

#endif