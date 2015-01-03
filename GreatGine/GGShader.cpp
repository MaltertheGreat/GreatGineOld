#include "GGShader.h"

#include <fstream>
#include <vector>
using namespace std;

GGShader::GGShader( const GGDirectXDevice& _device )
{
	ID3D11Device* device = _device.GetDevice();

	//device->CreateVertexShader()
}

void GGShader::LoadShader( string _fileName )
{
	/*ifstream file( _fileName, ios::in | ios::binary | ios::ate );
	vector<char> content;

	if( file.is_open() )
	{
	size = file.tellg();
	memblock = new char[ size ];
	file.seekg( 0, ios::beg );
	file.read( memblock, size );
	file.close();

	cout << "the entire file content is in memory";

	delete[] memblock;
	}
	else cout << "Unable to open file";*/
}