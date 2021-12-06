#pragma once
#include "GL/glm/vec3.hpp"
#include "GL/glm/mat3x3.hpp"
#include <vector>
//class Transform2D
//{
//private:
//	glm::mat3 result = glm::mat3(1);
//
//	glm::mat3 aspect = glm::mat3(1);
//	glm::mat3 scale = glm::mat3(1);
//	glm::mat3 rotate = glm::mat3(1);
//	glm::mat3 translate = glm::mat3(1);
//
//	void CalculateResult();
//public:
//	void SetScale(const glm::mat3&);
//	void SetRotation(const glm::mat3&);
//	void SetTranslation(const glm::mat3&);
//	void SetAspectRatio(float ar);
//
//	float GetAspectRatio();
//
//	glm::mat3& GetMatrix();
//	const glm::mat3& GetScaleMat() const { return scale; }
//	const glm::mat3& GetTranslateMat() const { return //translate; }
//	const glm::mat3& GetRotation() const { return rotate; }
//	const glm::mat3& GetAspectMat() const { return aspect; }
//};

class Transform
{
protected:
	glm::mat4 matrix = glm::mat4(1);
public:
	virtual const glm::mat4& GetMatrix() { return matrix; }
};

class ScaleTransform : public Transform
{
private:
	glm::vec3 scale;
	inline void UpdateMat() { matrix[0][0] = scale.x, matrix[1][1] = scale.y; matrix[2][2] = scale.z; }
public:
	ScaleTransform(float scaleX, float scaleY, float scaleZ)
		:scale(scaleX, scaleY, scaleZ) {
		UpdateMat();
	}
	ScaleTransform(const glm::vec3& scale)
		:scale(scale) {
		UpdateMat();
	}
	const glm::vec3& GetScale() { return scale; }
	void SetScale(const glm::vec3& val) { scale = val; UpdateMat(); }

	float GetScaleX() { return scale.x; }
	float GetScaleY() { return scale.y; }
	float GetScaleZ() { return scale.z; }

	void SetScaleX(float val) { scale.x = val; UpdateMat(); }
	void SetScaleY(float val) { scale.y = val; UpdateMat(); }
	void SetScaleZ(float val) { scale.z = val; UpdateMat(); }
};

class TranslateTransform : public Transform
{
private:
	glm::vec3 offset;
	inline void UpdateMat() { matrix[3][0] = offset.x, matrix[3][1] = offset.y; matrix[3][2] = offset.z; }
public:
	TranslateTransform(const glm::vec3& offset)
		:offset(offset) {
		UpdateMat();
	}
	TranslateTransform(float offsetX, float offsetY, float offsetZ)
		: offset(offsetX, offsetY, offsetZ) {
		UpdateMat();
	}
	void SetOffset(const glm::vec3& offset) { this->offset = offset; UpdateMat(); }
	const glm::vec3& GetOffset() { return offset; }

	float GetOffsetX() { return offset.x; }
	float GetOffsetY() { return offset.y; }
	float GetOffsetZ() { return offset.z; }

	void  SetOffsetX(float val) { offset.x = val; UpdateMat(); }
	void  SetOffsetY(float val) { offset.y = val; UpdateMat(); }
	void  SetOffsetZ(float val) { offset.z = val; UpdateMat(); }
};

class MatrixTransform : public Transform
{
public:
	MatrixTransform(const glm::mat4& mat) { matrix = mat; }

	glm::mat4& Matrix() { return matrix; }
	const glm::mat4& GetMatrix() { return matrix; }
};

class TransformGroup : public Transform
{
	void UpdateMat()
	{
		if (transforms.size() == 0)
			return;
		glm::mat4 rez = transforms[0].GetMatrix();
		for (int i = 1; i < transforms.size(); i++)
			rez = transforms[i].GetMatrix() * rez;
		matrix = std::move(rez);
		isUpdated = true;
	}
	std::vector<Transform> transforms = std::vector<Transform>(0);
	bool isUpdated = true;
public:
	/// Calculates the entire stack and puts the result into transforms[0]
	void Collapse() {
		if (isUpdated == false)
			UpdateMat();
		transforms.clear();
		transforms.push_back(MatrixTransform(matrix));
	}
	const std::vector<Transform>& ReadTransforms() { return transforms; }
	std::vector<Transform>& Transforms() { isUpdated = false; return transforms; }
	const glm::mat4& GetMatrix() {
		if (isUpdated == false)
			UpdateMat();
		return matrix;
	}
};