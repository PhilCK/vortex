#include <iostream>
#include <cmath>


class Quat
{
public:

	// Angle with axis.
	Quat(const float theta, const float setx, const float sety, const float setz)
	: w(setw)
	, x(setx)
	, y(sety)
	, z(setz)
	{
		
	}

	Quat() : Quat(1.f, 0.f, 0.f, 0.f) {}


	void toEuler() const
	{
		const float sqw  = w*w;
		const float sqx  = x*x;
		const float sqy  = y*y;
		const float sqz  = z*z;
		const float test = 2.0 * (y*w - x*z);

		auto equals = [](const double a, const double b, const double error) -> bool
		{
			return std::abs(std::abs(a) - std::abs(b)) <= error;
		};

		auto clamp = [](const double x, const double a, const double b)
		{
			return x < a ? a : (x > b ? b : x);
		};

		if (equals(test, 1.0, 0.000001))
		{
			// heading = rotation about z-axis
			float Z = (float) (-2.0*std::atan2(x, w));
			// bank = rotation about x-axis
			float X = 0;
			// attitude = rotation about y-axis
			float Y = (float) (3.1421/2.0);

			std::cout << X << ", " << Y << ", " << Z << std::endl;
		}
		else if (equals(test, -1.0, 0.000001))
		{
			// heading = rotation about z-axis
			float Z = (float) (2.0*std::atan2(x, w));
			// bank = rotation about x-axis
			float X = 0;
			// attitude = rotation about y-axis
			float Y = (float) (3.1421/-2.0);

			std::cout << X << ", " << Y << ", " << Z << std::endl;
		}
		else
		{
			// heading = rotation about z-axis
			float Z = (float) std::atan2(2.0 * (x*y +z*w),(sqx - sqy - sqz + sqw));
			// bank = rotation about x-axis
			float X = (float) std::atan2(2.0 * (y*z +x*w),(-sqx - sqy + sqz + sqw));
			// attitude = rotation about y-axis
			float Y = (float) std::asin( clamp(test, -1.0, 1.0) );

			std::cout << X << ", " << Y << ", " << Z << std::endl;
		}
	}


private:

	float w, x, y, z;

};


int main()
{
	std::cout << "QuickTest" << std::endl;

	Quat quat(3.142, 0, 0, 1);
	//Quat quat(0.952, 0, 0, 0.303);
	quat.toEuler();

	return 0;
}