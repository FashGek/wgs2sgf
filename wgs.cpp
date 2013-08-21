#include <sys/stat.h>
#include <time.h>
#include "wgs.h"

/*
 * ����qq�� ����ĳqq��16������ʽ����Ϊ��qq[6]={00 00 41 E4 03 E9}����qq�ż�����̣� 
 * (4*16^7+1*16^6) + (14*16^5+4*16^4)+ ...  =(4*16+1)*16^6 + (14*16+4)*16^4 + ......
 * ��41��ʾ��ʮ������Ϊ 4*16+1 ,E4��ʾ����Ϊ 14*16+4����ɼ�����Ϊ ��
 * qq��= qq[0]*16^10 + qq[1]*16^8 + qq[2]*16^6 + qq[3]*16^4 + +qq[4]*16^2 + qq[5]*16^0;
 */
 void wgsFile::isfileopen(char* filepath)
 {
	if(!read_wgs)
	{
		cerr<<"wgs2sgf: cannot convert `"<<filepath<<"`: No such file or directory"<<endl;
		exit(0);
	}
 }
 //����qq��
long long wgsFile::getqq(char* filepath,int p)
{
	read_wgs.open(filepath,ios::in|ios::binary);	//�����Ʒ�ʽ��
	isfileopen(filepath);

	read_wgs.seekg(p,ios::beg);						//��λ����4�ֽڴ���ȡ����qq��
	unsigned char b[6];
	for(int i=5;i>=0;i--)
	{
		read_wgs.read((char*)&b[i],1);				//��read��ȡ����������
	}
	read_wgs.close();
	
	unsigned int qq[6];
	for(int i=0;i<6;i++)
	{
		qq[i]=(unsigned int)b[i];				//�޷����ͷ�ֹQQ��ת����Ϊ����
	}
	long long qqnumber;
	qqnumber=qq[0]*1099511627776+qq[1]*4294967296+qq[2]*16777216+qq[3]*65536+qq[4]*256+qq[5];
	return qqnumber;
}

string wgsFile::getname(char* filepath,int p)
{
	read_wgs.open(filepath);
	isfileopen(filepath);
	
	read_wgs.seekg(p,ios::beg);	//�����������������ھ��ļ���ʼ10�ֽڴ�
	char n[22];					//��������
	string name;				//��������
	read_wgs.get(n,22);			//����������Ϣռ��22�ֽ�
	stringstream stream;
	stream<<n;
	stream>>name;
	read_wgs.close();
	return name;
}

string wgsFile::printname(char* filepath,int p)
{
	bqq=getqq(filepath,4);			//����QQ��
	wqq=getqq(filepath,52);			//����QQ��
	bname=getname(filepath,10);		//�����ǳ�
	wname=getname(filepath,58);		//�����ǳ�
	string bqqstr="";				//����QQ�ַ���
	string wqqstr="";				//����QQ�ַ���	
	stringstream stream;
	if(p==1)						//p=1 ����ʾqq��
	{
		stream<<"("<<bqq<<")";
		stream>>bqqstr;
		stream.clear();
		stream.str("");
		stream<<"("<<wqq<<")";
		stream>>wqqstr;
		stream.clear();
		stream.str("");
	}
	string str;
	stream<<"PB["<<bname<<bqqstr<<"]PW["<<wname<<wqqstr<<"]";
	stream>>str;
	stream.clear();
	stream.str("");
	return str;
}
int wgsFile::gethandicap(char* filepath)
{
	read_wgs.open(filepath);
	isfileopen(filepath);
	
	read_wgs.seekg(100,ios::beg);		//��������100�ֽڴ�
	char ch;
	read_wgs.read(&ch,1);
	handicap=(unsigned int)ch;
	read_wgs.close();
	return handicap;
}

