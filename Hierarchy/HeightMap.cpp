#include "HeightMap.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

HeightMap::HeightMap(char* filename, float gridSize)
{
	LoadHeightMap(filename, gridSize);

	m_pHeightMapBuffer = NULL;

	static const VertexColour MAP_COLOUR(200, 255, 255, 255);

	m_HeightMapVtxCount = (m_HeightMapLength - 1) * (m_HeightMapWidth - 1) * 6;
	m_pMapVtxs = new Vertex_Pos3fColour4ubNormal3f[m_HeightMapVtxCount];

	int vtxIndex = 0;
	int mapIndex = 0;

	XMVECTOR v0, v1, v2, v3;
	m_HeightMapVtxCount = ((m_HeightMapWidth - 1) * (m_HeightMapLength)) * 4; //New size for vertex nums
	m_pMapVtxs = new Vertex_Pos3fColour4ubNormal3f[m_HeightMapVtxCount];


	int vertexNum = 0;
	for (int i = 0; i < m_HeightMapLength - 1; i++)
	{
		for (int j = 0; j < m_HeightMapWidth; j++)
		{
			int gridindex = (j + (i*m_HeightMapLength));

			//Grab vectors
			XMVECTOR origin = XMLoadFloat3(&m_pHeightMap[gridindex]);
			XMVECTOR originDown = XMLoadFloat3(&m_pHeightMap[gridindex + m_HeightMapWidth]);
			XMVECTOR originRight = XMLoadFloat3(&m_pHeightMap[gridindex + 1]);
			//calculate normal
			XMVECTOR triangleNormal = -XMVector3Normalize(XMVector3Cross(originDown - origin, originRight - originDown));
			//store back as floats
			XMFLOAT3 v1, v2, v3, norm1, norm2;
			XMStoreFloat3(&v1, origin);
			XMStoreFloat3(&v2, originDown);
			XMStoreFloat3(&v3, originRight);
			XMStoreFloat3(&norm1, triangleNormal);

			//Store the top and bottom vertex for each loop
			m_pMapVtxs[vertexNum++] = Vertex_Pos3fColour4ubNormal3f(v1, MAP_COLOUR, norm1);
			m_pMapVtxs[vertexNum++] = Vertex_Pos3fColour4ubNormal3f(v2, MAP_COLOUR, norm1);
			//If we are the final column, make a degenerative triangle going down at the very end
			if (j == m_HeightMapLength - 1) {
				m_pMapVtxs[vertexNum++] = Vertex_Pos3fColour4ubNormal3f(v2, MAP_COLOUR, norm1);
				DirectX::XMFLOAT3 v3 = m_pHeightMap[gridindex + m_HeightMapWidth - (m_HeightMapLength - 1)];
				m_pMapVtxs[vertexNum++] = Vertex_Pos3fColour4ubNormal3f(v3, MAP_COLOUR, norm1);
				m_pMapVtxs[vertexNum++] = Vertex_Pos3fColour4ubNormal3f(v3, MAP_COLOUR, norm1);
			}


		}

	}

	m_pHeightMapBuffer = CreateImmutableVertexBuffer(Application::s_pApp->GetDevice(), sizeof Vertex_Pos3fColour4ubNormal3f * m_HeightMapVtxCount, m_pMapVtxs);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

HeightMap::~HeightMap()
{
	Release(m_pHeightMapBuffer);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void HeightMap::Draw(void)
{
	Application::s_pApp->DrawUntexturedLit(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, m_pHeightMapBuffer, NULL, m_HeightMapVtxCount);
}

//////////////////////////////////////////////////////////////////////
// LoadHeightMap
// Original code sourced from rastertek.com
//////////////////////////////////////////////////////////////////////
bool HeightMap::LoadHeightMap(char* filename, float gridSize)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_HeightMapWidth = bitmapInfoHeader.biWidth;
	m_HeightMapLength = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_HeightMapWidth * m_HeightMapLength * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if(!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	XMFLOAT3* pUnsmoothedMap = new XMFLOAT3[m_HeightMapWidth * m_HeightMapLength];
	m_pHeightMap = new XMFLOAT3[m_HeightMapWidth * m_HeightMapLength];

	if(!m_pHeightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	// Bitmaps store the data bottom to top so it needs to be flipped!
	for(j = m_HeightMapLength-1; j>=0; j--)
	{
		for(i = 0; i < m_HeightMapWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_HeightMapWidth * j) + i;

			m_pHeightMap[index].x = (float)(i - (m_HeightMapWidth / 2)) * gridSize;
			m_pHeightMap[index].y = (float)height / 6 * gridSize;
			m_pHeightMap[index].z = (float)((m_HeightMapLength / 2) - j) * gridSize;

			pUnsmoothedMap[index].y = (float)height / 6 * gridSize;

			k += 3;
		}
	}

	// Smoothing the landscape mesh makes a difference to the look of the shading (separate to smoothing the normals)
	for(int s = 0; s < 2; ++s)
	{
		for(j = 0; j < m_HeightMapLength; j++)
		{
			for(i = 0; i < m_HeightMapWidth; i++)
			{
				index = (m_HeightMapWidth * j) + i;

				if(j > 0 && j < m_HeightMapLength - 1 && i > 0 && i < m_HeightMapWidth - 1)
				{
					m_pHeightMap[index].y = 0.0f;
					m_pHeightMap[index].y += pUnsmoothedMap[index - m_HeightMapWidth - 1].y + pUnsmoothedMap[index - m_HeightMapWidth].y + pUnsmoothedMap[index - m_HeightMapWidth + 1].y;
					m_pHeightMap[index].y += pUnsmoothedMap[index - 1].y + pUnsmoothedMap[index].y + pUnsmoothedMap[index + 1].y;
					m_pHeightMap[index].y += pUnsmoothedMap[index + m_HeightMapWidth - 1].y + pUnsmoothedMap[index + m_HeightMapWidth].y + pUnsmoothedMap[index + m_HeightMapWidth + 1].y;
					m_pHeightMap[index].y /= 9;
				}
			}
		}

		for(j = 0; j < m_HeightMapLength; j++)
		{
			for(i = 0; i < m_HeightMapWidth; i++)
			{
				index = (m_HeightMapWidth * j) + i;
				pUnsmoothedMap[index].y = m_pHeightMap[index].y;
			}
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	delete[] pUnsmoothedMap;
	bitmapImage = 0;

	return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
