#define OBS_GARO_COUNT 2
#define OBS_SERO_COUNT 3


// 가로 장애물 2개
struct Obs_Garo {
	int pos_x;
	int pos_y;
	int rand_num;
};

// 세로 장애물 3개
struct Obs_Sero {
	int pos_x;
	int pos_y;
	int rand_num;
};

static Obs_Garo obs_garo[2];
static Obs_Sero obs_sero[3];

void InitObstacle()
{
	for (int i = 0; i < OBS_GARO_COUNT; ++i) {
		obs_garo[i].pos_x = 950;
	}

	obs_garo[0].rand_num = 100;
	obs_garo[1].rand_num = 400;


	obs_sero[0].pos_x = 200;
	obs_sero[2].pos_y = 200;
}