#include "pch.h"
#include "Matrix.h"
#include "Math.h"

Matrix44::Matrix44()
{
	v[0][0] = 1;
	v[1][1] = 1;
	v[2][2] = 1;
	v[3][3] = 1;
}

Matrix44::Matrix44(double m[4][4])
{
	v[0][0] = m[0][0]; v[0][1] = m[0][1]; v[0][2] = m[0][2]; v[0][3] = m[0][3];
	v[1][0] = m[1][0]; v[1][1] = m[1][1]; v[1][2] = m[1][2]; v[1][3] = m[1][3];
	v[2][0] = m[2][0]; v[2][1] = m[2][1]; v[2][2] = m[2][2]; v[2][3] = m[2][3];
	v[3][0] = m[3][0]; v[3][1] = m[3][1]; v[3][2] = m[3][2]; v[3][3] = m[3][3];
}

Matrix44::Matrix44(double m[16])
{
	v[0][0] = m[0]; v[0][1] = m[1]; v[0][2] = m[2]; v[0][3] = m[3];
	v[1][0] = m[4]; v[1][1] = m[5]; v[1][2] = m[6]; v[1][3] = m[7];
	v[2][0] = m[8]; v[2][1] = m[9]; v[2][2] = m[10]; v[2][3] = m[11];
	v[3][0] = m[12]; v[3][1] = m[13]; v[3][2] = m[14]; v[3][3] = m[15];
}

Matrix44::Matrix44(Vector4 vec[4])
{
	v[0] = vec[0];
	v[1] = vec[1];
	v[2] = vec[2];
	v[3] = vec[3];
}

Matrix44::Matrix44(const Matrix44& m)
{
	v[0] = m[0];
	v[1] = m[1];
	v[2] = m[2];
	v[3] = m[3];
}

Matrix44 Matrix44::operator*(const Matrix44& c)
{
	return Matrix44(*this) *= c;
}

Vector4 Matrix44::operator*(const Vector4& c)
{
	Vector4 result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = v[i].Dot(c);
	}
	return result;

}

Matrix44 Matrix44::operator*(double f)
{
	Matrix44 result;

	result[0] *= f;
	result[1] *= f;
	result[2] *= f;
	result[3] *= f;

	return result;
}

Matrix44& Matrix44::operator*=(const Matrix44& c)
{
	Matrix44 temp(*this);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			double t = 0;
			for (int k = 0; k < 4; k++)
				t += temp[i][k] * c[k][j];
			v[i][j] = t;
		}
	}
	return *this;
}

bool Matrix44::operator==(const Matrix44& m) const
{
	return  v[0][0] == m[0][0] && v[0][1] == m[0][1] && v[0][2] == m[0][2] && v[0][3] == m[0][3] &&
			v[1][0] == m[1][0] && v[1][1] == m[1][1] && v[1][2] == m[1][2] && v[1][3] == m[1][3] &&
			v[2][0] == m[2][0] && v[2][1] == m[2][1] && v[2][2] == m[2][2] && v[2][3] == m[2][3] &&
			v[3][0] == m[3][0] && v[3][1] == m[3][1] && v[3][2] == m[3][2] && v[3][3] == m[3][3];

}

void Matrix44::MakeIdentity()
{
	v[0][0] = 1; v[0][1] = 0; v[0][2] = 0; v[0][3] = 0;
	v[1][0] = 0; v[1][1] = 1; v[1][2] = 0; v[1][3] = 0;
	v[2][0] = 0; v[2][1] = 0; v[2][2] = 1; v[2][3] = 0;
	v[3][0] = 0; v[3][1] = 0; v[3][2] = 0; v[3][3] = 1;
}

void Matrix44::MakeTranslate(const Vector3& c)
{
	v[0][0] = 1; v[0][1] = 0; v[0][2] = 0; v[0][3] = c[0];
	v[1][0] = 0; v[1][1] = 1; v[1][2] = 0; v[1][3] = c[1];
	v[2][0] = 0; v[2][1] = 0; v[2][2] = 1; v[2][3] = c[2];
	v[3][0] = 0; v[3][1] = 0; v[3][2] = 0; v[3][3] = 1;
}

void Matrix44::MakeTranslate(double x, double y, double z)
{
	v[0][0] = 1; v[0][1] = 0; v[0][2] = 0; v[0][3] = x;
	v[1][0] = 0; v[1][1] = 1; v[1][2] = 0; v[1][3] = y;
	v[2][0] = 0; v[2][1] = 0; v[2][2] = 1; v[2][3] = z;
	v[3][0] = 0; v[3][1] = 0; v[3][2] = 0; v[3][3] = 1;
}

void Matrix44::MakeRotateDeg(double angle, Vector3 axis)
{
	MakeRotateRad(DEG2RAD(angle), axis);
}

