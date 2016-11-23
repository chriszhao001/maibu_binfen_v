#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "maibu_sdk.h"
#include "maibu_res.h"

#define SIMULATE

/*标志位*/

//窗口ID
static int32_t g_window_id = -1;
//刷新图片图层ID
static int32_t g_layer_id_refreshing = -1;
//温度宽度
static int32_t g_temperature_width = -1;
//天气图片KEY
static int32_t g_weather_bmp_key = -1;
static int32_t g_tm_weather_bmp_key = -1;

//表盘的记录的当天日期，用于比较日期是否变为第二天
static uint8_t g_today_num = -1;

static char 	get_city_context[35]    = {0x01};//协议ID
static uint32_t g_gcity_timerid = -1;		//Timer ID for get city info
static uint32_t g_gweather_timerid = -1;	//Timer ID for get weather info
static uint32_t g_gcity_reqid = 0;
static uint32_t g_gweather_reqid = 0;
static char		weather_info[20] 	    = {0};//天气数据
static char 	g_if12Hours				= 0;
static char     g_ifBt					= 1;
static char		g_ifPM					= 0;
static char g_gcity_retry = 0;
static char g_gweather_retry = 0;

/*背景图片图层位置*/
#define SHOW_MORE_BG_ORIGIN_X			0	
#define SHOW_MORE_BG_ORIGIN_Y			0
#define SHOW_MORE_BG_SIZE_H				128	
#define SHOW_MORE_BG_SIZE_W				128	

/*海拔文本图层位置*/
#define SHOW_MORE_ALTITUDE_ORIGIN_X		15	
#define SHOW_MORE_ALTITUDE_ORIGIN_Y		1
#define SHOW_MORE_ALTITUDE_SIZE_H		12	
#define SHOW_MORE_ALTITUDE_SIZE_W		54	

/*蓝牙图层位置*/
#define SHOW_MORE_BT_ORIGIN_X		47	
#define SHOW_MORE_BT_ORIGIN_Y		1
#define SHOW_MORE_BT_SIZE_H			12	
#define SHOW_MORE_BT_SIZE_W			12	

/*电池剩余百分比文本图层位置*/
#define SHOW_MORE_BATTERY_PERCENT_ORIGIN_X		83	
#define SHOW_MORE_BATTERY_PERCENT_ORIGIN_Y		1
#define SHOW_MORE_BATTERY_PERCENT_SIZE_H		12	
#define SHOW_MORE_BATTERY_PERCENT_SIZE_W		24	

/*时间文本图层位置*/
#define SHOW_MORE_TIME12_ORIGIN_X		34	
#define SHOW_MORE_TIME12_ORIGIN_Y		15
#define SHOW_MORE_TIME12_SIZE_H			30	
#define SHOW_MORE_TIME12_SIZE_W			80	

#define SHOW_MORE_TIME24_ORIGIN_X		0	
#define SHOW_MORE_TIME24_ORIGIN_Y		14
#define SHOW_MORE_TIME24_SIZE_H			30	
#define SHOW_MORE_TIME24_SIZE_W			128	

/*AM/PM时间文本图层位置*/
#define SHOW_MORE_PM_ORIGIN_X			0	
#define SHOW_MORE_PM_ORIGIN_Y			15
#define SHOW_MORE_PM_SIZE_H				12	
#define SHOW_MORE_PM_SIZE_W				33	

/*定位错误图层位置*/
#define SHOW_MORE_LOCATIONERR_ORIGIN_X	60	
#define SHOW_MORE_LOCATIONERR_ORIGIN_Y	1
#define SHOW_MORE_LOCATIONERR_SIZE_H	12	
#define SHOW_MORE_LOCATIONERR_SIZE_W	9	

/*网络错误图层位置*/
#define SHOW_MORE_NETERR_ORIGIN_X		70	
#define SHOW_MORE_NETERR_ORIGIN_Y		1
#define SHOW_MORE_NETERR_SIZE_H			12	
#define SHOW_MORE_NETERR_SIZE_W			12	

/*城市名称文本图层位置*/
#define SHOW_MORE_CITY_ORIGIN_X			0	
#define SHOW_MORE_CITY_ORIGIN_Y			62
#define SHOW_MORE_CITY_SIZE_H			12	
#define SHOW_MORE_CITY_SIZE_W			34	

/*体感温度文本图层位置*/
#define SHOW_MORE_BTEMP_ORIGIN_X		94	
#define SHOW_MORE_BTEMP_ORIGIN_Y		62
#define SHOW_MORE_BTEMP_SIZE_H			12	
#define SHOW_MORE_BTEMP_SIZE_W			34	

