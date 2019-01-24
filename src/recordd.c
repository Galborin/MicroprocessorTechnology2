/*
 * recordd.c
 *
 *  Created on: 11.01.2019
 *      Author: piotr
 */

#define THRESHOLD 	100
#define CRY_THR 	10

#include "waverecorder.h"


const float32_t base[19][65] =
{
		{15.5194,16.0291,13.2536,12.8950,15.4579,10.0736,11.5148,11.5535,13.4629,14.2590,11.1572,12.3465,12.8945,12.8730,10.5300,10.1777,11.5817,10.8636,12.3430,12.0444,12.2593,11.7767,11.3484,12.4669,12.6126,13.7759,13.3000,12.9076,11.4859,10.6558,12.3885,12.7835,12.8530,12.6919,13.5346,13.0331,12.6029,15.7118,12.2447,15.5737,14.9635,14.9612,13.6965,14.8138,12.5109,14.1611,12.7687,14.1052,15.2769,16.4341,16.1361,16.2136,16.2288,15.2257,15.2137,15.5785,14.6923,14.5921,15.1388,15.1678,17.7487,15.4442,14.8170,14.7344,11.1606},
		{-5.2356,-4.8733,-5.5428,-4.4432,-6.7770,-6.0946,-6.6859,-6.6259,-7.5534,-6.3091,-7.6956,-6.7338,-7.1699,-6.4331,-6.6144,-6.0485,-5.6131,-5.5593,-5.5353,-5.4326,-5.1073,-4.7818,-4.6732,-5.1673,-4.6583,-4.3475,-4.9187,-5.0421,-4.7174,-5.5061,-4.6470,-5.5963,-4.7000,-5.4320,-5.3438,-4.1697,-4.0002,-4.3053,-4.3365,-5.4993,-4.4735,-4.2286,-4.7989,-4.4561,-4.0504,-4.2277,-4.6987,-5.3314,-5.7291,-4.6774,-4.7805,-4.0131,-4.4300,-4.9455,-5.5278,-4.0518,-3.6408,-2.4751,-3.4101,-1.9166,-2.0627,-4.5005,-5.2432,-4.8521,-3.7811},
		{-7.6042,-7.6798,-6.6570,-5.0965,-6.7549,-5.5631,-6.3214,-6.0100,-7.2394,-5.6145,-5.9672,-5.6253,-6.3454,-6.8026,-6.8626,-6.3078,-6.2396,-6.2223,-8.1332,-7.6699,-7.0726,-6.2078,-6.2759,-7.7330,-9.0371,-9.9661,-9.0239,-7.2693,-6.3991,-7.2459,-6.4230,-7.7765,-7.1146,-6.8974,-8.3080,-7.2356,-6.9168,-7.3112,-6.4824,-7.9632,-7.7978,-7.6873,-7.9606,-7.1311,-6.0256,-5.9377,-6.3769,-7.7802,-8.9422,-9.2029,-9.8782,-9.9857,-9.6165,-8.6781,-9.7131,-8.4100,-7.2943,-6.4494,-8.1860,-6.5851,-6.5867,-7.4364,-7.8437,-7.5123,-0.4096},
		{2.7136,3.0428,1.5306,1.4966,2.8239,0.7635,0.5651,1.1368,0.9408,2.2741,1.4317,1.4291,1.9399,2.5182,0.9555,0.5202,0.8159,1.1747,2.0596,1.6184,1.7186,1.2113,1.7078,1.2693,2.1104,2.4773,1.9247,1.3103,0.7669,-0.0737,1.4606,2.8300,2.0219,1.2111,0.8067,0.8555,0.0214,1.6640,-0.0323,0.9309,1.5174,1.3138,1.7342,2.2942,0.4725,1.2228,1.1107,1.8916,1.6646,2.6812,1.7804,1.4345,1.3509,2.0053,0.9997,0.9281,-0.3406,-1.2069,-1.4143,-1.4758,0.4013,0.2391,0.4544,0.7066,0.4662},
		{1.4964,2.2560,1.1306,0.9412,0.0694,0.7894,0.1398,0.0928,-0.1171,1.0431,0.4904,0.0734,0.3545,0.6387,0.9242,0.6185,1.0308,1.3232,1.1787,1.4171,1.6944,1.5493,1.0177,1.5540,1.4912,2.9919,2.2317,1.3518,1.4484,0.2523,0.7951,1.6830,2.8721,1.8096,1.7332,2.2136,2.0806,2.3225,1.6615,1.3979,2.3732,1.8776,2.0857,2.3407,1.7562,2.4779,1.8450,1.8865,1.5045,1.3122,0.7524,0.8803,1.2835,1.9576,1.0599,0.9588,0.7412,0.3488,0.0845,0.8038,1.2059,0.8619,0.2214,1.2193,0.0179},
		{-2.0251,-2.7764,-1.9676,-1.8739,-1.1268,-0.1863,-1.0048,-1.8165,-3.7913,-3.7398,-2.7707,-2.8677,-4.7063,-5.5381,-4.0945,-3.9608,-3.3667,-3.4846,-5.9899,-5.0712,-3.9804,-3.7634,-4.3458,-4.3417,-4.4464,-4.8812,-4.0291,-3.1123,-2.4876,-3.2555,-2.4979,-2.7792,-1.3759,-2.1096,-2.8234,-2.0720,-1.2751,-1.8774,-1.6142,-2.5624,-1.5207,-1.5799,-1.7110,-2.0778,-0.9591,-0.8953,-1.5998,-2.4506,-3.9315,-3.8036,-4.7899,-4.2323,-3.2183,-3.3837,-3.5577,-3.0176,-2.1464,-1.2271,-2.6460,-1.0071,-1.7733,-2.2061,-3.0292,-2.3775,-0.4906},
		{-0.8183,-0.9704,-0.9160,-1.1295,0.7498,0.7042,0.8260,0.4982,-0.0205,0.4819,-0.1547,0.5736,1.0686,0.5988,-1.5463,-2.0048,-0.7318,-1.0998,-0.5541,-0.2207,-0.3005,-1.3474,-1.3834,-0.9408,-0.8660,1.2461,-0.2633,-0.6317,-0.5629,-0.4712,-0.0349,0.3568,-0.7733,-1.4639,-1.1461,-1.6089,-1.7634,-0.5274,-2.5222,0.1013,-0.4839,-0.1315,-0.5338,-0.4584,-1.4866,-1.1967,-1.7323,-0.6335,-0.2134,1.2959,1.3094,0.7939,0.3581,-0.3701,0.7288,0.4345,-0.8820,-0.4248,-0.1180,0.0274,0.6212,0.1186,0.1118,-0.4506,0.3971},
		{0.1239,-0.7007,-0.2713,0.1850,0.6973,1.4287,1.1897,1.1498,1.8139,1.5767,1.4190,1.2996,1.5046,1.0333,1.4112,1.2852,1.9861,1.9665,1.5182,2.0132,1.7212,2.0304,2.3277,1.6357,1.3258,1.8775,0.3012,1.4342,1.0899,1.3115,1.4453,-1.4977,-1.3858,-1.2353,-1.6778,-1.5632,-0.9163,-1.2220,-1.1512,-1.2587,-1.3900,-0.4577,-0.9632,-1.7741,-0.8335,-0.5422,-0.9596,-1.3433,-1.9198,-1.4709,-1.4182,-1.0747,-1.3686,-2.3583,-2.0065,-1.1353,-0.4912,-0.9358,-0.9930,-1.1930,-0.5383,-0.4800,-0.5042,0.1826,-0.1330},
		{0.3115,0.4238,0.5377,2.0362,1.3449,2.4040,1.9040,1.4006,-0.4022,0.8742,2.0511,1.0405,1.7870,2.1202,2.7240,2.6232,2.7929,3.1822,2.7726,3.5866,3.7456,3.8445,4.6974,3.6056,3.8140,1.9941,1.4935,1.7453,2.5535,1.5973,0.8282,1.0129,1.6319,0.7209,1.0394,1.2778,1.6594,1.5578,0.8819,1.4556,0.9952,0.9143,1.2933,1.1297,0.4384,1.1447,0.8198,0.9673,-0.2357,-0.3763,-1.1508,-0.0478,0.1140,0.7775,0.1397,-0.0925,0.3709,-0.3782,0.0512,-0.8662,0.3906,-0.0138,0.0726,0.1531,-0.2769},
		{0.2075,-0.0182,0.7125,2.4496,3.0252,4.0101,4.1157,4.0964,5.2482,4.3679,4.2165,3.7228,4.1906,3.2862,2.6351,2.3701,2.3681,2.7198,2.6145,3.1852,2.7999,3.0010,3.5092,2.0650,1.6052,2.8439,2.2532,1.3304,2.3327,1.4371,0.4789,1.1502,1.2955,1.4181,1.1657,1.5729,1.8420,1.8621,1.0376,0.8034,0.1063,0.2263,1.2614,1.3962,0.8500,1.2785,1.4144,1.6324,1.4038,0.3612,-0.2070,0.7510,1.7379,1.4022,0.4708,-0.4678,-1.0745,-0.7543,-0.8104,-0.8306,-0.8057,-1.1272,-1.0019,-0.6254,0.5180},
		{2.1929,3.3629,3.2140,2.2254,3.2444,1.9479,2.0267,1.9404,2.4967,2.7342,3.1981,2.8238,3.1546,2.6623,2.6069,2.3945,2.8407,2.8532,2.9496,2.9108,3.0585,2.6262,3.2774,3.0046,3.2495,3.5711,3.7041,3.6494,3.7308,3.7611,4.3001,4.7851,3.6357,3.1626,2.2910,2.4365,2.8329,2.3026,1.1701,2.0367,1.4857,1.7659,3.1515,3.0932,1.5628,1.7004,2.2501,2.8946,1.9422,1.0973,0.8388,0.8685,0.5430,1.8630,1.1899,-0.1391,-0.5225,-0.3738,-0.5327,-0.2393,-0.6870,-0.1247,0.3158,0.7357,-0.6254},
		{0.7069,1.7316,1.2598,-1.1565,-3.2706,-3.4013,-3.8704,-4.1934,-5.2518,-3.9752,-2.1593,-2.9023,-3.2796,-2.7987,-0.0957,-0.2742,0.7554,0.5011,-0.5564,-0.8451,-0.5527,-0.5346,-1.0212,0.2416,0.5233,1.4162,2.7615,2.5470,2.0619,3.1597,3.3236,2.6167,2.1957,2.3193,2.4943,2.6725,2.8104,2.3812,1.8065,1.5321,1.1628,1.4434,1.9983,1.9137,1.4468,1.8235,1.8091,2.0395,1.5400,0.1238,-0.4022,0.5942,1.5441,1.5782,0.8206,0.9765,1.3875,1.4113,1.6582,1.2261,1.2595,1.5720,1.3851,0.9949,0.5487},
		{2.1703,0.4257,-0.5076,-2.2776,-2.8924,-3.5531,-3.5160,-4.2631,-4.4786,-3.2347,-3.1763,-3.0377,-2.5165,-2.2775,-2.3269,-2.7202,-1.9790,-2.2466,-1.9108,-1.8777,-2.2576,-2.9134,-2.5400,-2.4348,-2.4626,-0.8346,-1.1666,-0.7069,-1.7325,-1.4466,-0.6894,0.9420,0.5949,0.3793,1.6293,1.0922,1.1114,1.9419,1.6657,3.1217,2.8101,2.7414,1.1934,1.0010,1.3219,1.7630,1.5996,1.5662,1.7771,3.0007,3.1453,3.5884,3.4153,2.4285,3.3764,3.5456,2.8268,3.1819,3.4326,2.6316,3.1530,2.6460,2.7369,1.4391,-0.3836},
		{-0.0983,-3.2549,-3.4886,-1.2124,-0.1898,0.4088,1.9402,0.6266,-0.1463,-1.4032,-0.4709,0.0376,-1.4706,-2.9057,-2.8109,-2.8465,-2.7763,-2.7804,-2.8341,-3.0910,-3.1615,-2.6509,-2.0555,-2.9596,-3.0112,-4.2296,-3.9537,-3.7555,-3.0177,-3.9329,-5.3158,-3.6092,-2.7587,-2.2922,-1.4989,-1.8612,-2.0719,-1.2451,-1.1407,-0.1313,-0.4465,0.2712,-1.0980,-1.2060,-1.0250,-0.9365,-1.1690,-1.1199,-1.1061,-0.2709,0.3659,0.3556,-0.6788,-1.6645,-0.4599,1.3935,1.3574,1.9083,1.1522,1.7770,0.8434,1.6740,1.9218,0.9463,0.2402},
		{-0.8644,-2.4738,-0.8836,1.0865,2.3888,2.0050,3.1798,3.0014,4.3029,3.4656,2.3118,2.9489,3.0973,1.6503,1.1364,0.9862,0.7892,0.9110,1.8764,2.2579,1.5062,1.7700,2.8892,1.3454,1.0958,-1.2438,-1.2787,-0.8477,-0.8269,-1.4359,-1.8108,-2.6358,-4.0080,-3.3180,-1.9255,-2.9488,-3.1133,-2.0508,-2.8449,-0.9492,-1.5078,-0.9713,-2.7224,-3.2514,-2.5678,-2.5955,-2.6415,-2.3776,-1.6371,-1.4030,-1.3247,-1.0883,-1.0372,-1.7143,-0.8767,-0.1134,0.2705,0.7232,0.4045,1.0970,-0.0207,0.2288,0.1214,0.2598,-0.1125},
		{-1.7004,-0.7624,0.0784,0.5200,-0.8607,-0.3214,0.4192,-0.4028,-1.7265,-1.2835,-0.2245,-0.3386,-0.4611,0.5351,1.1666,1.0217,1.1220,1.0977,1.3738,1.4477,0.9483,1.2568,2.0640,1.0989,1.1142,1.1192,0.2458,-0.5526,0.1216,0.1179,-0.2382,-0.6734,-1.7799,-1.7187,-1.8547,-1.9739,-1.4179,-2.3694,-1.9704,-1.7325,-1.9278,-1.4767,-1.6627,-1.9100,-2.0539,-2.1364,-2.2185,-2.6098,-2.7397,-1.9579,-1.4321,-1.1383,-1.4959,-2.7607,-1.4391,-1.2796,-1.3322,-1.2570,-0.8940,-1.4514,-1.5477,-1.2604,-1.4413,-0.9913,0.1002},
		{1.0317,0.9236,0.2909,-0.7064,-0.5041,-1.0780,-0.3429,-0.7189,-0.3337,-0.4624,-1.0565,-0.2905,-0.7891,-1.7501,-1.7281,-1.7092,-1.4115,-1.6512,-1.6112,-2.1243,-2.1399,-2.0158,-1.7256,-1.7707,-1.7554,-1.7110,-0.1726,0.1304,-0.7053,-0.0571,0.7248,1.4273,1.4565,1.2797,1.6545,1.6231,1.2473,1.9967,1.0877,1.3293,1.0027,0.7084,1.6269,1.8958,0.8404,1.0729,1.4401,2.0381,2.0742,0.3174,-0.3829,0.2220,0.9214,2.0394,0.8161,-0.5529,-0.8558,-0.8335,-0.8035,-1.5927,-0.9960,-1.0719,-0.5651,-0.1193,-0.0547},
		{0.6859,0.7461,-0.0709,-0.4716,-0.2707,-0.3654,0.3822,-0.4760,-1.6304,-1.3115,-0.6870,-0.3330,-1.1343,-1.5011,-1.1975,-1.2592,-0.9988,-1.1817,-0.9019,-1.4718,-1.4768,-1.7758,-1.8586,-1.4094,-1.4415,-1.7964,-0.7294,-0.4225,-0.5284,-0.6973,-0.8060,0.9289,1.0043,0.9793,0.4878,0.7572,0.8639,0.5226,0.7099,0.6058,0.2144,0.4250,1.0117,1.1628,0.5554,0.6002,0.9853,0.8403,0.5980,0.1503,-0.0653,0.4271,0.3294,0.7017,0.1635,0.1696,-0.2135,-0.2882,-0.0455,-0.4031,-0.2169,-0.1231,0.2026,0.3102,0.0920},
		{-0.6395,-0.1069,0.8538,0.4351,0.7156,0.1327,0.4041,0.0104,0.0920,-0.2321,-0.0516,0.1247,-0.4957,-0.4760,0.3181,0.0959,0.2844,0.2431,-0.0866,0.0898,0.1481,0.0885,-0.3194,0.4150,0.6461,1.6671,1.7100,1.0150,0.6127,0.4741,0.2654,-0.0969,-0.8782,-0.1943,0.3591,0.1395,-0.0254,0.0077,-0.0959,0.0303,-0.2331,-0.0063,-0.7166,-0.6893,-0.4104,-0.2952,-0.0094,-0.1917,-0.1547,-0.0343,0.4163,0.2953,0.0579,0.0639,-0.0074,0.4980,1.1968,1.1170,1.2919,1.5793,1.0301,1.0277,0.9505,0.4067,-0.0474}

};

