
#define M_PI 3.14159265358979323846

union v2
{
	struct{ float x, y; };
	struct{ float a, b; };
	float data[2];
};


inline v2
operator +(v2 inca, v2 incb)
{
	v2 Return;

	Return.x = inca.x + incb.x;
	Return.y = inca.y + incb.y;

	return(Return);
}

inline v2
operator -(v2 inca, v2 incb)
{
	v2 Return;

	Return.x = inca.x - incb.x;
	Return.y = inca.y - incb.y;

	return(Return);
}

inline v2
operator *(v2 inca, v2 incb)
{
	v2 Return;

	Return.x = inca.x * incb.x;
	Return.y = inca.y * incb.y;

	return(Return);
}

inline v2
operator *(v2 inca, float incb)
{
	v2 Return;

	Return.x = inca.x * incb;
	Return.y = inca.y * incb;

	return(Return);
}

inline v2
operator *(float incb, v2 inca)
{
	v2 Return;

	Return.x = inca.x * incb;
	Return.y = inca.y * incb;

	return(Return);
}

inline v2
operator /(v2 inca, float incb)
{
	v2 Return;

	Return.x = inca.x / incb;
	Return.y = inca.y / incb;

	return(Return);
}

union v3
{
	struct{ float x, y, z; };
	struct{ float a, b, c; };
	float data[3];
};



inline v3
operator -(v3 inca, v3 incb)
{
	v3 Return;

	Return.x = inca.x - incb.x;
	Return.y = inca.y - incb.y;
	Return.z = inca.z - incb.z;

	return(Return);
}

union
v2i
{
	struct{ int x, y; };
	struct{ int a, b; };
	int data[2];
};

inline void
clamp(int &value, int min, int max)
{
	value > max ? value = max : (value < min ? value = min : value = value);
}

inline void
clamp(float &value, float min, float max)
{
	value > max ? value = max : (value < min ? value = min : value = value);
}

inline void
clamp_wrap(int &value, int min, int max)
{
	value > max ? value = min + value - max : (value < min ? value = max - value + min : value = value);
}

inline void
clamp_wrap(float &value, float min, float max)
{
	value > max ? value = min + value - max : (value < min ? value = max - value + min : value = value);
}

inline void
clamp_v2magnitude(v2 &value, float min, float max)
{
	float magnitude = sqrt(value.x * value.x + value.y * value.y);
	if(magnitude > max)
	{
		value.x = (value.x/magnitude) * max;
		value.y = (value.y/magnitude) * max;
	} else if(magnitude < min)
	{
		value.x = (value.x/magnitude) * min;
		value.y = (value.y/magnitude) * min;
	}
}

inline bool
valueinside(float value, float min, float max)
{
	bool Return = false;

	if( value < max && value > min)
	{
		Return = true;
	}

	return(Return);
}

inline void
rotatev2(v2 &value, float tempsin, float tempcos)
{
	float tempx = value.x;
	value.x = value.x * tempcos - value.y * tempsin;
	value.y = tempx * tempsin + value.y * tempcos;
}

inline float
distancev2(v2 &vec0, v2 &vec1)
{
	float x = vec0.x - vec1.x;
	float y = vec0.y - vec1.y;
	return(sqrt(x*x + y*y));
}

bool
within_distance(v2 *inca, v2 *incb, float range)
{
	bool Return;
	float distance = sqrt(pow((inca->x-incb->x),2)+pow(inca->y-incb->y,2));

	Return = distance < range;

	return(Return);
}

v3
CrossProduct(v3 u, v3 v)
{
	v3 Return = { 	u.y * v.z - u.z * v.y,
					u.x * v.z - u.z * v.x,
					u.x * v.y - u.y * v.x };
	return(Return);
}

float
DotProduct(v3 u, v3 v)
{
	float Return = u.x * v.x + u.y * v.y + u.z * v.z;
	return(Return);
}

bool
SameSide(v3 p1, v3 p2, v3 a, v3 b)
{
	bool Return = false;
    v3 cp1 = CrossProduct(b-a, p1-a);
	v3 cp2 = CrossProduct(b-a, p2-a);
    if( DotProduct(cp1,cp2) >= 0)
    {
    	Return = true;
    }
    return(Return);
}

bool
PointInTriangle(v2 incpoint, v2 incvecA, v2 incvecB, v2 incvecC)
{
	bool Return  = false;

	v3 p = {incpoint.x,incpoint.y, 0};
	v3 a = {incvecA.x,incvecA.y,0};
	v3 b = {incvecB.x,incvecB.y,0};
	v3 c = {incvecC.x,incvecC.y,0};

	if ( SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b) )
    {
    	Return = true;
    }
    
    return(Return);
}
