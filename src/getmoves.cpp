#include "wgs.h"

string wgsFile::getmoves(char* filepath)
{
	gethandicap(filepath);
	int start=0;				//��־�Ƿ��������� ������������У������������
	moves="";
	if(handicap>0)
	{
		start=1;
		switch(handicap)
		{
		case 2:
			moves="AB[pd][dp]\n";							//��2��(λ���� ���Ϻ�����)
			break;
		case 3:
			moves="AB[dd][pd][dp]\n";						//��3��
			break;
		case 4:
			moves="AB[dd][pd][dp][pp]\n";					//��4��
			break;
		case 5:
			moves="AB[dd][pd][dp][pp][jj]\n";				//��5��
			break;
		case 6:
			moves="AB[dd][pd][dp][pp][dj][pj]\n";			//��6��
			break;
		case 7:
			moves="AB[dd][pd][dp][pp][dj][pj][jj]\n";		//��7��
			break;
		case 8:
			moves="AB[dd][pd][dp][pp][jd][dj][pj][jp]\n";	//��8��
			break;
		case 9:
			moves="AB[dd][pd][dp][pp][jd][dj][pj][jp][jj]\n";	//��9��
			break;
		default:
			moves="";
			break;
		}
	}
	
	read_wgs.open(filepath,ios::in|ios::binary);
	isfileopen(filepath);
	int filesize;
	read_wgs.seekg(0,ios::end);
	filesize=read_wgs.tellg();		//�ļ���С
	read_wgs.seekg(122,ios::beg);	//ָ�붨λ�����ļ��ײ�122�ֽڴ�(�Ծֿ�ʼ��)
	int* tmp=new int[filesize-122];		//������Ϣ
	
	char* b=new char[filesize];		//��ȡÿһ���� ��ʱ����
	for(int i=122;i<filesize;i++)
	{
		read_wgs.read(&b[i],1);
		tmp[i-122]=(unsigned int)b[i];
	}
	
	
	
	int newline=0;	//sgf�ļ�����
	stringstream stream;				//����ת��
	string tmpstr="";					//��������ת������ʱ����
	
	/*
	 * 97~122��Ӧ��ĸa~z, �У�32��Ӧi,0��Ӧa,4��Ӧb,8��Ӧc,....;97+t[i]/4����ʵ��0->97,4->98,8->99...��ӳ�䣬
	 * �У�0->a,1->b,2->c... t[j]+97ʵ��ӳ�䣻
	 */
	
	for(int j=0;j<filesize-122;j++)
	{
		if((j/2+start)%2==0)		//�жϺڰף�������Ϊ�ף�ż����Ϊ�� start��־������
		{
			if(tmp[j]==1&&tmp[j+1]==0)		//passһ�֣���wgs��Ӧ16���� 01 00,��sgf�ж�ӦB[]����W[]
			{
				tmpstr=";B[]";
				moves+=tmpstr;
				tmpstr="";
				j++;
			}
			else
			{
				stream<<";B["<<((char)(97+tmp[j]/4))<<((char)(97+tmp[j+1]))<<"]";
				stream>>tmpstr;
				moves+=tmpstr;
				stream.clear();			//���stream���� ����ÿ�ζ�ȡ�Ķ�һ��
				stream.str("");
				j++;			//��ȡ�������ֽڣ����1
			}
		}
		else
		{
			if(tmp[j]==1&&tmp[j+1]==0)			//passһ�֣���wgs��Ӧ16���� 01 00,��sgf�ж�ӦB[]����W[]
			{
				tmpstr=";W[]";
				moves+=tmpstr;
				tmpstr="";
				j++;
			}
			else
			{
				stream<<";W["<<((unsigned char)(97+tmp[j]/4))<<((unsigned char)(97+tmp[j+1]))<<"]";
				stream>>tmpstr;
				moves+=tmpstr;
				stream.clear();
				stream.str("");
				j++;
			}
		}
		
		newline++;
		if(newline%14==0)		//14�ֻ�һ��
			moves+="\n";
		
	}
	moves+=")";
	delete [] b;
	delete [] tmp;
	read_wgs.close();
	return moves;
}