/*节假日文本图层位置*/
#define SHOW_MORE_HOLIDAY_ORIGIN_X		94	
#define SHOW_MORE_HOLIDAY_ORIGIN_Y		62
#define SHOW_MORE_HOLIDAY_SIZE_H		12	
#define SHOW_MORE_HOLIDAY_SIZE_W		34	

/*月日周文本图层位置*/
#define SHOW_MORE_M_D_W_ORIGIN_X		0	
#define SHOW_MORE_M_D_W_ORIGIN_Y		46
#define SHOW_MORE_M_D_W_SIZE_H			12	
#define SHOW_MORE_M_D_W_SIZE_W			128	

/*农历文本图层位置*/
#define SHOW_MORE_LUNAR_ORIGIN_X		0	
#define SHOW_MORE_LUNAR_ORIGIN_Y		61
#define SHOW_MORE_LUNAR_SIZE_H			12	
#define SHOW_MORE_LUNAR_SIZE_W			128	

/*步数文本图层位置*/
#define SHOW_MORE_STEPS_ORIGIN_X		89	
#define SHOW_MORE_STEPS_ORIGIN_Y		90
#define SHOW_MORE_STEPS_SIZE_H			12	
#define SHOW_MORE_STEPS_SIZE_W			36	

/*楼层文本图层位置*/
#define SHOW_MORE_FLOORS_ORIGIN_X		93	
#define SHOW_MORE_FLOORS_ORIGIN_Y		114
#define SHOW_MORE_FLOORS_SIZE_H			12	
#define SHOW_MORE_FLOORS_SIZE_W			32	

/*行进距离图层位置*/
#define SHOW_MORE_DISTANCE_ORIGIN_X		80	
#define SHOW_MORE_DISTANCE_ORIGIN_Y		114
#define SHOW_MORE_DISTANCE_SIZE_H		12	
#define SHOW_MORE_DISTANCE_SIZE_W		45
/*天气刷新图片图层位置*/
#define SHOW_MORE_REFRESHING_ORIGIN_X	5
#define SHOW_MORE_REFRESHING_ORIGIN_Y	83
#define SHOW_MORE_REFRESHING_SIZE_H		41	
#define SHOW_MORE_REFRESHING_SIZE_W		83

/*天气图片图层位置*/
#define SHOW_MORE_WEATHER_ORIGIN_X		5	
#define SHOW_MORE_WEATHER_ORIGIN_Y		83
#define SHOW_MORE_WEATHER_SIZE_H		40
#define SHOW_MORE_WEATHER_SIZE_W		40	

/*PM2.5文本图层位置*/
#define SHOW_MORE_PM2_5_ORIGIN_X		51	
#define SHOW_MORE_PM2_5_ORIGIN_Y		90
#define SHOW_MORE_PM2_5_SIZE_H			12
#define SHOW_MORE_PM2_5_SIZE_W			24	

/*温度文本图层位置*/
#define SHOW_MORE_TEMPERATURE_ORIGIN_X	51	
#define SHOW_MORE_TEMPERATURE_ORIGIN_Y	107
#define SHOW_MORE_TEMPERATURE_SIZE_H	20
#define SHOW_MORE_TEMPERATURE_SIZE_W	38	

/*度号文本图层位置*/
#define SHOW_MORE_DU_HAO_ORIGIN_X		(SHOW_MORE_TEMPERATURE_ORIGIN_X + g_temperature_width + 1)
#define SHOW_MORE_DU_HAO_ORIGIN_Y		109
#define SHOW_MORE_DU_HAO_SIZE_H			7
#define SHOW_MORE_DU_HAO_SIZE_W			7	


/*初始数据*/

//天气相关数据
#define WEATHER_KEY			"temperature,pm25,info"
/*beijing:%e5%8c%97%e4%ba%ac*/
#define WEATHER_URL			"http://op.juhe.cn/onebox/weather/query?key=abdff0e3712c02d19845d2684bed6678&cityname="

#define WEATHER_CLOUDY		"多云"
#define WEATHER_FOG_1		"雾"
#define WEATHER_FOG_2		"霾"
#define WEATHER_OVERCAST	"阴"
#define WEATHER_RAIN		"雨"
#define WEATHER_SNOW		"雪"
#define WEATHER_SUNNY		"晴"

#define CITY_EMPTY			3
#define JSON_ERROR			-1

static char    g_city[20]   = {0};

