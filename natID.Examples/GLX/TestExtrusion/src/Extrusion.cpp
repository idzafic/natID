#pragma once
#include "Extrusion.h"
#include "cnt/PushBackVector.h"


void Extrusion::AddIndices(std::vector<uint32_t>& indices, int start)
{
	int poc = indices.size();
	for (int i = 0; i < 2 * pointCnt; i++) {
		//if (i == 2 * pointCnt - 1) continue;
		if (i == 2 * pointCnt - 1) {
			if (addNormals) {
				//indices.push_back(4 * (1 + i / 2) + 2);
				//indices.push_back(4 * i / 2 + 3);
				//indices.push_back(4 * ((i / 2 + 1 + pointCnt)  % (2 * pointCnt)));
				indices.push_back(2);
				indices.push_back(4 * pointCnt + 3);
				indices.push_back(4 * (pointCnt - 1));

			}
			else {
				indices.push_back(1 + i / 2);
				indices.push_back(i / 2);
				indices.push_back((i / 2 + 1 + pointCnt) % (2 * pointCnt));
			}
		}
		else if (i % 2 == 0) {
			if (addNormals) {
				indices.push_back(4 * (i / 2 + 1));
				indices.push_back(4 * (pointCnt + i / 2) + 3);
				indices.push_back(4 * i / 2 + 1);
			}
			else {
				indices.push_back(i / 2 + 1);
				indices.push_back(pointCnt + i / 2);
				indices.push_back(i / 2);
			}
		}
		else {
			if (addNormals) {
				indices.push_back(4 * ((i / 2 + 1 + pointCnt) % (2 * pointCnt)) + 2);
				indices.push_back(4 * (i / 2 + pointCnt) + 2);
				indices.push_back(4 * (1 + i / 2));
			}
			else {
				indices.push_back((i / 2 + 1 + pointCnt) % (2 * pointCnt));
				indices.push_back(i / 2 + pointCnt);
				indices.push_back(1 + i / 2);
			}
		}
		indices[poc + 3 * i] += start;
		indices[poc + 3 * i + 1] += start;
		indices[poc + 3 * i + 2] += start;
	}
	if (!outsideVisible) {
		std::reverse(indices.begin() + poc, indices.end());
	}
}
void Extrusion::setxTexture()
{
	xTexture.clean();
	float sum = 0;
	for (int i = 0; i < pointCnt; i++) {
		xTexture.push_back(sum);
		sum += glm::distance(points[i], points[(i + 1) % pointCnt]);
	}
	for (int i = 0; i < pointCnt; i++) {
		xTexture[i] /= 1.0 * sum;
		xTexture[i] *= repeat;
		xTexture[i] = xTexture[i] - floor(xTexture[i]);
	}
}

void Extrusion::setyTexture(const std::vector<glm::vec3>& path, int size)
{
	yTexture.clean();
	float sumx = 0;
	for (int i = 0; i < pointCnt; i++) {
		sumx += glm::distance(points[i], points[(i + 1) % pointCnt]);
	}
	float sum = 0;
	float tempsum = 0;
	yTexture.push_back(sum);
	for (int i = 0; i < size; i++) {
		if (i == 0) tempsum = glm::distance(glm::vec3(0, 0, 0), path[i]);
		else tempsum = glm::distance(path[i], path[(i - 1)]);
		if (addMidpoint) {
			for (int j = 0; j < _steps[i]; j++) {
				sum += 1.0 * tempsum / (_steps[i]);
				yTexture.push_back(sum);

			}
		}
		else {
			sum += tempsum;
			yTexture.push_back(sum);
		}
	}
	for (int i = 0; i < yTexture.size(); i++) {
		yTexture[i] /= 1.0 * sum;
		yTexture[i] *= sum / sumx * repeat;
		yTexture[i] = yTexture[i] - floor(yTexture[i]); //eventualno izbaciti
	}
}

Extrusion::Extrusion(glm::vec2* points, int pointCnt) :
	points(points), pointCnt(pointCnt)
{

}