const float32_t base2[19][65] =
{
		{12.7351,12.6540,12.2856,10.8437,10.6656,9.4282,10.7141,10.2667,10.8206,10.6974,10.8727,10.6787,9.9045,8.8330,10.8271,9.8762,10.3295,9.0551,9.1978,9.3458,9.9456,9.7654,10.8644,10.9449,11.0927,9.3237,9.1235,9.7531,9.7206,8.7807,8.6957,8.1880,6.8873,7.5726,7.4474,5.5236,6.4751,4.3984,4.8379,6.0192,5.0688,6.6537,6.3316,5.2578,6.5728,7.0999,5.1395,7.2218,6.2626,8.4936,8.3642,8.5020,9.9229,10.7396,11.6414,11.7117,12.5496,11.7831,12.7365,12.3260,12.8910,12.4547,13.4374,13.0406,6.8136},
		{-4.2672,-4.9496,-5.0382,-5.4331,-6.3958,-9.2973,-8.1706,-7.4275,-8.3162,-8.0406,-7.3381,-6.0267,-6.3476,-5.9932,-5.3670,-6.1124,-5.8248,-5.1860,-4.9268,-5.7532,-5.6353,-4.4844,-3.4137,-3.6632,-3.5567,-4.4769,-5.7075,-5.8427,-5.3580,-6.8893,-8.4624,-8.4318,-7.6467,-7.6924,-7.1705,-8.1401,-7.2161,-7.6596,-7.2874,-6.7205,-6.8430,-5.6772,-5.8618,-7.5519,-7.7381,-8.7560,-9.1329,-8.3022,-9.5032,-7.3783,-7.8105,-9.0757,-8.5908,-8.6120,-7.0694,-7.3004,-6.6690,-7.5551,-5.8383,-5.2144,-5.3842,-5.8340,-5.1026,-6.6280,-1.6378},
		{-4.7891,-5.2644,-5.1327,-5.0175,-5.7587,-5.6657,-4.5954,-4.7788,-4.6000,-4.5275,-5.0650,-3.7462,-4.6328,-5.0414,-4.4672,-3.6835,-3.7046,-4.4989,-4.2570,-5.7215,-6.0544,-5.4093,-6.2762,-5.1911,-5.9021,-5.4290,-6.8998,-4.1370,-4.9517,-5.5129,-7.3085,-7.4924,-6.5466,-6.9853,-6.4322,-7.9747,-7.5258,-7.4445,-6.6924,-9.0598,-6.5466,-6.1404,-6.6183,-6.5954,-8.3442,-7.5322,-8.0189,-6.6832,-7.6576,-7.1786,-8.2472,-8.4274,-8.4211,-7.8144,-6.3558,-6.2073,-7.6111,-6.6729,-5.2189,-5.4233,-6.2523,-5.6878,-5.2754,-5.2750,-0.7585},
		{0.4575,0.2419,-0.4245,-0.2777,-0.5792,-1.4645,-0.6710,0.1752,-0.4627,-0.4596,0.0012,0.5614,0.3279,0.1606,1.2288,0.7413,1.4939,0.7732,1.0725,1.2458,0.1724,-0.2602,-0.3319,0.9589,0.3468,0.0582,0.1294,1.4309,0.2479,-0.7298,-2.0024,-1.3088,-0.8560,-0.2670,0.1490,-1.1060,-0.4267,-0.5540,0.5427,0.0689,-1.5831,-2.6503,-2.3836,-1.2589,0.4446,0.2265,-0.6298,0.0784,-1.0091,-0.4832,0.8206,0.0862,1.3836,1.4035,1.3849,1.1859,0.8817,1.1712,1.9383,0.6783,1.0334,1.4380,1.5212,2.2091,0.3564},
		{3.7080,3.3430,2.6808,3.8342,3.7856,2.6336,3.4095,3.3710,3.0189,2.9992,3.5133,3.5890,3.5623,3.9620,4.2313,3.3846,4.1623,3.9250,3.6300,4.7188,4.2347,2.5576,3.1211,3.8615,3.3644,3.3880,3.7939,4.1354,4.0645,3.1727,1.4564,2.9808,1.9898,3.5761,3.9893,2.5299,0.9294,-0.1770,-0.9159,0.6290,0.0820,0.5262,0.4377,-0.8054,-0.0261,1.5693,0.6394,-0.1362,-0.9461,-0.3005,-0.0288,-0.1705,1.0015,0.3711,1.3123,0.2725,0.8769,0.7071,1.8680,1.1108,1.6001,1.4464,1.6144,2.8636,-0.1489},
		{-1.7877,-2.1946,-2.0810,-1.2836,-2.2327,-3.5703,-3.0846,-2.9409,-2.7685,-2.7154,-2.6169,-3.0631,-2.8371,-2.7746,-3.3555,-2.9745,-2.8658,-3.0503,-3.2271,-2.7467,-2.2457,-3.0306,-3.1062,-2.4418,-3.1075,-2.8655,-3.5277,-3.1480,-3.4049,-2.5157,-2.7063,-2.7691,-1.5368,-2.7523,-4.4274,-4.1232,-4.8497,-3.7337,-2.7545,-2.5698,-2.2981,-2.3968,-2.5133,-2.3602,-2.8146,-3.7919,-3.7831,-4.6625,-5.0588,-5.2084,-5.1366,-4.6306,-4.5170,-4.6394,-3.2782,-3.6111,-4.4595,-4.2211,-3.8324,-3.4853,-4.0714,-4.1430,-3.8969,-3.4349,-0.0218},
		{0.8980,0.1208,0.4500,-0.6415,-0.5368,-0.2922,0.1158,0.1169,-0.4849,-0.3037,0.1275,0.3703,-0.0514,0.0275,0.7563,0.1838,0.0459,1.0906,0.5516,0.2007,0.0112,0.1808,-0.2552,-0.1850,-0.0384,0.2524,-0.1869,0.6667,1.6649,1.0889,1.1262,1.5392,2.1710,2.4063,2.1699,1.4538,2.4871,1.4754,0.5885,1.3678,1.6497,2.5248,2.6839,1.8662,1.7385,2.7607,1.9830,1.0554,1.1272,0.9668,1.2216,0.9039,0.5184,0.3716,0.1494,0.0093,-0.5436,-0.4162,-0.0357,-0.7318,-0.4773,-0.1551,0.8095,0.6068,-0.0622},
		{-0.3471,-0.3675,-0.6037,-1.2319,-1.4937,0.2604,-0.1632,0.2090,0.3466,0.1105,0.3660,0.8507,0.4491,-0.2363,0.6752,1.4732,0.2052,-0.3398,0.4662,-0.3696,-1.0478,-0.7956,0.0938,-0.5704,-0.4636,-1.4291,-1.2930,0.7163,-0.4375,0.8856,1.6152,2.5684,2.4184,3.8599,4.5973,4.2301,3.4580,3.5036,3.5895,4.1233,4.6074,3.9703,3.6406,4.7640,4.2148,3.0395,2.7692,2.1823,3.1133,1.2923,0.6526,0.4852,0.3537,-0.1220,0.4145,-0.4047,-0.2742,-0.6215,0.5080,0.2052,-0.3503,-0.1537,0.6740,0.7797,0.0276},
		{-1.0290,-1.0048,-2.0472,-2.6617,-1.7140,-1.6760,-1.5123,-1.3322,-1.1514,-1.6304,-1.0488,-0.6656,-0.1919,-0.1954,-1.1786,-0.7102,-0.8343,-1.0285,-1.1527,0.2101,-1.7788,-1.4252,-0.6879,-0.2367,-0.6479,-1.7464,-0.6564,-0.6723,-0.2201,0.9653,1.2882,1.5438,2.0909,3.2330,4.2889,3.2740,3.6705,4.6331,5.3431,3.8779,4.3881,4.0972,4.3130,4.6834,5.8981,4.9769,4.2906,3.1825,3.4691,2.3082,1.9288,0.3776,-0.0370,0.1710,-1.3429,-0.6148,-0.7779,-2.3687,-1.8646,-0.8273,-1.3801,-1.4827,-1.2967,-1.3710,-0.0474},
		{1.5918,0.8533,1.4981,1.5136,1.2984,1.2690,1.9065,2.0506,2.2186,1.9976,1.1478,1.0385,0.6880,-0.0095,0.3955,1.0607,2.1072,1.9611,1.0924,0.7326,0.9385,0.7005,0.4709,1.0685,0.9371,0.3395,0.7348,1.6064,2.5215,3.8404,4.8293,3.6825,3.8280,1.6434,1.6127,1.5475,1.1184,1.1059,0.7119,3.4556,1.3332,1.0585,1.5278,1.6646,1.3885,1.3446,3.3705,3.7874,5.1587,2.9503,2.4714,1.4258,1.3547,0.2570,-0.5882,-0.1663,-0.3223,-0.8489,-0.9530,-0.9393,-0.8860,-1.6880,-1.0662,-0.0879,-0.0022},
		{-0.5111,-1.0276,-0.2992,-0.0087,-0.6389,-0.7540,0.4171,0.5306,0.0517,0.3230,-0.4517,-0.7461,-0.6017,-0.7735,-0.3995,-0.6827,0.3884,1.9786,0.4662,-0.4429,-0.0462,-0.0035,-0.5904,-0.8687,-0.6534,0.1127,-0.4511,0.5559,2.7638,2.6920,2.5347,0.6536,0.6946,-1.1539,-1.2135,-0.9441,-1.9696,-3.2778,-4.5517,-6.0405,-5.4490,-6.0692,-6.1944,-6.3728,-4.6978,-2.1603,0.1665,0.5746,2.5992,2.3361,1.9138,1.8892,1.4151,0.0906,0.9356,0.3266,-0.7321,-1.0555,-0.9919,-0.8903,-1.1300,-1.4967,-0.9465,-0.5242,-0.0030},
		{-1.4247,-1.1172,-0.6886,-0.0695,0.3880,0.4634,1.4355,1.3571,1.6474,1.2842,0.6134,0.2826,0.5125,0.5915,-0.5923,0.8811,0.7005,1.4907,0.1987,0.6607,-0.2825,-0.8302,-0.6329,-1.5912,-1.0548,-0.3311,-0.0055,0.4352,1.5386,1.3625,-2.1421,-4.0819,-4.1715,-4.8017,-6.6938,-5.8868,-6.2774,-5.7121,-5.6545,-6.1616,-3.9312,-3.6336,-4.1793,-5.0528,-5.7362,-5.8018,-4.4586,-4.0518,-3.4204,-1.0098,0.4949,1.2999,0.6105,0.0353,0.6393,0.1318,-1.1052,-1.8567,-1.2352,-0.6928,-1.5297,-1.4549,-1.1593,-1.5577,-0.0054},
		{0.2745,0.5371,0.6266,1.4561,1.5261,2.3947,1.7588,1.6774,2.4571,2.0073,3.2860,3.3570,3.9100,2.8071,2.9198,3.2682,2.1011,1.5648,2.2085,2.6862,1.5784,0.5313,0.9331,0.0257,1.0678,1.3734,2.4169,2.2680,0.4697,-0.5533,-1.9901,-3.4890,-3.9768,-1.5285,-0.9337,-0.2554,0.4250,0.2656,0.5737,2.6728,1.5555,2.1930,2.3138,1.9718,0.6497,-1.6520,-3.2944,-3.8322,-3.2505,-1.2863,-1.0706,-0.0658,1.8561,2.5114,1.3138,2.0623,0.7709,-0.4456,-0.0458,-0.4488,-0.1814,0.0906,0.6641,0.0422,0.0129},
		{0.1963,0.5018,0.5100,2.4529,1.7282,0.3729,-0.3328,-0.4419,-0.2044,-0.4995,0.5156,0.9216,1.5721,2.1445,1.3266,1.1916,-0.4177,-0.7094,0.0747,1.9639,1.5260,1.0725,0.7010,0.5016,0.7925,1.9898,1.2948,0.0861,-1.0160,-2.4886,0.3976,1.0624,1.1759,0.6523,1.3530,1.2227,0.5776,0.8026,1.2874,-0.2634,0.9237,1.1063,1.0689,0.8701,1.1686,0.7277,1.1990,0.9169,0.1619,-1.6724,-1.9961,-2.4669,0.7171,1.2784,1.5718,1.4552,0.2227,-0.4838,-0.5289,-0.8167,-0.5532,-0.3405,-0.0065,-0.1777,0.0023},
		{0.1035,0.2741,1.0928,2.2220,1.1573,-1.4071,-1.6373,-1.8156,-1.2484,-1.7383,-1.5003,-1.5741,-1.2020,-0.6358,-2.1736,-1.0775,-2.0956,-1.4463,-1.4489,-0.0246,1.4456,1.0327,0.4815,0.4584,0.6811,1.2864,0.0212,-1.5166,-2.8135,-1.7014,1.7335,1.0036,1.7851,0.2658,1.5009,0.8196,1.0413,1.1266,1.0451,0.3474,0.1321,-1.2393,-1.2058,-0.1689,0.3930,1.7339,2.6061,2.4361,2.4314,0.5573,-0.6936,-1.3218,-0.8384,0.1270,1.7302,1.3549,0.8836,-0.0571,-0.2449,-0.5804,-0.6381,-0.2351,-0.3595,-0.0096,0.0214},
		{0.2671,0.5626,0.7681,-0.8408,-0.4635,-1.8689,-2.1807,-2.5710,-2.3128,-2.2419,-1.4648,-1.5069,-0.8061,-0.5331,-0.7253,-1.7006,-2.3265,-2.5763,-2.5042,-0.6736,-0.9844,0.3430,0.4516,-0.2388,0.5838,-0.1766,-0.5242,-2.2828,-1.8381,-0.2574,-0.7953,-0.9627,-0.7979,-0.3354,0.5528,0.0594,-0.0251,0.0841,0.4034,0.5062,0.3910,-0.1233,0.1753,-0.3065,0.1903,-0.0389,-0.0694,0.5277,-0.0588,0.0961,-0.1423,-0.6559,-1.5577,-0.6321,0.0107,1.0474,0.7233,0.0863,0.1114,-0.2839,-0.2840,-0.8285,-0.5989,-0.3248,0.0071},
		{0.9723,1.2507,0.6970,-0.7860,-1.0151,-0.0785,0.8928,0.7797,1.1039,0.7766,-0.1009,-0.4317,-0.4571,-1.3667,-1.3907,-0.5529,0.4785,1.2749,0.4410,-0.3628,-0.8935,0.4778,1.9288,1.3052,1.2196,-0.3179,-0.8900,0.2978,1.2108,-0.3150,-0.3932,0.5335,0.0997,1.3351,2.3224,1.8002,1.6456,1.7357,2.2280,2.4991,1.5217,1.3687,1.4687,1.8635,2.2400,2.0778,0.9483,0.3523,-0.9020,-0.8729,-0.7931,0.1905,1.0106,-0.0267,0.1393,1.4219,1.1985,1.6126,1.5044,1.0507,1.2628,0.5337,0.6692,0.1594,0.0338},
		{0.3428,0.4751,-1.2906,-1.5393,-1.2239,-1.0564,-0.1535,-0.2070,-0.4060,-0.4133,-1.0226,-0.8380,-0.8212,-0.9903,-1.4538,-0.5983,-0.2975,0.2678,-0.4195,-1.0872,-1.3016,-1.0277,-0.7807,0.5563,-0.6480,-1.4525,-1.4187,-0.4889,0.3803,-1.0778,-0.6308,-0.7548,-0.3992,0.4170,0.6848,0.8740,0.6294,0.8293,0.6891,0.3230,0.9278,1.0065,1.0047,0.9602,0.8272,0.5671,-0.3673,-1.0220,-2.0002,-1.2217,-1.4033,-0.9081,0.4940,0.2677,-0.4316,-0.8720,0.2086,0.8084,0.7013,0.5275,0.3795,0.1360,0.2386,-0.8460,0.0095},
		{1.0349,0.9580,-0.3518,-0.2502,0.5604,0.8667,0.1003,0.1550,0.6141,0.4914,1.1389,1.4556,1.3520,1.1667,1.9013,1.3862,0.8891,0.1154,0.6697,0.6608,0.4414,-0.2303,0.6754,1.1077,0.3942,-0.2965,1.1962,0.8266,-0.4532,0.9904,0.8050,0.0175,0.2916,-0.6535,-1.8553,-1.5263,-1.2711,-1.1697,-0.7063,-1.2155,-0.5169,-0.4619,-0.7469,-0.4565,-1.3040,-1.7594,-1.4235,-1.4998,-1.4369,0.1015,0.5912,-0.1865,0.0752,1.1703,-0.4859,-1.0969,0.6273,1.0197,0.8050,0.3105,-0.0124,0.5298,0.6580,0.3650,0.0048}
};

