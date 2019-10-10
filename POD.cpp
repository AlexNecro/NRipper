/*
	POD.h
	structures for reading Terminal Velocity's game archive
	Necro, 25.11.2010
*/
#include "POD.h"

const size_t MAXENTRYCOUNT=10000;
const size_t MAXFILENAMELENGTH=32;


NArchiveReaderPOD::NArchiveReaderPOD()
	:header(0), ft(0), handle(0)
{
}

NArchiveReaderPOD::~NArchiveReaderPOD()
{
	Close();
}

unsigned int NArchiveReaderPOD::Open(const nstring& libname)//on success returns true
{
	Close();
	handle = _tfopen(libname.c_str(),TEXT("rb"));
	if (!handle) return 0;
	header = new NFileHeaderPOD;
	size_t read = fread(header,sizeof(header[0]),1,(FILE*)handle);
	if (read!=1) {Close(); return 0;}
	if (header->EntryCount>MAXENTRYCOUNT) {Close(); return 0;}//too big
	ft = new NFileTableEntryPOD[header->EntryCount];
	read = fread(ft,sizeof(ft[0]),header->EntryCount,(FILE*)handle);
	if (read!=header->EntryCount) {Close(); return 0;}//file even damaged or it's not a valid file
	return header->EntryCount;
}

void NArchiveReaderPOD::Close()//frees all associated data
{
	if (handle) fclose((FILE*)handle);
	if (ft) delete[] ft;
	if (header) delete header;
}

unsigned long NArchiveReaderPOD::GetFileCount()
{
	if (!header) return 0;
	return header->EntryCount;
}

nstring NArchiveReaderPOD::GetFileName(unsigned long index)
{
	if (!header) return TEXT("");
	if (index>=header->EntryCount) return TEXT("");
	//name seems to be always zero-terminated. add strncpy if a problem occurs
#ifdef UNICODE	
	TCHAR xstrval[MAXFILENAMELENGTH+2];	
	mbstowcs(xstrval, ft[index].Name, strlen(ft[index].Name) + 1);
	xstrval[strlen(ft[index].Name) + 1]=0;
	return xstrval;
#else
	return ft[index].Name;
#endif

}

unsigned long NArchiveReaderPOD::GetFileSize(unsigned long index)
{
	if (!header) return 0;
	if (index>=header->EntryCount) return 0;
	return ft[index].Size;
}

unsigned long NArchiveReaderPOD::GetFileOffset(unsigned long index)
{
	if (!header) return 0;
	if (index>=header->EntryCount) return 0;
	return ft[index].Offset;
}

size_t NArchiveReaderPOD::ReadFile(unsigned long index, unsigned long offset, unsigned long size, void* buffer)
{
	if (!header) return 0;
	if (index>=header->EntryCount) return 0;
	fseek((FILE*)handle,ft[index].Offset,SEEK_SET);
	return fread(buffer,size,1,(FILE*)handle);
}