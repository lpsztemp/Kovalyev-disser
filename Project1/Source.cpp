#include <iostream>
#include <cmath>
using namespace std;
//https://en.cppreference.com/w/cpp/numeric/math/cos

const double PI = 3.1415926535897932384626433832795;

struct complex
{
	double r, i;
};

struct matrix
{
	/*cos*/double a00; /*-sin*/double a01;
	/*sin*/double a10; /*cos*/ double a11;
};

/*
https://en.cppreference.com/w/c/language/for
https://en.cppreference.com/w/c/language/if

*/

void fft(const complex* time_domain, unsigned size, complex* freq_domain, unsigned step)
{
	if (size == 1)
		freq_domain[0] = time_domain[0];
	else
	{
		fft(time_domain, size/2, freq_domain + 0, 2 * step);
		fft(time_domain + step, size/2, freq_domain + size / 2, 2 * step);
		for (unsigned k = 0; k < size / 2; k = k + 1)
		{
			complex t = freq_domain[k];
			double angle = 2 * PI * k / size;
			matrix T =
			{
				cos(angle), -sin(angle),
				sin(angle), cos(angle)
			};
			freq_domain[k] =
			{
				t.r + freq_domain[k + size / 2].r * T.a00 + freq_domain[k + size / 2].i * T.a10,
				t.i + freq_domain[k + size / 2].r * T.a01 + freq_domain[k + size / 2].i * T.a11
			};
			freq_domain[k + size / 2] =
			{
				t.r - (freq_domain[k + size / 2].r * T.a00 + freq_domain[k + size / 2].i * T.a10),
				t.i - (freq_domain[k + size / 2].r * T.a01 + freq_domain[k + size / 2].i * T.a11)
			};
		}
	}
}

void fft_inplace(complex* data, unsigned size, unsigned step)
{
}

int main(int argc, char** argv)
{
	complex time[128];
	const unsigned size = sizeof(time) / sizeof(complex);
	double angle = 0;
	double angle_step = 2 * PI / size;
	for (unsigned i = 0; i < size / 2; i = i + 1)
	{
		time[i].r = i;
		time[i].i = 0;
		angle = angle + angle_step;
	}
	for (unsigned i = size / 2; i < size; i = i + 1)
	{
		time[i].r = size / 2 - (i - size / 2);
		time[i].i = 0;
		angle = angle + angle_step;
	}
	std::cout << "Time:\n";
	for (unsigned i = 0; i < size; i = i + 1)
	{
		cout << sqrt(time[i].r * time[i].r + time[i].i * time[i].i) << '\n';
	}
	complex freq[size];
	fft(time, size, freq, 1);
	//fft_inplace(time, size, 1);
	std::cout << "Magnitude:\n";
	for (unsigned i = 0; i < size; i = i + 1)
	{
		cout << sqrt(freq[i].r * freq[i].r + freq[i].i * freq[i].i) << '\n';
	}
	std::cout << "Phase:\n";
	for (unsigned i = 0; i < size; i = i + 1)
	{
		if (freq[i].r == 0)
		{
			if (freq[i].i == 0)
				cout << 0 << '\n';
			else
				cout << PI/2 << '\n';
		}else
			cout << atan(freq[i].i / freq[i].r) << '\n';
	}
	return 0;
}