void Extrusion::reset()
{
	rollBackMatrix = glm::mat4(1.0f);
	totalRotation = glm::mat4(1.0f);
	finishedSetup = false;
	calledAgain = false;
	direction = { 0.0f , 0.0f, 0.0f };
	xTexture.clean();
	yTexture.clean();
	saveState.clear();
}

std::vector<glm::vec3> Extrusion::generateSmoothPoints(const std::vector<glm::vec3>& roughPoints, int pointsPerLenght, const glm::vec3* startAngle, const glm::vec3* endAngle)
{
	std::vector<glm::vec3> vec;

	constexpr float endVal = 1;
	constexpr float minDot = -3;
	constexpr float pi = glm::pi<float>();

	if (roughPoints.size() < 1)
		return vec;

	static float epsilon = glm::epsilon<float>();
	int startIndex = 0, endIndex = roughPoints.size() - 1;

	while (glm::length(roughPoints[startIndex]) < epsilon)
		if (++startIndex == roughPoints.size())
			return vec;

	glm::vec3 lastDir;

	if (startAngle != nullptr && (glm::length(*startAngle) > epsilon))
		lastDir = glm::normalize(*startAngle);
	else if (glm::length(direction) > epsilon)
		lastDir = glm::normalize(direction);
	else {
		lastDir = glm::normalize(roughPoints[startIndex]);
	}


	glm::vec3 end;

	while (glm::length(roughPoints[endIndex]) < epsilon)
		if (--endIndex < 0)
			return vec;

	if (endAngle != nullptr && (glm::length(*endAngle) > epsilon))
		end = glm::normalize(*endAngle);
	else {
		end = glm::normalize(roughPoints[endIndex]);
	}


	vec.reserve((endIndex - startIndex + 1) * pointsPerLenght);

	static glm::mat4 mat(glm::inverse(glm::transpose(glm::mat4( //aproksimacija polinomom cetvrtog reda
		0, 0, 0, 1,
		0, 0, endVal, 0,
		endVal * endVal * endVal, endVal * endVal, endVal, 0,
		3 * endVal * endVal, 2 * endVal, 1, 0
	))));




	glm::vec4 p;
	glm::vec4 endPointPoly = { endVal * endVal * endVal, endVal * endVal, endVal, 1 };
	glm::vec4 startPointPoly = { 0,0,0,1 };

	bool noMidPoints = true;
	glm::vec3 lastPoint = { 0.0f, 0.0f, 0.0f };
	glm::vec3 currentPoint = lastPoint;
	glm::vec3 nextDir(0.0f), currentDelta(0.0f), missingDelta(0.0f);
	glm::vec3 scale(0.0f);

	glm::vec4 rj1, rj2, rj3;

	for (int i = startIndex; i <= endIndex;) {


		currentDelta = (noMidPoints) ? roughPoints[i] : missingDelta;


		if (i == endIndex) {
			nextDir = end;
			++i; //da bi se prekinuo loop na iducoj iteraciji
		}
		else {
			while (glm::length(roughPoints[++i]) < epsilon);

			nextDir = glm::normalize(roughPoints[i]);

		}


		noMidPoints = true;
		float dot = glm::dot(lastDir, currentDelta);
		missingDelta = { 0.0f,0.0f,0.0f };
		while ((glm::dot(lastDir, nextDir) * ((dot < 0) ? -1 : 1) - (dot < 0)) < minDot) //kreiranje podtacaka ako je pocetni i krajni ugao krive prevelik
		{
			glm::vec3 midPointDir(lastDir + nextDir);

			if (glm::length(midPointDir) < epsilon) {
				auto axis = glm::cross(lastDir, nextDir);
				auto rotMatrix = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2, axis);
				midPointDir = rotMatrix * glm::vec4(lastDir.x, lastDir.y, lastDir.z, 1.0f);
			}
			else
				midPointDir = glm::normalize(midPointDir);
			midPointDir = (glm::dot(midPointDir, currentDelta) < 0) ? -midPointDir : midPointDir;

			nextDir = midPointDir;
			currentDelta /= 2;
			missingDelta += currentDelta;
			dot = glm::dot(lastDir, currentDelta);
			noMidPoints = false;
		}

		if (!noMidPoints)
			--i;

		rj1 = { 0, lastDir.x, (currentDelta.x > 0) ? 1 : -1, nextDir.x };
		rj2 = { 0, lastDir.y, (currentDelta.y > 0) ? 1 : -1, nextDir.y };
		rj3 = { 0, lastDir.z, (currentDelta.z > 0) ? 1 : -1, nextDir.z };

		glm::vec3 zadnje = { glm::dot(mat * rj1, endPointPoly), glm::dot(mat * rj2, endPointPoly), glm::dot(mat * rj3, endPointPoly) };

		for (int j = 0; j < 3; ++j)
			scale[j] = (zadnje[j] != 0) ? currentDelta[j] / zadnje[j] : 1;


		int lim = pointsPerLenght * glm::length(currentDelta);
		lastPoint = { 0.0f, 0.0f, 0.0f };
		for (int j = 1; j <= lim; ++j) {
			p[3] = 1;
			p.z = (1 - endVal * glm::cos(j * pi / lim)) / 2;
			p.y = p.z * p.z;
			p.x = p.y * p.z;
			currentPoint = { glm::dot(mat * rj1, p), glm::dot(mat * rj2, p), glm::dot(mat * rj3, p) };
			vec.emplace_back((currentPoint.x - lastPoint.x) * scale.x, (currentPoint.y - lastPoint.y) * scale.y, (currentPoint.z - lastPoint.z) * scale.z);
			lastPoint = std::move(currentPoint);
		}

		lastDir = nextDir;


	}

	


	return vec;
}


