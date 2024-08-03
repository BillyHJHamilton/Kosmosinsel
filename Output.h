#pragma once

class CColourScope
{
public:
	CColourScope(const char* code);
	~CColourScope();
};

class CDarkGreyScope : public CColourScope
{
public:
	CDarkGreyScope() : CColourScope("90") {}
};

class CRedScope : public CColourScope
{
public:
	CRedScope() : CColourScope("91") {}
};

class CGreenScope : public CColourScope
{
public:
	CGreenScope() : CColourScope("92") {}
};

class CYellowScope : public CColourScope
{
public:
	CYellowScope() : CColourScope("93") {}
};

class CMagentaScope : public CColourScope
{
public:
	CMagentaScope() : CColourScope("95") {}
};
