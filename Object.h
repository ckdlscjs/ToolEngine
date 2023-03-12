#pragma once
#include "EngineSystem.h"
#include "Mesh.h"
#include "Material.h"
#include "FSelect.h"

struct Transform
{
	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR scale;
	friend std::ostream& operator<<(std::ostream& os, const Transform& transform)
	{
		os << "position:" << XMVectorGetX(transform.position) << " " << XMVectorGetY(transform.position) << " " << XMVectorGetZ(transform.position) << ", ";
		os << "rotation:" << XMVectorGetX(transform.rotation) << " " << XMVectorGetY(transform.rotation) << " " << XMVectorGetZ(transform.rotation) << ", ";
		os << "scale:" << XMVectorGetX(transform.scale) << " " << XMVectorGetY(transform.scale) << " " << XMVectorGetZ(transform.scale);
		return os;
	}
	friend std::istringstream& operator>>(std::istringstream& is, Transform& transform)
	{
		// "pos: x y z, tex: x y, normal: x y z, color: r g b a"와 같은 형태의 문자열에서 필드 값을 추출합니다.
		std::string line;
		std::getline(is, line);

		// pos 값을 추출합니다.
		size_t pos_start = line.find("position:") + strlen("position:");
		size_t pos_end = line.find(",", pos_start);
		std::string pos_str = line.substr(pos_start, pos_end - pos_start);
		std::istringstream pos_stream(pos_str);
		XMFLOAT3 position;
		pos_stream >> position.x >> position.y >> position.z;
		transform.position = XMLoadFloat3(&position);

		// rotation 값을 추출합니다.
		size_t rot_start = line.find("rotation:") + strlen("rotation:");
		size_t rot_end = line.find(",", rot_start);
		std::string rot_str = line.substr(rot_start, rot_end - rot_start);
		std::istringstream rot_stream(rot_str);
		XMFLOAT3 rotation;
		rot_stream >> rotation.x >> rotation.y >> rotation.z;
		transform.rotation = XMLoadFloat3(&rotation);

		// scale 값을 추출합니다.
		size_t scale_start = line.find("scale:") + strlen("scale:");
		size_t scale_end = line.find(",", scale_start);
		std::string normal_str = line.substr(scale_start, scale_end - scale_start);
		std::istringstream scale_stream(normal_str);
		XMFLOAT3 scale;
		scale_stream >> scale.x >> scale.y >> scale.z;
		transform.scale = XMLoadFloat3(&scale);

		return is;
	}

	friend std::stringstream& operator>>(std::stringstream& is, Transform& transform)
	{
		// "pos: x y z, tex: x y, normal: x y z, color: r g b a"와 같은 형태의 문자열에서 필드 값을 추출합니다.
		std::string line;
		std::getline(is, line);

		// pos 값을 추출합니다.
		size_t pos_start = line.find("position:") + strlen("position:");
		size_t pos_end = line.find(",", pos_start);
		std::string pos_str = line.substr(pos_start, pos_end - pos_start);
		std::istringstream pos_stream(pos_str);
		XMFLOAT3 position;
		pos_stream >> position.x >> position.y >> position.z;
		transform.position = XMLoadFloat3(&position);

		// rotation 값을 추출합니다.
		size_t rot_start = line.find("rotation:") + strlen("rotation:");
		size_t rot_end = line.find(",", rot_start);
		std::string rot_str = line.substr(rot_start, rot_end - rot_start);
		std::istringstream rot_stream(rot_str);
		XMFLOAT3 rotation;
		rot_stream >> rotation.x >> rotation.y >> rotation.z;
		transform.rotation = XMLoadFloat3(&rotation);

		// scale 값을 추출합니다.
		size_t scale_start = line.find("scale:") + strlen("scale:");
		size_t scale_end = line.find(",", scale_start);
		std::string normal_str = line.substr(scale_start, scale_end - scale_start);
		std::istringstream scale_stream(normal_str);
		XMFLOAT3 scale;
		scale_stream >> scale.x >> scale.y >> scale.z;
		transform.scale = XMLoadFloat3(&scale);

		return is;
	}
};
//상호작용을 위한 열거
enum class INTERACTIVE_MODE
{
	MODE_NONE = 0,
	MODE_INTERACTIVE,
};

enum class CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK,
	CULL_MODE_NONE,
};


class Object
{
public:
	T_BOX CreateBoundingBox();
	bool Intersect(FSelect& select, float distance);
	std::wstring GetObjectName();
	Transform GetTransform();
	XMVECTOR GetPosition();
	XMVECTOR GetRotation();
	XMVECTOR GetScale();
	void SetTransform(Transform transform);
	void SetMesh(Mesh* pMesh);
	void SetMaterial(Material* pMaterial);
	void SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader);
	void SetShaderName(std::wstring vsName, std::wstring psName);
	void SetConstantData(constant cc);
	void SetCullMode(CULL_MODE mode);
	void SetInteraciveMode(INTERACTIVE_MODE mode);
	void SetDrawMode(DRAW_MODE mode);
	CULL_MODE GetCullMode();
	virtual void Update();
	virtual void Render();
public:
	/*void DeSerialize() const;
	void Serialize(std::ofstream& os) const;*/
public:
	Object(std::wstring szFullPath);
	Object();
	virtual ~Object();

protected:
	T_BOX m_Box;
	std::wstring m_szFullPath;
	Transform m_Transform;
	Mesh* m_pMesh;
	Material* m_pMaterial;
	std::wstring m_szVSName;
	std::wstring m_szPSName;
	VertexShader* m_pVertexShader;
	PixelShader* m_pPixelShader;
	constant m_ConstantData;
	ConstantBuffer* m_pConstantBuffer;
	CULL_MODE m_CullMode = CULL_MODE::CULL_MODE_NONE;
	INTERACTIVE_MODE m_InteractiveMode = INTERACTIVE_MODE::MODE_NONE;
	DRAW_MODE m_DrawMode = DRAW_MODE::MODE_SOLID;

	friend class FQuadTree;
	friend class FNode;
	friend class ToolSystemMap;
	friend class ImguiSystem;
};