//zmienne do lcd - filter/nofilter
#define TOUCH_RECORD_XMIN       180
#define TOUCH_RECORD_XMAX       210
#define TOUCH_RECORD_YMIN       210
#define TOUCH_RECORD_YMAX       239

#define TOUCH_STOP_XMIN         100
#define TOUCH_STOP_XMAX         125
#define TOUCH_STOP_YMIN         210
#define TOUCH_STOP_YMAX         239

#define TOUCH_PAUSE_XMIN        20
#define TOUCH_PAUSE_XMAX        60
#define TOUCH_PAUSE_YMIN        210
#define TOUCH_PAUSE_YMAX        239

#define TOUCH_CHOOSE1_XMIN 	    20
#define TOUCH_CHOOSE1_XMAX 		125
#define TOUCH_CHOOSE1_YMIN 		100
#define TOUCH_CHOOSE1_YMAX 		239

#define TOUCH_CHOOSE2_XMAX 		230
#define TOUCH_CHOOSE2_XMIN 		125
#define TOUCH_CHOOSE2_YMIN 		100
#define TOUCH_CHOOSE2_YMAX 		239
//Private variables
//uint8_t Header_Buffer[44];
static AUDIO_IN_BufferTypeDef  BufferCtl;
static __IO uint32_t uwVolume = 100;
//extern WAVE_FormatTypeDef WaveFormat;
//extern FIL WavFile;
//static uint32_t  display_update = 1;
static float32_t mfcc_section[19][65];//mfcc for section
uint16_t base_length = 65;