//字符串
//电量百分比
static char g_show_more_str_battery_percent[4] = {0};
//海拔高度
// static char g_show_more_str_altitude[8] = {"0m"};
static char g_show_more_str_altitude[8] = {"0"};
//时间
static char g_show_more_str_time[6] = {"00:00"};
//月日星期
static char g_show_more_str_month_day_week[21] = {"0月0日 星期零"};
//农历月日
static char g_show_more_str_lunar[13] = {0};
//步数
static char g_show_more_str_steps[6] = {0};
//楼层
static char g_show_more_str_floors[5] = {0};
//距离
static char g_show_distance[16];
//pm2.5
static char g_show_more_str_PM2_5[4] = {0};
//温度
static char g_show_more_str_temperature[4] = {0};
//体感温度
static char g_show_more_str_bodytemperature[4] = {0};

//节日

#define HOLIDAY_LEN 		8
#define HOLIDAY_NORMAL		"平时"

static char g_show_holiday[HOLIDAY_LEN];
static char g_show_tm_temp[8];

/*其他图片数组*/
static int32_t bmp_array_name_key[7]=
{
	RES_BITMAP_WATCHFACE_WEATHER_BG,
	RES_BITMAP_WATCHFACE_WEATHER_REFRESHING,
	RES_BITMAP_WATCHFACE_WEATHER_DU_HAO,
	RES_BITMAP_WATCHFACE_STATUS_BT,
	RES_BITMAP_WATCHFACE_LOCATIONFAIL,
	RES_BITMAP_WATCHFACE_NETFAIL,
	0
};

static char wday_str[7][11]={"星期天","星期一","星期二","星期三","星期四","星期五","星期六"}; 

static P_Window init_window(void);


static uint32_t getSecondUntilNow()
{
	struct date_time t_now;
	
	app_service_get_datetime(&t_now);
	return app_get_time(&t_now);

}

static int getNumberStrlenInPix(char* pnum, int pixpernum, int pixpersign)
{
	int numlen = 0, ret = 0;

	if((pnum == NULL) || ((numlen = strlen(pnum)) <= 0))
		return 0;

	if(atoi(pnum) >= 0)
	{
		ret = numlen * pixpernum;
	}
	else
	{
		ret = (numlen - 1) *  pixpernum + pixpersign;
	}

	return ret;
}


static uint32_t get_weather_icon(char * str)
{
	uint32_t weather_icon_key = 0;
	if(strlen(str) == 0)
	{

	}
	else if(memcmp(str,WEATHER_SUNNY,strlen(WEATHER_SUNNY)) == 0 )
	{
		weather_icon_key = RES_BITMAP_WATCHFACE_WEATHER_SUNNY;
	}
	else if(memcmp(str,WEATHER_OVERCAST,strlen(WEATHER_OVERCAST)) == 0 )
	{
		weather_icon_key = RES_BITMAP_WATCHFACE_WEATHER_OVERCAST;
	}
	else if(memcmp(str,WEATHER_CLOUDY,strlen(WEATHER_CLOUDY)) == 0 )
	{
		weather_icon_key = RES_BITMAP_WATCHFACE_WEATHER_CLOUDY;
	}
	else if((memcmp(str,WEATHER_FOG_1,strlen(WEATHER_FOG_1)) == 0 )||(memcmp(str,WEATHER_FOG_2,strlen(WEATHER_FOG_2)) == 0 ))
	{
		weather_icon_key = RES_BITMAP_WATCHFACE_WEATHER_FOG;
	}
	else
	{
		if(strstr(str,WEATHER_SNOW) != NULL)
		{
			weather_icon_key = RES_BITMAP_WATCHFACE_WEATHER_SNOW;
		}
		else
		{
			weather_icon_key = RES_BITMAP_WATCHFACE_WEATHER_RAIN;
		}
	}

	return weather_icon_key;
}


//重新载入并刷新窗口所有图层
static void window_reloading(void)
{
	/*根据窗口ID获取窗口句柄*/
	P_Window p_old_window = app_window_stack_get_window_by_id(g_window_id); 
	if (NULL != p_old_window)
	{
		P_Window p_window = init_window();
		if (NULL != p_window)
		{
			g_window_id = app_window_stack_replace_window(p_old_window, p_window);
		}	
	}
	
}

/*创建并显示图片图层，需要坐标值，得到icon_key的数组，数组需要的参数值，P_Window*/
static int32_t display_target_layer(P_Window p_window,GRect *temp_p_frame,enum GAlign how_to_align,enum GColor black_or_white,int32_t bmp_array_name_key[],int bmp_id_number)
{	

	GBitmap bmp_point;
	P_Layer temp_P_Layer = NULL;

	res_get_user_bitmap(bmp_array_name_key[bmp_id_number], &bmp_point);
	LayerBitmap layer_bitmap_struct_l = {bmp_point, *temp_p_frame, how_to_align};
 	temp_P_Layer = app_layer_create_bitmap(&layer_bitmap_struct_l);
	if(temp_P_Layer != NULL)
	{
		app_layer_set_bg_color(temp_P_Layer, black_or_white);
		return app_window_add_layer(p_window, temp_P_Layer);
	}

	return 0;
}


