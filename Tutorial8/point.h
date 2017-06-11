#include <math.h>

typedef struct SVector2
{
	// --- member variables
	float x, y;

	// constructors
	SVector2(){ /* do nothing */ };
	SVector2( float _x, float _y ){ x = _x; y = _y; };

	// --- methods
	// Calculate the length of the vector
	float	Length() const // const methods can't modify member variables nor call non-const methods
	{
		float l = x*x+y*y; // calculate squared length
		if( l ) // prevent division by zero if the vector is zero length
			return sqrtf( l ); // calculate actual length
		else
			return 0;
	};

	// Get the angle of this vector in relation to other vector
	float	AngleWith( const SVector2& v ) const  // const methods ensure member values don't get modified
	{
		float fLengths = (this->Length()*v.Length());
		if( fLengths ) // prevent divide by zero error
			return (float)acos( Dot(v) / fLengths );
		return 0;
	};

	// calculate the scalar product (dot product) of the vector with another vector
	float	Dot( const SVector2& other ) const
	{
		return x*other.x+y*other.y;
	};	

	// Multiply vector elements by a scalar value
	void	Scale( float _scalar ) 
	{
		x *= _scalar; 
		y *= _scalar;
	};	

	// Resize the vector to make it a unit length
	void	Normalize()
	{
		float l = x*x+y*y; // calculate squared length
		if( l ) // prevent division by zero if the vector is zero length
		{
			l = sqrtf( l ); // calculate actual length
			x = x/l;
			y = y/l;
		}
	};

	// rotate the vector by an angle in radians
	SVector2&	Rotate( float _theta_in_rads ) 
	{
		float theta = _theta_in_rads;
		float cs = cosf(theta);
		float sn = sinf(theta);

		float px = x * cs - y * sn; 
		float py = x * sn + y * cs;

		x = px;
		y = py;
		return *this;
	}

	// arithmetic operators ------------------------------
	SVector2 operator + ( const SVector2& other ) const
	{
		return SVector2( x + other.x, y + other.y );
	}
} SPoint2;