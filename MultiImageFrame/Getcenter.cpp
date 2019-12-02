#include "Getcenter.h"

Getcenter::Getcenter()
{
	reset();
}

void Getcenter::reset()
{
	npoint = 0;
	centerx = centery =0;
}

TPoint Getcenter::operator()()const
{
	return TPoint(int(centerx),int(centery));
}

void Getcenter::addpoint( TPoint p)
{
	centerx = (centerx * npoint + p.x)/(npoint+1);
	centery = (centery * npoint + p.y)/(npoint+1);
	npoint++;
}