//FIR Coefficients buffer generated using fir1() MATLAB function.
//fir1(28, 6/24)
/*const float32_t firCoeffs32[NUM_TAPS] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
};//*/
/*const float32_t firCoeffs32[NUM_TAPS] = {
		1, -0.9735
};//*/
// Declare State buffer of size (numTaps + blockSize - 1)
//static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];
//arm_fir_instance_f32 S;
//Private functions
//static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t* pHeader);
//static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);
//static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);
static void AUDIO_REC_DisplayButtons(void);
static void DisplayChoose(void);
static void LCD_ClearZone(void);

AUDIO_ErrorTypeDef AUDIO_REC_Start(void)
{
  uint32_t byteswritten = 0;
  uwVolume = 100;

  //init filter
  //arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], BLOCK_SIZE);
  /* Create a new file system */
 // if(f_open(&WavFile, REC_WAVE_NAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
 // {
    /* Initialize header file */
 //   WavProcess_EncInit(DEFAULT_AUDIO_IN_FREQ, Header_Buffer);
    /* Write header file */
  //  if(f_write(&WavFile, Header_Buffer, 44, (void*)&byteswritten) == FR_OK)
  //  {
      AudioState = AUDIO_STATE_PRERECORD_CHOOSE;
      DisplayChoose();
  //    {
  //      if(byteswritten != 0)
  //      {
          BSP_AUDIO_IN_InitEx(INPUT_DEVICE_DIGITAL_MICROPHONE_2, BSP_AUDIO_FREQUENCY_8K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
          BSP_AUDIO_IN_Record((uint16_t*)&BufferCtl.pcm_buff[0], AUDIO_IN_BUFFER_SIZE);//zmiana na float
          BufferCtl.fptr = byteswritten;
          BufferCtl.pcm_ptr = 0;
          BufferCtl.offset = 0;
          BufferCtl.wr_state = BUFFER_EMPTY;
          return AUDIO_ERROR_NONE;
 //       }
 //   }
//  }
 // return AUDIO_ERROR_IO;
}

AUDIO_ErrorTypeDef AUDIO_REC_Process(void)
{
  static uint32_t t = 0;
  static uint32_t cry_count = 0;
  float32_t accumulated = 0;
  float32_t accumulatedtemp = 0;
  //uint32_t byteswritten = 0;
  float32_t cepstrumtest[20]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float32_t dctout[20]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  AUDIO_ErrorTypeDef audio_error = AUDIO_ERROR_NONE;
  //uint32_t elapsed_time;
  //static uint32_t prev_elapsed_time = 0xFFFFFFFF;
  uint8_t str[20];
  uint8_t x,y;
  static TS_StateTypeDef  TS_State={0};

  switch(AudioState)
  {
  case AUDIO_STATE_PRERECORD_CHOOSE:
 	 if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
     {
 		BSP_TS_GetState(&TS_State);
     }
 	 else
 	 {
 		 BSP_TS_GetState(&TS_State);
 		 x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
 		 y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
 		 if(TS_State.touchDetected == 1)
 		 {
 			 if ((x > TOUCH_CHOOSE1_XMIN) && (x < TOUCH_CHOOSE1_XMAX) &&
 					 (y > TOUCH_CHOOSE1_YMIN) && (y < TOUCH_CHOOSE1_YMAX))
 	         {
 				 AudioState = AUDIO_STATE_BEFORE_PRERECORD;
 	         }
 			 else if ((x > TOUCH_CHOOSE2_XMIN) && (x < TOUCH_CHOOSE2_XMAX) &&
 					 (y > TOUCH_CHOOSE2_YMIN) && (y < TOUCH_CHOOSE2_YMAX))
 	         {
 				 AudioState = AUDIO_STATE_BEFORE_PRERECORD;
 	         }
 		 }
 		 else
 		 {
 			 AudioState = AUDIO_STATE_PRERECORD_CHOOSE;
 		 }
 	 }
   	  break;

  case AUDIO_STATE_BEFORE_PRERECORD:
		LCD_ClearZone();
	    AUDIO_REC_DisplayButtons();
	  AudioState = AUDIO_STATE_PRERECORD;
	break;

  case AUDIO_STATE_PRERECORD:
    if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
    {
      BSP_TS_GetState(&TS_State);
    }
    else
    {
      BSP_TS_GetState(&TS_State);
      x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      if(TS_State.touchDetected == 1)
      {
        if ((x > TOUCH_STOP_XMIN) && (x < TOUCH_STOP_XMAX) &&
            (y > TOUCH_STOP_YMIN) && (y < TOUCH_STOP_YMAX))
        {
          AudioState = AUDIO_STATE_STOP;
        }
        else if ((x > TOUCH_RECORD_XMIN) && (x < TOUCH_RECORD_XMAX) &&
                 (y > TOUCH_RECORD_YMIN) && (y < TOUCH_RECORD_YMAX))
        {
//          display_update = 1;
          AudioState = AUDIO_STATE_RECORD;
        }
      }
      else
      {
        AudioState = AUDIO_STATE_PRERECORD;
      }
    }
    break;
    break;

  case AUDIO_STATE_RECORD:
//    if (display_update)
 //   {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);    /* Display red record circle */
      BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                         (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                         (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
      BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_DisplayStringAt(15, LINE(6), (uint8_t *)"  [RECORD]", RIGHT_MODE);
 //     display_update = 0;
//    }

    if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
    {
      BSP_TS_GetState(&TS_State);
    }
    else
    {
      BSP_TS_GetState(&TS_State);
      x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      if(TS_State.touchDetected == 1)
      {
        if ((x > TOUCH_STOP_XMIN) && (x < TOUCH_STOP_XMAX) &&
            (y > TOUCH_STOP_YMIN) && (y < TOUCH_STOP_YMAX))
        {
          AudioState = AUDIO_STATE_STOP;
        }
        else if ((x > TOUCH_PAUSE_XMIN) && (x < TOUCH_PAUSE_XMAX) &&
                 (y > TOUCH_PAUSE_YMIN) && (y < TOUCH_PAUSE_YMAX))
        {
          AudioState = AUDIO_STATE_PAUSE;
        }
      }
    }

    /* Check if there are Data to write to USB Key */
    if(BufferCtl.wr_state == BUFFER_FULL)
    {
      /* write buffer in file */ //+ filtracja
    	//arm_fir_f32(&S, (uint8_t*)(BufferCtl.pcm_buff + BufferCtl.offset), outputF32, BLOCK_SIZE);
    	//outputF32=mfcc();
    	if(t<65){
    		mfcc((uint16_t *)(BufferCtl.pcm_buff + BufferCtl.offset),cepstrumtest);
    		DCT_custom(cepstrumtest,dctout,20);
    		for(uint8_t n = 1; n<20;++n){
    			mfcc_section[n-1][t] = cepstrumtest[n];
    		}
    		++t;
    	}
    	else
    	{
    		accumulated = classify(base_length,base,mfcc_section);
    		accumulatedtemp = classify(base_length,base2,mfcc_section);
    		if(accumulatedtemp<accumulated)
    			accumulated = accumulatedtemp;

    		sprintf((char *)str, "%02f", (accumulated));
    		BSP_LCD_ClearStringLine(9);
    		BSP_LCD_DisplayStringAt(83, LINE(9), str, LEFT_MODE);
    		t = 0;
    		if(accumulated<THRESHOLD)
    		{
    			BSP_LCD_ClearStringLine(14);
    			BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"CRY");
    			cry_count+=3;
    			if(cry_count>CRY_THR)
    			{
        			BSP_LCD_ClearStringLine(7);
        			BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)"ALARM", LEFT_MODE);
    			}
    		}
    		else
    		{
    			BSP_LCD_ClearStringLine(14);
    			BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"NO CRY");
    			if(cry_count>0)
    				cry_count--;
    			if(cry_count<CRY_THR/2)
    			{
    				BSP_LCD_ClearStringLine(7);
    			}
    		}
    	}

    	//if(f_write(&WavFile,(uint8_t*)(BufferCtl.pcm_buff + BufferCtl.offset), //FIXME
      //         AUDIO_IN_BUFFER_SIZE,
      //         (void*)&byteswritten) != FR_OK)
   	//	{
     //  		BSP_LCD_SetTextColor(LCD_COLOR_RED);
     //      	BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"RECORD FAIL");
    //       	return AUDIO_ERROR_IO;
   	//	}
      BufferCtl.wr_state =  BUFFER_EMPTY;
    }

    /* Display elapsed time */
 //   elapsed_time = BufferCtl.fptr / (DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * 2);
//    if(prev_elapsed_time != elapsed_time)
//    {
 //     prev_elapsed_time = elapsed_time;
 //     sprintf((char *)str, "[%02d:%02d]", (int)(elapsed_time /60), (int)(elapsed_time%60));
 //     BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
 //     BSP_LCD_DisplayStringAt(31, LINE(9), str, RIGHT_MODE);
 //     sprintf((char *)str, "%4d KB", (int)((int32_t)BufferCtl.fptr/1024));
 //     BSP_LCD_DisplayStringAt(83, LINE(9), str, LEFT_MODE);
//    }
    break;

  case AUDIO_STATE_STOP:
    /* Stop recorder */
	cry_count = 0;
    BSP_AUDIO_IN_Stop(CODEC_PDWN_SW);
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);   /* Display blue cyan record circle */
    BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                       (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                       (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(TOUCH_STOP_XMIN, TOUCH_STOP_YMIN , /* Stop rectangle */
                     TOUCH_STOP_XMAX - TOUCH_STOP_XMIN,
                     TOUCH_STOP_YMAX - TOUCH_STOP_YMIN);
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
//    display_update = 1;
    HAL_Delay(150);
//    if(f_lseek(&WavFile, 0) == FR_OK)
//    {
      /* Update the wav file header save it into wav file */
///      WavProcess_HeaderUpdate(Header_Buffer, &WaveFormat);
//      if(f_write(&WavFile, Header_Buffer, sizeof(WAVE_FormatTypeDef), (void*)&byteswritten) == FR_OK)
//      {
///        audio_error = AUDIO_ERROR_EOF;
//      }
 //     else
 //     {
//        audio_error = AUDIO_ERROR_IO;
//        BSP_LCD_SetTextColor(LCD_COLOR_RED);
//        BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"RECORD FAIL");
//      }
 //   }
//    else
 //   {
//      BSP_LCD_SetTextColor(LCD_COLOR_RED);
//      BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"RECORD FAIL");
//      audio_error = AUDIO_ERROR_IO;
//    }
    audio_error = AUDIO_ERROR_EOF;
    AudioState = AUDIO_STATE_IDLE;
    /* Close file */
//    f_close(&WavFile);
    break;

  case AUDIO_STATE_PAUSE:
	cry_count = 0;
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    /* Displays red pause rectangles */
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);   /* Display blue cyan record circle */
    BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                       (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                       (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_DisplayStringAt(15, LINE(6), (uint8_t *)"  [PAUSE] ", RIGHT_MODE);
    BSP_AUDIO_IN_Pause();
    AudioState = AUDIO_STATE_WAIT;
    break;

  case AUDIO_STATE_RESUME:
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);    /* Displays blue cyan pause rectangles */
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    /* Display red record circle */
    BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                       (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                       (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_DisplayStringAt(15, LINE(6), (uint8_t *)"  [RECORD]", RIGHT_MODE);
    BSP_AUDIO_IN_Resume();
    AudioState = AUDIO_STATE_RECORD;
    break;

  case AUDIO_STATE_NEXT:
  case AUDIO_STATE_PREVIOUS:
    AudioState = AUDIO_STATE_RECORD;
    break;

  case AUDIO_STATE_WAIT:
    if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
    {
      BSP_TS_GetState(&TS_State);
    }
    else
    {
      BSP_TS_GetState(&TS_State);
      x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      if(TS_State.touchDetected == 1)
      {
        if ((x > TOUCH_RECORD_XMIN) && (x < TOUCH_RECORD_XMAX) &&
            (y > TOUCH_RECORD_YMIN) && (y < TOUCH_RECORD_YMAX))
        {
          AudioState = AUDIO_STATE_RESUME;
        }
        else if ((x > TOUCH_PAUSE_XMIN) && (x < TOUCH_PAUSE_XMAX) &&
                 (y > TOUCH_PAUSE_YMIN) && (y < TOUCH_PAUSE_YMAX))
        {
          AudioState = AUDIO_STATE_RESUME;
        }
      }
    }
    break;

  case AUDIO_STATE_IDLE:
  case AUDIO_STATE_INIT:
  default:
    /* Do Nothing */
    break;
  }
  return audio_error;
}

void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  BufferCtl.pcm_ptr+= AUDIO_IN_BUFFER_SIZE/2;
  if(BufferCtl.pcm_ptr == AUDIO_IN_BUFFER_SIZE/2)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = 0;
  }

  if(BufferCtl.pcm_ptr >= AUDIO_IN_BUFFER_SIZE)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = AUDIO_IN_BUFFER_SIZE/2;
    BufferCtl.pcm_ptr = 0;
  }
}

