#ifndef REPORT_ONCE
#define	REPORT_ONCE

#include <iostream>
#include <string>

using namespace std;

class Report_Once{
public:
	Report_Once();
	void reset();
	void report(const string&);
private:
	bool needreport;
};

#endif
