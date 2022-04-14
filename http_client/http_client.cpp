// http_client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
 
#include "CppUnit/TestCase.h"
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
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
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
using Poco::StreamCopier;

void uploadFile(const char* uploadLink, const char* filePath, const char* filename)
{
    try
    {
        //Prepare request
        Poco::URI uri(uploadLink);

        std::cout << "host:" << uri.getHost() << std::endl;
        std::cout << "port:" << uri.getPort() << std::endl;
        Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

        session.setKeepAlive(true);

        // prepare path
        std::string path(uri.getPathAndQuery());
        if (path.empty())
        {
            path = "/";
        }

        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPMessage::HTTP_1_1);

        // Set headers
        // Set Multipart type and some boundary value 
        req.setContentType(" application/octet-stream");
       
        req.setHost(uri.getHost());
        
        req.add("filename", filename);
        
        // Read File Data in binary
        std::ifstream file(filePath, std::ios::binary);
      
        std::ostringstream ostrm;
        ostrm << file.rdbuf();
        
        std::string reqBody;
       
        
        
        reqBody.append(ostrm.str());
        
       
        req.setContentLength(reqBody.length());

        req.setKeepAlive(true);

        // sends request, returns open stream
        std::ostream& myOStream = session.sendRequest(req);
        // sends the body
        myOStream << reqBody;

        Poco::Net::HTTPResponse res;

        std::istream& rs = session.receiveResponse(res);

        //Get status code
        int statusCode = (int)res.getStatus();

        //Get status
        std::string status = res.getReason();

        std::string response;
        while (rs)
        {
            response.push_back(char(rs.get()));
        }
        std::cout << "\nStatusCode: " << statusCode << "\nStatus: " << status << "\nResponse: " << response;
    }
    catch (Poco::Exception& exception)
    {
        //Set Response for Exception
        std::cout << "\nException occurred while uploading: " << exception.displayText();
    }
}
int main()
{
    std::cout << "http client!\n";
    uploadFile("http://127.0.0.1:8008/uploadFile","d:\\ThirdParty.rar","ThirdParty1.rar");
	 system("pause");

	 
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
