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
		<<"-qq   ת��qq��\n"
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
	string str=common_info()+printdan(filepath);
	str=str+"C[�ڷ�:    "+getname(filepath,10)+"    ";
	string tmpstr="";
	stringstream stream;
	if(brank<0)
		stream<<abs(brank)<<"��";
	else
		stream<<brank<<"��";
	stream>>tmpstr;
	str+=tmpstr;
	stream.clear();
	stream.str("");
	
	if(blackdan>0)
	{
		stream<<"(ҵ��"<<blackdan<<"��)";
		stream>>tmpstr;
	}
	str=str+tmpstr+"\n";
	stream.clear();
	stream.str("");
	
	str=str+"�׷�:    "+getname(filepath,58)+"    ";
	
	if(wrank<0)
		stream<<abs(wrank)<<"��";
	else
		stream<<wrank<<"��";
	stream>>tmpstr;
	str+=tmpstr;
	stream.clear();
	stream.str("");
	tmpstr="";
	
	if(whitedan>0)
	{
		stream<<"(ҵ��"<<whitedan<<"��)";
		stream>>tmpstr;
	}
	str=str+tmpstr+"\n";
	stream.clear();
	stream.str();
	
	getresult(filepath);
	getdate(filepath);
	
	str=str+"���:    "+result.substr(3,result.length()-4)+"\n";
	str=str+"����:    "+date.substr(3,date.length()-4)+"]";
	
	str=str+getcommon(filepath)+printname(filepath,p)+date+result+"\n"+getmoves(filepath);
	return str;

}