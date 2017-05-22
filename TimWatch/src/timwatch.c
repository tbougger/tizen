#include <tizen.h>
#include <device/battery.h>
#include "timwatch.h"

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *bg;
	Evas_Object *conform;
	Evas_Object *label_time;
	Evas_Object *label_date;
	Evas_Object *label_charge;
	Evas_Object *hour_hand;
	Evas_Object *hour_hand_shadow;
	Evas_Object *minute_hand;
	Evas_Object *minute_hand_shadow;
	Evas_Object *second_hand;
	Evas_Object *second_hand_shadow;
	Evas_Object *battery;
	Evas_Object *battery_ambient;
	Evas_Object *charge;
} appdata_s;

#define TEXT_BUF_SIZE 256

static void
move_the_hand(Evas_Object *hand, int degree)
{
	Evas_Map *m = NULL;
	m = evas_map_new(4);
	evas_map_util_points_populate_from_object(m, hand);
	evas_map_util_rotate(m, degree, 360/2, 360/2);
	evas_object_map_set(hand, m);
	evas_object_map_enable_set(hand, EINA_TRUE);
	evas_map_free(m);
}


static void
move_the_shadow(Evas_Object *hand, int degree)
{
	Evas_Map *m = NULL;
	m = evas_map_new(4);
	evas_map_util_points_populate_from_object(m, hand);
	evas_map_util_rotate(m, degree, 360/2, 360/2 + 10);
	evas_object_map_set(hand, m);
	evas_object_map_enable_set(hand, EINA_TRUE);
	evas_map_free(m);
}


