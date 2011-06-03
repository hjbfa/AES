#define PTHREAD
#include "aes.h"
char processing_filename[100]={0};
unsigned char s_box[256] = {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
 //F
unsigned char mix_colum[4][4] = {
	//1  2 	3  4
	{2,  3,  1, 1},{
	1,  2,  3, 1},{
	1,  1,  2, 3},{
	3,  1,  1, 2}
};
unsigned char inv_s_box[256] = {
0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

unsigned char inv_mix_colum[4][4] = {
	{0x0e, 0x0b, 0x0d, 0x09},
	{0x09, 0x0e, 0x0b, 0x0d},
	{0x0d, 0x09, 0x0e, 0x0b},
	{0x0b, 0x0d, 0x09, 0x0e}
};

unsigned long int t0[256]={
0xa56363c6,0x847c7cf8,0x997777ee,0x8d7b7bf6,0x0df2f2ff,0xbd6b6bd6,0xb16f6fde,0x54c5c591,0x50303060,0x03010102,0xa96767ce,0x7d2b2b56,0x19fefee7,0x62d7d7b5,0xe6abab4d,0x9a7676ec,0x45caca8f,0x9d82821f,0x40c9c989,0x877d7dfa,0x15fafaef,0xeb5959b2,0xc947478e,0x0bf0f0fb,0xecadad41,0x67d4d4b3,0xfda2a25f,0xeaafaf45,0xbf9c9c23,0xf7a4a453,0x967272e4,0x5bc0c09b,0xc2b7b775,0x1cfdfde1,0xae93933d,0x6a26264c,0x5a36366c,0x413f3f7e,0x02f7f7f5,0x4fcccc83,0x5c343468,0xf4a5a551,0x34e5e5d1,0x08f1f1f9,0x937171e2,0x73d8d8ab,0x53313162,0x3f15152a,0x0c040408,0x52c7c795,0x65232346,0x5ec3c39d,0x28181830,0xa1969637,0x0f05050a,0xb59a9a2f,0x0907070e,0x36121224,0x9b80801b,0x3de2e2df,0x26ebebcd,0x6927274e,0xcdb2b27f,0x9f7575ea,0x1b090912,0x9e83831d,0x742c2c58,0x2e1a1a34,0x2d1b1b36,0xb26e6edc,0xee5a5ab4,0xfba0a05b,0xf65252a4,0x4d3b3b76,0x61d6d6b7,0xceb3b37d,0x7b292952,0x3ee3e3dd,0x712f2f5e,0x97848413,0xf55353a6,0x68d1d1b9,0x00000000,0x2cededc1,0x60202040,0x1ffcfce3,0xc8b1b179,0xed5b5bb6,0xbe6a6ad4,0x46cbcb8d,0xd9bebe67,0x4b393972,0xde4a4a94,0xd44c4c98,0xe85858b0,0x4acfcf85,0x6bd0d0bb,0x2aefefc5,0xe5aaaa4f,0x16fbfbed,0xc5434386,0xd74d4d9a,0x55333366,0x94858511,0xcf45458a,0x10f9f9e9,0x06020204,0x817f7ffe,0xf05050a0,0x443c3c78,0xba9f9f25,0xe3a8a84b,0xf35151a2,0xfea3a35d,0xc0404080,0x8a8f8f05,0xad92923f,0xbc9d9d21,0x48383870,0x04f5f5f1,0xdfbcbc63,0xc1b6b677,0x75dadaaf,0x63212142,0x30101020,0x1affffe5,0x0ef3f3fd,0x6dd2d2bf,0x4ccdcd81,0x140c0c18,0x35131326,0x2fececc3,0xe15f5fbe,0xa2979735,0xcc444488,0x3917172e,0x57c4c493,0xf2a7a755,0x827e7efc,0x473d3d7a,0xac6464c8,0xe75d5dba,0x2b191932,0x957373e6,0xa06060c0,0x98818119,0xd14f4f9e,0x7fdcdca3,0x66222244,0x7e2a2a54,0xab90903b,0x8388880b,0xca46468c,0x29eeeec7,0xd3b8b86b,0x3c141428,0x79dedea7,0xe25e5ebc,0x1d0b0b16,0x76dbdbad,0x3be0e0db,0x56323264,0x4e3a3a74,0x1e0a0a14,0xdb494992,0x0a06060c,0x6c242448,0xe45c5cb8,0x5dc2c29f,0x6ed3d3bd,0xefacac43,0xa66262c4,0xa8919139,0xa4959531,0x37e4e4d3,0x8b7979f2,0x32e7e7d5,0x43c8c88b,0x5937376e,0xb76d6dda,0x8c8d8d01,0x64d5d5b1,0xd24e4e9c,0xe0a9a949,0xb46c6cd8,0xfa5656ac,0x07f4f4f3,0x25eaeacf,0xaf6565ca,0x8e7a7af4,0xe9aeae47,0x18080810,0xd5baba6f,0x887878f0,0x6f25254a,0x722e2e5c,0x241c1c38,0xf1a6a657,0xc7b4b473,0x51c6c697,0x23e8e8cb,0x7cdddda1,0x9c7474e8,0x211f1f3e,0xdd4b4b96,0xdcbdbd61,0x868b8b0d,0x858a8a0f,0x907070e0,0x423e3e7c,0xc4b5b571,0xaa6666cc,0xd8484890,0x05030306,0x01f6f6f7,0x120e0e1c,0xa36161c2,0x5f35356a,0xf95757ae,0xd0b9b969,0x91868617,0x58c1c199,0x271d1d3a,0xb99e9e27,0x38e1e1d9,0x13f8f8eb,0xb398982b,0x33111122,0xbb6969d2,0x70d9d9a9,0x898e8e07,0xa7949433,0xb69b9b2d,0x221e1e3c,0x92878715,0x20e9e9c9,0x49cece87,0xff5555aa,0x78282850,0x7adfdfa5,0x8f8c8c03,0xf8a1a159,0x80898909,0x170d0d1a,0xdabfbf65,0x31e6e6d7,0xc6424284,0xb86868d0,0xc3414182,0xb0999929,0x772d2d5a,0x110f0f1e,0xcbb0b07b,0xfc5454a8,0xd6bbbb6d,0x3a16162c
};
unsigned long int  t1[256]={
0x6363c6a5,0x7c7cf884,0x7777ee99,0x7b7bf68d,0xf2f2ff0d,0x6b6bd6bd,0x6f6fdeb1,0xc5c59154,0x30306050,0x01010203,0x6767cea9,0x2b2b567d,0xfefee719,0xd7d7b562,0xabab4de6,0x7676ec9a,0xcaca8f45,0x82821f9d,0xc9c98940,0x7d7dfa87,0xfafaef15,0x5959b2eb,0x47478ec9,0xf0f0fb0b,0xadad41ec,0xd4d4b367,0xa2a25ffd,0xafaf45ea,0x9c9c23bf,0xa4a453f7,0x7272e496,0xc0c09b5b,0xb7b775c2,0xfdfde11c,0x93933dae,0x26264c6a,0x36366c5a,0x3f3f7e41,0xf7f7f502,0xcccc834f,0x3434685c,0xa5a551f4,0xe5e5d134,0xf1f1f908,0x7171e293,0xd8d8ab73,0x31316253,0x15152a3f,0x0404080c,0xc7c79552,0x23234665,0xc3c39d5e,0x18183028,0x969637a1,0x05050a0f,0x9a9a2fb5,0x07070e09,0x12122436,0x80801b9b,0xe2e2df3d,0xebebcd26,0x27274e69,0xb2b27fcd,0x7575ea9f,0x0909121b,0x83831d9e,0x2c2c5874,0x1a1a342e,0x1b1b362d,0x6e6edcb2,0x5a5ab4ee,0xa0a05bfb,0x5252a4f6,0x3b3b764d,0xd6d6b761,0xb3b37dce,0x2929527b,0xe3e3dd3e,0x2f2f5e71,0x84841397,0x5353a6f5,0xd1d1b968,0x00000000,0xededc12c,0x20204060,0xfcfce31f,0xb1b179c8,0x5b5bb6ed,0x6a6ad4be,0xcbcb8d46,0xbebe67d9,0x3939724b,0x4a4a94de,0x4c4c98d4,0x5858b0e8,0xcfcf854a,0xd0d0bb6b,0xefefc52a,0xaaaa4fe5,0xfbfbed16,0x434386c5,0x4d4d9ad7,0x33336655,0x85851194,0x45458acf,0xf9f9e910,0x02020406,0x7f7ffe81,0x5050a0f0,0x3c3c7844,0x9f9f25ba,0xa8a84be3,0x5151a2f3,0xa3a35dfe,0x404080c0,0x8f8f058a,0x92923fad,0x9d9d21bc,0x38387048,0xf5f5f104,0xbcbc63df,0xb6b677c1,0xdadaaf75,0x21214263,0x10102030,0xffffe51a,0xf3f3fd0e,0xd2d2bf6d,0xcdcd814c,0x0c0c1814,0x13132635,0xececc32f,0x5f5fbee1,0x979735a2,0x444488cc,0x17172e39,0xc4c49357,0xa7a755f2,0x7e7efc82,0x3d3d7a47,0x6464c8ac,0x5d5dbae7,0x1919322b,0x7373e695,0x6060c0a0,0x81811998,0x4f4f9ed1,0xdcdca37f,0x22224466,0x2a2a547e,0x90903bab,0x88880b83,0x46468cca,0xeeeec729,0xb8b86bd3,0x1414283c,0xdedea779,0x5e5ebce2,0x0b0b161d,0xdbdbad76,0xe0e0db3b,0x32326456,0x3a3a744e,0x0a0a141e,0x494992db,0x06060c0a,0x2424486c,0x5c5cb8e4,0xc2c29f5d,0xd3d3bd6e,0xacac43ef,0x6262c4a6,0x919139a8,0x959531a4,0xe4e4d337,0x7979f28b,0xe7e7d532,0xc8c88b43,0x37376e59,0x6d6ddab7,0x8d8d018c,0xd5d5b164,0x4e4e9cd2,0xa9a949e0,0x6c6cd8b4,0x5656acfa,0xf4f4f307,0xeaeacf25,0x6565caaf,0x7a7af48e,0xaeae47e9,0x08081018,0xbaba6fd5,0x7878f088,0x25254a6f,0x2e2e5c72,0x1c1c3824,0xa6a657f1,0xb4b473c7,0xc6c69751,0xe8e8cb23,0xdddda17c,0x7474e89c,0x1f1f3e21,0x4b4b96dd,0xbdbd61dc,0x8b8b0d86,0x8a8a0f85,0x7070e090,0x3e3e7c42,0xb5b571c4,0x6666ccaa,0x484890d8,0x03030605,0xf6f6f701,0x0e0e1c12,0x6161c2a3,0x35356a5f,0x5757aef9,0xb9b969d0,0x86861791,0xc1c19958,0x1d1d3a27,0x9e9e27b9,0xe1e1d938,0xf8f8eb13,0x98982bb3,0x11112233,0x6969d2bb,0xd9d9a970,0x8e8e0789,0x949433a7,0x9b9b2db6,0x1e1e3c22,0x87871592,0xe9e9c920,0xcece8749,0x5555aaff,0x28285078,0xdfdfa57a,0x8c8c038f,0xa1a159f8,0x89890980,0x0d0d1a17,0xbfbf65da,0xe6e6d731,0x424284c6,0x6868d0b8,0x414182c3,0x999929b0,0x2d2d5a77,0x0f0f1e11,0xb0b07bcb,0x5454a8fc,0xbbbb6dd6,0x16162c3a
};

unsigned long int  t2[256]={
0x63c6a563,0x7cf8847c,0x77ee9977,0x7bf68d7b,0xf2ff0df2,0x6bd6bd6b,0x6fdeb16f,0xc59154c5,0x30605030,0x01020301,0x67cea967,0x2b567d2b,0xfee719fe,0xd7b562d7,0xab4de6ab,0x76ec9a76,0xca8f45ca,0x821f9d82,0xc98940c9,0x7dfa877d,0xfaef15fa,0x59b2eb59,0x478ec947,0xf0fb0bf0,0xad41ecad,0xd4b367d4,0xa25ffda2,0xaf45eaaf,0x9c23bf9c,0xa453f7a4,0x72e49672,0xc09b5bc0,0xb775c2b7,0xfde11cfd,0x933dae93,0x264c6a26,0x366c5a36,0x3f7e413f,0xf7f502f7,0xcc834fcc,0x34685c34,0xa551f4a5,0xe5d134e5,0xf1f908f1,0x71e29371,0xd8ab73d8,0x31625331,0x152a3f15,0x04080c04,0xc79552c7,0x23466523,0xc39d5ec3,0x18302818,0x9637a196,0x050a0f05,0x9a2fb59a,0x070e0907,0x12243612,0x801b9b80,0xe2df3de2,0xebcd26eb,0x274e6927,0xb27fcdb2,0x75ea9f75,0x09121b09,0x831d9e83,0x2c58742c,0x1a342e1a,0x1b362d1b,0x6edcb26e,0x5ab4ee5a,0xa05bfba0,0x52a4f652,0x3b764d3b,0xd6b761d6,0xb37dceb3,0x29527b29,0xe3dd3ee3,0x2f5e712f,0x84139784,0x53a6f553,0xd1b968d1,0x00000000,0xedc12ced,0x20406020,0xfce31ffc,0xb179c8b1,0x5bb6ed5b,0x6ad4be6a,0xcb8d46cb,0xbe67d9be,0x39724b39,0x4a94de4a,0x4c98d44c,0x58b0e858,0xcf854acf,0xd0bb6bd0,0xefc52aef,0xaa4fe5aa,0xfbed16fb,0x4386c543,0x4d9ad74d,0x33665533,0x85119485,0x458acf45,0xf9e910f9,0x02040602,0x7ffe817f,0x50a0f050,0x3c78443c,0x9f25ba9f,0xa84be3a8,0x51a2f351,0xa35dfea3,0x4080c040,0x8f058a8f,0x923fad92,0x9d21bc9d,0x38704838,0xf5f104f5,0xbc63dfbc,0xb677c1b6,0xdaaf75da,0x21426321,0x10203010,0xffe51aff,0xf3fd0ef3,0xd2bf6dd2,0xcd814ccd,0x0c18140c,0x13263513,0xecc32fec,0x5fbee15f,0x9735a297,0x4488cc44,0x172e3917,0xc49357c4,0xa755f2a7,0x7efc827e,0x3d7a473d,0x64c8ac64,0x5dbae75d,0x19322b19,0x73e69573,0x60c0a060,0x81199881,0x4f9ed14f,0xdca37fdc,0x22446622,0x2a547e2a,0x903bab90,0x880b8388,0x468cca46,0xeec729ee,0xb86bd3b8,0x14283c14,0xdea779de,0x5ebce25e,0x0b161d0b,0xdbad76db,0xe0db3be0,0x32645632,0x3a744e3a,0x0a141e0a,0x4992db49,0x060c0a06,0x24486c24,0x5cb8e45c,0xc29f5dc2,0xd3bd6ed3,0xac43efac,0x62c4a662,0x9139a891,0x9531a495,0xe4d337e4,0x79f28b79,0xe7d532e7,0xc88b43c8,0x376e5937,0x6ddab76d,0x8d018c8d,0xd5b164d5,0x4e9cd24e,0xa949e0a9,0x6cd8b46c,0x56acfa56,0xf4f307f4,0xeacf25ea,0x65caaf65,0x7af48e7a,0xae47e9ae,0x08101808,0xba6fd5ba,0x78f08878,0x254a6f25,0x2e5c722e,0x1c38241c,0xa657f1a6,0xb473c7b4,0xc69751c6,0xe8cb23e8,0xdda17cdd,0x74e89c74,0x1f3e211f,0x4b96dd4b,0xbd61dcbd,0x8b0d868b,0x8a0f858a,0x70e09070,0x3e7c423e,0xb571c4b5,0x66ccaa66,0x4890d848,0x03060503,0xf6f701f6,0x0e1c120e,0x61c2a361,0x356a5f35,0x57aef957,0xb969d0b9,0x86179186,0xc19958c1,0x1d3a271d,0x9e27b99e,0xe1d938e1,0xf8eb13f8,0x982bb398,0x11223311,0x69d2bb69,0xd9a970d9,0x8e07898e,0x9433a794,0x9b2db69b,0x1e3c221e,0x87159287,0xe9c920e9,0xce8749ce,0x55aaff55,0x28507828,0xdfa57adf,0x8c038f8c,0xa159f8a1,0x89098089,0x0d1a170d,0xbf65dabf,0xe6d731e6,0x4284c642,0x68d0b868,0x4182c341,0x9929b099,0x2d5a772d,0x0f1e110f,0xb07bcbb0,0x54a8fc54,0xbb6dd6bb,0x162c3a16
};

unsigned long int t3[256]={
0xc6a56363,0xf8847c7c,0xee997777,0xf68d7b7b,0xff0df2f2,0xd6bd6b6b,0xdeb16f6f,0x9154c5c5,0x60503030,0x02030101,0xcea96767,0x567d2b2b,0xe719fefe,0xb562d7d7,0x4de6abab,0xec9a7676,0x8f45caca,0x1f9d8282,0x8940c9c9,0xfa877d7d,0xef15fafa,0xb2eb5959,0x8ec94747,0xfb0bf0f0,0x41ecadad,0xb367d4d4,0x5ffda2a2,0x45eaafaf,0x23bf9c9c,0x53f7a4a4,0xe4967272,0x9b5bc0c0,0x75c2b7b7,0xe11cfdfd,0x3dae9393,0x4c6a2626,0x6c5a3636,0x7e413f3f,0xf502f7f7,0x834fcccc,0x685c3434,0x51f4a5a5,0xd134e5e5,0xf908f1f1,0xe2937171,0xab73d8d8,0x62533131,0x2a3f1515,0x080c0404,0x9552c7c7,0x46652323,0x9d5ec3c3,0x30281818,0x37a19696,0x0a0f0505,0x2fb59a9a,0x0e090707,0x24361212,0x1b9b8080,0xdf3de2e2,0xcd26ebeb,0x4e692727,0x7fcdb2b2,0xea9f7575,0x121b0909,0x1d9e8383,0x58742c2c,0x342e1a1a,0x362d1b1b,0xdcb26e6e,0xb4ee5a5a,0x5bfba0a0,0xa4f65252,0x764d3b3b,0xb761d6d6,0x7dceb3b3,0x527b2929,0xdd3ee3e3,0x5e712f2f,0x13978484,0xa6f55353,0xb968d1d1,0x00000000,0xc12ceded,0x40602020,0xe31ffcfc,0x79c8b1b1,0xb6ed5b5b,0xd4be6a6a,0x8d46cbcb,0x67d9bebe,0x724b3939,0x94de4a4a,0x98d44c4c,0xb0e85858,0x854acfcf,0xbb6bd0d0,0xc52aefef,0x4fe5aaaa,0xed16fbfb,0x86c54343,0x9ad74d4d,0x66553333,0x11948585,0x8acf4545,0xe910f9f9,0x04060202,0xfe817f7f,0xa0f05050,0x78443c3c,0x25ba9f9f,0x4be3a8a8,0xa2f35151,0x5dfea3a3,0x80c04040,0x058a8f8f,0x3fad9292,0x21bc9d9d,0x70483838,0xf104f5f5,0x63dfbcbc,0x77c1b6b6,0xaf75dada,0x42632121,0x20301010,0xe51affff,0xfd0ef3f3,0xbf6dd2d2,0x814ccdcd,0x18140c0c,0x26351313,0xc32fecec,0xbee15f5f,0x35a29797,0x88cc4444,0x2e391717,0x9357c4c4,0x55f2a7a7,0xfc827e7e,0x7a473d3d,0xc8ac6464,0xbae75d5d,0x322b1919,0xe6957373,0xc0a06060,0x19988181,0x9ed14f4f,0xa37fdcdc,0x44662222,0x547e2a2a,0x3bab9090,0x0b838888,0x8cca4646,0xc729eeee,0x6bd3b8b8,0x283c1414,0xa779dede,0xbce25e5e,0x161d0b0b,0xad76dbdb,0xdb3be0e0,0x64563232,0x744e3a3a,0x141e0a0a,0x92db4949,0x0c0a0606,0x486c2424,0xb8e45c5c,0x9f5dc2c2,0xbd6ed3d3,0x43efacac,0xc4a66262,0x39a89191,0x31a49595,0xd337e4e4,0xf28b7979,0xd532e7e7,0x8b43c8c8,0x6e593737,0xdab76d6d,0x018c8d8d,0xb164d5d5,0x9cd24e4e,0x49e0a9a9,0xd8b46c6c,0xacfa5656,0xf307f4f4,0xcf25eaea,0xcaaf6565,0xf48e7a7a,0x47e9aeae,0x10180808,0x6fd5baba,0xf0887878,0x4a6f2525,0x5c722e2e,0x38241c1c,0x57f1a6a6,0x73c7b4b4,0x9751c6c6,0xcb23e8e8,0xa17cdddd,0xe89c7474,0x3e211f1f,0x96dd4b4b,0x61dcbdbd,0x0d868b8b,0x0f858a8a,0xe0907070,0x7c423e3e,0x71c4b5b5,0xccaa6666,0x90d84848,0x06050303,0xf701f6f6,0x1c120e0e,0xc2a36161,0x6a5f3535,0xaef95757,0x69d0b9b9,0x17918686,0x9958c1c1,0x3a271d1d,0x27b99e9e,0xd938e1e1,0xeb13f8f8,0x2bb39898,0x22331111,0xd2bb6969,0xa970d9d9,0x07898e8e,0x33a79494,0x2db69b9b,0x3c221e1e,0x15928787,0xc920e9e9,0x8749cece,0xaaff5555,0x50782828,0xa57adfdf,0x038f8c8c,0x59f8a1a1,0x09808989,0x1a170d0d,0x65dabfbf,0xd731e6e6,0x84c64242,0xd0b86868,0x82c34141,0x29b09999,0x5a772d2d,0x1e110f0f,0x7bcbb0b0,0xa8fc5454,0x6dd6bbbb,0x2c3a1616
};
void sig_alrm(int sigo){
}
void key_expansion(unsigned char key[60][4]/*[43][4]*/, E_CLASS type, unsigned int loop)
{
	unsigned char temp[4] = {0};
	unsigned char temp1[4] = {0};
	unsigned char temp2[4] = {0};
	unsigned char xor[10][4]={
							 {0x01,0x00,0x00,0x00},\
							 {0x02,0x00,0x00,0x00},\
							 {0x04,0x00,0x00,0x00},\
							 {0x08,0x00,0x00,0x00},\
							 {0x10,0x00,0x00,0x00},\
							 {0x20,0x00,0x00,0x00},\
							 {0x40,0x00,0x00,0x00},\
							 {0x80,0x00,0x00,0x00},\
							 {0x1b,0x00,0x00,0x00},\
						     {0x36,0x00,0x00,0x00}};
	int j = 0;
	int i = loop;
	int end = loop+4;

/*	if (!right_type_and_loop (type, loop)){

		Error_report ("key expansion error");
		exit(0);
	}
*/
	if (type == BIT_128){
	while (i<end){
		while (j != 4) {//将需要运算的两行存入temp
			temp2[j] = key[i][j];
			temp1[j] = key[i-3][j];
			j++;
		}
		j = 0;

		//(i+1)%4==0时需要移位
		if (( i + 1 ) % 4 == 0 ){
			temp[0] = temp2[0];
			while (j!=3){
				temp2[j] = temp2[j+1];
				j++;
			}
			j = 0;
			temp2[3] = temp[0];
		}

		//s盒替换
		if (( i + 1 ) % 4 == 0 ){
			while (j!=4)
			{
				temp2[j] = s_box[temp2[j]];
				j++;
			}
			j = 0;
		}
		while (j != 4){//先存入temp1
			temp1[j] = temp2[j];
			j++;
		}
		j = 0;

		if ((i+1)%4==0)
		{
			while (j!=4){
				temp1[j] ^= xor[i/4][j];
				j++;
			}
			j = 0;
		}
		//与第i-3行 进行^运算
		while (j != 4){
			temp1[j] ^= key[i-3][j];
			j++;
		}
		j = 0;
		i++;//需要存入下一列
		while (j!=4)
		{
			key [i][j] = temp1[j];
			j++;
		}
		j = 0;
	}}

	if (type == BIT_192){
	while (i < end){
		while (j != 4) {//将需要运算的两行存入temp
			temp2[j] = key[i][j];
			temp1[j] = key[i-5][j];
			j++;
		}
		j = 0;

		//(i+1)%4==0时需要移位
		if (( i + 1 ) % 6 == 0 ){
			temp[0] = temp2[0];
			while (j!=3){
				temp2[j] = temp2[j+1];
				j++;
			}
			j = 0;
			temp2[3] = temp[0];
		}

		//s盒替换
		if (( i + 1 ) % 6 == 0 ){
			while (j!=4)
			{
				temp2[j] = s_box[temp2[j]];
				j++;
			}
			j = 0;
		}
		while (j != 4){//先存入temp1
			temp1[j] = temp2[j];
			j++;
		}
		j = 0;

		if ((i+1)%6==0)
		{
			while (j!=4){
				temp1[j] ^= xor[i/6][j];
				j++;
			}
			j = 0;
		}
		//与第i-5行 进行^运算
		while (j != 4){
			temp1[j] ^= key[i-5][j];
			j++;
		}
		j = 0;
		i++;//需要存入下一列
		while (j!=4)
		{
			key [i][j] = temp1[j];
			j++;
		}
		j = 0;
	}}
	if (type == BIT_256){
		while (i<end){
		while (j != 4) {//将需要运算的两行存入temp
			temp2[j] = key[i][j];
			temp1[j] = key[i-7][j];
			j++;
		}
		j = 0;

		//(i+1)%8==0时需要移位
		if (( i + 1 ) % 8 == 0 ){
			temp[0] = temp2[0];
			while (j!=3){
				temp2[j] = temp2[j+1];
				j++;
			}
			j = 0;
			temp2[3] = temp[0];
		}

		//s盒替换,四轮一次
		if (( i + 1 ) % 4 == 0 ){
			while (j!=4)
			{
				temp2[j] = s_box[temp2[j]];
				j++;
			}
			j = 0;
		}
		while (j != 4){//先存入temp1
			temp1[j] = temp2[j];
			j++;
		}
		j = 0;

		/*xor，8轮一次，存入temp1*/
		if ((i+1)%8==0)
		{
			while (j!=4){
				temp1[j] ^= xor[i/8][j];
				j++;
			}
			j = 0;
		}
		//与第i-7行 进行^运算
		while (j != 4){
			temp1[j] ^= key[i-7][j];
			j++;
		}
		j = 0;
		i++;//需要存入下一列
		while (j!=4)
		{
			key [i][j] = temp1[j];
			j++;
		}
		j = 0;
	}}
	i = 0;
	j = 0;
	return ;
}

void *aes_encrypt (void *input)
{
	AES_INFO *all_input = (AES_INFO *)input;
	unsigned int max_loop = 0;
	unsigned long h0, h1, h2, h3;
	unsigned char (*aes_input)[4] = all_input->input;
	/*初始化loop*/
	int loop = 0;
	all_input->loop = 0;
	switch (all_input->type){
	case BIT_128:
		max_loop = BIT_128_ENCRYPT_LOOP_STEP;
		break;
	case BIT_192:
		max_loop = BIT_192_ENCRYPT_LOOP_STEP;
		break;
	case BIT_256:
		max_loop = 	BIT_256_ENCRYPT_LOOP_STEP;
		break;
	default:
		Error_report ("wrong type");
		exit (0);
	}

	/*检查输入明文长度是否是128*/
	if (sizeof(all_input->input) != 4){
		Error_report ("wrong plain text length");
		exit(0);
	}

	add_round_key (all_input);
	for (loop=0; loop < max_loop; loop++){
		all_input->loop++;/*检查最后一次*/
/*		sub_bytes (all_input);
		shift_rows (all_input);
		mix_columns (all_input);
		add_round_key (all_input);
*/
		h0 = t0[aes_input[0][0]] ^ t1[aes_input[1][1]] ^ t2[aes_input[2][2]] ^ t3[aes_input[3][3]]/* ^ *(unsigned long *)(*(all_input->key_info->begin[4 * all_input->loop+0]))*/;
		h1 = t0[aes_input[1][0]] ^ t1[aes_input[2][1]] ^ t2[aes_input[3][2]] ^ t3[aes_input[0][3]];
		h2 = t0[aes_input[2][0]] ^ t1[aes_input[3][1]] ^ t2[aes_input[0][2]] ^ t3[aes_input[1][3]];
		h3 = t0[aes_input[3][0]] ^ t1[aes_input[0][1]] ^ t2[aes_input[1][2]] ^ t3[aes_input[2][3]];
		*(unsigned long int *)(*(aes_input+0)) = h0;
		*(unsigned long int *)(*(aes_input+1)) = h1;
		*(unsigned long int *)(*(aes_input+2)) = h2;
		*(unsigned long int *)(*(aes_input+3)) = h3;
		add_round_key (all_input);

	}
	all_input->loop++;
	sub_bytes_shift_rows (all_input);
//	shift_rows (all_input);
	add_round_key (all_input);
	pthread_exit((void *)0);
}
void key_expansion_wrap(KEY_INFO *key_info)
{
	int max_loop = 0;
	unsigned int *key_loop = &(key_info->key_loop);
	unsigned char (*key)[4] = key_info->key;
	E_CLASS *type = &(key_info->type);

	switch (key_info->type){
	case BIT_128:
		*key_loop = BIT_128_LOOP_START;
		max_loop = BIT_128_LOOP_END;
		break;
	case BIT_192:
		*key_loop = BIT_192_LOOP_START;
		max_loop = BIT_192_LOOP_END;
		break;
	case BIT_256:
		*key_loop = BIT_256_LOOP_START;
		max_loop = BIT_256_LOOP_END;
		break;
	default:
		Error_report ("wrong encrypt type");
		exit(0);
	}
	while (*key_loop < max_loop){
		key_expansion (key, *type, *key_loop);
		*key_loop += 4;
	}
	return ;
}

void sub_bytes_shift_rows (AES_INFO *all_input)
{
	unsigned char (*input)[4] = all_input->input;
	unsigned char temp[4][4];

/*	while (loop != 4){
		input[loop][0] = s_box[input[loop][0]];
		input[loop][1] = s_box[input[loop][1]];
		input[loop][2] = s_box[input[loop][2]];
		input[loop][3] = s_box[input[loop][3]];
		loop+=1;
	}
	*/
	temp[0][0] = s_box[input[0][0]];
	temp[0][1] = s_box[input[1][1]];
	temp[0][2] = s_box[input[2][2]];
	temp[0][3] = s_box[input[3][3]];

	temp[1][0] = s_box[input[1][0]];
	temp[1][1] = s_box[input[2][1]];
	temp[1][2] = s_box[input[3][2]];
	temp[1][3] = s_box[input[0][3]];

	temp[2][0] = s_box[input[2][0]];
	temp[2][1] = s_box[input[3][1]];
	temp[2][2] = s_box[input[0][2]];
	temp[2][3] = s_box[input[1][3]];

	temp[3][0] = s_box[input[3][0]];
	temp[3][1] = s_box[input[0][1]];
	temp[3][2] = s_box[input[1][2]];
	temp[3][3] = s_box[input[2][3]];

	*(unsigned int *)(*(input+0)) = *(unsigned int *)(*(temp+0));
	*(unsigned int *)(*(input+1)) = *(unsigned int *)(*(temp+1));
	*(unsigned int *)(*(input+2)) = *(unsigned int *)(*(temp+2));
	*(unsigned int *)(*(input+3)) = *(unsigned int *)(*(temp+3));
}
void add_round_key (AES_INFO *all_input)
{
	int first_key_row = 4 * all_input->loop;
	unsigned char (*input)[4] = all_input->input;
	unsigned char (*key)[4] = all_input->key_info->begin;

	*(int *)(*(input+0)) ^= *(int *)(*(key+first_key_row));
	*(int *)(*(input+1)) ^= *(int *)(*(key+first_key_row+1));
	*(int *)(*(input+2)) ^= *(int *)(*(key+first_key_row+2));
	*(int *)(*(input+3)) ^= *(int *)(*(key+first_key_row+3));

	return ;
}
/*
void shift_rows(AES_INFO *all_input)
{
	int current_row = 0;
	unsigned char (*input)[4] = all_input->input;
	int i,j=0;
	sq_queue store_move;

	for (current_row = 1; current_row<= 3; current_row++){//用队列，如列和出列。
		i = 0;
		Q_init (&store_move);
		while (i!=current_row){//临时存储
			En_queue (&store_move, all_input->input[i][current_row]);
			i++;
		}
		while (i!=4){//位移
			all_input->input[i-current_row][current_row] = all_input->input[i][current_row];
			i++;
		}
		i = current_row;
		j = 4-current_row;
		while (i!=0){
			all_input->input[j][current_row]=De_queue(&store_move);
			i--;
			j++;
		}
		Q_destroy (&store_move);
	}


}
*/
void mix_columns(AES_INFO *all_input)
{
	int col = 0;
	int row = 0;
	unsigned char store_mix[4] = {0};
	for (row = 0; row < 4; row++){
		for (col = 0; col < 4; col++){
			store_mix[col] = all_input->input[row][col];
		}
		all_input->input[row][0] =  aes_mul(2, store_mix[0]) ^ aes_mul(3, store_mix[1]) ^store_mix[2] ^ store_mix[3];
		all_input->input[row][1] =  store_mix[0] ^ aes_mul (2, store_mix[1]) ^ aes_mul(3,store_mix[2]) ^ store_mix[3];
		all_input->input[row][2] =  store_mix[0] ^ store_mix[1] ^ aes_mul(2, store_mix[2]) ^ aes_mul (3, store_mix[3]);
		all_input->input[row][3] =  aes_mul(3, store_mix[0]) ^ store_mix[1] ^ store_mix[2] ^ aes_mul (2, store_mix[3]);

	}
}

unsigned char aes_mul (int a, int b)
{
	   int res = 0;
	   int hbit = 0x10000;		// bit to test if we need to take action
	   int modulus = 0x11b00;	// modulus - XOR by this to change value
	   while( a > 0 )
	   {
	      if ( a&1 )
	         res = res ^ b;		// "add" to the result
	      a >>= 1;			// shift a to get next higher-order bit
	      b <<= 1;			// shift multiplier also
	   }

	   // now reduce it modulo x**8 + x**4 + x**3 + x + 1

	   while( hbit >= 0x100 )
	   {
	      if ( res & hbit )		// if the high-order bit is set
	         res ^= modulus;	// XOR with the modulus

	      // prepare for the next loop
	      hbit >>= 1;
	      modulus >>= 1;
	   }

	   return (unsigned char)res;
 }
void *inv_aes_encrypt (void *input)
{
	AES_INFO *all_input = (AES_INFO *)input;

	switch (all_input->type){
	case BIT_128:
		all_input->loop = 10;
		break;
	case BIT_192:
		all_input->loop = 12;
		break;
	case BIT_256:
		all_input->loop = 14;
		break;
	default:
		Error_report ("wrong type");
	}
	add_round_key(all_input);
	while (all_input->loop != 1){
		all_input->loop--;
		inv_shift_rows(all_input);
		inv_sub_bytes(all_input);
		add_round_key(all_input);
		inv_mix_columns(all_input);


	}
	all_input->loop--;
	inv_shift_rows(all_input);
	inv_sub_bytes (all_input);
	add_round_key (all_input);
	pthread_exit((void *)0);
}

/*
unsigned char aes_mul (int a, int b)
{
	int temp[8] = {0};
	int flag[8] = {0};
	int c = b;
	int i;
	for(i= 0;i < 8;i++){
		int c1,c2;
		c1 = c / 2;
		c2 = c % 2;
		if(c2 == 1)
			flag[i] = 1;
		else
			flag[i] = 0;
		c = c1;
		if(i==0)
			temp[i] = a;
		else{
			temp[i] = temp[i-1]*2;
			if(temp[i]>255)
				temp[i] = (temp[i]%256)^27;
		}
	}
	a = 0;
	for(i = 0; i < 8; i++)
		a = a^(temp[i]*flag[i]);
	return a;
}

*/

void inv_shift_rows(AES_INFO *all_input)
{//移位方案二：读取，移位，写入。注意行列意义。
	unsigned char input[4];
	int cur_rows = 0, cur_cols = 0;
	int i = 0, j=0;;
	sq_queue store_move;
	for (cur_rows = 1; cur_rows<4; cur_rows++){//第0行不变
		for (cur_cols=0; cur_cols < 4; cur_cols++){
			input[cur_cols] = all_input->input[cur_cols][cur_rows];
		}
		Q_init (&store_move);
		/*入队列i个*/
		i = cur_rows;
		j = 3;
		while (i){
			En_queue (&store_move, input[j]);
			i--;
			j--;
		}
		//i == 0
		i = 4 - cur_rows;/*位移次数*/
		j = cur_rows;/*位移量*/
		while (i){/*移位*/
			input[i-1+j] = input[i-1];
			i--;
		}
		/*队列出队*/
		i = cur_rows;
		while (i){
			input[i-1] = De_queue (&store_move);
			i--;
		}
		/*写回all_input*/
		for (cur_cols=0; cur_cols < 4; cur_cols++){
			all_input->input[cur_cols][cur_rows] = input[cur_cols];
		}
		Q_destroy (&store_move);
	}

}
void inv_sub_bytes(AES_INFO *all_input)
{
	int loop = SUB_BYTE_START;

	while (loop != SUB_BYTE_END){
		all_input->input[loop/4][loop%4] = inv_s_box[all_input->input[loop/4][loop%4]];
		loop++;
	}
}
void inv_mix_columns(AES_INFO *all_input)
{
	int col = 0;
	int row = 0;
	int i=0;
	unsigned char store_mix[4] = {0};
	unsigned char result = 0;
	for (row = 0; row < 4; row++){
		for (col = 0; col < 4; col++){
			store_mix[col] = all_input->input[row][col];
		}
		for (i = 0; i < 4; i++){
			result = aes_mul(inv_mix_colum[i][0], store_mix[0]) ^ aes_mul(inv_mix_colum[i][1], store_mix[1]) ^ aes_mul(inv_mix_colum[i][2], store_mix[2]) ^ aes_mul(inv_mix_colum[i][3], store_mix[3]);
			all_input->input[row][i] = result;
		}
	}
}

void create_key_file(void)
{
	FILE *key_file;
	FILE *random;
	char type[100];
	char key[33]={0};
	size_t key_length;
	E_CLASS key_type ;



	key_file = fopen("key", "w");
	if ((random = fopen ("/dev/random", "r")) == NULL){
		printf("Internal Error\n");
		exit(0);
	}
	//strcpy (processing_filename, "key");
	printf("input encrypt type(128bit or 192bit or 256bit):");
input:	scanf ("%s", type);
     /*从/dev/random读出随机数，写入key file*/
	if (!strcmp (type, "128bit")) {
		printf("now create your key, keep your PC busy\n");
		key_length = 16;
		while  (( key_length -= fread(key, 1, key_length, random))){
			printf("Don't watch me, do something else\n");
		}
		key_type = BIT_128;
		fwrite ((void *)&key_type, sizeof(E_CLASS), 1, key_file);
		fwrite ((void *)key, sizeof(char), 16, key_file);
	} else if (!strcmp (type, "192bit")){
		printf("now create your key, keep your PC busy\n");
		key_length = 24;
		while  (( key_length -= fread(key, 1, key_length, random))){
			printf("Don't watch me, do something else\n");
		}
		key[24]='\0';
		key_type = BIT_192;
		fwrite ((void *)&key_type, sizeof(E_CLASS), 1, key_file);
		fwrite ((void *)key, sizeof(char), 24, key_file);
	} else if (!strcmp (type, "256bit")) {
		printf("now create your key, keep your PC busy\n");
		key_length = 32;
		while  (( key_length -= fread(key, 1, key_length, random))){
			printf("Don't watch me, do something else\n");
		}
		key[32]='\0';
		key_type = BIT_256;
		fwrite ((void *)&key_type, sizeof(E_CLASS), 1, key_file);
		fwrite ((void *)key, sizeof(char), 32, key_file);
	} else {
		printf("input \"128bit\" or \"192bit\" or \"256bit\":\n ");
		goto input;
	}

/*	if ((key_length = strlen(key)) < 16){
		printf("you must input more than 16 characters\n");
		exit (0);
	} else if (key_length < 24) {
		type = BIT_128;
		fwrite ((void *)&type, sizeof(E_CLASS), 1, key_file);
		fwrite ((void *)key, 1, 16, key_file);
		exit (0);
	} else if (key_length < 32) {
		type = BIT_192;
		fwrite ((void *)&type, sizeof(E_CLASS), 1, key_file);
		fwrite ((void *)key, 1, 24, key_file);
		exit (0);
	} else {
		type = BIT_256;
		fwrite ((void *)&type, sizeof(E_CLASS), 1, key_file);
		fwrite ((void *)key, 1, 32, key_file);

		exit (0);
	}
	*/

}

void aes_encrypt_wrap (char *input_filename)/*读写文件都在此函数中完成。*/
{
	char output_filename[100] = {0};
	FILE *key_file, *input_file, *output_file;
	AES_INFO all_input_first, all_input_second;
	KEY_INFO key_input;
	pthread_t e_first,e_second;
	unsigned char key[60][4] = {0};
	unsigned char (*fore_reading_first)[4];
	unsigned char (*fore_reading_second)[4];
/*	unsigned char (*write_first)[4];
	unsigned char (*write_second)[4];*/
	unsigned char (*vector1)[4] = calloc (sizeof(unsigned char),16);
	unsigned char (*vector2)[4] = calloc (sizeof(unsigned char),16);
	unsigned char last_num_of_bytes_first, last_num_of_bytes_second;/*最后两次次读入的数据大小*/
	unsigned char padding_length;/*填充长度,写入额外的一个字符*/
	unsigned char last_num_of_bytes_first_backup;
	E_CLASS type;
	int outputfd;


	signal (SIGALRM, sig_alrm);
	memset (vector1, 1, 16);
	memset (vector2, 1, 16);
	strcpy (output_filename, input_filename);
	strcat (output_filename, ".aes");
	key_file = Fopen ("key","r");
	output_file = Fopen (output_filename, "wb");
	outputfd = open (output_filename, O_WRONLY|O_APPEND);
	strcpy (processing_filename, output_file);
	input_file = Fopen (input_filename, "r");

	/*读出密钥和type，填写KEY_INFO的信息*/
	if (fread((void *)&type, sizeof(E_CLASS), 1, key_file) == 0){
		fprintf(stderr,"empty key file, run \"aes -k\" create key");
		exit(0);
	}
	//fseek (key_file, SEEK_SET, 6);
	switch (type){
	case BIT_128:
		fseek (key_file, SEEK_SET, 8);
		if (fread((void *)&key[0][0],1,16, key_file) != 16){
			fprintf(stderr, "read key file error");
			exit (0);
		}
		break;
	case BIT_192:
		if (fread ((void *)&key[0][0], 1,24, key_file) != 24){
			fprintf(stderr, "read key file error");
			exit (0);
		}
		break;
	case BIT_256:
		if (fread ((void *)&key[0][0], 1, 32, key_file) != 32){
			fprintf(stderr, "read key file error");
			exit (0);
		}
		break;
	default:
		fprintf(stderr, "read key file error");
		exit (0);
	}
	key_input.type = type;
	key_input.begin = key;
	key_input.key = key;
	key_expansion_wrap (&key_input);
	/*准备AES_INFO的信息*/
	all_input_second.key_info = all_input_first.key_info = &key_input;
	all_input_second.type = all_input_first.type = type;
	all_input_first.input = calloc(sizeof(unsigned char),16);
	all_input_second.input = calloc(sizeof(unsigned char),16);
	/*	CBC模式
	 * 读 不足256bit就用0进行加密
	 * 加密 不足256bit跳入末尾
	 * while(1){
	 * 分配fore_reading
	 * 读文件
	 * 		若文件已经读完，跳出（若只有1byte，仍然继续循环）。
	 * 等待上一轮
	 * writing = input
	 * input = fore_reading
	 * 加密
	 * vector xor= writing
	 * 写入vector
	 * 释放writing
	 * }
	 * 等待
	 * vector xor = input
	 * 写入vector
	 * 释放内存input
	 * 释放vector
	 * 结束
	 */
	 if ((last_num_of_bytes_first = fread ((void *)&all_input_first.input[0][0], 1, 16, input_file)) == 0){
		 fprintf(stderr, "empty file ,program quit\n");
		 exit (0);
	 }
	 if ((last_num_of_bytes_second = fread ((void *)&all_input_second.input[0][0], 1, 16, input_file)) != 16){
		 xor_16bytes ((void *)&all_input_first.input[0][0], (void *)&vector1[0][0]);
		 xor_16bytes ((void *)&all_input_second.input[0][0], (void *)&vector2[0][0]);
		 Pthread_create (&e_first, NULL, aes_encrypt, (void *)&all_input_first);
		 Pthread_create (&e_second, NULL, aes_encrypt, (void *)&all_input_second);
		 goto end;

	 }
	 /*在这里加如vector异或*/
	 xor_16bytes ((void *)&all_input_first.input[0][0],(void *)&vector1[0][0]);
	 xor_16bytes ((void *)&all_input_second.input[0][0],(void *)&vector2[0][0]);
	 Pthread_create (&e_first, NULL, aes_encrypt, (void *)&all_input_first);
	 Pthread_create (&e_second, NULL, aes_encrypt, (void *)&all_input_second);
	 free (vector1);
	 free (vector2);
	 while (1){
		 /*自动填充0*/
		 fore_reading_first = calloc (sizeof(unsigned char), 16);
		 fore_reading_second = calloc (sizeof(unsigned char), 16);
		 last_num_of_bytes_first_backup = last_num_of_bytes_first;
		 if ((last_num_of_bytes_first = fread((void *)&fore_reading_first[0][0], 1, 16, input_file)) == 0){
			 last_num_of_bytes_first = last_num_of_bytes_first_backup;
			 free (fore_reading_first);
			 free (fore_reading_second);
			 break;
	 }
		 last_num_of_bytes_second = fread ((void *)&fore_reading_second[0][0], 1, 16, input_file);

		 Pthread_join (e_first,NULL);
		 Pthread_join (e_second,NULL);
		 /*应当把写入指针赋值给vector*/
		 vector1 = all_input_first.input;
		 vector2= all_input_second.input;
		 xor_16bytes ((void *)&fore_reading_first[0][0], (void *)&vector1[0][0]);
		 xor_16bytes ((void *)&fore_reading_second[0][0],(void *)&vector2[0][0]);
		 all_input_first.input = fore_reading_first;
		 all_input_second.input = fore_reading_second;
		 Pthread_create (&e_first, NULL, aes_encrypt, (void *)&all_input_first);
		 Pthread_create (&e_second, NULL, aes_encrypt, (void *)&all_input_second);
		 if (fwrite ((void *)&vector1[0][0], 1, 16, output_file) != 16){
			 fprintf(stderr, "fwrite error: %s", output_filename);
		 }
		 if (fwrite ((void *)&vector2[0][0], 1,16,output_file) != 16){
			 fprintf(stderr, "fwrite error: %s", output_filename);
		 }
	  /*should free vector*/
		 free (vector1);
		 free (vector2);
	 }
end: Pthread_join (e_first,NULL);
	 Pthread_join (e_second,NULL);
	/*把写入指针赋值给vector*/
	 vector1 = all_input_first.input;
	 vector2= all_input_second.input;
	 padding_length = 32 - last_num_of_bytes_first - last_num_of_bytes_second;
	 if (fwrite ((void *)&vector1[0][0], 1, 16, output_file) !=16){
		 fprintf(stderr, "fwrite error: %s", output_filename);
	 }
	 if (fwrite ((void *)&vector2[0][0], 1, 16, output_file) !=16){
		 fprintf(stderr, "fwrite error: %s", output_filename);
	 }
	 if (fwrite ((void *)&padding_length, 1, 1, output_file) != 1){
		 fprintf(stderr, "fwrite error");
	 }
	 fflush (output_file);
	 free (all_input_first.input);
	 free (all_input_second.input);
	 free (vector1);
	 free (vector2);

}
void xor_16bytes(void *left, void *right)
{
	unsigned int *a = (unsigned int *)left;
	unsigned int *b = (unsigned int *)right;

	a[0] ^= b[0];
	a[1] ^= b[1];
	a[2] ^= b[2];
	a[3] ^= b[3];
}
void inv_aes_encrypt_wrap (char *filename)
{

	char output_filename[100] = {0};
	char *t;
	char temp;
	FILE *key_file, *input_file, *output_file;
	AES_INFO all_input_first, all_input_second;
	KEY_INFO key_input;
	pthread_t e_first,e_second;
	unsigned char key[60][4];
	unsigned char (*fore_reading_first)[4];
	unsigned char (*fore_reading_second)[4];
	unsigned char (*write_first)[4];
	unsigned char (*write_second)[4];
	unsigned char (*vector1_cur)[4] = calloc (sizeof(unsigned char),16);
	unsigned char (*vector2_cur)[4] = calloc (sizeof(unsigned char),16);
	unsigned char (*vector1_last)[4] = calloc (sizeof(unsigned char),16);
	unsigned char (*vector2_last)[4] = calloc (sizeof(unsigned char),16);
	unsigned char (*vector1_next)[4] = calloc (sizeof(unsigned char),16);
	unsigned char (*vector2_next)[4] = calloc (sizeof(unsigned char),16);
	unsigned char padding_length;
	unsigned char write_length_first;
	unsigned char (*temp1)[4];
	E_CLASS type;
	struct  stat inputfile_stat;

	memset (vector1_last, 1, 16);
	memset (vector2_last, 1, 16);

	key_file = Fopen ("key","r");
	input_file = Fopen (filename,"r");

	/*判断文件大小是不是mod16字节(256bit)余1，若不是，说明文件被修改过*/
	if (lstat(filename, &inputfile_stat) == -1){
		fprintf(stderr,"lstat function error\n");
		exit(0);
	}
	if (inputfile_stat.st_size % 16 != 1 ){
		fprintf(stderr,"Error: this file is NOT the cipher file\n");
		exit(0);
	}


	t = &filename[strlen(filename)];
	t = &t[-4];
	/*检查输入后缀是否是.aes*/
	if ((temp = strcmp(".aes",t))!=0){
		printf ("the suffix must be \".aes\"\n");
		exit(0);
	}
	t[0] = '\0';
	/*去掉.aes后缀并且复制*/
	strcpy (output_filename, filename);
	/*检查将写入的文件是否存在*/
	if (output_file = fopen (output_filename, "r") != NULL){
		printf("the file exits,input 'y' for overwrite, others for exit\n");
		if (getchar() != 'y'){
			exit(0);
		}
	}
	output_file = Fopen (output_filename,"w");


	/*读取key文件*/
	if (fread((void *)&type, sizeof(E_CLASS), 1, key_file) == 0){
		printf("read key file error");
		exit(0);
	}
	switch (type){
	case BIT_128:
		if (fread((void *)&key[0][0],1,16, key_file) != 16){
			fprintf(stderr, "read key file error");
			exit (0);
		}
		break;
	case BIT_192:
		if (fread ((void *)&key[0][0], 1,24, key_file) != 24){
			fprintf(stderr, "read key file error");
			exit (0);
		}
		break;
	case BIT_256:
		if (fread ((void *)&key[0][0], 1, 32, key_file) != 32){
			fprintf(stderr, "read key file error");
			exit (0);
		}
		break;
	default:
		fprintf(stderr, "read key file error");
		exit (0);
	}
	key_input.type = type;
	key_input.begin = key;
	key_input.key = key;
	key_expansion_wrap (&key_input);
	/*准备AES_INFO的信息*/
	all_input_second.key_info = all_input_first.key_info = &key_input;
	all_input_second.type = all_input_first.type = type;
	all_input_first.input = calloc(sizeof(unsigned char),16);
	all_input_second.input = calloc(sizeof(unsigned char),16);
	/*CBC模式
	 *读,复制到vector_cur
	 *解密
	 * while（1）{
	 * last = cur(交换指针）
	 * 	分配预读空间forereading
	 * 	预读
	 * 		若无数据，跳出循环。
	 * 	forereading复制到vector_cur
	 * 	等待上一轮解密完成
	 * 	转移解密后的指针给写指针
	 * 	转移预读指针给解密指针
	 * 	解密
	 * 	写指针异或vector_last
	 * 	写如文件
	 * 	释放写指针内存
	 * 	}
	 * 	等待解密
	 * 	last异或解密
	 * 	写入)
	 * 	释放所有指针
	 * 	*/;
	 /*先读出最后一个字节的填充长度，然后重新设定指针*/
	 if (fseek (input_file, -1, SEEK_END)!=0){
		 fprintf(stderr,"fseek error");;
	 }
	 if (fread ((void *)&padding_length,1 ,1, input_file)!=1){
		 fprintf(stderr,"fread error");
	 }
	 if (fseek (input_file, 0, SEEK_SET)!=0)
		 fprintf(stderr,"fseek error");
	 if (fread ((void *)&all_input_first.input[0][0], 1, 16, input_file)!= 16){
		 fprintf(stderr,"fread error");
	 }
	 if (fread ((void *)&all_input_second.input[0][0], 1, 16, input_file)!=16){
		 fprintf(stderr,"fread error");
	 }
	 strncpy ((void *)&vector1_cur[0][0], (void *)&all_input_first.input[0][0], 16);
	 strncpy ((void *)&vector2_cur[0][0], (void *)&all_input_second.input[0][0], 16);
	 Pthread_create (&e_first, NULL, inv_aes_encrypt, (void *)&all_input_first);
	 Pthread_create (&e_second, NULL, inv_aes_encrypt, (void *)&all_input_second);
	 while (1){
		 fore_reading_first = calloc (sizeof (unsigned char), 16);
		 if ((fread ((void *)&fore_reading_first[0][0], 1, 16, input_file))!=16){
			 free (fore_reading_first);
			 break;
		 }
		 fore_reading_second = calloc (sizeof (unsigned char), 16);
		 fread ((void *)&fore_reading_second[0][0], 1, 16, input_file);
		 memcpy ((void *)&vector1_next[0][0], (void *)&fore_reading_first[0][0], 16);
		 memcpy ((void *)&vector2_next[0][0], (void *)&fore_reading_second[0][0], 16);
		 Pthread_join (e_first, NULL);
		 Pthread_join (e_second, NULL);
		 write_first= all_input_first.input;
		 write_second = all_input_second.input;
		 all_input_first.input = fore_reading_first;
		 all_input_second.input = fore_reading_second;
		 Pthread_create (&e_first, NULL, inv_aes_encrypt, (void *)&all_input_first);
		 Pthread_create (&e_second, NULL, inv_aes_encrypt, (void *)&all_input_second);
		 xor_16bytes ((void *)&write_first[0][0], (void *)&vector1_last[0][0]);
		 xor_16bytes ((void *)&write_second[0][0], (void *)&vector2_last[0][0]);
		 SWAP (vector1_cur, vector1_last, temp1);
		 SWAP (vector2_cur, vector2_last, temp1);
		 SWAP (vector1_cur, vector1_next, temp1);
		 SWAP (vector2_cur, vector2_next, temp1);
		 if (fwrite ((void *)&write_first[0][0], 1, 16, output_file) != 16){
			 fprintf(stderr, "fwrite error\n");
			 exit (0);
		 }
		 if (fwrite ((void *)&write_second[0][0], 1, 16, output_file) != 16){
			 fprintf (stderr, "fwrite error\n");
			 exit(0);
		 }
		 free (write_first);
		 free (write_second);
	 }
	Pthread_join (e_first, NULL);
	Pthread_join (e_second, NULL);
	xor_16bytes ((void *)&vector1_last[0][0], (void *)&all_input_first.input[0][0]);
	xor_16bytes ((void *)&vector2_last[0][0], (void *)&all_input_second.input[0][0]);
	if (32-padding_length > 16) {
		if (fwrite ((void *)&vector1_last[0][0], 1, 16, output_file) != 16){
			fprintf (stderr, "fwrite error\n");
		}
	} else {
		if (fwrite ((void *)&vector1_last[0][0], 1, 32-padding_length, output_file) != (32-padding_length)){
			fprintf (stderr, "fwrite error\n");
		}
	}
	if (padding_length < 16){
		fwrite ((void *)&vector2_last[0][0], 1, 16 - padding_length, output_file);

	}
	free (vector1_last);
	free (vector2_last);
	free (vector1_cur);
	free (vector2_cur);
	free (all_input_first.input);
	free (all_input_second.input);
}


void destroy_key(void)
{
	E_CLASS key_type;
	unsigned char write1[32]={0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
	unsigned char write2[32]={0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
	FILE *key_file;
	int key_file_fd;
	int i = 500;
	int key_length;

	key_file = Fopen ("key","r");
	if (fread ((void *)&key_type, sizeof(E_CLASS), 1, key_file) != 1){
		fprintf(stderr,"read key file error");
		/*continue destroy key*/
	}
	switch (key_type){
	case BIT_128:
		key_length = 16;
		break;
	case BIT_192:
		key_length = 24;
		break;
	case BIT_256:
		key_length = 32;
		break;
	default:
		key_length = 32;
		break;
	}
	fclose(key_file);
	/*使用直写模式*/
	key_file_fd = open ("key",O_WRONLY, O_DIRECT);
	while (i--){
		write (key_file_fd, (void *)write2, key_length);
		lseek(key_file_fd, SEEK_SET, 0);
		write (key_file_fd, (void *)write1, key_length);
		lseek(key_file_fd, SEEK_SET, 0);
	}
	unlink("key");
	fprintf(stdout,"key destroyed succeeful\n");
	close(key_file_fd);
	return;



}