string wgsFile::getdate(char* filepath)
{
	read_wgs.open(filepath);
	isfileopen(filepath);
	
	struct stat statbuf;
	stat(filepath, &statbuf);		/* get information about the file */ 
	
	read_wgs.close();

	long *m_second=&statbuf.st_mtime;				//st_mtime �ļ��������һ���޸�ʱ��(Unixʱ���) ����wgs������˵�������ǳ��δ���ʱ�䣬���Ծ�ʱ��
	struct tm *m_localtime=localtime(m_second);		//localtime��ʱ���ת��Ϊ����ʱ��

	stringstream stream;
	stream<<"DT["<<m_localtime->tm_year+1900<<"-"<<m_localtime->tm_mon+1<<"-"<<m_localtime->tm_mday<<"]";
	stream>>date;
	stream.clear();
	stream.str("");
	return date;
}

/*
 * ������Ϣ��λ����λ����QQ������
 * �����л��֣�c2 ff ff ff���������� ff ff ff c2�����룬���Ϊ80 00 00 3E��ת��Ϊʮ���ƣ�-62��
 */
long long wgsFile::getrank(char* filepath,int p)
{
	read_wgs.open(filepath,ios::in|ios::binary);
	isfileopen(filepath);
	
	long long rank;		//����
	read_wgs.seekg(p,ios::beg);
	unsigned char ch[4];
	for(int i=3;i>=0;i--)
	{
		read_wgs.read((char*)&ch[i],1);
	}
	unsigned int tmp[4];
	for(int i=0;i<4;i++)
	{
		tmp[i]=(unsigned int)ch[i];
	}
	if(tmp[0]<128)
	{
		rank=tmp[0]*16777216+tmp[1]*65536+tmp[2]*256+tmp[3];
	}
	else if(tmp[0]>=128)
	{
		//������Ҫ����,����int������4���ֽڣ���32λ��ffȡ����Ϊffffff00������ϣ���Ľ��������ֻȡ��8λ����
		unsigned int mask=0x000000ff;
		for(int i=0;i<4;i++)
		{
			tmp[i] = (~tmp[i])&mask;
		}
		rank=tmp[0]*16777216+tmp[1]*65536+tmp[2]*256+tmp[3]+1;
		rank=-rank;
	}
	
	/*
	 * 9 �� =-800  8 �� =-700  7 �� =-600  6 �� =-500  5 �� =-400  4 �� =-300  3 �� =-200  2 �� =-100  1 �� =0 
	 * 1 �� =100   2 �� =200   3 �� =300   4 �� =400   5 �� =500   6 �� =600   7 �� =700   8 �� =800   9 �� =900 
     */
	rank=rank/100;
	if(rank<1)			//����0~100֮��Ϊ1������� ��40/100=0,0-1 = -1Ϊ1��
		rank=rank-1;
	if(rank>9)
		rank=9;
	if(rank<-9)
		rank=-9;
		
	read_wgs.close();
	
	return rank;
}

int wgsFile::getdan(char* filepath,int p)
{
	read_wgs.open(filepath,ios::in|ios::binary);
	isfileopen(filepath);
	
	read_wgs.seekg(p,ios::beg);
	char d[1];
	read_wgs.read(&d[0],1);
	int dan=(unsigned int)d[0];
	read_wgs.close();
	return dan;
	
}

string wgsFile::printdan(char* filepath)
{
	int black=getrank(filepath,36);		//�����λ,36�ֽڴ�
	int white=getrank(filepath,84);		//�����λ,84�ֽڴ�
	int blackdan=getdan(filepath,42);	//���������λ,42�ֽڴ�
	int whitedan=getdan(filepath,90);	//���������λ,90�ֽڴ�
	read_wgs.open(filepath,ios::in|ios::binary);
	isfileopen(filepath);
	string str="BR[";
	string tmpstr="";
	stringstream stream;
	if(black<0)
	{
		stream<<abs(black)<<"��";
	}
	else
	{
		stream<<black<<"��";
	}
	stream>>tmpstr;
	stream.clear();
	stream.str("");
	str+=tmpstr;
	if(blackdan>0)
	{
		stream<<"(ҵ��"<<blackdan<<"��)";
		stream>>tmpstr;
		stream.clear();
		stream.str("");
		str+=tmpstr;
	}
	str+="]WR[";
	
	if(white<0)
	{
		stream<<abs(white)<<"��";
	}
	else
	{
		stream<<white<<"��";
	}
	stream>>tmpstr;
	stream.clear();
	stream.str("");
	str+=tmpstr;
	if(whitedan>0)
	{
		stream<<"(ҵ��"<<whitedan<<"��)";
		stream>>tmpstr;
		stream.clear();
		stream.str("");
		str+=tmpstr;
	}
	str+="]";
	
	read_wgs.close();
	return str;
}

