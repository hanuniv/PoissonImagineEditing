#ifndef GET_CENTER_H_
#define GET_CENTER_H_

#include "MyPoint.h"

class Getcenter{
	//һ�����ڶ��������ݵ����һ���ȽϺ��ʵ����ĵ�С��
	//������(),��������һ��ʹ��
public:
	Getcenter();
	void reset();
	void addpoint(TPoint);
	TPoint operator()() const; //ע��������const,�������õ���const����
private:
	double centerx,centery; //��¼��ǰ������Ŀ������
	int npoint;

};

#endif