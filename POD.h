/*
	POD.h
	structures for reading Terminal Velocity's game archive
	Necro, 25.11.2010
*/
#include "stdafx.h"

struct NFileHeaderPOD //84 bytes
{
	unsigned long	EntryCount;
	char			UnknownData[80];
};//NFileHeaderPOD

struct NFileTableEntryPOD //40 bytes
{
	char			Name[32];
	unsigned long	Size;
	unsigned long	Offset;
};//struct NFileTableEntryPOD

class NArchiveReaderPOD
{
public:
	NArchiveReaderPOD();
	~NArchiveReaderPOD();
	unsigned int Open(const nstring& libname);//returns number of files inside
	void Close();//frees all associated data
	unsigned long GetFileCount();
	nstring GetFileName(unsigned long index);
	unsigned long GetFileSize(unsigned long index);
	unsigned long GetFileOffset(unsigned long index);
	size_t ReadFile(unsigned long index, unsigned long offset, unsigned long size, void* buffer);//returns number of bytes read

protected:
	NFileHeaderPOD		*header;
	NFileTableEntryPOD	*ft;
	void* handle;
};//class NArchiveReaderPOD