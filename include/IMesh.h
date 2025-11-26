#pragma once

#include "basicIncludes.h"

class IMesh
{
public:
	virtual int buffCount() const { return NULL; }
	virtual const void* buffData(int buffIndex) const { return NULL; }
	virtual size_t buffSize(int buffIndex) const { return NULL; }
	virtual int attrCount(int buffIndex) const { return NULL; }
	virtual uint32_t attrType(int buffIndex, int attrIndex) const { return NULL; }
	virtual int attrSize(int buffIndex, int attrIndex) const { return NULL; }
	virtual int attrStride(int buffIndex, int attrIndex) const { return NULL; }
	virtual size_t attrOffset(int buffIndex, int attrIndex) const { return NULL; }
	virtual bool attrIsInteger(int buffIndex, int attrIndex) const { return false; }
	virtual size_t vertCount() const { return NULL; }
	virtual const void* indexBuffData() const { return NULL; }
	virtual size_t indexBuffSize() const { return NULL; }
};
