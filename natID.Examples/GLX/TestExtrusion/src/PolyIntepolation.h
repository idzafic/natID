#pragma once
#include <glm/common.hpp>
#include <vector>
#include <type_traits>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

static class polyInterpolation {

	enum class controlType { tangent, point, none };


	template<controlType tip, int size>
	struct vektor {
	};



	template<>
	struct vektor<controlType::point, 3> {
		using type = glm::vec3;
		static type getVal(float position) {
			return type(position * position, position, 1);
		}
	};

	template<>
	struct vektor<controlType::point, 4> {
		using type = glm::vec4;
		static type getVal(float position) {
			return type(position * position * position, position * position, position, 1);
		}
	};

	template<>
	struct vektor<controlType::tangent, 3> {
		using type = glm::vec3;
		static type getVal(float position) {
			return type(2 * position, 1, 0);
		}
	};

	template<>
	struct vektor<controlType::tangent, 4> {
		using type = glm::vec4;
		static type getVal(float position) {
			return type(3 * position * position, 2 * position, 1, 0);
		}
	};


	template<controlType c1, controlType c2, controlType c3, controlType c4>
	struct Matrix {
		static glm::mat4 getVal(float val1, float val2, float val3, float val4) {
			return glm::inverse(glm::transpose(glm::mat4(
				vektor<c1, 4>::getVal(val1), vektor<c2, 4>::getVal(val2), vektor<c3, 4>::getVal(val3), vektor<c4, 4>::getVal(val4)
			)));
		}
	};



	template<controlType c1, controlType c2, controlType c3>
	struct Matrix<c1, c2, c3, controlType::none> {
		static glm::mat3 getVal(float val1, float val2, float val3) {
			return glm::inverse(glm::transpose(glm::mat3(
				vektor<c1, 3>::getVal(val1), vektor<c2, 3>::getVal(val2), vektor<c3, 3>::getVal(val3)
			)));
		}
	};


	template<controlType c1, controlType c2, controlType c3, controlType c4, float val1, float val2, float val3, float val4>
	struct MatrixStatic {
		static inline glm::mat4 matrix = glm::inverse(glm::transpose(glm::mat4(
			vektor<c1, 4>::getVal(val1), vektor<c2, 4>::getVal(val2), vektor<c3, 4>::getVal(val3), vektor<c4, 4>::getVal(val4)
		)));
	};


	template<controlType c1, controlType c2, controlType c3, float val1, float val2, float val3, float val4>
	struct MatrixStatic<c1, c2, c3, controlType::none, val1, val2, val3, val4>
	{
		static inline glm::mat3 matrix = glm::inverse(glm::transpose(glm::mat3(
			vektor<c1, 3>::getVal(val1), vektor<c2, 3>::getVal(val2), vektor<c3, 3>::getVal(val3)
		)));
	};


private:

	int pointsPerLength;

	glm::vec3 lastPoint = { 0,0,0 };
	glm::vec3 currentPoint;

	std::vector<glm::vec3> vec;
	bool differentialMode = false;
	glm::vec3 startVal;


	template<typename vType>
	void interpolate(const vType& koeficijentiX, const vType& koeficijentiY, const vType& koeficijentiZ, const glm::vec3& endVal)
	{

		int pointCnt = pointsPerLength * glm::length(endVal - startVal);
		vec.reserve(vec.size() + pointCnt - (differentialMode ? 1 : 0));

		vType point;
		for (int i = differentialMode ? 1 : 0; i < pointCnt; ++i) {
			if constexpr (std::is_same_v<vType, glm::vec3>) {
				point.z = 1;
				point.y = (double)i / (pointCnt - 1);
				point.x = point.y * point.y;
			}
			else if constexpr (std::is_same_v<vType, glm::vec4>) {
				point[3] = 1;
				point.z = (double)i / (pointCnt - 1);
				point.y = point.z * point.z;
				point.x = point.y * point.z;
			}
			else
				static_assert("interpolate works with vec3 or vec4");


			if (differentialMode) {
				currentPoint = { glm::dot(koeficijentiX, point),  glm::dot(koeficijentiY, point), glm::dot(koeficijentiZ, point) };
				vec.emplace_back((currentPoint.x - lastPoint.x), (currentPoint.y - lastPoint.y), (currentPoint.z - lastPoint.z));
				lastPoint = std::move(currentPoint);
			}
			else
				vec.emplace_back(glm::dot(koeficijentiX, point) + startVal.x, glm::dot(koeficijentiY, point), glm::dot(koeficijentiZ, point));


		}


	}



public:

