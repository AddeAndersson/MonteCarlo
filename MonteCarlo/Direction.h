#pragma once
class Direction
{
	public:
		Direction(double in_x, double in_y, double in_z);
		~Direction();
		void Normalize();

	private:
		double x, y, z;
		
};

