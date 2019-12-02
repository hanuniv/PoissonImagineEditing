#include "Report_Once.h"
#include <iostream>

using namespace std;

Report_Once::Report_Once()
{
	needreport = true;
}

void Report_Once::report( const string& info)
{
	if (needreport)
	{
		cout<<info;
		needreport = false;
	}
	
}

void Report_Once::reset()
{
	needreport = true;
}