	polyInterpolation(unsigned int pointsPerLength, bool differentialMode, glm::vec3 startingPoint = { 0.0f, 0.0f, 0.0f }) :
		pointsPerLength(pointsPerLength),
		differentialMode(differentialMode),
		startVal(startingPoint)
	{
	}

	polyInterpolation(polyInterpolation& pol)
	{
		pointsPerLength = pol.pointsPerLength;
		vec = pol.vec;
		lastPoint = pol.lastPoint;
		differentialMode = pol.differentialMode;
		startVal = pol.startVal;
	}
	polyInterpolation(polyInterpolation&& pol)
	{
		*this = pol;
	}
	polyInterpolation operator=(polyInterpolation&& pol)
	{
		pointsPerLength = pol.pointsPerLength;
		vec = std::move(pol.vec);
		lastPoint = pol.lastPoint;
		differentialMode = pol.differentialMode;
		startVal = pol.startVal;
	}
	polyInterpolation operator=(polyInterpolation& pol)
	{
		*this = std::move(polyInterpolation(pol));
	}

	std::vector<glm::vec3>& getPoints() {
		return vec;
	}

	void resetPoints(glm::vec3 startingPoint = { 0.0f, 0.0f, 0.0f })
	{
		lastPoint = { 0.0f, 0.0f, 0.0f };
		startVal = startingPoint;
	}


	polyInterpolation& addLines(std::vector<glm::vec3> points) {
		vec.reserve(vec.size() + points.size());
		for (auto& tacke : points)
			vec.emplace_back(tacke);
		startVal = points.back();
		return *this;
	}

	polyInterpolation& changePointsPerLength(unsigned int pointsPerLength) {
		this->pointsPerLength = pointsPerLength;
		return *this;
	}

	struct pointHelper2 {
		glm::vec3 endLocation; //end point of function
		std::pair<glm::vec3, glm::vec3> firstControlVar;

		/*parametric control of value of first control variable(control point or tangent).
		So {{0.5,0.5,0.5}, {2,2,2}} means at around half-point of the curve the value will be {2,2,2}
		the parametric values should be in the range [0,1.0].

		cp in method name is control point so the second vector will define the point your curve will have to pass trough
		tp in method name is tangent point so the second vector will define the tanget your curve will have at some point

		parametic vector (first of the 2) can be (0.25, 0.25, 0.25) and for the second cp (0.85, 0.85, 0.85) for good results

		*/

		std::pair<glm::vec3, glm::vec3> secondControlVar;

		pointHelper2(glm::vec3 endLoc, std::pair<glm::vec3, glm::vec3> firstCtrlVar, std::pair<glm::vec3, glm::vec3> secondCtrlVar)
			: endLocation(endLoc), firstControlVar(firstCtrlVar), secondControlVar(secondCtrlVar) {}
	};