void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  BufferCtl.pcm_ptr+= AUDIO_IN_BUFFER_SIZE/2;
  if(BufferCtl.pcm_ptr == AUDIO_IN_BUFFER_SIZE/2)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = 0;
  }

  if(BufferCtl.pcm_ptr >= AUDIO_IN_BUFFER_SIZE)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = AUDIO_IN_BUFFER_SIZE/2;
    BufferCtl.pcm_ptr = 0;
  }
}

//static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t *pHeader)
//{
  /* Initialize the encoder structure */
//  WaveFormat.SampleRate = Freq;        /* Audio sampling frequency */
 // WaveFormat.NbrChannels = 1;          /* Number of channels: 1:Mono or 2:Stereo */
//  WaveFormat.BitPerSample = 16;        /* Number of bits per sample (16, 24 or 32) */
//  WaveFormat.FileSize = 0x001D4C00;    /* Total length of useful audio data (payload) */
//  WaveFormat.SubChunk1Size = 44;       /* The file header chunk size */
//  WaveFormat.ByteRate = (WaveFormat.SampleRate * \
//                        (WaveFormat.BitPerSample/8) * \
//                         WaveFormat.NbrChannels);     /* Number of bytes per second  (sample rate * block align)  */
 // WaveFormat.BlockAlign = WaveFormat.NbrChannels * \