/*创建并显示文本图层*/
static int32_t display_target_layerText(P_Window p_window,GRect  *temp_p_frame,enum GAlign how_to_align,enum GColor color,char * str,uint8_t font_type)
{
	LayerText temp_LayerText = {0};
	temp_LayerText.text = str;
	temp_LayerText.frame = *temp_p_frame;
	temp_LayerText.alignment = how_to_align;
	temp_LayerText.font_type = font_type;
	
	P_Layer p_layer = app_layer_create_text(&temp_LayerText);
	
	if(p_layer != NULL)
	{
		app_layer_set_bg_color(p_layer, color);
		return app_window_add_layer(p_window, p_layer);
	}
	return 0;
}

static P_Window init_window(void)
{
	P_Window p_window = app_window_create();
	if (NULL == p_window)
	{
		return NULL;
	}

	//添加背景图层
	GRect temp_frame ={{SHOW_MORE_BG_ORIGIN_X,SHOW_MORE_BG_ORIGIN_Y},
						{SHOW_MORE_BG_SIZE_H,SHOW_MORE_BG_SIZE_W}};
	
	display_target_layer(p_window,&temp_frame,GAlignLeft,GColorWhite,bmp_array_name_key,0);

	//添加海拔文本图层
	temp_frame.origin.x = SHOW_MORE_ALTITUDE_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_ALTITUDE_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_ALTITUDE_SIZE_H;
	temp_frame.size.w = SHOW_MORE_ALTITUDE_SIZE_W;
	// {
	// sprintf(g_show_more_str_altitude, "%d", 1234);  
	// // char MMM[2] = {'m','\0'};
	// // strcat(g_show_more_str_altitude,MMM);
	// }
	display_target_layerText(p_window,&temp_frame,GAlignLeft,GColorWhite,g_show_more_str_altitude,U_ASCII_ARIAL_12);

	//电池剩余百分比文本图层
	temp_frame.origin.x = SHOW_MORE_BATTERY_PERCENT_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_BATTERY_PERCENT_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_BATTERY_PERCENT_SIZE_H;
	temp_frame.size.w = SHOW_MORE_BATTERY_PERCENT_SIZE_W;
	// {
	// 	sprintf(g_show_more_str_battery_percent,"%d",(int)100);
	// }
	display_target_layerText(p_window,&temp_frame,GAlignRight,GColorWhite,g_show_more_str_battery_percent,U_ASCII_ARIAL_12);
	

	//添加时间文本图层
	if(g_if12Hours)
	{
		temp_frame.origin.x = SHOW_MORE_TIME12_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_TIME12_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_TIME12_SIZE_H;
		temp_frame.size.w = SHOW_MORE_TIME12_SIZE_W;
		display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,g_show_more_str_time,U_ASCII_ARIALBD_30);
		
		//添加PM/AM文本图层
		temp_frame.origin.x = SHOW_MORE_PM_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_PM_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_PM_SIZE_H;
		temp_frame.size.w = SHOW_MORE_PM_SIZE_W;
		if(g_ifPM)
			display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,"PM",U_ASCII_ARIALBD_12);
		else
			display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,"AM",U_ASCII_ARIALBD_12);
	}
	else
	{
		temp_frame.origin.x = SHOW_MORE_TIME24_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_TIME24_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_TIME24_SIZE_H;
		temp_frame.size.w = SHOW_MORE_TIME24_SIZE_W;
		display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,g_show_more_str_time,U_ASCII_ARIALBD_30);
	}
	
	//添加城市位置信息
	if(strlen(g_city) > 0)
	{
		temp_frame.origin.x = SHOW_MORE_CITY_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_CITY_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_CITY_SIZE_H;
		temp_frame.size.w = SHOW_MORE_CITY_SIZE_W;
		display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,g_city,U_ASCII_ARIALBD_12);
	}

	//添加体感温度信息
	// if(strlen(g_show_more_str_bodytemperature) > 0)
	// {
	// 	temp_frame.origin.x = SHOW_MORE_BTEMP_ORIGIN_X;
	// 	temp_frame.origin.y = SHOW_MORE_BTEMP_ORIGIN_Y;
	// 	temp_frame.size.h = SHOW_MORE_BTEMP_SIZE_H;
	// 	temp_frame.size.w = SHOW_MORE_BTEMP_SIZE_W;
	// 	display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,g_show_more_str_bodytemperature,U_ASCII_ARIALBD_12);
	// }

	if(strlen(g_show_holiday) > 0)
	{
		temp_frame.origin.x = SHOW_MORE_HOLIDAY_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_HOLIDAY_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_HOLIDAY_SIZE_H;
		temp_frame.size.w = SHOW_MORE_HOLIDAY_SIZE_W;
		display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,g_show_holiday,U_ASCII_ARIALBD_12);
	}

