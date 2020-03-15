// TEAcipher.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "string"
#include "fstream"
#include "ostream"
using namespace std;

int main(int argc, char* argv[])
{
	void encrypt(char [] ,unsigned int []);
	void decrypt(char [] ,unsigned int []);
	if(argc!=5){
		cout<<"usegethod:TEAcipher e(encrypt) password sourcefilenmae outfilename"<<endl;
		cout<<"usegethod:TEAcipher d(decrypt) password sourcefilenmae outfilename"<<endl;
		return 0;
	}
	int j,t;
	//密钥存储到整形数组
	unsigned int password[4]={0};
	for(int i=0;i<4;i++){
		j=i<<1;
		t=j+4;
		while(j<t){
		password[i]=(password[i]<<8)^argv[2][j];
		j++;
		}
	}

	//以只读模式打开源文件
	ifstream fpin;
	fpin.open(argv[3],ios::in|ios::binary);		

	//以写模式打开输出文件
	ofstream fpout;
	fpout.open(argv[4],ios::out|ios::binary);

	//源文件和输出文件都成功打开
	if(fpin.is_open()&&fpout.is_open()){
		char msg[8]={'\0'};
		while(!fpin.eof()){
			for(int i=0;i<8;i++){
				msg[i]='\0';
			}
			fpin.read(msg,8);
			    if(msg==NULL) break;
			if((argv[1][0]=='e')||(argv[1][0]=='E'))
				encrypt(msg,password);
			else
				decrypt(msg,password);
			fpout.write(msg,sizeof(msg));		
		}
	fpin.close();
	fpout.close();
	}
	return 0;
}

void encrypt(char msg[8],unsigned int password[4]){
	unsigned sum=0;
	unsigned int delta=0x9e3779b9;
	unsigned int left=0;
	unsigned int right=0;
	int n=32;
	for(int i=0;i<4;i++){
		left=(left<<8)^msg[i];
		right=(right<<8)^msg[i+4];
	}
	while(n--){
		sum+=delta;
		left+=((right<<4)+password[0])^(right+sum)^((right>>5)+password[1]);
		right+=((left<<4)+password[2])^(left+sum)^((left>>5)+password[3]);
	}
	for(int i=3;i>=0;i--){
		msg[i]=left&0xff;
		left=left>>8;
		msg[i+4]=right&0xff;
		right=right>>8;
	}
	return;
}

void decrypt(char msg[],unsigned int password[]){
	unsigned int delta=0x9e3779b9;
	unsigned int sum=delta<<5;
	unsigned int left=0;
	unsigned int right=0;
	int n=1;
	for(int i=0;i<4;i++){
		left=(left<<8)^msg[i];
		right=(right<<8)^msg[i+4];
	}
	while(n--){
		right-=((left<<4)+password[2])^(left+sum)^((left>>5)+password[3]);
		left-=((right<<4)+password[0])^(right+sum)^((right>>5)+password[1]);
		sum-=delta;
	}
	for(int i=3;i>=0;i--){
		msg[i]=left&0xff;
		left=left>>8;
		msg[i+4]=right&0xff;
		right=right>>8;
	}
	for(int i=0;i<8;i++)
	{
		cout<<msg[i];
	}
	return;
}