void Extrusion::Extrude(const std::vector<glm::vec3>& path, std::vector<float>& verts, std::vector<uint32_t>& inds) // napraviti da se ponovnim pozivom funkcije extruzija nastavlja. Ovaj interni vektor ce se std::move pri povratku unistiti tako da treba napraviti nesto sto ce sacuvati gjde se stalo
{
	_steps.clean();
	constexpr float epsilon = glm::epsilon<float>() * 3;
	std::vector<float> vektor(0);
	if (pointCnt <= 0 || path.size() == 0)
		return;

	vektor.reserve(path.size() * 3); //ovo ce trebati popraviti kad se dodaju normale i ostalo

	int path_start = 0, path_end = 0;
	while (glm::length(path[path_start]) < epsilon) // preskacu se tacke koje nemaju pomjeraja
		if (++path_start == path.size())
			return;

	for (int i = path.size() - 1; i >= path_start; --i)
		if (glm::length(path[i]) > epsilon) {
			path_end = i;
			break;
		}

	glm::vec3 newDirection;

	if (glm::length(direction) < epsilon)
		newDirection = glm::normalize(path[path_start]);
	else
		newDirection = glm::normalize(direction);


	glm::vec3 rotatedPoint, translate;
	glm::vec3 futureDirection, direction = { 0.0f, 0.0f, 1.0f }; // pocetni pravac ako je oblik definisan samo u xy ravni


	if (pointsNormalized == nullptr) {
		pointsNormalized = new glm::vec2[pointCnt];

		for (int i = 0; i < pointCnt; ++i)
			if (glm::length(points[i]) > epsilon)
				pointsNormalized[i] = glm::normalize(points[i]);
			else
				pointsNormalized[i] = { 0.0f, 0.0f };

	}

	int maxIndex = -1;

	const auto findSpecialAxis = [this](int immobilePointIndex) {//kad je novi pravac suprotnom starom axis treba da bude u pravcu nemobilne tacke
		return glm::normalize(glm::vec3(totalRotation * glm::vec4(points[immobilePointIndex].x, points[immobilePointIndex].y, 0.0f, 1.0f)));
	};

	const auto findRotMatrix = [&findSpecialAxis](const glm::vec3& oldDirection, const glm::vec3& newDirection, const float& dotProduct, int immobilePointIndex) {
		if (glm::abs(dotProduct - 1) < epsilon)
			return glm::mat4(1.0f);

		if (glm::epsilonNotEqual(dotProduct, -1.0f, 0.001f))
			return glm::rotate(glm::mat4(1.0f), std::acos(glm::clamp(dotProduct, -1.0f, 1.0f)), glm::cross(oldDirection, newDirection));
		else
			return glm::rotate(glm::mat4(1.0f), std::acos(glm::clamp(dotProduct, -1.0f, 1.0f)), findSpecialAxis(immobilePointIndex));
	};


	static auto dot2d = [](const glm::vec2& v1, const glm::vec3& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	};

	const auto getImmobilePoint = [this](const glm::vec3& direction, const glm::mat4& rollBackMatrix)
	{
		glm::vec3 directionBasisChange = rollBackMatrix * glm::vec4(direction.x, direction.y, direction.z, 1.0f);
		float maxDot = dot2d(pointsNormalized[0], directionBasisChange);
		int maxIndex = 0;
		float dot;
		for (int j = 1; j < pointCnt; ++j) {
			dot = dot2d(pointsNormalized[j], directionBasisChange);
			if ((dot > maxDot) || \
				((dot - maxDot) < epsilon && (dot2d(points[j], directionBasisChange) > dot2d(points[maxIndex], directionBasisChange))))//ako je pravac isti onda se uzima tacka koja ima veci dot produkt
			{
				maxDot = dot;
				maxIndex = j;
			}
		}
		return maxIndex;
	};

	maxIndex = getImmobilePoint(newDirection, rollBackMatrix);
	glm::mat4 rotMatrix(findRotMatrix(direction, newDirection, glm::dot(direction, newDirection), maxIndex));
	bool finishedSetup = false;
	int futureMaxIndex;
	int tempIndex;
	glm::mat4 futureRotMatrix;


	if (calledAgain) //samo pri ponovnim pozivima extrude
	{
		direction = newDirection;
		newDirection = glm::normalize(path[path_start]);

		futureRotMatrix = findRotMatrix(direction, newDirection, glm::dot(direction, newDirection), maxIndex);
		futureMaxIndex = getImmobilePoint(newDirection, rollBackMatrix);


		vektor.resize(saveState.size());
		std::copy(saveState.begin(), saveState.end(), vektor.begin());


	}




	float directionDot;
	int removed_initial = vektor.size();


	glm::vec3 translateMidpoint(0.0f);

	for (int i = path_start;;) {
		_steps.push_back(0);

		directionDot = glm::dot(newDirection, direction);

		if (finishedSetup)
		{
			
			maxIndex = futureMaxIndex;
			if(glm::epsilonNotEqual(directionDot, -1.0f, 0.001f))
				rotMatrix = futureRotMatrix;
			else
				rotMatrix = findRotMatrix(direction, newDirection, glm::dot(direction, newDirection), maxIndex);
		}

		if (i != path_end || !finishedSetup) {
			tempIndex = i;
			if (finishedSetup)
				while (glm::length(path[++i]) < epsilon); //ne moram se sekirati da li ce 'i' izaci van opsega odnosno da li ima barem jedan element sa nenultom duzinom

			futureDirection = glm::normalize(path[i]);
			futureRotMatrix = findRotMatrix(newDirection, futureDirection, glm::dot(newDirection, futureDirection), maxIndex);

			rollBackMatrix *= glm::inverse(rotMatrix);
			futureMaxIndex = getImmobilePoint(futureDirection, rollBackMatrix);
			i = tempIndex; //vracanje na trenutnu iteraciju
		}
		else //nema buducnosti :(
			futureRotMatrix = glm::mat4(1.0f);

		/*
		* mogao bi se jos jedan branch napraviti tako da se buduci elementi nikad ne racunaju odnosno kad je keepCenter = false ali nije neka optimizacija sa obzirom
		* da ce ta opcija biti ukljucena najverovatnije. U svakom slucaju ostavit cu funkcije kao lambda funkcije da bi se to moglo eventualno dodati
		*/


		glm::vec3 totalShift(0.0f, 0.0f, 0.0f);
		if (finishedSetup) //totalShift je ukupna translacija od centra
		{
			int index = vektor.size() - 3 * (pointCnt - maxIndex); /*treba izmijeniti kad dodu normale*/
			glm::vec4 immobilePoint = glm::vec4(vektor[index], vektor[index + 1], vektor[index + 2], 1.0f);
			totalShift = immobilePoint - (totalRotation * glm::vec4(points[maxIndex].x, points[maxIndex].y, 0.0f, 1.0f));
		}

		if (glm::abs(directionDot - 1) > epsilon)  //ako novi pravac nije jednak starom
		{


			if (!finishedSetup || (keepCentered && i == path_start))
				translate = { 0.0f, 0.0f, 0.0f }; //u sustini bez translacije ce ruzno izgledati ali ne moze se odrzati centar drugacije (ne treba zadavati pocetni pravac u tom slucaju)
			else {

				translate = totalRotation * glm::vec4(points[maxIndex].x, points[maxIndex].y, 0.0f, 1.0f) - (rotMatrix * totalRotation * glm::vec4(points[maxIndex].x, points[maxIndex].y, 0.0f, 1.0f)); //pomjeranje objekta oko tacke maxIndex

				if (keepCentered) {
					glm::vec3 onlyNewDir = newDirection - ((glm::dot(direction, newDirection) / glm::dot(direction, direction)) * direction);
					if(glm::length(onlyNewDir) > 0.001)
						translate -= glm::dot(onlyNewDir, translate) / glm::dot(onlyNewDir, onlyNewDir) * onlyNewDir; //putanja se smanjuje jer se prilikom rotacije mali dio vec translira po putanji
				}

			}



		}
		else { //stari pravac je isti novom pravcu
			translate = { 0.0f, 0.0f, 0.0f };
		}

		if (finishedSetup) //pomjeranje objekta kako bi ga naredna rotacija bacila na centar
		{
			if (keepCentered)
			{
				glm::vec3 futureTrans = rotMatrix * totalRotation * glm::vec4(points[futureMaxIndex].x, points[futureMaxIndex].y, 0.0f, 1.0f) - (futureRotMatrix * rotMatrix * totalRotation * glm::vec4(points[futureMaxIndex].x, points[futureMaxIndex].y, 0.0f, 1.0f));
				//ovo treba optimizovati jer ovdje se racuna iduca translacija ali onda u narednoj iteraciji ovo se ponovo racuna
				if(glm::length(newDirection) > 0.001f)
					translate -= glm::dot(newDirection, futureTrans) / glm::dot(newDirection, newDirection) * newDirection;
			}

		}
		else // za dodavanje pocetnih tacaka
		{
			_steps[i]++;
			for (int j = 0; j < pointCnt; ++j) {
				rotatedPoint = rotMatrix * glm::vec4(points[j].x, points[j].y, 0.0f, 1.0f);

				vektor.emplace_back(rotatedPoint.x + translate.x);
				vektor.emplace_back(rotatedPoint.y + translate.y);
				vektor.emplace_back(rotatedPoint.z + translate.z);
			}
			totalRotation = rotMatrix * totalRotation;
			goto CONTINUE_ITERATION; //pocetno postavljanje tacaka je zavrseno. Prelazi se na novu iteraciju
		}



		if (!(keepCentered && i == path_start) && finishedSetup && addMidpoint && glm::abs(directionDot - 1) > 0.01) //midpoint dodaje dodatne tacke da se bolje extruzira pod ostrim uglovima ali nema potrebe ako su slicni novi i stari ugao
		{
			glm::vec3 axis;
			if (glm::epsilonNotEqual(directionDot, -1.0f, 0.001f))
				axis = glm::cross(direction, newDirection);
			else
				axis = findSpecialAxis(maxIndex);
			
			auto angle = glm::acos(glm::clamp(glm::dot(direction, newDirection), -1.0f, 1.0f));
			int steps = int(angle / maxAngle) + 1;
			angle /= steps;

			glm::mat4 rotMatrixStep(1.0f);

	
			for (int j = 0; j < steps; ++j) {

				if (j < steps - 1)
					rotMatrixStep = rotMatrixStep * glm::rotate(glm::mat4(1.0f), angle, axis);
				else
					rotMatrixStep = rotMatrix; // radi greske u postepenom rotiranju, moze i bez ovoga
				_steps[i]++;
				for (int k = 0; k < pointCnt; ++k)
				{
					rotatedPoint = glm::vec4(totalShift, 1.0f) + rotMatrixStep * totalRotation * glm::vec4(points[k].x, points[k].y, 0.0f, 1.0f);
					translateMidpoint = totalRotation * glm::vec4(points[maxIndex].x, points[maxIndex].y, 0.0f, 1.0f) - (rotMatrixStep * totalRotation * glm::vec4(points[maxIndex].x, points[maxIndex].y, 0.0f, 1.0f));

					vektor.emplace_back(rotatedPoint.x + translateMidpoint.x);
					vektor.emplace_back(rotatedPoint.y + translateMidpoint.y);
					vektor.emplace_back(rotatedPoint.z + translateMidpoint.z);
				}

			}


		}


		totalRotation = rotMatrix * totalRotation;
		_steps[i]++;
		for (int j = 0; j < pointCnt; ++j)
		{
			rotatedPoint = glm::vec4(totalShift, 1.0f) + totalRotation * glm::vec4(points[j].x, points[j].y, 0.0f, 1.0f);

			vektor.emplace_back(rotatedPoint.x + translate.x + path[i].x);
			vektor.emplace_back(rotatedPoint.y + translate.y + path[i].y);
			vektor.emplace_back(rotatedPoint.z + translate.z + path[i].z);
		}



	CONTINUE_ITERATION:

		if (i == path_end && finishedSetup)
			break;

		direction = newDirection;
		newDirection = futureDirection;

		if (!finishedSetup) {
			finishedSetup = true;
			continue;
		}


		while (glm::length(path[++i]) < epsilon);


		if (calledAgain && removed_initial > 0) //ukidanje prvobitnih clanova sa vektora koji su tu stavljeni samo za ocitavanje proslog stanja
		{
			//vektor.erase(vektor.begin(), vektor.begin() + removed_initial);
			removed_initial = 0;
		}

	}



	//calledAgain = true;
	rollBackMatrix *= glm::inverse(rotMatrix);
	this->direction = newDirection;

	const int& index = 3 * (pointCnt); // prepraviti kad dodu normale
	saveState.resize(index);
	std::copy(vektor.end() - index, vektor.end() - index + 3 * pointCnt, saveState.begin());


	//return;
	std::vector<std::vector<glm::vec3> >faces;
	std::vector<std::vector<glm::vec2> >textures;
	std::vector<std::vector<glm::vec3> >rightback;
	std::vector<std::vector<glm::vec3> >leftback;
	std::vector<std::vector<glm::vec3> >rightfront;
	std::vector<std::vector<glm::vec3> >leftfront;
	int k = 0, j = 0;
	faces.push_back(std::vector<glm::vec3>());
	textures.push_back(std::vector<glm::vec2>());
	setxTexture();
	setyTexture(path, path.size());
	for (int i = 0; i < vektor.size(); i += 3) {
		faces[j].push_back({ vektor[i], vektor[i + 1], vektor[i + 2] });
		textures[j].push_back({ xTexture[k], yTexture[j] });
		k++;
		if (k == pointCnt) {
			j++;
			k = 0;
			if (i + 3 < vektor.size()) {
				faces.push_back(std::vector<glm::vec3>());
				textures.push_back(std::vector<glm::vec2>());
			}
		}
	}

	for (int i = 0; i < faces.size(); i++) {
		rightback.push_back(std::vector<glm::vec3>());
		leftback.push_back(std::vector<glm::vec3>());
		rightfront.push_back(std::vector<glm::vec3>());
		leftfront.push_back(std::vector<glm::vec3>());
		for (int j = 0; j < faces[i].size(); j++) {
			if (i >= faces.size() - 1) {
				leftfront[i].push_back({ 0, 0, 0 });
				rightfront[i].push_back({ 0, 0, 0 });
			}
			else {
				leftfront[i].push_back(glm::normalize(glm::cross(faces[i][(j + 1) % pointCnt] - faces[i][j], faces[i + 1][j] - faces[i][j])));
				rightfront[i].push_back(-glm::normalize(glm::cross(faces[i][(j - 1 + pointCnt) % pointCnt] - faces[i][j], faces[i + 1][j] - faces[i][j])));
			}

			if (i == 0) {
				leftback[i].push_back({ 0, 0, 0 });
				rightback[i].push_back({ 0, 0, 0 });
			}
			else {
				leftback[i].push_back(-glm::normalize(glm::cross(faces[i][(j + 1) % pointCnt] - faces[i][j], faces[i - 1][j] - faces[i][j])));
				rightback[i].push_back(glm::normalize(glm::cross(faces[i][(j - 1 + pointCnt) % pointCnt] - faces[i][j], faces[i - 1][j] - faces[i][j])));
			}
		}
	}
	int minus = 1;
	if (!outsideVisible) minus = -1;
	if (addNormals) {
		for (int i = 0; i < faces.size(); i++) {
			for (int j = 0; j < faces[i].size(); j++) {
				for (int k = 0; k < 4; k++) {
					verts.push_back(faces[i][j][0]);
					verts.push_back(faces[i][j][1]);
					verts.push_back(faces[i][j][2]);
					if (addTextureCoord) {
						verts.push_back(textures[i][j][0]);
						verts.push_back(textures[i][j][1]);
					}
					switch (k) {
					case 0:
						verts.push_back(minus*leftfront[i][j][0]);
						verts.push_back(minus*leftfront[i][j][1]);
						verts.push_back(minus*leftfront[i][j][2]);
						break;
					case 1:
						verts.push_back(minus * rightfront[i][j][0]);
						verts.push_back(minus* rightfront[i][j][1]);
						verts.push_back(minus* rightfront[i][j][2]);
						break;
					case 2:
						verts.push_back(minus * leftback[i][j][0]);
						verts.push_back(minus* leftback[i][j][1]);
						verts.push_back(minus* leftback[i][j][2]);
						break;
					case 3:
						verts.push_back(minus * rightback[i][j][0]);
						verts.push_back(minus* rightback[i][j][1]);
						verts.push_back(minus* rightback[i][j][2]);
						break;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < faces.size(); i++) {
			for (int j = 0; j < faces[i].size(); j++) {
				verts.push_back(faces[i][j][0]);
				verts.push_back(faces[i][j][1]);
				verts.push_back(faces[i][j][2]);
				if (addTextureCoord) {
					verts.push_back(textures[i][j][0]);
					verts.push_back(textures[i][j][1]);
				}
			}
		}
	}

	for (int j = 0; j < vektor.size() / pointCnt / 3 - 1; j++) {
		if (addNormals) AddIndices(inds, 4 * j * pointCnt);
		else AddIndices(inds, j * pointCnt);
	}

	return;
}


void Extrusion::setDirection(const glm::vec3& direction) {
	this->direction = direction;
}

glm::vec3 Extrusion::getDirection() {
	return direction;
}

void Extrusion::setVisibleSide(bool outsideVisible) {
	this->outsideVisible = outsideVisible;
}

void Extrusion::setAddMidpoints(bool addMidpoint, float maxDegree) {
	this->addMidpoint = addMidpoint;
	maxAngle = glm::radians(maxDegree) + 0.01; // 0.01 se dodaje kako se midpoint ne bi radzvajao na 2 ugla ako je ugao maxDegree
}

void Extrusion::setKeepCentered(bool keepShapeCenteredInPath) {
	keepCentered = keepShapeCenteredInPath;
}

void Extrusion::setAddNormals(bool addNormals) {
	this->addNormals = addNormals;
}

void Extrusion::setAddTextureCoordinates(bool addTextureCoord, int x, int y) {
	this->addTextureCoord = addTextureCoord;
	coords += addTextureCoord ? 2 : -2;
	repeatx = x;
	repeaty = y;
}

void Extrusion::setAddBlankSpace(int reserveSpacePerVertex) {
	addBlank = reserveSpacePerVertex;
}



void Extrusion::setRepeat(float rep)
{
	repeat = rep;
}

Extrusion::~Extrusion()
{
	delete pointsNormalized;
}