#define GCITYRETRY_ERRTIME	5
#define GWEATHERRETRY_ERRTIME	5
	if (g_gcity_retry > GCITYRETRY_ERRTIME)
	{
		//添加Location fail Image图层
		temp_frame.origin.x = SHOW_MORE_LOCATIONERR_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_LOCATIONERR_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_LOCATIONERR_SIZE_H;
		temp_frame.size.w = SHOW_MORE_LOCATIONERR_SIZE_W;

		display_target_layer(p_window, &temp_frame, GAlignCenter, GColorWhite, bmp_array_name_key, 4);
	}
	if (g_gweather_retry > GWEATHERRETRY_ERRTIME)
	{
		//添加Net fail Image图层
		temp_frame.origin.x = SHOW_MORE_NETERR_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_NETERR_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_NETERR_SIZE_H;
		temp_frame.size.w = SHOW_MORE_NETERR_SIZE_W;

		display_target_layer(p_window, &temp_frame, GAlignCenter, GColorWhite, bmp_array_name_key, 5);
	}

	if(!g_ifBt)
	{
		//添加BlueTooth Image图层
		temp_frame.origin.x = SHOW_MORE_BT_ORIGIN_X;
		temp_frame.origin.y = SHOW_MORE_BT_ORIGIN_Y;
		temp_frame.size.h = SHOW_MORE_BT_SIZE_H;
		temp_frame.size.w = SHOW_MORE_BT_SIZE_W;
		
		display_target_layer(p_window,&temp_frame,GAlignCenter,GColorWhite,bmp_array_name_key,3);
	}

	//添加月日周文本图层
	temp_frame.origin.x = SHOW_MORE_M_D_W_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_M_D_W_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_M_D_W_SIZE_H;
	temp_frame.size.w = SHOW_MORE_M_D_W_SIZE_W;
	
	display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,g_show_more_str_month_day_week,U_ASCII_ARIAL_12);

	//添加农历文本图层
	temp_frame.origin.x = SHOW_MORE_LUNAR_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_LUNAR_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_LUNAR_SIZE_H;
	temp_frame.size.w = SHOW_MORE_LUNAR_SIZE_W;

	display_target_layerText(p_window,&temp_frame,GAlignCenter,GColorWhite,g_show_more_str_lunar,U_ASCII_ARIAL_12);
	
	//添加步数文本图层
	temp_frame.origin.x = SHOW_MORE_STEPS_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_STEPS_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_STEPS_SIZE_H;
	temp_frame.size.w = SHOW_MORE_STEPS_SIZE_W;

	display_target_layerText(p_window,&temp_frame,GAlignRight,GColorBlack,g_show_more_str_steps,U_ASCII_ARIALBD_12);

	// //添加楼层文本图层
	// temp_frame.origin.x = SHOW_MORE_FLOORS_ORIGIN_X;
	// temp_frame.origin.y = SHOW_MORE_FLOORS_ORIGIN_Y;
	// temp_frame.size.h = SHOW_MORE_FLOORS_SIZE_H;
	// temp_frame.size.w = SHOW_MORE_FLOORS_SIZE_W;

	// display_target_layerText(p_window,&temp_frame,GAlignRight,GColorBlack,g_show_more_str_floors,U_ASCII_ARIALBD_12);

	//添加距离文本图层
	temp_frame.origin.x = SHOW_MORE_DISTANCE_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_DISTANCE_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_DISTANCE_SIZE_H;
	temp_frame.size.w = SHOW_MORE_DISTANCE_SIZE_W;

	display_target_layerText(p_window,&temp_frame,GAlignRight,GColorBlack,g_show_distance,U_ASCII_ARIALBD_12);

	//添加天气刷新图片图层
	temp_frame.origin.x = SHOW_MORE_REFRESHING_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_REFRESHING_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_REFRESHING_SIZE_H;
	temp_frame.size.w = SHOW_MORE_REFRESHING_SIZE_W;

	g_layer_id_refreshing = display_target_layer(p_window, &temp_frame, GAlignLeft, GColorWhite, bmp_array_name_key, 1);

	if(g_weather_bmp_key != -1)
	{
		maibu_layer_set_visible_status(app_window_get_layer_by_id(p_window,g_layer_id_refreshing),false);
	}
	else
	{
		maibu_layer_set_visible_status(app_window_get_layer_by_id(p_window,g_layer_id_refreshing),true);
		return p_window;
	}
	
	//添加天气图片图层
	temp_frame.origin.x = SHOW_MORE_WEATHER_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_WEATHER_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_WEATHER_SIZE_H;
	temp_frame.size.w = SHOW_MORE_WEATHER_SIZE_W;

	bmp_array_name_key[sizeof(bmp_array_name_key) - 1] = g_weather_bmp_key;
	display_target_layer(p_window,&temp_frame,GAlignLeft,GColorWhite,bmp_array_name_key,(sizeof(bmp_array_name_key) - 1));
	
	//添加PM2.5文本图层
	temp_frame.origin.x = SHOW_MORE_PM2_5_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_PM2_5_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_PM2_5_SIZE_H;
	temp_frame.size.w = SHOW_MORE_PM2_5_SIZE_W;
	
	display_target_layerText(p_window,&temp_frame,GAlignLeft,GColorBlack,g_show_more_str_PM2_5,U_ASCII_ARIALBD_12);

	//添加温度文本图层
	temp_frame.origin.x = SHOW_MORE_TEMPERATURE_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_TEMPERATURE_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_TEMPERATURE_SIZE_H;
	temp_frame.size.w = SHOW_MORE_TEMPERATURE_SIZE_W;
			
	display_target_layerText(p_window,&temp_frame,GAlignLeft,GColorBlack,g_show_more_str_temperature,U_ASCII_ARIALBD_20);

	//添加度号图片图层
	temp_frame.origin.x = SHOW_MORE_DU_HAO_ORIGIN_X;
	temp_frame.origin.y = SHOW_MORE_DU_HAO_ORIGIN_Y;
	temp_frame.size.h = SHOW_MORE_DU_HAO_SIZE_H;
	temp_frame.size.w = SHOW_MORE_DU_HAO_SIZE_W;
					
	display_target_layer(p_window,&temp_frame,GAlignLeft,GColorWhite,bmp_array_name_key,2);


	return p_window;
}