void Matrix44::MakeRotateRad(double angle, Vector3 axis)
{
	axis.Normalize();

	double radians = angle;
	double c = cos(radians);
	double s = sin(radians);
	double t = 1 - c;
	v[0][0] = t * axis[0] * axis[0] + c;
	v[0][1] = t * axis[0] * axis[1] - s * axis[2];
	v[0][2] = t * axis[2] * axis[0] + s * axis[1];
	v[0][3] = 0;
	v[1][0] = t * axis[0] * axis[1] + s * axis[2];
	v[1][1] = t * axis[1] * axis[1] + c;
	v[1][2] = t * axis[1] * axis[2] - s * axis[0];
	v[1][3] = 0;
	v[2][0] = t * axis[2] * axis[0] - s * axis[1];
	v[2][1] = t * axis[1] * axis[2] + s * axis[0];
	v[2][2] = t * axis[2] * axis[2] + c;
	v[2][3] = 0;
	v[3][0] = 0;
	v[3][1] = 0;
	v[3][2] = 0;
	v[3][3] = 1;
}

void Matrix44::MakeRotateDeg(double angle, double x, double y, double z)
{
	MakeRotateDeg(angle, Vector3(x, y, z));
}

void Matrix44::MakeRotateRad(double angle, double x, double y, double z)
{
	MakeRotateRad(angle, Vector3(x, y, z));
}

void Matrix44::MakeScale(const Vector3& c)
{
	v[0][0] = c[0];		v[0][1] = 0;		v[0][2] = 0;		v[0][3] = 0;
	v[1][0] = 0;		v[1][1] = c[1];		v[1][2] = 0;		v[1][3] = 0;
	v[2][0] = 0;		v[2][1] = 0;		v[2][2] = c[2];		v[2][3] = 0;
	v[3][0] = 0;		v[3][1] = 0;		v[3][2] = 0;		v[3][3] = 1;
}

void Matrix44::MakeScale(double x, double y, double z)
{
	v[0][0] = x;	v[0][1] = 0;	v[0][2] = 0;	v[0][3] = 0;
	v[1][0] = 0;	v[1][1] = y;	v[1][2] = 0;	v[1][3] = 0;
	v[2][0] = 0;	v[2][1] = 0;	v[2][2] = z;	v[2][3] = 0;
	v[3][0] = 0;	v[3][1] = 0;	v[3][2] = 0;	v[3][3] = 1;
}

/*
struct DetCalc
{
	DetCalc(Matrix44& m):
		mat(m)
	{
		cols[0] = cols[1] = cols[2] = cols[3] = false;
		rows[0] = rows[1] = rows[2] = rows[3] = false;
	}
	Matrix44& mat;
	bool cols[4];
	bool rows[4];
	double Det4()
	{
		return mat[0][0]*Det3(1,2,3,1,2,3) - mat[0][1]*Det3(1,2,3,0,2,3) + mat[0][2]*Det3(1,2,3,0,1,3) - mat[0][3]*Det3(1,2,3,0,1,2);
	}
	double Det3(int i1,int i2,int i3,int j1,int j2,int j3)
	{
		return mat[i1][j1]*Det2(i2,i3,j2,j3)-mat[i1][j2]*Det2(i2,i3,j1,j3)+mat[i1][j3]*Det2(i2,i3,j1,j2);
	}
	double Det2(int i1,int i2,int j1,int j2)
	{
		return mat[i1][j1]*mat[i2][j2]-mat[i1][j2]*mat[i2][j1];
	}
	double operator()(int row,int col,int left=3)
	{
		if(left == 1)
			return mat[row][col];

		double sum = 0;
		rows[row] = true;
		cols[col] = true;
		bool even = true;
		for(int i=0;i<4;i++)
		{
			if(rows[i]) continue;
			for(int j=0;j<4;j++)
			{
				if(cols[j]) continue;
				double det = mat[i][j] * (*this)(i,j,left-1);
				if(!even)
					det *= -1;

				sum += det;

				even = !even;
			}
		}
		rows[row] = false;
		cols[col] = false;
		return sum;
	}
};
//negatives are jacked

void Matrix44::Invert()
{
	//(A^-1)ij = Aji/detA
	//Aji = (-1)^(i+j)*detA(removing i,j)
	DetCalc calc(*this);

	double det = 0;
	//for(int i=0;i<4;i++)
	//	det += calc(i,0) * v[i][0] * (i%2?-1:1);

	det = calc.Det4();
	std::cerr<<"Determinant: "<<det<<std::endl;
	int i1[]={1,0,0,0};
	int i2[]={2,2,1,1};
	int i3[]={3,3,3,2};
	int j1[]={1,0,0,0};
	int j2[]={2,2,1,1};
	int j3[]={3,3,3,2};
	Matrix44 result;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			DetCalc calc(*this);
			double val = calc.Det3(i1[i],i2[i],i3[i],j1[j],j2[j],j3[j]);
			//result[i][j] = calc(i,j)/det;
			result[i][j] = val/det;
			if((i + j) % 2)
				result[i][j] *= -1;
		}
	}
	*this = result;
}
*/
double Matrix44::Det4() const
{
	return v[0][0] * Det3(1, 2, 3, 1, 2, 3) - v[0][1] * Det3(1, 2, 3, 0, 2, 3) + v[0][2] * Det3(1, 2, 3, 0, 1, 3) - v[0][3] * Det3(1, 2, 3, 0, 1, 2);
}

