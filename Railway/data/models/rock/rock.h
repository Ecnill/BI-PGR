#ifndef __ROCK_H
#define __STONE_H
#include <vector>

const int stoneNAttribsPerVertex = 8;
const int stoneNVertices = 76;
const int stoneNTriangles = 80;

const std::vector<float> stoneVertices = {
	0.0458717f,0.0492035f,-0.637814f,-0.087588f,-0.017884f,-0.995972f,0.573889f,0.240299f,
	0.445193f,-0.27776f,-0.705126f,0.166387f,-0.219031f,-0.961394f,0.516462f,0.364522f,
	-0.115244f,-0.381183f,-0.5787f,-0.116306f,-0.339305f,-0.933439f,0.452699f,0.225238f,
	0.722946f,-0.524087f,-0.446966f,0.621845f,-0.638478f,-0.453444f,0.465378f,0.44468f,
	0.848459f,0.00348933f,-0.504127f,0.803949f,0.100284f,-0.586138f,0.609159f,0.444428f,
	-0.45726f,0.0270171f,-0.595342f,-0.202765f,-0.037202f,-0.978484f,0.536173f,0.112488f,
	-0.132162f,0.461018f,-0.641052f,-0.024964f,0.407392f,-0.9129f,0.668167f,0.16877f,
	0.44727f,0.321441f,-0.638053f,0.28486f,0.412885f,-0.865078f,0.66833f,0.32522f,
	0.722946f,-0.524087f,-0.446966f,0.621845f,-0.638478f,-0.453444f,0.710921f,0.44468f,
	0.848459f,0.00348933f,-0.504127f,0.803949f,0.100284f,-0.586138f,0.840293f,0.457786f,
	0.93787f,-0.302001f,0.0123022f,0.929533f,-0.361736f,0.071169f,0.729457f,0.573059f,
	-0.224449f,-0.686441f,-0.344765f,-0.177465f,-0.854946f,-0.48735f,0.320549f,0.157507f,
	0.260314f,-0.697499f,-0.445319f,0.105472f,-0.836573f,-0.537584f,0.363086f,0.280708f,
	0.0311513f,-0.809787f,0.00295669f,0.023194f,-0.999146f,-0.033906f,0.261443f,0.22345f,
	-0.890678f,0.000345868f,-0.441387f,-0.800653f,-0.010529f,-0.598987f,0.921714f,0.135623f,
	-0.674257f,-0.483165f,-0.504799f,-0.499069f,-0.562487f,-0.659139f,0.998804f,0.241134f,
	-0.928549f,-0.295388f,-0.00557069f,-0.901273f,-0.427869f,0.068056f,0.850727f,0.255318f,
	-0.25371f,0.72401f,-0.360004f,-0.085543f,0.872311f,-0.481338f,0.606254f,0.705944f,
	-0.675952f,0.487804f,-0.515344f,-0.543657f,0.495895f,-0.677114f,0.664972f,0.811772f,
	-0.547923f,0.757224f,0.0017933f,-0.46971f,0.882778f,0.00647f,0.514898f,0.787042f,
	0.727849f,0.48588f,-0.252598f,0.685202f,0.643696f,-0.340739f,0.580523f,0.44468f,
	0.242124f,0.65324f,-0.345432f,0.248604f,0.835505f,-0.489975f,0.600592f,0.574443f,
	0.527959f,0.723403f,0.189876f,0.491073f,0.868313f,0.069399f,0.455463f,0.505049f,
	0.722946f,-0.524087f,-0.446966f,0.621845f,-0.638478f,-0.453444f,0.36086f,0.408793f,
	0.93787f,-0.302001f,0.0123022f,0.929533f,-0.361736f,0.071169f,0.235361f,0.482851f,
	0.517018f,-0.640853f,-0.00174783f,0.482223f,-0.873562f,0.065615f,0.261281f,0.357278f,
	-0.483681f,-0.673889f,-0.0262629f,-0.466994f,-0.884152f,0.012665f,0.234531f,0.096367f,
	-0.940298f,0.296133f,0.000117522f,-0.942259f,0.334239f,0.019318f,0.782553f,0.114937f,
	-0.00501097f,0.853085f,0.0655786f,0.037935f,0.999268f,0.001343f,0.486373f,0.646344f,
	0.727849f,0.48588f,-0.252598f,0.685202f,0.643696f,-0.340739f,0.954223f,0.550516f,
	0.527959f,0.723403f,0.189876f,0.491073f,0.868313f,0.069399f,1.0f,0.679753f,
	0.871982f,0.292221f,0.145802f,0.933226f,0.349528f,0.082766f,0.875244f,0.64098f,
	0.270372f,-0.707747f,0.432713f,0.313852f,-0.889187f,0.332835f,0.157199f,0.297471f,
	0.650296f,-0.395897f,0.524804f,0.669027f,-0.537339f,0.513443f,0.114379f,0.412447f,
	0.164574f,-0.45921f,0.845167f,0.152745f,-0.478774f,0.864528f,0.028193f,0.289056f,
	-0.625494f,-0.428093f,0.38423f,-0.664846f,-0.628803f,0.40315f,0.10499f,0.078468f,
	-0.208795f,-0.715494f,0.487306f,-0.273873f,-0.866451f,0.417402f,0.127821f,0.174038f,
	-0.403176f,-0.290697f,0.79784f,-0.441969f,-0.264382f,0.857173f,0.0f,0.149564f,
	-0.692439f,0.505448f,0.422798f,-0.60271f,0.574755f,0.553484f,0.201248f,0.836679f,
	-0.837497f,0.0107009f,0.493037f,-0.802667f,-0.026673f,0.595782f,0.068385f,0.842228f,
	-0.363373f,0.285169f,0.674312f,-0.270913f,0.389477f,0.880276f,0.149939f,0.726058f,
	0.239821f,0.711293f,0.450602f,0.091556f,0.807581f,0.582568f,0.316811f,0.612931f,
	-0.229281f,0.697608f,0.41776f,-0.175665f,0.786004f,0.5927f,0.281414f,0.731498f,
	0.177747f,0.397223f,0.653232f,-0.006836f,0.490585f,0.871334f,0.218998f,0.598307f,
	0.813406f,0.0426995f,0.514768f,0.861873f,-0.014618f,0.506882f,0.794794f,0.72289f,
	0.650292f,0.460465f,0.602666f,0.544328f,0.527665f,0.652089f,0.903933f,0.770326f,
	0.531425f,0.0434878f,0.78168f,0.419691f,0.06711f,0.905148f,0.795222f,0.794662f,
	0.260314f,-0.697499f,-0.445319f,0.105472f,-0.836573f,-0.537584f,0.39237f,0.337824f,
	-0.224449f,-0.686441f,-0.344765f,-0.177465f,-0.854946f,-0.48735f,0.363086f,0.211155f,
	0.44727f,0.321441f,-0.638053f,0.28486f,0.412885f,-0.865078f,0.94961f,0.445029f,
	-0.674257f,-0.483165f,-0.504799f,-0.499069f,-0.562487f,-0.659139f,0.390479f,0.087507f,
	-0.890678f,0.000345868f,-0.441387f,-0.800653f,-0.010529f,-0.598987f,0.499323f,0.0f,
	-0.132162f,0.461018f,-0.641052f,-0.024964f,0.407392f,-0.9129f,0.692243f,0.667166f,
	-0.675952f,0.487804f,-0.515344f,-0.543657f,0.495895f,-0.677114f,0.638725f,0.025445f,
	0.44727f,0.321441f,-0.638053f,0.28486f,0.412885f,-0.865078f,0.692523f,0.512429f,
	-0.928549f,-0.295388f,-0.00557069f,-0.901273f,-0.427869f,0.068056f,0.171273f,0.0f,
	-0.483681f,-0.673889f,-0.0262629f,-0.466994f,-0.884152f,0.012665f,0.914236f,0.337575f,
	-0.674257f,-0.483165f,-0.504799f,-0.499069f,-0.562487f,-0.659139f,0.319367f,0.048335f,
	-0.547923f,0.757224f,0.0017933f,-0.46971f,0.882778f,0.00647f,0.744625f,0.0f,
	-0.692439f,0.505448f,0.422798f,-0.60271f,0.574755f,0.553484f,0.66833f,0.108933f,
	-0.675952f,0.487804f,-0.515344f,-0.543657f,0.495895f,-0.677114f,0.892225f,0.008256f,
	0.239821f,0.711293f,0.450602f,0.091556f,0.807581f,0.582568f,0.392637f,0.58292f,
	0.650296f,-0.395897f,0.524804f,0.669027f,-0.537339f,0.513443f,0.692523f,0.702948f,
	-0.837497f,0.0107009f,0.493037f,-0.802667f,-0.026673f,0.595782f,0.702041f,0.236722f,
	-0.625494f,-0.428093f,0.38423f,-0.664846f,-0.628803f,0.40315f,0.784659f,0.326554f,
	-0.229281f,0.697608f,0.41776f,-0.175665f,0.786004f,0.5927f,0.407559f,0.705829f,
	-0.547923f,0.757224f,0.0017933f,-0.46971f,0.882778f,0.00647f,0.31258f,0.842193f,
	0.650292f,0.460465f,0.602666f,0.544328f,0.527665f,0.652089f,0.363086f,0.472086f,
	0.164574f,-0.45921f,0.845167f,0.152745f,-0.478774f,0.864528f,0.0f,0.54379f,
	0.531425f,0.0434878f,0.78168f,0.419691f,0.06711f,0.905148f,0.150817f,0.482851f,
	0.057967f,0.00957814f,0.853376f,-0.025422f,0.217902f,0.975616f,0.099982f,0.594397f,
	0.531425f,0.0434878f,0.78168f,0.419691f,0.06711f,0.905148f,0.000337f,0.407321f,
	0.813406f,0.0426995f,0.514768f,0.861873f,-0.014618f,0.506882f,0.07368f,0.47578f,
	-0.403176f,-0.290697f,0.79784f,-0.441969f,-0.264382f,0.857173f,0.000894f,0.697496f,
	-0.403176f,-0.290697f,0.79784f,-0.441969f,-0.264382f,0.857173f,0.679723f,0.337033f,
	0.650292f,0.460465f,0.602666f,0.544328f,0.527665f,0.652089f,0.274471f,0.4871f,
}; // end stoneVertices

