#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Transform 
{
public:
	Transform();
	~Transform();

// MEMBER VARIABLES
public:
	//scale
	XMFLOAT4 m_scaleQuat;
	XMFLOAT4X4 m_scaleMatrix;

	//orientation
	XMFLOAT3 m_dir;
	XMFLOAT3 m_right;
	XMFLOAT3 m_up;
	XMFLOAT4 m_currentRotateQuat;
	XMFLOAT4X4 m_currentRotateMatrix;

	//position
	XMFLOAT4 m_positionQuat;
	XMFLOAT4X4 m_positionMatrix;

	//"merged" matrix
	XMFLOAT4X4 m_transformMatrix;

// METHODES
public:
	void Identity();
	void Rotate(float yaw, float pitch, float roll);
	void Update();
};