static void getTimeInfo()
{
	struct date_time datetime_perminute;
	app_service_get_datetime(&datetime_perminute);
	g_today_num = datetime_perminute.mday;
	
	if(g_if12Hours)
	{
		if(datetime_perminute.hour > 12)
		{
			datetime_perminute.hour -= 12;
			g_ifPM = 1;
		}
		else
			g_ifPM = 0;
	}
	sprintf(g_show_more_str_time, "%2d:%02d", datetime_perminute.hour, datetime_perminute.min);
}

static void data_handler_per_minute()
{
	//获取海拔数据
	float altitude, accuracy;
    maibu_get_altitude(&altitude, &accuracy);  
	double zhuan = (double)altitude;
	int zhuan2 = (int)zhuan;
	//将浮点型数据转变成整形数据
	sprintf(g_show_more_str_altitude, "%dm", zhuan2);  
	// char MMMM[2] = {'m','\0'};
	// strcat(g_show_more_str_altitude,MMMM);

	//获取电量数据
	char battery_temp = 0;
    maibu_get_battery_percent(&battery_temp);

	sprintf(g_show_more_str_battery_percent,"%d",(int)battery_temp);

	//获取时间数据
	getTimeInfo();

	//获取步数楼层
	SportData sport_datas;
    maibu_get_sport_data(&sport_datas, 0);   

	// sprintf(g_show_more_str_floors, "%d", sport_datas.floor);
	sprintf(g_show_distance, "%d", (sport_datas.distance)/100);
	sprintf(g_show_more_str_steps, "%d", sport_datas.step);
	/*
	g_steps = sport_datas.step;
	if ((g_ifShowPraise != 0) && (g_steps < PRAISE_STEP_NUM))
		g_ifShowPraise = 0;
	*/
	enum BLEStatus bstatus = maibu_get_ble_status();
	if((bstatus != BLEStatusConnected) && (bstatus != BLEStatusUsing)) 
		g_ifBt = 0;
	else
		g_ifBt = 1;

	float temp = 0;
	if(maibu_get_temperature (&temp) == 0)
	{
		sprintf(g_show_more_str_bodytemperature, "%2d", (int)temp);
	}
	
}