const std::vector<unsigned> stoneTriangles = {
	0, 1, 2,
	3, 1, 4,
	0, 2, 5,
	0, 5, 6,
	0, 6, 7,
	8, 9, 10,
	11, 12, 13,
	14, 15, 16,
	17, 18, 19,
	20, 21, 22,
	23, 24, 25,
	11, 13, 26,
	14, 16, 27,
	17, 19, 28,
	29, 30, 31,
	32, 33, 34,
	35, 36, 37,
	38, 39, 40,
	41, 42, 43,
	44, 45, 46,
	2, 47, 48,
	2, 1, 47,
	1, 3, 47,
	9, 49, 29,
	4, 1, 7,
	1, 0, 7,
	5, 50, 51,
	5, 2, 50,
	2, 48, 50,
	52, 18, 17,
	6, 5, 53,
	5, 51, 53,
	54, 21, 20,
	54, 52, 21,
	52, 17, 21,
	10, 31, 44,
	10, 9, 31,
	9, 29, 31,
	13, 25, 32,
	13, 12, 25,
	12, 23, 25,
	55, 26, 35,
	16, 15, 56,
	57, 11, 26,
	58, 27, 59,
	58, 60, 27,
	60, 14, 27,
	22, 28, 61,
	22, 21, 28,
	21, 17, 28,
	25, 33, 32,
	25, 24, 33,
	10, 44, 62,
	26, 36, 35,
	26, 13, 36,
	13, 32, 36,
	27, 63, 59,
	27, 16, 63,
	16, 64, 63,
	28, 65, 61,
	28, 19, 65,
	66, 38, 42,
	31, 45, 44,
	31, 30, 45,
	22, 61, 67,
	68, 69, 70,
	34, 33, 71,
	33, 72, 71,
	73, 68, 70,
	37, 36, 34,
	36, 32, 34,
	40, 73, 70,
	40, 39, 73,
	63, 64, 74,
	43, 40, 70,
	43, 42, 40,
	42, 38, 40,
	69, 43, 70,
	69, 75, 43,
	75, 41, 43,
};

#endif // __STONE_H