static const char *DAY_OF_WEEK_STR[7] = {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"};
static const char *MONTH_STR[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
//static const char *BLACK = "000022";
//static const char *BLUE = "a1e8ff";

/*static void
update_watch(appdata_s *ad, int ambient)
{
    char date_str[TEXT_BUF_SIZE] = {0};
    char charge_str[TEXT_BUF_SIZE] = {0};
	int day_of_week = 0;
	int month = 0;
	int day = 0 ;
    char watch_text[TEXT_BUF_SIZE];
	watch_time_h watch_time = NULL;
	int hour, minute, second, millisecond;

	watch_time_get_current_time(&watch_time);

	watch_time_get_hour(watch_time, &hour);
	watch_time_get_minute(watch_time, &minute);
	watch_time_get_second(watch_time, &second);
	watch_time_get_day_of_week(watch_time, &day_of_week);
	watch_time_get_month(watch_time, &month);
	watch_time_get_day(watch_time, &day);
	watch_time_get_millisecond(watch_time, &millisecond);

    int hr = (hour*30) + (minute/2);
    //if (hr >= 360)
    //	hr = hr - 360;
	move_the_hand(ad->hour_hand, hr);
	move_the_shadow(ad->hour_hand_shadow, hr);
	move_the_hand(ad->minute_hand, minute*6);
	move_the_shadow(ad->minute_hand_shadow, minute*6);

	if (ambient == 0)
	{
		int sec = (second*6) + (6*millisecond)/1000;
		move_the_hand(ad->second_hand, sec);
		move_the_shadow(ad->second_hand_shadow, sec);

	    // Percent
	    int bat = 99;
	    char percent = '%';
	    device_battery_get_percent(&bat);
	    if (bat < 30)
	    {
	    	if (bat < 20)
	    		sprintf(charge_str, "<align=left><font_size=15><color=#FF0000>%2d%c</color></font_size></align>", bat, percent);
	    	else
	    		sprintf(charge_str, "<align=left><font_size=15><color=#000022>%2d%c</color></font_size></align>", bat, percent);
		    evas_object_color_set(ad->charge, 255, 0, 0, 255);
	    }
	    else
	    {
	    	sprintf(charge_str, "<align=left><font_size=15><color=#000022>%d%c</color></font_size></align>", bat, percent);
		    evas_object_color_set(ad->charge, 0, 0, 0, 255);
	    }
	    double remain = floor(17*(bat/100.0));
		evas_object_resize(ad->charge, (int)remain, 6);
	    elm_object_text_set(ad->label_charge, charge_str);

	    //Time
		snprintf(watch_text, TEXT_BUF_SIZE, "<align=center><color=#000022>%02d:%02d:%02d</color></align>", hour, minute, second);
	    elm_object_text_set(ad->label_time, watch_text);
	}
	else
	{
	    //Time
		snprintf(watch_text, TEXT_BUF_SIZE, "<align=center><color=#000022>%02d:%02d</color></align>", hour, minute);
	    elm_object_text_set(ad->label_time, watch_text);
	}

    // Date
    sprintf(date_str, "<align=center><font_size=25><color=#000022>%s %d %s</color></font_size></align>", DAY_OF_WEEK_STR[day_of_week-1], day, MONTH_STR[month-1]);
    elm_object_text_set(ad->label_date, date_str);

}*/

static void
updateHourMinute(appdata_s *ad, int hour, int minute, int ambient)
{
	int hr = (hour*30) + (minute/2);
	if (hr >= 360)
		hr = hr - 360;
	move_the_hand(ad->hour_hand, hr);
	move_the_hand(ad->minute_hand, minute*6);

	if (ambient == 0)
	{
		move_the_shadow(ad->hour_hand_shadow, hr);
		move_the_shadow(ad->minute_hand_shadow, minute*6);
	}
}

static void
update_watch(appdata_s *ad, int ambient, bool ambient_changed)
{
    char watch_text[TEXT_BUF_SIZE];
	watch_time_h watch_time = NULL;
	int hour, minute, second;//, millisecond;

	watch_time_get_current_time(&watch_time);


	if ( (ambient == 0) || (ambient_changed) )
	{
		watch_time_get_second(watch_time, &second);
		//watch_time_get_millisecond(watch_time, &millisecond);

		//int sec = (6*millisecond)/1000;
		//move_the_hand(ad->second_hand, (second*6) + sec);
		//move_the_shadow(ad->second_hand_shadow, (second*6) + sec);
		move_the_hand(ad->second_hand, second*6);
		move_the_shadow(ad->second_hand_shadow, second*6);

		//if (sec == 0)
		//{
		    char date_str[TEXT_BUF_SIZE] = {0};
		    char charge_str[TEXT_BUF_SIZE] = {0};
			int day_of_week = 0;
			int month = 0;
			int day = 0 ;

			watch_time_get_day_of_week(watch_time, &day_of_week);
			watch_time_get_month(watch_time, &month);
			watch_time_get_day(watch_time, &day);
			watch_time_get_hour(watch_time, &hour);
			watch_time_get_minute(watch_time, &minute);

			//Time
			snprintf(watch_text, TEXT_BUF_SIZE, "<align=center><color=#000022>%02d:%02d:%02d</color></align>", hour, minute, second);
			elm_object_text_set(ad->label_time, watch_text);

			if ( (second == 0) || (ambient_changed) )
			{
			    /*char txt_color[6] = {0};
			    if (ambient == 2)
					snprintf(txt_color, 6, "%s", BLUE);
			    else
					snprintf(txt_color, 6, "%s", BLACK);*/

				// Percent
				int bat = 99;
				char percent = '%';
				device_battery_get_percent(&bat);
				if (bat < 30)
				{
					if (bat < 20)
						sprintf(charge_str, "<align=left><font_size=15><color=#FF0000>%2d%c</color></font_size></align>", bat, percent);
					else if (ambient == 2)
						sprintf(charge_str, "<align=left><font_size=15><color=#a1e8ff>%2d%c</color></font_size></align>", bat, percent);
					else
						sprintf(charge_str, "<align=left><font_size=15><color=#000022>%2d%c</color></font_size></align>", bat, percent);

					evas_object_color_set(ad->charge, 255, 0, 0, 255);
				}
				else
				{
					if (ambient == 2)
						sprintf(charge_str, "<align=left><font_size=15><color=#a1e8ff>%d%c</color></font_size></align>", bat, percent);
					else
						sprintf(charge_str, "<align=left><font_size=15><color=#000022>%d%c</color></font_size></align>", bat, percent);

					if (ambient == 2)
						evas_object_color_set(ad->charge, 161, 232, 255, 255);//a1e8ff
					else
						evas_object_color_set(ad->charge, 0, 0, 0, 255);

				}

				int remain = (17*bat) / 100;
				evas_object_resize(ad->charge, remain, 6);
				elm_object_text_set(ad->label_charge, charge_str);

				updateHourMinute(ad, hour, minute, 0);

			    // Date
				if (ambient == 2)
					sprintf(date_str, "<align=center><font_size=25><color=#a1e8ff>%s %d %s</color></font_size></align>", DAY_OF_WEEK_STR[day_of_week-1], day, MONTH_STR[month-1]);
			    else
			    	sprintf(date_str, "<align=center><font_size=25><color=#000022>%s %d %s</color></font_size></align>", DAY_OF_WEEK_STR[day_of_week-1], day, MONTH_STR[month-1]);

			    elm_object_text_set(ad->label_date, date_str);
			}
		//}
	}
	else
	{
		watch_time_get_hour(watch_time, &hour);
		watch_time_get_minute(watch_time, &minute);

		//Time
		snprintf(watch_text, TEXT_BUF_SIZE, "<align=center><color=#a1e8ff>%02d:%02d</color></align>", hour, minute);
	    elm_object_text_set(ad->label_time, watch_text);

	    int mod = minute - 5 * (minute / 5);
	    if (mod == 0)
	    	updateHourMinute(ad, hour, minute, 1);
	}
}

static void
create_base_gui(appdata_s *ad, int width, int height)
{
	int ret;
	//watch_time_h watch_time = NULL;

	/* Window */
	ret = watch_app_get_elm_win(&ad->win);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get window. err = %d", ret);
		return;
	}

	evas_object_resize(ad->win, width, height);

	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	//ret = watch_time_get_current_time(&watch_time);
	//if (ret != APP_ERROR_NONE)
	//	dlog_print(DLOG_ERROR, LOG_TAG, "failed to get current time. err = %d", ret);

	char *resource_path = NULL;
	resource_path = app_get_resource_path();


	/* Background */
	//Evas_Object *bg = NULL;
	char bg_path[1024];
	snprintf(bg_path, sizeof(bg_path), "%s%s%s", resource_path, "images/", "watch_bg.png");
	ad->bg = elm_bg_add(ad->win);
	elm_bg_file_set(ad->bg, bg_path, NULL);
	evas_object_move(ad->bg, 0, 0);
	evas_object_resize(ad->bg, 360, 360);
	evas_object_show(ad->bg);


	/*Second Hand Shadow*/
	char second_hand_shadow_path[1024];
	snprintf(second_hand_shadow_path, sizeof(second_hand_shadow_path), "%s%s%s", resource_path, "images/", "hand_second_shadow.png");
	ad->second_hand_shadow = elm_image_add(ad->bg);
	elm_image_file_set(ad->second_hand_shadow, second_hand_shadow_path, NULL);
	evas_object_move(ad->second_hand_shadow, 180-4/2, 180-140+10);
	evas_object_resize(ad->second_hand_shadow, 4, 170);
	evas_object_show(ad->second_hand_shadow);

	/*Minute Hand Shadow*/
	char minute_hand_shadow_path[1024];
	snprintf(minute_hand_shadow_path, sizeof(minute_hand_shadow_path), "%s%s%s", resource_path, "images/", "hand_minute_shadow.png");
	ad->minute_hand_shadow = elm_image_add(ad->bg);
	elm_image_file_set(ad->minute_hand_shadow, minute_hand_shadow_path, NULL);
	evas_object_move(ad->minute_hand_shadow, 180-6/2, 180-140+10);
	evas_object_resize(ad->minute_hand_shadow, 6, 170);
	evas_object_show(ad->minute_hand_shadow);


	/*Hour Hand Shadow*/
	char hour_hand_shadow_path[1024];
	snprintf(hour_hand_shadow_path, sizeof(hour_hand_shadow_path), "%s%s%s", resource_path, "images/", "hand_hour_shadow.png");
	ad->hour_hand_shadow = elm_image_add(ad->bg);
	elm_image_file_set(ad->hour_hand_shadow, hour_hand_shadow_path, NULL);
	evas_object_move(ad->hour_hand_shadow, 180-10/2, 180-110+10);
	evas_object_resize(ad->hour_hand_shadow, 10, 140);
	evas_object_show(ad->hour_hand_shadow);

	/*Second Hand*/
	char second_hand_path[1024];
	snprintf(second_hand_path, sizeof(second_hand_path), "%s%s%s", resource_path, "images/", "hand_second.png");
	ad->second_hand = elm_image_add(ad->bg);
	elm_image_file_set(ad->second_hand, second_hand_path, NULL);
	evas_object_move(ad->second_hand, 180-4/2, 180-140);
	evas_object_resize(ad->second_hand, 4, 170);
	evas_object_show(ad->second_hand);

    /*Minute Hand*/
	char minute_hand_path[1024];
	snprintf(minute_hand_path, sizeof(minute_hand_path), "%s%s%s", resource_path, "images/", "hand_minute.png");
	ad->minute_hand = elm_image_add(ad->bg);
	elm_image_file_set(ad->minute_hand, minute_hand_path, NULL);
	evas_object_move(ad->minute_hand, 180-6/2, 180-140);
	evas_object_resize(ad->minute_hand, 6, 170);
	evas_object_show(ad->minute_hand);

    /*Hour Hand*/
	char hour_hand_path[1024];
	snprintf(hour_hand_path, sizeof(hour_hand_path), "%s%s%s", resource_path, "images/", "hand_hour.png");
	ad->hour_hand = elm_image_add(ad->bg);
	elm_image_file_set(ad->hour_hand, hour_hand_path, NULL);
	evas_object_move(ad->hour_hand, 180-10/2, 180-110);
	evas_object_resize(ad->hour_hand, 10, 140);
	evas_object_show(ad->hour_hand);

    /*Center*/
	Evas_Object *center = NULL;
	char center_path[1024];
	snprintf(center_path, sizeof(center_path), "%s%s%s", resource_path, "images/", "hands_center.png");
	center = elm_image_add(ad->bg);
	elm_image_file_set(center, center_path, NULL);
	evas_object_move(center, 180-12/2, 180-8/2);
	evas_object_resize(center, 12, 12);
	evas_object_show(center);

    /*Battery*/
	char battery_path[1024];
	snprintf(battery_path, sizeof(battery_path), "%s%s%s", resource_path, "images/", "battery.png");
	ad->battery = elm_image_add(ad->bg);
	elm_image_file_set(ad->battery, battery_path, NULL);
	evas_object_move(ad->battery, 180-27, 123);
	evas_object_resize(ad->battery, 25, 10);
	evas_object_show(ad->battery);


    /*Battery Ambient*/
	char battery_ambient_path[1024];
	snprintf(battery_ambient_path, sizeof(battery_ambient_path), "%s%s%s", resource_path, "images/", "battery_ambient.png");
	ad->battery_ambient = elm_image_add(ad->bg);
	elm_image_file_set(ad->battery_ambient, battery_ambient_path, NULL);
	evas_object_move(ad->battery_ambient, 180-27, 123);
	evas_object_resize(ad->battery_ambient, 25, 10);

    /*Charge*/
//	char charge_path[1024];
//	snprintf(charge_path, sizeof(charge_path), "%s%s%s", resource_path, "images/", "charge.png");
//	ad->charge = elm_image_add(bg);
//	elm_image_file_set(ad->charge, charge_path, NULL);
//	evas_object_move(ad->charge, 180-27+3, 125);
//	evas_object_resize(ad->charge, 17, 6);
//	evas_object_show(ad->charge);
	Evas *e;
	e = evas_object_evas_get(ad->bg);
    ad->charge = evas_object_rectangle_add(e);
    evas_object_move(ad->charge, 180-27+3, 125);
    evas_object_resize(ad->charge, 17, 6);
    evas_object_color_set(ad->charge, 0, 0, 0, 255);
    evas_object_show(ad->charge);


	/* Time Label*/
	ad->label_time = elm_label_add(ad->conform);
	evas_object_resize(ad->label_time, width, height / 5);
	evas_object_move(ad->label_time, 0, height / 5);
	evas_object_show(ad->label_time);

	/* Date Label*/
	ad->label_date = elm_label_add(ad->conform);
	evas_object_resize(ad->label_date, width, height/5);
	evas_object_move(ad->label_date, 0, 288-(height/5));
	evas_object_show(ad->label_date);

	/* Percent Label*/
	ad->label_charge = elm_label_add(ad->conform);
	evas_object_resize(ad->label_charge, 190, 15);
	evas_object_move(ad->label_charge, 180, 120);
	evas_object_show(ad->label_charge);

    //watch_app_set_time_tick_frequency(6,WATCH_APP_TIME_TICKS_PER_SECOND);
    //watch_app_set_ambient_tick_type(WATCH_APP_AMBIENT_TICK_EVERY_FIVE_MINUTES);

	//update_watch(ad, watch_time, 0);
	//watch_time_delete(watch_time);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}

static bool
app_create(int width, int height, void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad, width, height);

	update_watch(ad, 0, true);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
app_time_tick(watch_time_h watch_time, void *data)
{
	/* Called at each second while your app is visible. Update watch UI. */
	appdata_s *ad = data;
	update_watch(ad, 0, false);
}

static void
app_ambient_tick(watch_time_h watch_time, void *data)
{
	/* Called at each minute while the device is in ambient mode. Update watch UI. */
	//appdata_s *ad = data;
	//update_watch(ad, watch_time, 1);
	appdata_s *ad = data;
	update_watch(ad, 1, false);
}

static void
app_ambient_changed(bool ambient_mode, void *data)
{
	/* Update your watch UI to conform to the ambient mode */
	appdata_s *ad = data;
	//update_watch(ad, 1);
	if (ambient_mode)
	{
		evas_object_hide(ad->hour_hand_shadow);
		evas_object_hide(ad->minute_hand_shadow);
		evas_object_hide(ad->second_hand_shadow);
		evas_object_hide(ad->second_hand);
		evas_object_hide(ad->bg);
		evas_object_hide(ad->battery);
		evas_object_show(ad->battery_ambient);
		update_watch(ad, 2, true);
	}
	else
	{
		evas_object_show(ad->hour_hand_shadow);
		evas_object_show(ad->minute_hand_shadow);
		evas_object_show(ad->second_hand_shadow);
		evas_object_show(ad->second_hand);
		evas_object_show(ad->bg);
		evas_object_hide(ad->battery_ambient);
		evas_object_show(ad->battery);
		update_watch(ad, 0, true);
	}
}

static void
watch_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	app_event_get_language(event_info, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
watch_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	watch_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;
	event_callback.time_tick = app_time_tick;
	event_callback.ambient_tick = app_ambient_tick;
	event_callback.ambient_changed = app_ambient_changed;

	watch_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED],
		APP_EVENT_LANGUAGE_CHANGED, watch_app_lang_changed, &ad);
	watch_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED],
		APP_EVENT_REGION_FORMAT_CHANGED, watch_app_region_changed, &ad);
	ret = watch_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "watch_app_main() is failed. err = %d", ret);
	}

	return ret;
}

