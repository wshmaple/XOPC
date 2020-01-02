#pragma once
#include "opcsvrda2/opcsvrda2.h"
#ifndef _MY_OPC_TAGS_H_
#define _MY_OPC_TAGS_H_
typedef struct t_tagitemtest
{
	char			sname[24];	// 名称
	char			sdes[32];	// 描述
	char			sunit[16];	// 单位
	unsigned char	uctype;		// 数据类型
	unsigned char	unwriteable;// 可写标志
	char			sres[6];	// 保留
}T_TAGITEMTEST, *PTAGITEMTEST;	// 测试用的标签定义 sizeof() = 80

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
