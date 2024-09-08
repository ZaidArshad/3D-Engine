#include "cube.h"
#include "shader/color.h"

Cube::Cube()
{
	m_vertCount = 0;
	m_length = 0;
}

Cube::Cube(float length)
{
	m_vertCount = 8;
	m_length = length;
	m_pVA->addBuffer(getPosition(), 3);

	std::vector<float> colors;
	for (int i = 0; i < m_vertCount; i++)
	{
		colors.insert(colors.end(), COLOR_WHITE.begin(), COLOR_WHITE.end());
	}
	m_pVA->addBuffer(colors, 4);

	std::vector<float> textureCoords{
		0.0f, 0.0f, // 0
		0.0f, 0.0f, // 1
		1.0f, 0.0f, // 2
		1.0f, 0.0f, // 3
		0.0f, 1.0f, // 4
		0.0f, 1.0f, // 5
		1.0f, 1.0f, // 6
		1.0f, 1.0f, // 7
	};
	m_pVA->addBuffer(textureCoords, 2);

	// Generating index buffer
	std::vector<unsigned int> indices{
		0, 1, 2, // A
		0, 3, 2, // A
		3, 0, 4, // B
		3, 7, 4, // B
		0, 1, 5, // C
		0, 4, 5, // C
		6, 5, 1, // D
		6, 2, 1, // D
		6, 5, 4, // F
		6, 7, 4, // F
		7, 3, 2, // E
		7, 6, 2	 // E

	};
	m_pVA->setIndices(indices);
}

Cube::~Cube()
{
}

std::vector<float> Cube::getPosition()
{
	std::vector<float> positions{
		-m_length, m_length, m_length,	 // 0
		-m_length, m_length, -m_length,	 // 1
		m_length, m_length, -m_length,	 // 2
		m_length, m_length, m_length,	 // 3
		-m_length, -m_length, m_length,	 // 4
		-m_length, -m_length, -m_length, // 5
		m_length, -m_length, -m_length,	 // 6
		m_length, -m_length, m_length,	 // 7
	};
	return positions;
}
