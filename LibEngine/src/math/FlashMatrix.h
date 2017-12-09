#ifndef __MATH_FLASH_MATRIX_H__
#define __MATH_FLASH_MATRIX_H__


QIMP_BEGIN_NAMESPACE

class FlashMatrix : public Matrix4x4f
{

public:

	FlashMatrix()
	{
		Set( 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 );		// identity.
	}

	FlashMatrix( float a, float b, float c, float d, float tx, float ty )
	{
		Set( a, b, c, d, tx, ty );
	}

	~FlashMatrix()
	{
		//
	}

	static FlashMatrix Create(float a, float b, float c, float d, float tx, float ty)
	{
		FlashMatrix matrix( a, b, c, d, tx, ty );
		return matrix;
	}

	//===============================================
	//
	//   http://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/geom/Matrix.html
	//
	//	[  a   c   tx  ]		flash matrix
	//	[  b   d   ty  ]
	//	[  0   0   1   ]
	//
	//
	//	[  a   b   0   0  ]		Flash 4x4 matrix.
	//	[  c   d   0   0  ]
	//	[  0   0   0   0  ]
	//	[  tx  ty  0   0  ]
	//
	//===============================================


	void Set ( const FlashMatrix &matrix )
	{
		Set( matrix.GetA(), matrix.GetB(), matrix.GetC(), matrix.GetD(), matrix.GetTx(), matrix.GetTy() );
	}

	void Set ( float a, float b, float c, float d, float tx, float ty )
	{
		Matrix4x4f::Set
			(
			a,  c,  0,  tx,
			b,  d,  0,  ty,
			0,  0,  1,  0,
			0,  0,	0,  1
			);
	}

	void SetA	( float v ) { Get(0, 0) = v; }
	void SetB	( float v ) { Get(1, 0) = v; }
	void SetC	( float v ) { Get(0, 1) = v; }
	void SetD	( float v ) { Get(1, 1) = v; }
	void SetTx	( float v ) { Get(0, 3) = v; }
	void SetTy	( float v ) { Get(1, 3) = v; }

	float GetA	() const { return Get(0, 0); }
	float GetB	() const { return Get(1, 0); }
	float GetC	() const { return Get(0, 1); }
	float GetD	() const { return Get(1, 1); }
	float GetTx	() const { return Get(0, 3); }
	float GetTy	() const { return Get(1, 3); }

	bool operator != ( const FlashMatrix &matrix ) const
	{
		return (( GetA()  != matrix.GetA()  ) ||
				( GetB()  != matrix.GetB()  ) ||
				( GetC()  != matrix.GetC()  ) ||
				( GetD()  != matrix.GetD()  ) ||
				( GetTx() != matrix.GetTx() ) ||
				( GetTy() != matrix.GetTy() ));
	}

	bool operator == ( const FlashMatrix &matrix ) const
	{
		return (( GetA()  == matrix.GetA()  ) &&
				( GetB()  == matrix.GetB()  ) &&
				( GetC()  == matrix.GetC()  ) &&
				( GetD()  == matrix.GetD()  ) &&
				( GetTx() == matrix.GetTx() ) &&
				( GetTy() == matrix.GetTy() ));
	}


	//===============================================

	void Transform ( Vector3f& p ) const
	{
		float sx;		// 'a'  in AS Matrix.
		float shx;		// 'b'  in AS Matrix.
		float shy;		// 'c'  in AS Matrix.
		float sy;		// 'd'  in AS Matrix.
		float tx;		// 'tx' in AS Matrix.
		float ty;		// 'ty' in AS Matrix.

		sx	= GetA();
		shx	= GetB();
		shy	= GetC();
		sy	= GetD();
		tx	= GetTx();
		ty	= GetTy();

		float t0 = ( sx  * p.x ) + ( shy * p.y ) + tx;
		float t1 = ( shx * p.x ) + ( sy  * p.y ) + ty;

		p.x = t0;
		p.y = t1;
	}

	//===============================================

	void Concatenate ( const FlashMatrix& m )
	{
		float a;
		float b;
		float c;
		float d;
		float tx;
		float ty;

		a	= ( this->GetA() * m.GetA()  ) + ( this->GetC() * m.GetB()  );
		b	= ( this->GetB() * m.GetA()  ) + ( this->GetD() * m.GetB()  );
		c	= ( this->GetA() * m.GetC()  ) + ( this->GetC() * m.GetD()  );
		d	= ( this->GetB() * m.GetC()  ) + ( this->GetD() * m.GetD()  );
		tx	= ( this->GetA() * m.GetTx() ) + ( this->GetC() * m.GetTy() ) + this->GetTx();
		ty	= ( this->GetB() * m.GetTx() ) + ( this->GetD() * m.GetTy() ) + this->GetTy();

		Set( a, b, c, d, tx, ty );
	}

	//=============================================== INVERT.

