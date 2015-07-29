#pragma once

class GGGUI
{
public:
	GGGUI( UINT _width, UINT _height );

public:
	void SetRenderable( bool _renderable );

	void AddRectangle( int _x, int _y, int _width, int _height, float _color[4] );

	bool IsRenderable() const;

private:
	bool m_renderable;
	UINT m_width;
	UINT m_height;
};