//                         (WaveFormat.BitPerSample/8); /* channels * bits/sample / 8 */

  /* Parse the wav file header and extract required information */
//  if(WavProcess_HeaderInit(pHeader, &WaveFormat))
///  {
//    return 1;
//  }
//  return 0;
//}

//static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
//{
  /* Write chunkID, must be 'RIFF'  ------------------------------------------*/
 // pHeader[0] = 'R';
 // pHeader[1] = 'I';
///  pHeader[2] = 'F';
//  pHeader[3] = 'F';

  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
//  pHeader[4] = 0x00;
//  pHeader[5] = 0x4C;
//  pHeader[6] = 0x1D;
//  pHeader[7] = 0x00;
  /* Write the file format, must be 'WAVE' -----------------------------------*/
//  pHeader[8]  = 'W';
//  pHeader[9]  = 'A';
//  pHeader[10] = 'V';
//  pHeader[11] = 'E';

  /* Write the format chunk, must be'fmt ' -----------------------------------*/
//  pHeader[12]  = 'f';
//  pHeader[13]  = 'm';
//  pHeader[14]  = 't';
//  pHeader[15]  = ' ';

  /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
//  pHeader[16]  = 0x10;
//  pHeader[17]  = 0x00;
//  pHeader[18]  = 0x00;
//  pHeader[19]  = 0x00;

  /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