static void data_handler_per_day()
{
	//获取时间月日周数据
	struct date_time datetime_perday;
	int slen = 0;
	app_service_get_datetime(&datetime_perday);
	

	sprintf(g_show_more_str_month_day_week, "%d月%02d日 %s",datetime_perday.mon,datetime_perday.mday,wday_str[datetime_perday.wday]);

	//获取农历数据

	SLunarData lunar_calendar_data_struct = {
		.mon = "农历",
		.day = "暂缺",
		};
	
	maibu_get_lunar_calendar(NULL,&lunar_calendar_data_struct);

	sprintf(g_show_more_str_lunar, "%s%s",lunar_calendar_data_struct.mon,lunar_calendar_data_struct.day);
	
	slen = strlen(lunar_calendar_data_struct.festival);
	if((slen > 0) && (slen < (HOLIDAY_LEN - 1)))
	{
		strcpy(g_show_holiday, lunar_calendar_data_struct.festival);
	}
	else
	{
		slen = strlen(lunar_calendar_data_struct.solar_term);
		if((slen > 0) && (slen < (HOLIDAY_LEN - 1)))
		{
			strcpy(g_show_holiday, lunar_calendar_data_struct.solar_term);
		}
		else
		{
			strcpy(g_show_holiday, HOLIDAY_NORMAL);
		}
	}

}



static void time_change (enum SysEventType type, void *context)
{
	/*时间更改*/
	if (type == SysEventTypeTimeChange)
	{
		struct date_time datetime_now_change;
		app_service_get_datetime(&datetime_now_change);

		if(g_today_num != datetime_now_change.mday)
		{
			data_handler_per_day();
		}

		data_handler_per_minute();

		window_reloading();
	}
	
}
/*获取天气时间*/

//判断前后台
static int32_t get_front_or_back_flag(void)
{
	if(NULL == app_window_stack_get_window_by_id(g_window_id))
		return 0;
	else
		return 1;
}

static void request_get_city_info(date_time_t tick_time, uint32_t millis, void* pContext);
static void request_web_info(date_time_t tick_time, uint32_t millis, void* pContext);

static int setGetCityTimer()
{
#define GCITY_FIRST_TIMEOUT		(5 * 60 * 1000)
#define GCITY_UPDATE_TIMEOUT	(15 * 60 * 1000)

	static char	g_gcity_timer = -1;

	if (g_gcity_timer > 0)
		app_service_timer_unsubscribe(g_gcity_timer);

	if (get_front_or_back_flag() == 0)
		return -1;

	if (strlen(g_city) == 0)
		g_gcity_timer = app_service_timer_subscribe(GCITY_FIRST_TIMEOUT, request_get_city_info, NULL);
	else
	{
		if (g_gcity_retry == 0)
			g_gcity_timer = app_service_timer_subscribe((GCITY_UPDATE_TIMEOUT * 4 * 6), request_get_city_info, NULL);
		else
			g_gcity_timer = app_service_timer_subscribe(GCITY_UPDATE_TIMEOUT, request_get_city_info, NULL);
	}

	return 0;
}

//请求城市数据
static void request_get_city_info(date_time_t tick_time, uint32_t millis, void* pContext)
{
	if(setGetCityTimer() < 0)
		return;
	g_gcity_reqid = maibu_comm_request_phone(ERequestPhoneSelfDefine,(void *)&get_city_context,34);

	//如果已有城市数据同时天气数据为空，一般是表盘刷新了，这里主动多请求一次天气数据，避免位置信息出问题导致的天气数据得不到
	if ((strlen(g_city) > 0) && (g_weather_bmp_key < 0))
		request_web_info(NULL, 0, g_city);

	g_gcity_retry++;
}

//请求城市数据回调
static void get_city_info_callback(enum ERequestPhone  type, uint8_t* context, uint16_t size)
{
	if(type == ERequestPhoneSelfDefine)
	{	
		if(*(int16_t *)context == 1)
		{
			g_gcity_retry = 0;

			int8_t *context_city_name_point = (int8_t *)context + 4;	
					
			memcpy(g_city,context_city_name_point,20);
			g_city[19]='\0';
			
			if (g_weather_bmp_key < 0)
				request_web_info(NULL, 0, g_city);
		}
	}
}


static int setGetWeatherTimer()
{
#define GWEATHER_FIRST_TIMEOUT		(5 * 60 * 1000)
#define GWEATHER_UPDATE_TIMEOUT		(15 * 60 * 1000)

	static char	g_gweather_timer = -1;


	if (g_gweather_timer > 0)
		app_service_timer_unsubscribe(g_gweather_timer);

	if (get_front_or_back_flag() == 0)
		return -1;

	if (g_weather_bmp_key < 0)
		g_gweather_timer = app_service_timer_subscribe(GWEATHER_FIRST_TIMEOUT, request_web_info, g_city);
	else
	{
		if (g_gweather_retry == 0)
			g_gweather_timer = app_service_timer_subscribe((GWEATHER_UPDATE_TIMEOUT * 4), request_web_info, g_city);
		else
			g_gweather_timer = app_service_timer_subscribe(GWEATHER_UPDATE_TIMEOUT, request_web_info, g_city);
	}
		

	return 0;
}