string wgsFile::getcommon(char* filepath)
{
	gethandicap(filepath);
	read_wgs.open(filepath,ios::in|ios::binary);
	isfileopen(filepath);
	read_wgs.seekg(103,ios::beg);
	char tmp[4];
	for(int i=0;i<2;i++)
	{
		read_wgs.read(&tmp[i],1);
	}
	komi=(unsigned int)tmp[0]+0.1*(unsigned int)tmp[1];
	
	read_wgs.seekg(101,ios::beg);
	read_wgs.read(&tmp[2],1);
	gobansize=(unsigned int)tmp[2];
	
	stringstream stream;
	string str;
	stream<<"SZ["<<gobansize<<"]HA["<<handicap<<"]KM["<<komi<<"]";
	stream>>str;
	stream.clear();
	stream.str("");

	read_wgs.close();
	return str;
}

string wgsFile::getresult(char* filepath)
{
	read_wgs.open(filepath,ios::in|ios::binary);
	isfileopen(filepath);
	char r[4];
	read_wgs.seekg(105,ios::beg);
	read_wgs.read(&r[0],1);
	int flags=(unsigned int)r[0];
	
	//��Ŀ�����108,109�ֽڣ�����ȡ���룬��2�ټ�0.25
	read_wgs.seekg(108,ios::beg);
	read_wgs.read(&r[1],1);
	read_wgs.read(&r[2],1);

	unsigned int c=(unsigned int)r[1];
	unsigned int d=(unsigned int)r[2];
	unsigned int mask=0x000000ff;
	float point;
	if((d&mask)<128)			//��λû��1��Ϊ����
	{
		point=(d*256+c)/2.0 - 0.25;
	}
	else if((d&mask)>128)
	{
		d=(~d)&mask;
		c=(~c)&mask;
		point=(d*256+c+1)/2.0;
		point=-point-0.25;
		point=-point;
	}	
	
	int rst;
	read_wgs.seekg(106,ios::beg);
	read_wgs.read(&r[3],1);
	rst=(unsigned int)r[3];
	string winner="";
	string loser="";
	if(rst==0)
	{
		winner="��ʤ";
		loser="��";
	}
	else if(rst==1)
	{
		winner="��ʤ";
		loser="��";
	}
	
	string pts;			//point���ַ�����ʽ
	stringstream stream;
	stream<<point;
	stream>>pts;
	stream.clear();
	stream.str("");
	
	switch(flags)
	{
	case 0:
		result="RE[�Ծ����ڽ�����]";
		break;
	case 1:
		result="RE[�����������޺���]";
		break;
	case 2:
		result="RE[����]";
		break;
	case 3:
		result="RE[����]";
		break;
	case 4:
		result="RE[����]";
		break;
	case 5:
		result="RE[��Ϸ����ɢ��ʤ��]";
		break;
	case 6:
		result="RE["+loser+"����/"+winner+"]";
		break;
	case 7:
		result="RE[�ڶԷ�ͬ�����˳�δ�ֳ�ʤ��]";
		break;
	case 8:
		result="RE["+loser+"��ʱ/"+winner+"]";
		break;
	case 9:
		result="RE["+loser+"����/"+winner+"]";
		break;
	case 10:
		result="RE["+winner+pts+"��]";
		break;
	default:
		result="RE[δ�ֳ�ʤ��]";
		break;
	}
	
	read_wgs.close();
	return result;
}