double Matrix44::Det3(int i1, int i2, int i3, int j1, int j2, int j3) const
{
	return v[i1][j1] * Det2(i2, i3, j2, j3) - v[i1][j2] * Det2(i2, i3, j1, j3) + v[i1][j3] * Det2(i2, i3, j1, j2);
}

double Matrix44::Det2(int i1, int i2, int j1, int j2) const
{
	return v[i1][j1] * v[i2][j2] - v[i1][j2] * v[i2][j1];
}

bool Matrix44::Invert()
{
	double det = Det4();
	if (Equals(det, 0))
	{
		return false;
	}
	double inverseDet = 1.0 / det;
	double d[4][4][4][4];
	for (int i1 = 0; i1 < 4; i1++)
		for (int i2 = 0; i2 < 4; i2++)
			for (int j1 = 0; j1 < 4; j1++)
				for (int j2 = 0; j2 < 4; j2++)
					d[i1][i2][j1][j2] = Det2(i1, i2, j1, j2);
	Matrix44 result;
	result[0][0] = v[1][1] * d[2][3][2][3] - v[1][2] * d[2][3][1][3] + v[1][3] * d[2][3][1][2];
	result[0][1] = v[1][0] * d[2][3][2][3] - v[1][2] * d[2][3][0][3] + v[1][3] * d[2][3][0][2];
	result[0][2] = v[1][0] * d[2][3][1][3] - v[1][1] * d[2][3][0][3] + v[1][3] * d[2][3][0][1];
	result[0][3] = v[1][0] * d[2][3][1][2] - v[1][1] * d[2][3][0][2] + v[1][2] * d[2][3][0][1];

	result[1][0] = v[0][1] * d[2][3][2][3] - v[0][2] * d[2][3][1][3] + v[0][3] * d[2][3][1][2];
	result[1][1] = v[0][0] * d[2][3][2][3] - v[0][2] * d[2][3][0][3] + v[0][3] * d[2][3][0][2];
	result[1][2] = v[0][0] * d[2][3][1][3] - v[0][1] * d[2][3][0][3] + v[0][3] * d[2][3][0][1];
	result[1][3] = v[0][0] * d[2][3][1][2] - v[0][1] * d[2][3][0][2] + v[0][2] * d[2][3][0][1];

	result[2][0] = v[0][1] * d[1][3][2][3] - v[0][2] * d[1][3][1][3] + v[0][3] * d[1][3][1][2];
	result[2][1] = v[0][0] * d[1][3][2][3] - v[0][2] * d[1][3][0][3] + v[0][3] * d[1][3][0][2];
	result[2][2] = v[0][0] * d[1][3][1][3] - v[0][1] * d[1][3][0][3] + v[0][3] * d[1][3][0][1];
	result[2][3] = v[0][0] * d[1][3][1][2] - v[0][1] * d[1][3][0][2] + v[0][2] * d[1][3][0][1];

	result[3][0] = v[0][1] * d[1][2][2][3] - v[0][2] * d[1][2][1][3] + v[0][3] * d[1][2][1][2];
	result[3][1] = v[0][0] * d[1][2][2][3] - v[0][2] * d[1][2][0][3] + v[0][3] * d[1][2][0][2];
	result[3][2] = v[0][0] * d[1][2][1][3] - v[0][1] * d[1][2][0][3] + v[0][3] * d[1][2][0][1];
	result[3][3] = v[0][0] * d[1][2][1][2] - v[0][1] * d[1][2][0][2] + v[0][2] * d[1][2][0][1];

	*this = result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((i + j) % 2 == 1)
				v[i][j] *= -1;
			v[i][j] *= inverseDet;
		}
	}
	Transpose();
	return true;
}

void Matrix44::Transpose()
{
	std::swap(v[0][1], v[1][0]);
	std::swap(v[0][2], v[2][0]);
	std::swap(v[1][2], v[2][1]);
	std::swap(v[0][3], v[3][0]);
	std::swap(v[1][3], v[3][1]);
	std::swap(v[2][3], v[3][2]);
}

Matrix44 Matrix44::GetInverse()
{
	Matrix44 x(*this);
	x.Invert();
	return x;
}

Matrix44 Matrix44::GetTranspose()
{
	Matrix44 x(*this);
	x.Transpose();
	return x;
}

//double Matrix44::Det3()
//{
//	return (v[0][0] * (v[1][1] * v[2][2] - v[2][1] * v[1][2]) -
//		v[0][1] * (v[1][0] * v[2][2] - v[2][0] * v[1][2]) +
//		v[0][2] * (v[1][0] * v[2][1] - v[2][0] * v[1][1]));
//}

std::ostream& operator<<(std::ostream& out, const Matrix44& m)
{
	return out << m[0] << std::endl << m[1] << std::endl << m[2] << std::endl << m[3] << std::endl;
}

std::istream& operator>>(std::istream& in, Matrix44& m)
{
	return in >> m[0] >> m[1] >> m[2] >> m[3];
}