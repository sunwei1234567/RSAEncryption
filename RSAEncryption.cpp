// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAEncryption.h"


FString URSAEncryption::RSAEncryptString_ue4(FString cmd)
{
	FString fstr = (TEXT("%s"), *cmd);
	string str = TCHAR_TO_UTF8(*fstr);

	SECURITY_ATTRIBUTES _security = { 0 };
	_security.bInheritHandle = true;
	_security.nLength = sizeof(_security);
	_security.lpSecurityDescriptor = NULL;
	HANDLE hRead = NULL, hWrite = NULL;
	if (!CreatePipe(&hRead, &hWrite, &_security, 0)) //return TEXT("创建管道失败");
	{
		return ("Create Pipe failed!");
	}
	/*cmd命令行转换为Unicode编*/
	int convLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)strlen(str.c_str()), NULL, 0);

	if (convLength <= 0)  //TEXT("字符串转换长度计算出错");
	{
		return ("convLength failed!");
	}

	wstring wCmdLine;
	wCmdLine.resize(convLength + 10);
	convLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)strlen(str.c_str()), &wCmdLine[0], (int)wCmdLine.size());

	if (convLength <= 0)// return TEXT("字符串转换出错");
	{
		return ("char  change failed!");
	}
	/* 创建新进程执行cmd命令并将结果写入到管道 */

	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE; // 隐藏cmd执行的窗口
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;

	if (!CreateProcess(
		NULL,
		&wCmdLine[0],
		//(LPWSTR)wstr.c_str(),
		/*wBuffer*/
		NULL,
		NULL,
		true,
		//FALSE,          // Set handle inheritance to FALSE
		0,
		//0,              // No creation flags
		NULL,
		NULL,
		&si,
		&pi))
	{
		return ("create process error");
	}
	string strRetTmp;

	/* 等待进程执行命令结束 */
	::WaitForSingleObject(pi.hThread, INFINITE);
	::WaitForSingleObject(pi.hProcess, INFINITE);
	/* 从管道中读取数据 */
	DWORD bufferLen = 10240;
	char* buffer = (char*)malloc(10240);
	::memset(buffer, '\0', bufferLen);
	DWORD recLen = 0;
	//strRetTmp = buffer;
	if (!ReadFile(hRead, buffer, bufferLen, &recLen, NULL))//从管道的输出端获取命令行写入的数据；
	{
		return ("read pip failed");
	}
	string ret(buffer);
	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	::free(buffer);
	return ret.c_str();
}

wstring URSAEncryption::StrToWstr(const string& str)
{
	//unsigned len = str.size() * 2;// 预留字节数
	//setlocale(LC_CTYPE, "");     //必须调用此函数
	//wchar_t* p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	//mbstowcs(p, str.c_str(), len);// 转换
	//std::wstring str1(p);
	//delete[] p;// 释放申请的内存
	//return str1;

	wstring result; 
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0); 
	if (len < 0)return result; 
	wchar_t* buffer = new wchar_t[len + 1]; 
	if (buffer == NULL)return result; 
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len); 
	buffer[len] = '\0'; 
	result.append(buffer); 
	delete[] buffer; 
	return result;
	
}

TCHAR* URSAEncryption::CharToTCHAR(const char* ch)
{
	int len;
	TCHAR* tch= TEXT("");
	len = MultiByteToWideChar(CP_ACP, 0, ch, strlen(ch) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, ch, strlen(ch) + 1, tch, len);
	return tch;
}