//  pHeader[20]  = 0x01;
 // pHeader[21]  = 0x00;

  /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
 // pHeader[22]  = pWaveFormatStruct->NbrChannels;
 // pHeader[23]  = 0x00;

  /* Write the Sample Rate in Hz ---------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
//  pHeader[24]  = (uint8_t)((pWaveFormatStruct->SampleRate & 0xFF));
//  pHeader[25]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 8) & 0xFF);
//  pHeader[26]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 16) & 0xFF);
//  pHeader[27]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 24) & 0xFF);

  /* Write the Byte Rate -----------------------------------------------------*/
//  pHeader[28]  = (uint8_t)((pWaveFormatStruct->ByteRate & 0xFF));
//  pHeader[29]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 8) & 0xFF);
//  pHeader[30]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 16) & 0xFF);
//  pHeader[31]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 24) & 0xFF);

  /* Write the block alignment -----------------------------------------------*/
//  pHeader[32]  = pWaveFormatStruct->BlockAlign;
//  pHeader[33]  = 0x00;

  /* Write the number of bits per sample -------------------------------------*/
//  pHeader[34]  = pWaveFormatStruct->BitPerSample;
//  pHeader[35]  = 0x00;

  /* Write the Data chunk, must be 'data' ------------------------------------*/
//  pHeader[36]  = 'd';
//  pHeader[37]  = 'a';
//  pHeader[38]  = 't';
//  pHeader[39]  = 'a';

  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
