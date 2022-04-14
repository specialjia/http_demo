// http_server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
 
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/AbstractHTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/StreamCopier.h"
#include "Poco\Path.h"
#include <sstream>
#include <fstream>

using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::AbstractHTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::ServerSocket;
using Poco::Net::SecureServerSocket;
using Poco::StreamCopier;
using Poco::Path;


class EchoBodyRequestHandler : public HTTPRequestHandler
{
public:
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		if (request.getChunkedTransferEncoding())
			response.setChunkedTransferEncoding(true);
		else if (request.getContentLength() != HTTPMessage::UNKNOWN_CONTENT_LENGTH)
			response.setContentLength(request.getContentLength());

		response.setContentType(request.getContentType());

		std::istream& istr = request.stream();
		std::string str;
		istr >> str;
		//std::cout << str << std::endl;

		std::ostream& ostr = response.send();
		str.clear();
		 
		 

		StreamCopier::copyStream(istr, ostr);
	}
};

class EchoHeaderRequestHandler : public HTTPRequestHandler
{
public:
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		std::ostringstream osstr;
		request.write(osstr);
		int n = (int)osstr.str().length();
		response.setContentLength(n);
		std::ostream& ostr = response.send();
		if (request.getMethod() != HTTPRequest::HTTP_HEAD)
			request.write(ostr);
	}
};

class RedirectRequestHandler : public AbstractHTTPRequestHandler
{
public:
	void run()
	{
		response().redirect("http://www.appinf.com/");
	}
};

class AuthRequestHandler : public HTTPRequestHandler
{
public:
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.requireAuthentication("/auth");
		response.send();
	}
};

class BufferRequestHandler : public HTTPRequestHandler
{
public:
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		std::string data("xxxxxxxxxx");
		response.sendBuffer(data.data(), data.length());
	}
};

class UploadFileRequestHandler : public HTTPRequestHandler
{
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		request.getContentLength();
		request.getContentType();
		request.getHost();
		request.getMethod();
		request.splitElements;

		
		std::string filename = request.get("filename");
		std::cout << "filename" <<filename << std::endl;
		request.clientAddress();
		
		std::istream& istr = request.stream();
		std::ofstream file;
		std::string path;
		path = "d:/";
		path.append(filename);
		file.open(path, std::ios::trunc);
		
		/*std::string str;
		istr >> str;
		std::cout << str.length() << str << std::endl;*/
		StreamCopier::copyStream(istr, file);
		file.close();

		std::string data("ok");
		response.sendBuffer(data.data(), data.length());
	}
};

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		if (request.getURI() == "/echoBody")
		{
			printf("/echoBody");
			return new EchoBodyRequestHandler;
		}			
		else if (request.getURI() == "/echoHeader")
		{
			printf("/echoHeader");
			return new EchoHeaderRequestHandler;
		}
			
		else if (request.getURI() == "/redirect")
		{
			printf("/redirect");
			return new RedirectRequestHandler();
		}
		else if (request.getURI() == "/auth")
		{
			printf("/auth");
			return new AuthRequestHandler();
		}
		else if (request.getURI() == "/auth")
		{
			printf("/auth");
			return new BufferRequestHandler();
		}
		else if (request.getURI() == "/uploadFile")
		{
			printf("/uploadFile");
			return new UploadFileRequestHandler;
		}
		else if (request.getURI() == "/downloadFile")
		{

		}else
			return 0;
	}
};


class TestRequestHandler : public HTTPRequestHandler
	/// Return a HTML document with the current date and time.
{
public:
	TestRequestHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType(request.getContentType());
		std::ostream& ostr = response.send();
		Poco::StreamCopier::copyStream(request.stream(), ostr);
	}

};

class TestRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	TestRequestHandlerFactory()
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		return new TestRequestHandler();
	}
};

int main()
{
    std::cout << "http server\n";
	/*SecureServerSocket svs(32322);
	HTTPServer srv(new TestRequestHandlerFactory(), svs, new HTTPServerParams());
	srv.start();*/
	 
    HTTPServer srv(new RequestHandlerFactory, 8008);
    srv.start();
	while (true)
	{
		Sleep(9999);
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
