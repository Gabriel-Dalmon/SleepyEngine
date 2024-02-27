#pragma once

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include "MathHelper.h"


class D3DUtils
{
public:
	static ID3DBlob* CompileFromFile( const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

};

