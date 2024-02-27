#include "Shader.h"
#include "D3DUtils.h"

using namespace DirectX;


void Shader::Release()
{
	delete m_pVSByteCode;
	delete m_pPSByteCode;
	m_pVSByteCode = nullptr;
	m_pPSByteCode = nullptr; 
}

void Shader::CompileVS(std::wstring fileName)  
{
	m_pVSByteCode = nullptr;

	D3DUtils::CompileFromFile(fileName, 
		nullptr, "VS", "vs_5_0");
}

void Shader::CompilePS()
{
	m_pVSByteCode = nullptr;

	D3DUtils::CompileFromFile(L"Shaders\\color.hlsl",
		nullptr, "PS", "vs_5_0");
}
