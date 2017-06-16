#include <cmath>

typedef struct SVector2 {
	float											x, y;	// --- member variables
	// constructors
													SVector2														()													= default;
													SVector2														( float _x, float _y )								: x(_x), y(_y)									{}

	// arithmetic operators ------------------------------
	SVector2										operator +														( const SVector2& other )					const	{ return SVector2( x + other.x, y + other.y );	}

	// assignment operators ------------------------------
	SVector2&										operator +=														( const SVector2& other )							{ x += other.x	; y += other.y	; return *this;	}
	SVector2&										operator *=														( float scalar )									{ x *= scalar	; y *= scalar	; return *this;	}

	// --- methods
	// Calculate the length of the vector
	float											Length															()											const	{	// const methods can't modify member variables nor call non-const methods
		float												vectorSquaredLength												= x * x + y * y; // calculate squared length
		return vectorSquaredLength ? ::std::sqrtf( vectorSquaredLength ) : 0; // prevent division by zero if the vector is zero length. Else calculate actual length
	}

	// Get the angle of this vector in relation to other vector
	float											AngleWith														( const SVector2& v )						const	{	// const methods ensure member values don't get modified
		float												fLengthsProduct													= (this->Length() * v.Length());
		return fLengthsProduct ? (float)acos( Dot(v) / fLengthsProduct ) : 0; // prevent divide by zero error
	}

	// calculate the scalar product (dot product) of the vector with another vector
	float											Dot																( const SVector2& other )					const	{ return x * other.x + y * other.y; } 

	// Multiply vector elements by a scalar value
	void											Scale															( float _scalar )									{
		x												*= _scalar; 
		y												*= _scalar;
	}	

	// Resize the vector to make it a unit length
	void											Normalize														()													{
		float												vectorSquaredLength												= x * x + y * y; // calculate squared length
		if( vectorSquaredLength ) { // prevent division by zero if the vector is zero length
			vectorSquaredLength								= ::std::sqrtf( vectorSquaredLength ); // calculate actual length
			x												= x / vectorSquaredLength;
			y												= y / vectorSquaredLength;
		}
	}

	// rotate the vector by an angle in radians
	SVector2&										Rotate															( float _theta_in_rads )							{
		float												theta															= _theta_in_rads;
		float												cs																= ::std::cosf(theta);
		float												sn																= ::std::sinf(theta);

		float												px																= x * cs - y * sn; 
		float												py																= x * sn + y * cs;

		x												= px;
		y												= py;
		return *this;
	}
} SPoint2;