	polyInterpolation& cpcp(std::vector<pointHelper2> points) {
		for (auto& helper : points) {
			interpolate(
				Matrix<controlType::point, controlType::point, controlType::point, controlType::point>::getVal(0.0f, 1.0f, helper.firstControlVar.first.x, helper.secondControlVar.first.x) * glm::vec4(startVal.x, helper.endLocation.x, helper.firstControlVar.second.x, helper.secondControlVar.second.x),
				Matrix<controlType::point, controlType::point, controlType::point, controlType::point>::getVal(0.0f, 1.0f, helper.firstControlVar.first.y, helper.secondControlVar.first.y) * glm::vec4(startVal.y, helper.endLocation.y, helper.firstControlVar.second.y, helper.secondControlVar.second.y),
				Matrix<controlType::point, controlType::point, controlType::point, controlType::point>::getVal(0.0f, 1.0f, helper.firstControlVar.first.z, helper.secondControlVar.first.z) * glm::vec4(startVal.z, helper.endLocation.z, helper.firstControlVar.second.z, helper.secondControlVar.second.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}


	polyInterpolation& cptp(std::vector<pointHelper2> points) {
		for (auto& helper : points) {
			interpolate(
				Matrix<controlType::point, controlType::point, controlType::point, controlType::tangent>::getVal(0.0f, 1.0f, helper.firstControlVar.first.x, helper.secondControlVar.first.x) * glm::vec4(startVal.x, helper.endLocation.x, helper.firstControlVar.second.x, helper.secondControlVar.second.x),
				Matrix<controlType::point, controlType::point, controlType::point, controlType::tangent>::getVal(0.0f, 1.0f, helper.firstControlVar.first.y, helper.secondControlVar.first.y) * glm::vec4(startVal.y, helper.endLocation.y, helper.firstControlVar.second.y, helper.secondControlVar.second.y),
				Matrix<controlType::point, controlType::point, controlType::point, controlType::tangent>::getVal(0.0f, 1.0f, helper.firstControlVar.first.z, helper.secondControlVar.first.z) * glm::vec4(startVal.z, helper.endLocation.z, helper.firstControlVar.second.z, helper.secondControlVar.second.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}

	polyInterpolation& tptp(std::vector<pointHelper2> points) {
		for (auto& helper : points) {
			interpolate(
				Matrix<controlType::point, controlType::point, controlType::tangent, controlType::tangent>::getVal(0.0f, 1.0f, helper.firstControlVar.first.x, helper.secondControlVar.first.x) * glm::vec4(startVal.x, helper.endLocation.x, helper.firstControlVar.second.x, helper.secondControlVar.second.x),
				Matrix<controlType::point, controlType::point, controlType::tangent, controlType::tangent>::getVal(0.0f, 1.0f, helper.firstControlVar.first.y, helper.secondControlVar.first.y) * glm::vec4(startVal.y, helper.endLocation.y, helper.firstControlVar.second.y, helper.secondControlVar.second.y),
				Matrix<controlType::point, controlType::point, controlType::tangent, controlType::tangent>::getVal(0.0f, 1.0f, helper.firstControlVar.first.z, helper.secondControlVar.first.z) * glm::vec4(startVal.z, helper.endLocation.z, helper.firstControlVar.second.z, helper.secondControlVar.second.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}


	struct pointHelperSimple2 {
		glm::vec3 endLocation; //end point of function
		glm::vec3 firstControlVar; //control value point for first control point
		glm::vec3 secondControlVar;

		pointHelperSimple2(glm::vec3 endLoc, glm::vec3 firstCtrlVar, glm::vec3 secondCtrlVar)
			: endLocation(endLoc), firstControlVar(firstCtrlVar), secondControlVar(secondCtrlVar) {}
	};


	template<float paramaterVal1, float paramaterVal2>
	polyInterpolation& tptpStatic(std::vector<pointHelperSimple2> points) {
		for (auto& helper : points) {
			interpolate(
				MatrixStatic<controlType::point, controlType::point, controlType::tangent, controlType::tangent, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.x, helper.endLocation.x, helper.firstControlVar.x, helper.secondControlVar.x),
				MatrixStatic<controlType::point, controlType::point, controlType::tangent, controlType::tangent, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.y, helper.endLocation.y, helper.firstControlVar.y, helper.secondControlVar.y),
				MatrixStatic<controlType::point, controlType::point, controlType::tangent, controlType::tangent, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.z, helper.endLocation.z, helper.firstControlVar.z, helper.secondControlVar.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}



	template<float paramaterVal1, float paramaterVal2>
	polyInterpolation& cptpStatic(std::vector<pointHelperSimple2> points) {
		for (auto& helper : points) {
			interpolate(
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::tangent, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.x, helper.endLocation.x, helper.firstControlVar.x, helper.secondControlVar.x),
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::tangent, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.y, helper.endLocation.y, helper.firstControlVar.y, helper.secondControlVar.y),
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::tangent, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.z, helper.endLocation.z, helper.firstControlVar.z, helper.secondControlVar.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}


	template<float paramaterVal1, float paramaterVal2>
	polyInterpolation& cpcpStatic(std::vector<pointHelperSimple2> points) {
		for (auto& helper : points) {
			interpolate(
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::point, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.x, helper.endLocation.x, helper.firstControlVar.x, helper.secondControlVar.x),
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::point, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.y, helper.endLocation.y, helper.firstControlVar.y, helper.secondControlVar.y),
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::point, 0.0f, 1.0f, paramaterVal1, paramaterVal2>::matrix * glm::vec4(startVal.z, helper.endLocation.z, helper.firstControlVar.z, helper.secondControlVar.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}

	struct pointHelper {
		glm::vec3 endLocation; //end point of function
		std::pair<glm::vec3, glm::vec3> firstControlVar;//control value point for first control point

		pointHelper(glm::vec3 endLoc, std::pair<glm::vec3, glm::vec3> firstCtrlVar)
			: endLocation(endLoc), firstControlVar(firstCtrlVar) {}
	};

	polyInterpolation& cp(std::vector<pointHelper> points) {
		for (auto& helper : points) {
			interpolate(
				Matrix<controlType::point, controlType::point, controlType::point, controlType::none>::getVal(0.0f, 1.0f, helper.firstControlVar.first.x) * glm::vec3(startVal.x, helper.endLocation.x, helper.firstControlVar.second.x),
				Matrix<controlType::point, controlType::point, controlType::point, controlType::none>::getVal(0.0f, 1.0f, helper.firstControlVar.first.y) * glm::vec3(startVal.y, helper.endLocation.y, helper.firstControlVar.second.y),
				Matrix<controlType::point, controlType::point, controlType::point, controlType::none>::getVal(0.0f, 1.0f, helper.firstControlVar.first.z) * glm::vec3(startVal.z, helper.endLocation.z, helper.firstControlVar.second.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}

	polyInterpolation& tp(std::vector<pointHelper> points) {
		for (auto& helper : points) {
			interpolate(
				Matrix<controlType::point, controlType::point, controlType::tangent, controlType::none>::getVal(0.0f, 1.0f, helper.firstControlVar.first.x) * glm::vec3(startVal.x, helper.endLocation.x, helper.firstControlVar.second.x),
				Matrix<controlType::point, controlType::point, controlType::tangent, controlType::none>::getVal(0.0f, 1.0f, helper.firstControlVar.first.y) * glm::vec3(startVal.y, helper.endLocation.y, helper.firstControlVar.second.y),
				Matrix<controlType::point, controlType::point, controlType::tangent, controlType::none>::getVal(0.0f, 1.0f, helper.firstControlVar.first.z) * glm::vec3(startVal.z, helper.endLocation.z, helper.firstControlVar.second.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}


	struct pointHelperSimple {
		glm::vec3 endLocation; //end point of function
		glm::vec3 firstControlVar;//control value point for first control point

		pointHelperSimple(glm::vec3 endLoc, glm::vec3 firstCtrlVar)
			: endLocation(endLoc), firstControlVar(firstCtrlVar) {}
	};



	template<float paramaterVal1>
	polyInterpolation& cpStatic(std::vector<pointHelperSimple> points) {
		for (auto& helper : points) {
			interpolate(
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::none, 0.0f, 1.0f, paramaterVal1, 0.0f>::matrix * glm::vec3(startVal.x, helper.endLocation.x, helper.firstControlVar.x),
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::none, 0.0f, 1.0f, paramaterVal1, 0.0f>::matrix * glm::vec3(startVal.y, helper.endLocation.y, helper.firstControlVar.y),
				MatrixStatic<controlType::point, controlType::point, controlType::point, controlType::none, 0.0f, 1.0f, paramaterVal1, 0.0f>::matrix * glm::vec3(startVal.z, helper.endLocation.z, helper.firstControlVar.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}


	template<float paramaterVal1>
	polyInterpolation& tpStatic(std::vector<pointHelperSimple> points) {
		for (auto& helper : points) {
			interpolate(
				MatrixStatic<controlType::point, controlType::point, controlType::tangent, controlType::none, 0.0f, 1.0f, paramaterVal1, 0.0f>::matrix * glm::vec3(startVal.x, helper.endLocation.x, helper.firstControlVar.x),
				MatrixStatic<controlType::point, controlType::point, controlType::tangent, controlType::none, 0.0f, 1.0f, paramaterVal1, 0.0f>::matrix * glm::vec3(startVal.y, helper.endLocation.y, helper.firstControlVar.y),
				MatrixStatic<controlType::point, controlType::point, controlType::tangent, controlType::none, 0.0f, 1.0f, paramaterVal1, 0.0f>::matrix * glm::vec3(startVal.z, helper.endLocation.z, helper.firstControlVar.z),
				helper.endLocation
			);
			startVal = helper.endLocation;
		}
		return *this;
	}


	polyInterpolation& spline(glm::vec3 startTangent, std::vector<pointHelperSimple> controlPointAndTangent) // {control point, tangent at control point}
	{
		glm::vec3 pastTangent(0.0f, 0.0f, 0.0f);
		for (auto it = controlPointAndTangent.begin(); it != controlPointAndTangent.end(); ++it) {
			auto& helper = *it;		
			if(it == controlPointAndTangent.begin())
				tptpStatic<0.0f, 1.0f>({ {helper.endLocation, startTangent, helper.firstControlVar} });
			else
				tptpStatic<0.0f, 1.0f>({ {helper.endLocation, pastTangent, helper.firstControlVar} });

			pastTangent = helper.firstControlVar;
		}

		return *this;
	}

};