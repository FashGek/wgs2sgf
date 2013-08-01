/*
	qq�����ļ�(.wgs)����(��UltraEdit��,(xxh,x)��ʾλ��)��
	(00h,a)~(10h,f)��ʾִ��������������22�ֽڣ�
	(30h,a)~(40h,f)��ʾִ��������������22�ֽڣ�
	��������ǰ�������ֽڣ��������qq����Ϣ���ӵ�λ����λ��ֱ��ת��Ϊʮ���ƣ�
		�������qq�ű�����(00h,4)~(00h,9)�ֽ��ڣ���ϢΪ
				E9 03 E4 41 00 00
		���ʾ��qq���� 00 00 41 E4 03 E9 ����ʾ��ʮ���������� 1105462249
	���̴�С ��wgs�ļ��У�����·����Ϣ������ 60h�У�5�У���101�ֽڴ�
	���������� (70h,8)~(70h,9);
	�Ծִ�(70h,a)��ʼ;
	

*/

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<regex>
using namespace std;

void help();				//������Ϣ
void version();				//�汾��Ϣ
string info();				//���������Ϣ
bool is_date(char* date);	//�ж������Ƿ�Ϸ�
int main(int argc, char* argv[])
{
	ifstream inFile;
	ofstream sgffile;
	int lu;		//���̴�С ��wgs�ļ��У�����·����Ϣ������ 60h�У�5�У���101�ֽڴ�
	int filesize;		//�����ļ���С
	char* filepath;	//�����ļ�·��
	char* newfilepath;	//sgf�����ļ�·��
	filepath=new char[200];
	newfilepath=new char[200];
	int display_qq=0;	//Ĭ�ϲ���ʾqq��
	int flags_qq=1;		//��ʾqq�Ŵ�����
	//int flags_qq2=1;	//��ʾqq�Ŵ�����
	if(argc==1)
	{
		help();
		exit(0);
	}
	for(int i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-v")==0)
		{
			version();
			exit(0);
		}
		if(strcmp(argv[i],"-help")==0)
		{
			help();
			exit(0);
		}
		if(strcmp(argv[i],"-qq")==0)
		{
			display_qq=1;
			flags_qq=2;
		}
	}
	
	for(int i=flags_qq;i<argc;i++)
	{
		
		/*���δ������������
		char *date;		//�Ծ�����,��qq������δ�Ķ����������Ч��Ŀǰ��δ�ܴ������ж����Ծ�ʱ����Ϣ����ʽ 20130430-2332 ������-ʱ��
		date=new char[14];
		//if(argv[i]
		memmove(date,argv[i],14);	//������argv[i]�и���13 ���ַ���date�У�������dateָ��
		date[14]='\0';	//��ӽ�����
		if(!is_date(date))
		{
			strcpy(date,"δ֪ʱ��");
		}*/
		
		//��ȡqqΧ���Զ����������еĶ�λ��Ϣ
		regex pattern("([0-9]+)-([0-9]+)(.)(.+)(\\[[0-9]+\\])(.)([^\\s]+)(.)(.+)(\\[[0-9]+\\])");
		cmatch res;
		regex_search(argv[i],res,pattern);
		char *remarks;	//��ע��Ϣ
		remarks=new char[200];
		strcpy(remarks,argv[i]);
		unsigned int len=strlen(remarks);	//�ļ�������
		for(unsigned int k=0;k<len;k++)
		{
			if(remarks[k]=='[')
				remarks[k]='<';
			if(remarks[k]==']')
				remarks[k]='>';
		}
		
		filepath=strcpy(filepath,argv[i]);
		newfilepath=strcpy(newfilepath,filepath);
		newfilepath=strcat(newfilepath,".sgf");
		
		
		inFile.open(filepath);
		if(!inFile)
		{
			cerr<<"wgs2sgf: cannot convert `"<<filepath<<"`: No such file or directory"<<endl;
			exit(0);
		}
		
		//�ļ���С�Ƿ�Ϸ�,qqΧ���ļ���С122�ֽ�
		inFile.seekg(0,ios::end);
		int tmpsize=inFile.tellg();
		if(tmpsize<123)
		{
			cerr<<"wgs2sgf: Error while laod file `"<<filepath<<"`"<<endl;
			exit(0);
		}
		
		inFile.seekg(10,ios::beg);	//�����������������ھ��ļ���ʼ10�ֽڴ�
		char* pb;	//������������
		char* pw;	//������������
		pb=new char[22];
		pw=new char[22];
		inFile.get(pb,22);	//����������Ϣռ��22�ֽ�
		inFile.seekg(58,ios::beg);	//�����������������ھ��ļ���ʼ58�ֽڴ�
		inFile.get(pw,22);	//����������Ϣռ��22�ֽ�
		
		inFile.close();		//��һ�Σ��Ƕ����Ʒ�ʽ�򿪣���ȡ��������
		
		inFile.open(filepath,ios::in|ios::binary);	//������
		if(!inFile)
		{
			cerr<<"wgs2sgf: cannot convert `"<<filepath<<"`: No such file or directory"<<endl;
			exit(0);
		}
		
		inFile.seekg(0,ios::end);
		filesize=inFile.tellg();

		unsigned char* b; 	//���׶������ļ�������
		b=new unsigned char[filesize];
		
		//�磺00 00 41 E4 03 E9��bqq[0]=00,bqq[1]=0,..,�����λ�ú���� bqq[]�ĺͼ�Ϊqq��
		int bqq[6];	//����qq��
		int wqq[6];	//����qq��
		long long  blackqq;	//����qq��,long long �����Ա㱣��10λ�������ϵ�qq��
		long long whiteqq;	//����qq��
		inFile.seekg(4,ios::beg);	//��λ����4�ֽڴ���ȡ����qq��
		for(int i=0;i<6;i++)
		{
			inFile.read((char*)&b[i],sizeof(char));
		}
		int k=0;
		for(int i=5;i>=0;i--)
		{
			bqq[k]=(int)b[i];
			k++;
		}
		/*
			����qq�� ����ĳqq��16������ʽ����Ϊ��qq[6]={00 00 41 E4 03 E9}����qq�ż�����̣� 
				(4*16^7+1*16^6) + (14*16^5+4*16^4)+ ...  =(4*16+1)*16^6 + (14*16+4)*16^4 + ......
			��41��ʾ��ʮ������Ϊ 4*16+1 ,E4��ʾ����Ϊ 14*16+4����ɼ�����Ϊ ��
			qq��= qq[0]*16^10 + qq[1]*16^8 + qq[2]*16^6 + qq[3]*16^4 + +qq[4]*16^2 + qq[5]*16^0;
		*/
		blackqq=bqq[0]*1099511627776+bqq[1]*4294967296+bqq[2]*16777216+bqq[3]*65536+bqq[4]*256+bqq[5];
		inFile.seekg(52,ios::beg);	//��λ����52�ֽڴ���ȡ����qq��
		for(int i=0;i<6;i++)
		{
			inFile.read((char*)&b[i],sizeof(char));
		}
		k=0;
		for(int i=5;i>=0;i--)
		{
			wqq[k]=(int)b[i];
			k++;
		}
		whiteqq=wqq[0]*1099511627776+wqq[1]*4294967296+wqq[2]*16777216+wqq[3]*65536+wqq[4]*256+wqq[5];
		
		//���û��-qqѡ����Ĭ�ϲ���ʾqq��
		if(!display_qq)
		{
			blackqq=whiteqq=0;
			strcpy(remarks,"qq����");
		}
		
		inFile.seekg(101,ios::beg);	//��λ�����ļ���ʼ101�ֽڴ���ȡ���̴�С��Ϣ
		inFile.read((char*)&b[101],sizeof(char));
		lu=(unsigned int)b[101];	//����·��
		inFile.seekg(122,ios::beg);	//ָ�붨λ�����ļ��ײ�122�ֽڴ�
		int shoushu=filesize-122;		//2������

		int* t;		//����������Ϣ
		t=new int[shoushu];
		for(int i=122;i<filesize;i++)
		{
			inFile.read((char*)&b[i],sizeof(char));
			t[i-122]=(unsigned int)b[i];
		}

		sgffile.open(newfilepath,ios::out);	//��д��򿪣�ȷ������д��ʱ������ļ�����
		sgffile.close();					//��Ҫ�ȹرղ����ٴ���׷�ӷ�ʽ��
		sgffile.open(newfilepath,ios::app);	//ios:app �������

		sgffile<<"(;CA[gb2312]SZ["<<lu<<"]"<<"PB["<<pb<<blackqq<<"]PW["<<pw<<whiteqq<<"]AP[MultiGo]GC[ԭʼ����:"<<remarks<<"]\n";
		sgffile<<"BR["<<res[4]<<"]WR["<<res[9]<<"]RE[��"<<res[7]<<"]TM[15����]KM[7.5]GN[QQΧ��]";
		sgffile<<"DT["<<res[1]<<"]"<<info()<<endl;
		
		/*
		  97~122��Ӧ��ĸa~z, �У�32��Ӧi,0��Ӧa,4��Ӧb,8��Ӧc,....;97+t[i]/4����ʵ��0->97,4->98,8->99...��ӳ�䣬
		  �У�0->a,1->b,2->c... t[j]+97ʵ��ӳ�䣻
		*/
		
		int tmp=0;	//sgf�ļ�����

		for(int j=0;j<shoushu;j++)
		{
			if((j/2)%2==0)		//�жϺڰף�������Ϊ�ڣ�ż����Ϊ��
			{
				if(t[j]==1&&t[j+1]==0)		//passһ�֣���wgs��Ӧ16���� 01 00,��sgf�ж�ӦB[]����W[]
				{
					sgffile<<";B[];";
					j++;
				}
				else
				{
					sgffile<<";B["<<((char)(97+t[j]/4))<<((char)(97+t[j+1]))<<"];";
					j++;			//��ȡ�������ֽڣ����1
				}
			}
			else
			{
				if(t[j]==1&&t[j+1]==0)			//passһ�֣���wgs��Ӧ16���� 01 00,��sgf�ж�ӦB[]����W[]
				{
					sgffile<<"W[]";
					j++;
				}
				else
				{
					sgffile<<"W["<<((unsigned char)(97+t[j]/4))<<((unsigned char)(97+t[j+1]))<<"]";
					j++;
				}
			}
			
			tmp++;
			if(tmp%14==0)		//14�ֻ�һ��
				sgffile<<endl;
			
		}

		delete []b;
		delete []t;
		sgffile<<")";
		
		cout<<"ת���ɹ����ѱ���Ϊ: "<<newfilepath<<endl;
		delete [] pb;
		delete [] pw;
		delete [] remarks;

		inFile.close();
		sgffile.close();
	}
	delete [] filepath;
	delete [] newfilepath;
	return 0;
}

void help()
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

void version()
{
	cout<<"\nQQΧ��wgs����תsgf���׹��� V 1.2 Release20130501\n"
		<<"����: annhe ��ϵ��ʽ:admin@annhe.net  ��ҳ: www.annhe.net\n";
}

string info()
{
	string str="PC[QQΧ��]US[wgs2sgf]SO[wgs2sgf]CP[power by wgs2sgf \ncopyright @ annhe]MULTIGOGM[1]";
	return str;
}
/*���������
bool is_date(char* date)
{
	if(date[8]!='-')
		return false;
	for(int i=0;i<8;i++)
	{
		if(int(date[i])>47&&int(date[i])<58)
			return true;
		else
			return false;
	}
	return false;
}*/