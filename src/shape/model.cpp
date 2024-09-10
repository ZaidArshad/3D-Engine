#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <numeric>
#include <filesystem>
#include <ranges>

#include "model.h"
#include "obj_constants.h"
#include "helper/app_tools.h"
#include "helper/logger.h"
#include "shader/color.h"

Model::Model(const std::string &dir)
{
	m_frameIndex = 0;
	m_frameGap = 0;

	// Reading the obj files from the given directory
	const auto fileRange = std::ranges::transform_view(
		std::filesystem::directory_iterator(dir),
		[](const auto &entry) { return entry.path().string(); });
	auto filePaths = std::ranges::to<std::vector<std::string>>(fileRange);
	std::sort(filePaths.begin(), filePaths.end());

	for (const auto &path : filePaths) {
		parseOBJFile(path);
	}

	if (m_vaFrames.empty())
	{
		Logger::log("Could not read models from directory: " + dir + "\n");
		return;
	}

	delete m_pVA;
	m_pVA = m_vaFrames.front();
}

Model::~Model()
{
	// Shape destructor will handle deletion of first VA
	m_vaFrames.erase(m_vaFrames.begin());
	for (auto va : m_vaFrames)
	{
		delete va;
	}
}

// Goes to the next frame of animation
void Model::nextFrame()
{
	if (m_frameGap == ANIMATION_FRAME_GAP)
	{
		m_frameGap = 0;
		m_frameIndex = (m_frameIndex + 1) % m_vaFrames.size();
	}
	m_frameGap++;
}

void Model::resetFrames()
{
	m_frameIndex = 0;
}

void Model::draw()
{
	if (m_pTexture)
	{
		m_pTexture->bind(0);
		m_vaFrames[m_frameIndex]->draw();
		m_pTexture->unbind();
	}
	else
	{
		m_vaFrames[m_frameIndex]->draw();
	}
}

template <class T>
void printVectorVecs(std::vector<T> &outerVec, int length)
{
	for (auto &innerVec : outerVec)
	{
		for (int i = 0; i < length; i++)
		{
			std::cout << innerVec[i] << " ";
		}
		std::cout << std::endl;
	}
}

template <class T, class U>
void pushVecIntoVector(std::vector<T> &container, const U &vec, int length)
{
	for (int i = 0; i < length; i++)
	{
		container.push_back(vec[i]);
	}
}

void Model::generateModel(std::vector<glm::vec3> &vPositions,
						  std::vector<glm::vec2> &vTextures,
						  std::vector<glm::vec3> &vNormals,
						  std::vector<Face> &faces)
{
	Frame *frame = new Frame();
	for (const Face &face : faces)
	{
		for (const auto &indices : face) // {v, vt, vn}
		{
			pushVecIntoVector(frame->vPositions, vPositions[indices[0] - 1], 3);
			pushVecIntoVector(frame->vTextures, vTextures[indices[1] - 1], 2);
			pushVecIntoVector(frame->vNormals, vNormals[indices[2] - 1], 3);
		}
	}

	if (m_vaFrames.empty())
	{
		m_vertCount = faces.size() * 3;
	}
	VertexArray *va = new VertexArray();
	va->bind();
	va->addBuffer(frame->vPositions, 3);
	std::vector<float> colors;
	for (int i = 0; i < m_vertCount; i++)
	{
		colors.insert(colors.end(), COLOR_TEXTURE.begin(), COLOR_TEXTURE.end());
	}
	va->addBuffer(colors, 4);
	va->addBuffer(frame->vTextures, 2);

	// Set indices from 0 -> m_vertCount-1
	std::vector<unsigned int> indices(m_vertCount);
	std::iota(indices.begin(), indices.end(), 0);
	va->setIndices(indices);
	va->unbind();
	m_vaFrames.push_back(va);
}

template <class T>
T bufferToVec(std::stringstream &buffer, int length)
{
	std::string payload;
	T vec{};
	for (int i = 0; i < length; i++)
	{
		buffer >> payload;
		vec[i] = std::stof(payload);
	}
	return vec;
}

void Model::parseOBJFile(const std::string &path)
{
	std::cout << std::fixed << std::setprecision(6);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;

	std::ifstream stream(path);
	std::string line;
	std::string opcode;
	std::string payload;

	while (std::getline(stream, line))
	{
		std::stringstream buffer(line);
		buffer >> opcode;

		if (opcode == STR_OBJ_VERTEX_POSITION)
		{
			positions.push_back(bufferToVec<glm::vec3>(buffer, 3));
		}
		else if (opcode == STR_OBJ_VERTEX_TEXTURE)
		{
			textures.push_back(bufferToVec<glm::vec2>(buffer, 2));
		}
		else if (opcode == STR_OBJ_VERTEX_NORMAL)
		{
			normals.push_back(bufferToVec<glm::vec3>(buffer, 3));
		}
		else if (opcode == STR_OBJ_FACE)
		{
			Face face;
			for (int i = 0; i < 3; i++)
			{
				// Reading the indices for the faces (v/vt/vn)
				glm::ivec3 indices;
				buffer >> payload;
				std::stringstream faceBuffer(payload);
				for (int j = 0; j < 3; j++)
				{
					std::string val;
					std::getline(faceBuffer, val, '/');
					indices[j] = std::stoi(val);
				}
				face.push_back(indices);
			}
			faces.push_back(face);
		}
	}

	generateModel(positions, textures, normals, faces);
}
