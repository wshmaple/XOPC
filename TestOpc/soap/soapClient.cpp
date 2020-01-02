/* soapClient.cpp
   Generated by gSOAP 2.8.69 for add.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapClient.cpp ver 2.8.69 2019-04-03 07:08:40 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__GetInfo(struct soap *soap, const char *soap_endpoint, const char *soap_action, const std::string& ConfigInfo, std::string &result)
{	struct ns__GetInfo soap_tmp_ns__GetInfo;
	struct ns__GetInfoResponse *soap_tmp_ns__GetInfoResponse;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://localhost:7890";
	soap_tmp_ns__GetInfo.ConfigInfo = ConfigInfo;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__GetInfo(soap, &soap_tmp_ns__GetInfo);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__GetInfo(soap, &soap_tmp_ns__GetInfo, "ns:GetInfo", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__GetInfo(soap, &soap_tmp_ns__GetInfo, "ns:GetInfo", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!static_cast<std::string*>(&result)) // NULL ref?
		return soap_closesock(soap);
	soap_default_std__string(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns__GetInfoResponse = soap_get_ns__GetInfoResponse(soap, NULL, "", NULL);
	if (!soap_tmp_ns__GetInfoResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns__GetInfoResponse->result;
	return soap_closesock(soap);
}

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapClient.cpp */
