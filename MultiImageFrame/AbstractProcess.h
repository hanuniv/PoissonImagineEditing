#ifndef ABSTRACT_CHANGE_H_
#define ABSTRACT_CHANGE_H_
#include "Eigen/Dense"
#include <QImage>



//������࣬����ͼ��汸�ݺͻָ�
//����ͼ������������̳�

class Abstract_Process
{
	public:
		typedef QImage Tpic;
		Abstract_Process();
		bool haschanged();		
		void set_changed();
		void set_backup(Tpic*);
		void retrieve_backup(Tpic*);

		//��������
		virtual void process(Tpic* image_); 
		virtual void setdata(int,int); //�����趨����
		virtual operator bool();
	protected:
		bool		_haschanged;								//whether should restore to previous state;
		Tpic* 		image_mat_backup_;//��ָ�룬����������
};

#endif