//  pHeader[40]  = 0x00;
//  pHeader[41]  = 0x4C;
//  pHeader[42]  = 0x1D;
//  pHeader[43]  = 0x00;

  /* Return 0 if all operations are OK */
//  return 0;
//}

//static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
//{
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
///  pHeader[4] = (uint8_t)(BufferCtl.fptr);
//  pHeader[5] = (uint8_t)(BufferCtl.fptr >> 8);
//  pHeader[6] = (uint8_t)(BufferCtl.fptr >> 16);
//  pHeader[7] = (uint8_t)(BufferCtl.fptr >> 24);
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
//  BufferCtl.fptr -=44;
///  pHeader[40] = (uint8_t)(BufferCtl.fptr);
//  pHeader[41] = (uint8_t)(BufferCtl.fptr >> 8);
//  pHeader[42] = (uint8_t)(BufferCtl.fptr >> 16);
 // pHeader[43] = (uint8_t)(BufferCtl.fptr >> 24);

  /* Return 0 if all operations are OK */
//  return 0;
//}

static void AUDIO_REC_DisplayButtons(void)
{
  BSP_LCD_SetFont(&LCD_LOG_HEADER_FONT);
  BSP_LCD_ClearStringLine(12);            /* Clear dedicated zone */
  BSP_LCD_ClearStringLine(13);
  BSP_LCD_ClearStringLine(14);
  BSP_LCD_ClearStringLine(15);

  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2, /* Record circle */
                     (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                     (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
  BSP_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);    /* Pause rectangles */
  BSP_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
  BSP_LCD_FillRect(TOUCH_STOP_XMIN, TOUCH_STOP_YMIN , /* Stop rectangle */
                   TOUCH_STOP_XMAX - TOUCH_STOP_XMIN,
                   TOUCH_STOP_YMAX - TOUCH_STOP_YMIN);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
  BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"Use record button to start record,");
  BSP_LCD_DisplayStringAtLine(15, (uint8_t *)"stop to exit");
  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
}

static void DisplayChoose(void)
{
	  LCD_ClearZone();
	  BSP_LCD_SetFont(&LCD_LOG_HEADER_FONT);
	  BSP_LCD_ClearStringLine(12);            /* Clear dedicated zone */
	  BSP_LCD_ClearStringLine(13);
	  BSP_LCD_ClearStringLine(14);
	  BSP_LCD_ClearStringLine(15);

	  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
	  BSP_LCD_FillRect(TOUCH_CHOOSE1_XMIN, TOUCH_CHOOSE1_YMIN , /* Stop rectangle */
			  TOUCH_CHOOSE1_XMAX - TOUCH_CHOOSE1_XMIN,
			  TOUCH_CHOOSE1_YMAX - TOUCH_CHOOSE1_YMIN);

	  //BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_FillRect(TOUCH_CHOOSE2_XMIN, TOUCH_CHOOSE2_YMIN , /* Stop rectangle */
			  TOUCH_CHOOSE2_XMAX - TOUCH_CHOOSE2_XMIN,
			  TOUCH_CHOOSE2_YMAX - TOUCH_CHOOSE2_YMIN);

	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	  BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
	  BSP_LCD_DisplayStringAt(0,80,(uint8_t *)"START", CENTER_MODE);
	 // BSP_LCD_DisplayStringAt(0,80,(uint8_t *)"NO FILTER", RIGHT_MODE);
	  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
}


static void LCD_ClearZone(void)
{
  uint8_t i = 0;

  for(i= 0; i < 13; i++)
  {
    BSP_LCD_ClearStringLine(i + 3);
  }
}



