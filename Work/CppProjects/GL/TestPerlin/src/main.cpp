
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cnt/BMPImage.h>
#include <rnd/Perlin.h>
#include <cnt/StringBuilder.h>

void test()
{
	rnd::Perlin perlinA(std::random_device{});
    rnd::Perlin perlinB;

	std::array<std::uint8_t, perlinA.size()> state;
	perlinA.toArray(state);
	perlinB.fromArray(state);

	assert(perlinA.accumulatedOctaveNoise3D(0.1, 0.2, 0.3, 4)
		== perlinB.accumulatedOctaveNoise3D(0.1, 0.2, 0.3, 4));

	perlinA.reseed(12342024);
	perlinB.reseed(12342024);

	assert(perlinA.accumulatedOctaveNoise3D(0.1, 0.2, 0.3, 4)
		== perlinB.accumulatedOctaveNoise3D(0.1, 0.2, 0.3, 4));

	perlinA.reseed(std::mt19937{ 12342024 });
	perlinB.reseed(std::mt19937{ 12342024 });

	assert(perlinA.accumulatedOctaveNoise3D(0.1, 0.2, 0.3, 4)
		== perlinB.accumulatedOctaveNoise3D(0.1, 0.2, 0.3, 4));
}

int main()
{
    //test without img serialization
	test();

	std::cout << "---------------------------------------" << std::endl;
	std::cout << "* Frequency range [0.1 .. 8.0 .. 64.0] " << std::endl;
	std::cout << "* Octaves range   [1 .. 8 .. 16]       " << std::endl;
	std::cout << "* seed            [0 .. 2^32-1]        " << std::endl;
	std::cout << "---------------------------------------" << std::endl;

	for (;;)
	{
		double frequency;
		std::cout << "Enter frequency [2] = ";
		std::cin >> frequency;
		frequency = std::clamp(frequency, 0.1, 64.0);

		std::int32_t octaves;
		std::cout << "Enter number of octaves [4]= ";
		std::cin >> octaves;
		octaves = std::clamp(octaves, 1, 16);

		std::uint32_t seed;
		std::cout << "Enter seed (any value) = ";
		std::cin >> seed;

		const rnd::Perlin perlin(seed);
        //create BMP image and store it on RAMDisk
        cnt::BMPImage image(4*perlin.size(), 4*perlin.size());
        
		const double fx = image.width() / frequency;
		const double fy = image.height() / frequency;

		for (td::UINT4 y = 0; y < image.height(); ++y)
		{
			for (td::UINT4 x = 0; x < image.width(); ++x)
			{
                //generate perlin noise at x,y point
				float perlinNoiseAtXandY = perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves);
                //put it in image (just for visualization purposes)
				image.set(x, y, perlinNoiseAtXandY);
			}
		}

		cnt::StringBuilderSmall ss;
#ifdef MU_MACOS
        ss << "/Volumes/RAMDisk/";
#else
        ss << "R:/";
#endif
        
		ss << 'f' << frequency << 'o' << octaves << '_' << seed << ".png";
        
        td::String str = ss.toString();

        //if (image.save(str))
        if (image.save(str, cnt::BMPImage::Type::PNGr)) //save single channel
		{
            std::cout << "...saved \"" << str << "\"" << std::endl;
		}
		else
		{
            std::cout << "...failed" << std::endl;
		}

		char c;
		std::cout << "To create another image enter [y], [y/n]?:";
		std::cin >> c;
		if (c != 'y')
            break;
		std::cout << std::endl;
	}
}
