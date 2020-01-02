#pragma once
#include "opcsvrda2/opcsvrda2.h"
#ifndef _MY_OPC_TAGS_H_
#define _MY_OPC_TAGS_H_
typedef struct t_tagitemtest
{
	char			sname[24];	// ����
	char			sdes[32];	// ����
	char			sunit[16];	// ��λ
	unsigned char	uctype;		// ��������
	unsigned char	unwriteable;// ��д��־
	char			sres[6];	// ����
}T_TAGITEMTEST, *PTAGITEMTEST;	// �����õı�ǩ���� sizeof() = 80

class MyOpcTags
{
public:
	MyOpcTags();
	~MyOpcTags();

private:

};

MyOpcTags::MyOpcTags()
{
	DA2_AddTag("", "", "", ODT_F64, OTEM_WRITE);
}

MyOpcTags::~MyOpcTags()
{
}

#endif