	FlashMatrix& Invert ()
	{
		float det = Determinant();

		if( det == 0 )
		{
			Set( 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 );	// set identity.

			return *this;
		}

		const float d	= 1.0f / det;
		const float t0	= GetD() * d;

		SetD(  GetA() * d );
		SetC( -GetC() * d );
		SetB( -GetB() * d );

		float t4 = -( ( GetTx() * t0 ) + ( GetTy() * GetC() ) );
		SetTy( -( ( GetTx() * GetB() ) + ( GetTy() * GetD() ) ) );

		SetA( t0 );
		SetTx( t4 );

		return *this;
	}

	float Determinant() const
	{
		return GetA() * GetD() - GetB() * GetC();
	}

	//=============================================== SETTERS.

	void SetScaleRotation ( float x_scale, float y_scale, float angle )
	{
		const float cos_angle = cos( angle );
		const float sin_angle = sin( angle );

		SetA( x_scale *  cos_angle );
		SetB( x_scale *  sin_angle );
		SetC( y_scale * -sin_angle );
		SetD( y_scale *  cos_angle );
	}

	void SetXScale ( float xscale )
	{
		const double rot_x = atan2( GetB(), GetA() );

		SetA( xscale * (float)cos( rot_x ) );
		SetB( xscale * (float)sin( rot_x ) );
	}
	//////////////////////////////////////////////////

	//skew x
	float GetSkewX()
	{
		return atan2(-GetC(), GetD());
	}

	void SetSkewX( float skew_x )
	{
		float scale_y = GetScaleY();
		SetC( -scale_y * sin(skew_x) );
		SetD( scale_y * cos(skew_x) );
	}

	//skew y
	float GetSkewY()
	{
		return atan2(GetB(), GetA());
	} 

	void SetSkewY(float skew_y)
	{
		float scale_x = GetScaleX();
		SetA(scale_x * cos(skew_y));
		SetB(scale_x * sin(skew_y));
	}

	//scale x
	float GetScaleX() const
	{
		float a	= GetA();
		float b	= GetB();
		float s = sqrt( ( a * a + b * b ) );

		return s;
	}


	void SetScaleX ( float scale_x )
	{
		float old_value = GetScaleX();
		// avoid division by zero 
		if (old_value > 0.00001f || old_value < -0.00001f)
		{
			float ratio = scale_x / old_value;
			SetA(GetA() * ratio);
			SetB(GetB() * ratio);
		}
		else
		{
			float skew_y_rad = GetSkewY();
			SetA( cos(skew_y_rad) * scale_x );
			SetB( sin(skew_y_rad) * scale_x );
		}
	}

	//scale y
	float GetScaleY () const
	{
		float c	= GetC();
		float d	= GetD();
		float s = sqrt( ( d * d + c * c ) );

		return s;
	}

	void SetScaleY(float scale_y)
	{
		float old_value = GetScaleY();
		// avoid division by zero 
		if (old_value > 0.00001f || old_value < -0.00001f)
		{
			float ratio = scale_y / old_value;
			SetC(GetC() * ratio);
			SetD(GetD() * ratio);
		}
		else
		{
			float skew_x_rad = GetSkewX();
			SetC( -sin(skew_x_rad) * scale_y );
			SetD( cos(skew_x_rad) * scale_y );
		}
	}


	/////////////////////////////////////////////////////
	void SetYScale ( float yscale )
	{
		const float rot_y = atan2( -GetC(), GetD() );

		SetC( yscale * sin( rot_y ) );
		SetD( yscale * cos( rot_y ) );
	}

	void SetScale ( float xscale, float yscale )
	{
		const float rotation = GetRotation();
		SetScaleRotation( xscale, yscale, rotation ); 
	}

	void SetRotation ( float rotation )
	{   
		const float rot_x	= atan2(  GetB(), GetA() );
		const float rot_y	= atan2( -GetC(), GetD() );
		const float scale_x	= GetXScale();
		const float scale_y	= GetYScale();

		SetA( scale_x * cos( rotation ) );
		SetB( scale_x * sin( rotation ) );
		SetC( scale_y * sin( rot_y - rot_x + rotation ) * -1 );		// reverse sign.
		SetD( scale_y * cos( rot_y - rot_x + rotation ) );
	}

	//=============================================== GETTERS.

	float GetXScale () const
	{
		float a	= GetA();
		float b	= GetB();
		float s = sqrt( ( a * a + b * b ) ) / 1.0f;

		return s;
	}

	float GetYScale () const
	{
		float c	= GetC();
		float d	= GetD();
		float s = sqrt( ( d * d + c * c ) ) / 1.0f;

		return s;
	}

	float GetRotation () const
	{
		float a = GetA();
		float b = GetB();
		float r = atan2( b, a ); 

		return r;
	}

};

QIMP_END_NAMESPACE

#endif//__MATH_FLASH_MATRIX_H__