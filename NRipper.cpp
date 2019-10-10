// NRipper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "POD.h"
NArchiveReaderPOD pod;

void ListContents(bool bare=false)
{
	nstring fullpath;
	if (!bare) _tprintf(TEXT("Contents:\n\r"));
	for (unsigned long i=0;i<pod.GetFileCount();i++)
	{
		fullpath = pod.GetFileName(i);
		if (bare)
			_tprintf(TEXT("%08d %s\n\r"), pod.GetFileSize(i), fullpath.c_str());
		else
			_tprintf(TEXT("[%d] %s offs=%d size=%d\n\r"),i, fullpath.c_str(), pod.GetFileOffset(i), pod.GetFileSize(i));
	}
}

void Extract(nstring fname, nstring destdir)
{
	nstring fullpath;
	TCHAR	filename[_MAX_FNAME];
	TCHAR	fileext[_MAX_EXT];
	TCHAR	filedir[_MAX_DIR];
	FILE* file;
	char	*buffer;
	_tprintf(TEXT("Extracting:\n\r"));
	return;
	for (unsigned long i=0;i<pod.GetFileCount();i++)
	{
		fullpath = pod.GetFileName(i);		
		_tprintf(TEXT("[%d] %s offs=%d size=%d\n\r"),i, fullpath.c_str(), pod.GetFileOffset(i), pod.GetFileSize(i));				
		_tsplitpath(fullpath.c_str(),NULL,filedir,filename,fileext);
		if (_tmkdir(filedir)==-1 && errno!=EEXIST)
		{
			_tprintf(TEXT("-- cannot create directory %s\n\r"),filedir);
			continue;
		}
		file = _tfopen(fullpath.c_str(),TEXT("wb"));
		if (!file)
		{
			_tprintf(TEXT("-- cannot create file %s%s\n\r"),filename, fileext);
			continue;
		}
		buffer = new char[pod.GetFileSize(i)];
		pod.ReadFile(i,0,pod.GetFileSize(i),buffer);
		fwrite(buffer,pod.GetFileSize(i),1,file);
		fclose(file);
		delete[] buffer;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc<2){
		_tprintf(TEXT("Terminal Velocity's POD file reader\n\r"));
		_tprintf(TEXT("USAGE: <thisexe> <archivename> <command> <parameters>\n\r"));
		_tprintf(TEXT("  <command> is 'list', 'extract', case-sensitive\n\r"));
		_tprintf(TEXT("  extract USAGE: <thisexe> <archivename> extract {filename|*} <output directory>'\n\r"));
		_tprintf(TEXT("Necro, 25.11.2010\n\r"));
		return 0;
	}
	
	if (!pod.Open(argv[1])) {
		_tprintf(TEXT("FAILED TO OPEN\n\r"));
		return 0;
	}
	if ((argc==3) && (nstring(argv[2]) == nstring(TEXT("list"))))
	{
		ListContents(true);
		return 0;
	}

	if (argc>4)
	{
		if (nstring(argv[2]) == nstring(TEXT("extract")))
		{
			Extract(argv[3],argv[4]);
		}
	}
	//default action:
	ListContents();
	return 0;
}