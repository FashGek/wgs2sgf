#include "wgs.h"

string wgsFile::common_info()
{
	stringstream stream;
	string str="(;CA[gb2312]PC[QQΧ��]US[wgs2sgf]SO[wgs2sgf]CP[power by wgs2sgf \ncopyright @ annhe]MULTIGOGM[1]";
	return str;
}
void wgsFile::help()
{
	cout<<"�÷�: wgs2sgf Դ�ļ�\n"
		<<"  or: wgs2sgf *.wgs(ʹ��ͨ���)\n"
		<<"  or: wgs2sgf [ѡ��]\n\n"
		<<"ʾ��: wgs2sgf qqgo.wgs\n\n"
		<<"����ѡ��:\n"
		<<"-qq   ��ʾqq��\n"
		<<"-v    ��ʾ�汾��Ϣ\n"
		<<"-help ��ʾ�˰�����Ϣ\n\n"
		<<"Copyright @ annhe email:admin@annhe.net\n";
}

void wgsFile::version()
{
	cout<<"\nQQΧ��wgs����תsgf���׹��� V 1.4 Release20130821\n"
		<<"����: annhe ��ϵ��ʽ:admin@annhe.net  ��ҳ: www.annhe.net\n";
}

bool wgsFile::testfile(char* filepath)
{
	//�ļ���С�Ƿ�Ϸ�,qqΧ���ļ���С122�ֽ�
	read_wgs.open(filepath);
	isfileopen(filepath);
	read_wgs.seekg(0,ios::end);
	int tmpsize=read_wgs.tellg();
	read_wgs.close();
	if(tmpsize<123)
	{
		cerr<<"wgs2sgf: Error while laod file `"<<filepath<<"`"<<endl;
		return false;
	}
	return true;
}

string wgsFile::getsgf(char* filepath,int p)
{
	string str;
	str=common_info()+getcommon(filepath)+printname(filepath,p)+getdate(filepath)+printdan(filepath)+getresult(filepath)+"\n"+getmoves(filepath);
	return str;
}