//请求天气数据
static void request_web_info(date_time_t tick_time, uint32_t millis, void* pContext)
{
	char url[256] = {0};
	char* city_name = (char*)pContext;

	if(setGetWeatherTimer() < 0)
		return;

	if ((city_name == NULL) || (strlen(city_name) < CITY_EMPTY))
		return;

	sprintf(url,"%s",WEATHER_URL);
	int i = 0,j = 0;
	for(i;(city_name[i] != '\0')&&(j < sizeof(url)) ;++i)
	{
		sprintf(url,"%s%%%x",url,(unsigned char)city_name[i]);
	}
	
	g_gweather_reqid = maibu_comm_request_web(url, WEATHER_KEY, 0);
	g_gweather_retry++;
}

//请求天气数据回调
static void get_web_info_callback(const uint8_t *buff, uint16_t size)
{
	char buffer[40] = { 0 };

	if (maibu_get_json_str(buff, "info", buffer, sizeof(weather_info)) != JSON_ERROR)
	{
		//防止数据为空导致的数据清空
		maibu_get_json_str(buff, "info", weather_info, sizeof(weather_info));
		g_weather_bmp_key = get_weather_icon(weather_info);

	}
	else
	{
		return;
	}

	g_gweather_retry = 0;

	memset(g_show_more_str_temperature, 0, sizeof(g_show_more_str_temperature));
	memset(g_show_more_str_PM2_5, 0, sizeof(g_show_more_str_PM2_5));

	maibu_get_json_str(buff, "temperature", g_show_more_str_temperature, sizeof(g_show_more_str_temperature));
	maibu_get_json_str(buff, "pm25", g_show_more_str_PM2_5, sizeof(g_show_more_str_PM2_5));

	g_temperature_width = getNumberStrlenInPix(g_show_more_str_temperature, 11, 7);

	window_reloading();

}

static void getinfo_result_callback(enum ECommResult result, uint32_t comm_id, void *context)
{
	/*如果上一次请求城市通讯失败，并且通讯ID相同，则重新发送*/
	if ((result == ECommResultFail) && (comm_id == g_gcity_reqid))
	{
		request_get_city_info(NULL, 0, NULL);
	}

	/*如果上一次请求WEB通讯失败，并且通讯ID相同，则重新发送*/
	if ((result == ECommResultFail) && (comm_id == g_gweather_reqid))
	{
		request_web_info(NULL, 0, g_city);
	}

}

static void watchapp_comm_callback(enum ESyncWatchApp type, uint8_t *buf, uint16_t len)
{
	
	if(type == ESyncWatchAppUpdateParam)
    {

    	// P_Window p_window = app_window_stack_get_window_by_id(g_window_id);
		// if (NULL != p_window)
		// {
		// 	show_waitings(p_window,true);
		// 	app_window_update(p_window);
		// }
	    	
		if(len > 0)
		{
			if(*buf == '1')
				g_if12Hours = 1;
			else
				g_if12Hours = 0;
			getTimeInfo();
			P_Window p_window = init_window(); 
			/*放入窗口栈显示*/
			g_window_id = app_window_stack_push(p_window);
		}
	}
}

int main(void)
{
#ifdef SIMULATE
	simulator_init();
#endif
	/*注册一个事件通知回调，当有改变时，改变表盘显示数据*/
   	maibu_service_sys_event_subscribe(time_change);
	time_change(SysEventTypeTimeChange,NULL);
	
	//Clear weather flags
	g_weather_bmp_key = -1;

	//注册手机设置回调函数
	maibu_comm_register_watchapp_callback(watchapp_comm_callback);

	/*注册接受手机数据回调函数和接受web请求数据回调函数*/
	maibu_comm_register_phone_callback(get_city_info_callback);
	maibu_comm_register_web_callback(get_web_info_callback);
	/*注册通讯结果回调*/
	maibu_comm_register_result_callback(getinfo_result_callback);


	P_Window p_window = init_window(); 

	/*放入窗口栈显示*/
	g_window_id = app_window_stack_push(p_window);

	//请求GPS数据并注册GPS数据请求超时回调
	request_get_city_info(NULL, 0, NULL);

#ifdef SIMULATE
	simulator_wait();
#endif

	return 0;

}

