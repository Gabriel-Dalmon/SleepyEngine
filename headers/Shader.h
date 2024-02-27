#pragma once
#include "pch.h"
#include <string>
#include "d3dcommon.h"

//struct Vertex {
//	XMFLOAT3 Pos;
//	XMFLOAT4 Color;
//};

class Shader
{
public:
	Shader();
	~Shader() {};

	// INIT
	virtual void Init() {};

	void Release();

private:
	void CompileVS(std::wstring fileName);
	void CompilePS();

	ID3DBlob* m_pVSByteCode;
	ID3DBlob* m_pPSByteCode;
};

