管理员权限
安装注册：
ZCOpcServer.exe  -reg
卸载：
ZCOpcServer.exe  -unreg


协议方式：gsoap 端口：17890
上位机发送消息 JSON 格式: 注意：区分大小写
{
	"key": "setopctag", //命令名称
	"data": [{
		"o_key": "key_hello_world", //<80字符 名称
		"o_type": "11", //<2字符 [1.char,5.int,9.float,11.string] 数据类型
		"o_writeable": "0", //<2字符[0.不可写,1.可写] 可写标志
		"o_val": "helloworld", //<32字节 值
		"o_descripe": "helloworld", //<80字符 描述
		"o_unit": "%" //<16字符 单位	
	}, {
		"o_key": "key_jone", //<80字符 名称
		"o_type": "11", //<2字符 [1.char,5.int,9.float,11.string] 数据类型
		"o_val": "aksdjflajdfjkjsldafj" //<32字节 值
	}]
}

测试用：
{\"key\": \"setopctag\",\"data\": [{\"o_key\": \"key_hello_world\",\"o_type\": \"11\", \"o_writeable\": \"0\", \"o_val\": \"helloworld\", \"o_descripe\": \"helloworld\", \"o_unit\": \"%\" }, {\"o_key\": \"key_jone\",\"o_type\": \"11\", \"o_val\": \"aksdjflajdfjkjsldafj\", \"o_descripe\": \"helloworld\"}]}