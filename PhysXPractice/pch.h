#pragma once

const int gWinSizeX = 800;
const int gWinSizeY = 600;

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <queue>

#include <memory>

// WIN
#include <windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <Directxtk/DDSTextureLoader.h>
#include <DirectXTex.h>
#include <Directxtk/WICTextureLoader.h>
#include <wrl/client.h> // For Microsoft::WRL::ComPtr

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

//assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post-processing flags

//simple math
#include "../Libraries/Include/SimpleMath/SimpleMath.h"

#include "Directxtk/VertexTypes.h"
#include "Directxtk/Effects.h"
#include "Directxtk/CommonStates.h"

//Physics
#include <physx/PxPhysicsAPI.h>
#include <physx/PxPhysics.h>
using namespace physx;

using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;
using Color = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;

#include "Struct.h"
#include "Game.h"
#include "D3DRenderer.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "PhysicsManager.h"