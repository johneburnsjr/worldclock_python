//zone_2_time = "13";//
//const char *zone_2_time
// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

using namespace rgb_matrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static int usage(const char *progname) {
  fprintf(stderr, "usage: %s [options]\n", progname);
  fprintf(stderr, "Reads text from stdin and displays it. "
          "Empty string: clear screen\n");
  fprintf(stderr, "Options:\n");
  rgb_matrix::PrintMatrixFlags(stderr);
  fprintf(stderr,
          "\t-d <time-format>  : Default '%%H:%%M'. See strftime()\n"
          "\t-f <font-file>    : Use given font.\n"
          "\t-b <brightness>   : Sets brightness percent. Default: 100.\n"
          "\t-x <x-origin>     : X-Origin of displaying text (Default: 0)\n"
          "\t-y <y-origin>     : Y-Origin of displaying text (Default: 0)\n"
          "\t-S <spacing>      : Spacing pixels between letters (Default: 0)\n"
          "\t-C <r,g,b>        : Color. Default 255,255,0\n"
          "\t-B <r,g,b>        : Background-Color. Default 0,0,0\n"
          "\t-O <r,g,b>        : Outline-Color, e.g. to increase contrast.\n"
  );

  return 1;
}

static bool parseColor(Color *c, const char *str) {
  return sscanf(str, "%hhu,%hhu,%hhu", &c->r, &c->g, &c->b) == 3;
}

static bool FullSaturation(const Color &c) {
    return (c.r == 0 || c.r == 255)
        && (c.g == 0 || c.g == 255)
        && (c.b == 0 || c.b == 255);
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;
  if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv,
                                         &matrix_options, &runtime_opt)) {
    return usage(argv[0]);
  }
	const char *zone_1_label;// const char *zone_1_time;
	const char *zone_2_label;//	const char *zone_2_time;
	const char *zone_3_label;//	const char *zone_3_time;
	const char *zone_4_label;//	const char *zone_4_time;
	const char *zone_5_label;//	const char *zone_5_time;
	const char *zone_6_label;//	const char *zone_6_time;
	const char *zone_7_label;//	const char *zone_7_time;
	const char *zone_8_label;//	const char *zone_8_time;


//===================================================================
//=========================== TIME ZONES ============================
//===================================================================

	zone_1_label = "NYC";           //zone_1_time = text_buffer05;
	zone_2_label = "LA";
	zone_3_label = "London";
	zone_4_label = "Paris";
	zone_5_label = "Moscow";
	zone_6_label = "Tokyo";
	zone_7_label = "Sydney";
	zone_8_label = "Hong K";

//===================================================================
//===================================================================
//===================================================================


  const char *time_format = "%H:%M";
  Color color(255,0,0);
  Color bg_color(0, 0, 0);
  Color outline_color(0,0,0);
  Color color1(255,255,255);
  Color color2(255,255,255);
  bool with_outline = false;

  const char *bdf_font_file = "../fonts/5x8.bdf";
// TESTING  const char  *chain_length = "4";
  int x_orig = 0;
  int y_orig = 0;
  int brightness = 90;
  int letter_spacing = 0;
  int opt;


  while ((opt = getopt(argc, argv, "x:y:f:C:B:O:b:S:d:")) != -1) {
    switch (opt) {
    case 'd': time_format = strdup(optarg); break;
    case 'b': brightness = atoi(optarg); break;
    case 'x': x_orig = atoi(optarg); break;
    case 'y': y_orig = atoi(optarg); break;
    case 'f': bdf_font_file = strdup(optarg); break;
    case 'S': letter_spacing = atoi(optarg); break;
    case 'C':
      if (!parseColor(&color, optarg)) {
        fprintf(stderr, "Invalid color spec: %s\n", optarg);
        return usage(argv[0]);
      }
      break;
    case 'B':
      if (!parseColor(&bg_color, optarg)) {
        fprintf(stderr, "Invalid background color spec: %s\n", optarg);
        return usage(argv[0]);
      }
      break;
    case 'O':
      if (!parseColor(&outline_color, optarg)) {
        fprintf(stderr, "Invalid outline color spec: %s\n", optarg);
        return usage(argv[0]);
      }
      with_outline = true;
      break;
    default:
      return usage(argv[0]);
    }
  }

  if (bdf_font_file == NULL) {
    fprintf(stderr, "Need to specify BDF font-file with -f\n");
    return usage(argv[0]);
  }

  /*
   * Load font. This needs to be a filename with a bdf bitmap font.
   */

  rgb_matrix::Font font;
  if (!font.LoadFont(bdf_font_file)) {
    fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file);
  return 1;
  }
  rgb_matrix::Font *outline_font = NULL;
  if (with_outline) {
      outline_font = font.CreateOutlineFont();
  }

  if (brightness < 1 || brightness > 100) {
    fprintf(stderr, "Brightness is outside usable range.\n");
    return 1;
  }

  RGBMatrix *matrix = rgb_matrix::CreateMatrixFromOptions(matrix_options,
                                                          runtime_opt);
  if (matrix == NULL)
    return 1;

  matrix->SetBrightness(brightness);

  const bool all_extreme_colors = (brightness == 100)
      && FullSaturation(color)
      && FullSaturation(bg_color)
      && FullSaturation(outline_color);
  if (all_extreme_colors)
      matrix->SetPWMBits(1);

  const int x = x_orig;
  int y = y_orig;
  FrameCanvas *offscreen = matrix->CreateFrameCanvas();

  char text_buffer[256];
//TESTING  const int chain = 4;
  
  //=============================================================
  //=================TIME ZONES==================================
  //=============================================================
  
  //05 = GMT-5    15 = GMT+5    112 = GMT+12
  
  //GMT-1 -------------------------------------------------------
  char text_buffer01[256];
  struct timespec next_time01;
  next_time01.tv_sec = time(NULL);
  next_time01.tv_nsec = 0;
  struct tm tm01;
  //GMT-2 -------------------------------------------------------
  char text_buffer02[256];
  struct timespec next_time02;
  next_time02.tv_sec = time(NULL);
  next_time02.tv_nsec = 0;
  struct tm tm02;
  //GMT-03 -------------------------------------------------------
  char text_buffer03[256];
  struct timespec next_time03;
  next_time03.tv_sec = time(NULL);
  next_time03.tv_nsec = 0;
  struct tm tm03;
  //GMT-04 -------------------------------------------------------
  char text_buffer04[256];
  struct timespec next_time04;
  next_time04.tv_sec = time(NULL);
  next_time04.tv_nsec = 0;
  struct tm tm04;
  //GMT-5 -------------------------------------------------------
  char text_buffer05[256];
  struct timespec next_time05;
  next_time05.tv_sec = time(NULL);
  next_time05.tv_nsec = 0;
  struct tm tm05;
  //GMT-06 -------------------------------------------------------
  char text_buffer06[256];
  struct timespec next_time06;
  next_time06.tv_sec = time(NULL);
  next_time06.tv_nsec = 0;
  struct tm tm06;
  //GMT-07 -------------------------------------------------------
  char text_buffer07[256];
  struct timespec next_time07;
  next_time07.tv_sec = time(NULL);
  next_time07.tv_nsec = 0;
  struct tm tm07;
  //GMT-08 -------------------------------------------------------
  char text_buffer08[256];
  struct timespec next_time08;
  next_time08.tv_sec = time(NULL);
  next_time08.tv_nsec = 0;
  struct tm tm08;
  //GMT-09 -------------------------------------------------------
  char text_buffer09[256];
  struct timespec next_time09;
  next_time09.tv_sec = time(NULL);
  next_time09.tv_nsec = 0;
  struct tm tm09;
  //GMT-010 -------------------------------------------------------
  char text_buffer010[256];
  struct timespec next_time010;
  next_time010.tv_sec = time(NULL);
  next_time010.tv_nsec = 0;
  struct tm tm010;
  //GMT-011 -------------------------------------------------------
  char text_buffer011[256];
  struct timespec next_time011;
  next_time011.tv_sec = time(NULL);
  next_time011.tv_nsec = 0;
  struct tm tm011;
  //GMT-012 -------------------------------------------------------
  char text_buffer012[256];
  struct timespec next_time012;
  next_time012.tv_sec = time(NULL);
  next_time012.tv_nsec = 0;
  struct tm tm012;
  //GMT-11 -------------------------------------------------------
  char text_buffer11[256];
  struct timespec next_time11;
  next_time11.tv_sec = time(NULL);
  next_time11.tv_nsec = 0;
  struct tm tm11;
  //GMT-12 -------------------------------------------------------
  char text_buffer12[256];
  struct timespec next_time12;
  next_time12.tv_sec = time(NULL);
  next_time12.tv_nsec = 0;
  struct tm tm12;
  //GMT-13 -------------------------------------------------------
  char text_buffer13[256];
  struct timespec next_time13;
  next_time13.tv_sec = time(NULL);
  next_time13.tv_nsec = 0;
  struct tm tm13;
  //GMT-14 -------------------------------------------------------
  char text_buffer14[256];
  struct timespec next_time14;
  next_time14.tv_sec = time(NULL);
  next_time14.tv_nsec = 0;
  struct tm tm14;
  //GMT-15 -------------------------------------------------------
  char text_buffer15[256];
  struct timespec next_time15;
  next_time15.tv_sec = time(NULL);
  next_time15.tv_nsec = 0;
  struct tm tm15;
  //GMT-16 -------------------------------------------------------
  char text_buffer16[256];
  struct timespec next_time16;
  next_time16.tv_sec = time(NULL);
  next_time16.tv_nsec = 0;
  struct tm tm16;
  //GMT-17 -------------------------------------------------------
  char text_buffer17[256];
  struct timespec next_time17;
  next_time17.tv_sec = time(NULL);
  next_time17.tv_nsec = 0;
  struct tm tm17;
  //GMT-18 -------------------------------------------------------
  char text_buffer18[256];
  struct timespec next_time18;
  next_time18.tv_sec = time(NULL);
  next_time18.tv_nsec = 0;
  struct tm tm18;
  //GMT-19 -------------------------------------------------------
  char text_buffer19[256];
  struct timespec next_time19;
  next_time19.tv_sec = time(NULL);
  next_time19.tv_nsec = 0;
  struct tm tm19;
  //GMT-110 -------------------------------------------------------
  char text_buffer110[256];
  struct timespec next_time110;
  next_time110.tv_sec = time(NULL);
  next_time110.tv_nsec = 0;
  struct tm tm110;
  //GMT-111 -------------------------------------------------------
  char text_buffer111[256];
  struct timespec next_time111;
  next_time111.tv_sec = time(NULL);
  next_time111.tv_nsec = 0;
  struct tm tm111;
  //GMT-112 -------------------------------------------------------
  char text_buffer112[256];
  struct timespec next_time112;
  next_time112.tv_sec = time(NULL);
  next_time112.tv_nsec = 0;
  struct tm tm112;
//=============================================================
//============= END ===========================================
//=============================================================
  
  struct timespec next_time;
  next_time.tv_sec = time(NULL);
  next_time.tv_nsec = 0;
  struct tm tm;
  
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);
//  int xoff=+32;

  while (!interrupt_received) {
      next_time.tv_sec = time(NULL);
      next_time.tv_nsec = 0;
      localtime_r(&next_time.tv_sec, &tm);
      strftime(text_buffer, sizeof(text_buffer), time_format, &tm);

//=================================================================
//===========TIME ZONES ===========================================
//=================================================================
      //Creates new text_beffer here for every time zone
      //GMT-01 -----------------------------------------------------
      next_time01.tv_sec = time(NULL)-3600;
      next_time01.tv_nsec = 0;
      localtime_r(&next_time01.tv_sec, &tm01);
      strftime(text_buffer01, sizeof(text_buffer01), time_format, &tm01);
      //GMT-02 -----------------------------------------------------
      next_time02.tv_sec = time(NULL)-7200;
      next_time02.tv_nsec = 0;
      localtime_r(&next_time02.tv_sec, &tm02);
      strftime(text_buffer02, sizeof(text_buffer02), time_format, &tm02);
      //GMT-03 -----------------------------------------------------
      next_time03.tv_sec = time(NULL)-10800;
      next_time03.tv_nsec = 0;
      localtime_r(&next_time03.tv_sec, &tm03);
      strftime(text_buffer03, sizeof(text_buffer03), time_format, &tm03);
      //GMT-04 -----------------------------------------------------
      next_time04.tv_sec = time(NULL)-14400;
      next_time04.tv_nsec = 0;
      localtime_r(&next_time04.tv_sec, &tm04);
      strftime(text_buffer04, sizeof(text_buffer04), time_format, &tm04);
      //GMT-5 -----------------------------------------------------
      next_time05.tv_sec = time(NULL)-18000;
      next_time05.tv_nsec = 0;
      localtime_r(&next_time05.tv_sec, &tm05);
      strftime(text_buffer05, sizeof(text_buffer05), time_format, &tm05);
      //GMT-06 -----------------------------------------------------
      next_time06.tv_sec = time(NULL)-21600;
      next_time06.tv_nsec = 0;
      localtime_r(&next_time06.tv_sec, &tm06);
      strftime(text_buffer06, sizeof(text_buffer06), time_format, &tm06);
      //GMT-07 -----------------------------------------------------
      next_time07.tv_sec = time(NULL)-25200;
      next_time07.tv_nsec = 0;
      localtime_r(&next_time07.tv_sec, &tm07);
      strftime(text_buffer07, sizeof(text_buffer07), time_format, &tm07);
      //GMT-08 -----------------------------------------------------
      next_time08.tv_sec = time(NULL)-28800;
      next_time08.tv_nsec = 0;
      localtime_r(&next_time08.tv_sec, &tm08);
      strftime(text_buffer08, sizeof(text_buffer08), time_format, &tm08);
      //GMT-09 -----------------------------------------------------
      next_time09.tv_sec = time(NULL)-32400;
      next_time09.tv_nsec = 0;
      localtime_r(&next_time09.tv_sec, &tm09);
      strftime(text_buffer09, sizeof(text_buffer09), time_format, &tm09);
      //GMT-010 -----------------------------------------------------
      next_time010.tv_sec = time(NULL)-36000;
      next_time010.tv_nsec = 0;
      localtime_r(&next_time010.tv_sec, &tm010);
      strftime(text_buffer010, sizeof(text_buffer010), time_format, &tm010);
      //GMT-011 -----------------------------------------------------
      next_time011.tv_sec = time(NULL)-39600;
      next_time011.tv_nsec = 0;
      localtime_r(&next_time011.tv_sec, &tm011);
      strftime(text_buffer011, sizeof(text_buffer011), time_format, &tm011);
      //GMT-012 -----------------------------------------------------
      next_time012.tv_sec = time(NULL)-43200;
      next_time012.tv_nsec = 0;
      localtime_r(&next_time012.tv_sec, &tm012);
      strftime(text_buffer012, sizeof(text_buffer012), time_format, &tm012);
      //GMT-11 -----------------------------------------------------
      next_time11.tv_sec = time(NULL)+3600;
      next_time11.tv_nsec = 0;
      localtime_r(&next_time11.tv_sec, &tm11);
      strftime(text_buffer11, sizeof(text_buffer11), time_format, &tm11);
      //GMT-12 -----------------------------------------------------
      next_time12.tv_sec = time(NULL)+7200;
      next_time12.tv_nsec = 0;
      localtime_r(&next_time12.tv_sec, &tm12);
      strftime(text_buffer12, sizeof(text_buffer12), time_format, &tm12);
      //GMT-13 -----------------------------------------------------
      next_time13.tv_sec = time(NULL)+10800;
      next_time13.tv_nsec = 0;
      localtime_r(&next_time13.tv_sec, &tm13);
      strftime(text_buffer13, sizeof(text_buffer13), time_format, &tm13);
      //GMT-14 -----------------------------------------------------
      next_time14.tv_sec = time(NULL)+14400;
      next_time14.tv_nsec = 0;
      localtime_r(&next_time14.tv_sec, &tm14);
      strftime(text_buffer14, sizeof(text_buffer14), time_format, &tm14);
      //GMT-15 -----------------------------------------------------
      next_time15.tv_sec = time(NULL)+18000;
      next_time15.tv_nsec = 0;
      localtime_r(&next_time15.tv_sec, &tm15);
      strftime(text_buffer15, sizeof(text_buffer15), time_format, &tm15);
      //GMT-16 -----------------------------------------------------
      next_time16.tv_sec = time(NULL)+21600;
      next_time16.tv_nsec = 0;
      localtime_r(&next_time16.tv_sec, &tm16);
      strftime(text_buffer16, sizeof(text_buffer16), time_format, &tm16);
      //GMT-17 -----------------------------------------------------
      next_time17.tv_sec = time(NULL)+25200;
      next_time17.tv_nsec = 0;
      localtime_r(&next_time17.tv_sec, &tm17);
      strftime(text_buffer17, sizeof(text_buffer17), time_format, &tm17);
      //GMT-18 -----------------------------------------------------
      next_time18.tv_sec = time(NULL)+28800;
      next_time18.tv_nsec = 0;
      localtime_r(&next_time18.tv_sec, &tm18);
      strftime(text_buffer18, sizeof(text_buffer18), time_format, &tm18);
      //GMT-19 -----------------------------------------------------
      next_time19.tv_sec = time(NULL)+32400;
      next_time19.tv_nsec = 0;
      localtime_r(&next_time19.tv_sec, &tm19);
      strftime(text_buffer19, sizeof(text_buffer19), time_format, &tm19);
      //GMT-110 -----------------------------------------------------
      next_time110.tv_sec = time(NULL)+36000;
      next_time110.tv_nsec = 0;
      localtime_r(&next_time110.tv_sec, &tm110);
      strftime(text_buffer110, sizeof(text_buffer110), time_format, &tm110);
      //GMT-111 -----------------------------------------------------
      next_time111.tv_sec = time(NULL)+39600;
      next_time111.tv_nsec = 0;
      localtime_r(&next_time111.tv_sec, &tm111);
      strftime(text_buffer111, sizeof(text_buffer111), time_format, &tm111);
      //GMT-112 -----------------------------------------------------
      next_time112.tv_sec = time(NULL)+43200;
      next_time112.tv_nsec = 0;
      localtime_r(&next_time112.tv_sec, &tm112);
      strftime(text_buffer112, sizeof(text_buffer112), time_format, &tm112);
//=============================================================
//=========== END =============================================
//=============================================================




      offscreen->Fill(bg_color.r, bg_color.g, bg_color.b);
      
      
      if (outline_font) {
          rgb_matrix::DrawText(offscreen, *outline_font,
                               x + 1, y + font.baseline(),
                               outline_color, NULL, text_buffer,
                               letter_spacing - 2);
      }

//x + xoff makes scroll
//---------Time Zone 1--------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x + 2, font.baseline() -1,
                           color, NULL, text_buffer05,
                           letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x + 2 , font.baseline()*2 -1,
                           color2, NULL, zone_1_label,
                           letter_spacing);
/*      rgb_matrix::DrawText(offscreen, font, x, font.baseline()*3 -6,
                           color, NULL, "________________________________________",
                           letter_spacing -1);
*/
//-------Time Zone 2----------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x +2, 3 + font.baseline()*3,
                         color, NULL, text_buffer08 ,
                         letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x +2, 3 + font.baseline()*4,
                           color2, NULL, zone_2_label ,
                           letter_spacing);
//-------Time Zone 3----------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x + 34, font.baseline() -1,
                         color, NULL, text_buffer ,
                         letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x + 34, font.baseline()*2 -1,
                           color2, NULL, zone_3_label ,
                           letter_spacing);
//-------Time Zone 4----------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x + 34, 3 + font.baseline()*3,
                         color, NULL, text_buffer11 ,
                         letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x +34, 3 + font.baseline()*4,
                           color2, NULL, zone_4_label ,
                           letter_spacing);
//-------Time Zone 5----------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x + 66, font.baseline() -1,
                         color, NULL, text_buffer12 ,
                         letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x + 66, font.baseline()*2 -1,
                           color2, NULL, zone_5_label ,
                           letter_spacing);
//-------Time Zone 6----------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x + 66, 3 + font.baseline()*3,
                         color, NULL, text_buffer18 ,
                         letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x +66, 3 + font.baseline()*4,
                           color2, NULL, zone_6_label ,
                           letter_spacing);
//-------Time Zone 7----------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x + 98, font.baseline () -1,
                         color, NULL, text_buffer19 ,
                         letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x + 98, font.baseline()*2 -1,
                           color2, NULL, zone_7_label ,
                           letter_spacing);
//-------Time Zone 8----------------------------------------------------
      rgb_matrix::DrawText(offscreen, font, x + 98, 3 + font.baseline()*3,
                         color, NULL, text_buffer17 ,
                         letter_spacing);
      rgb_matrix::DrawText(offscreen, font, x +98, 3 + font.baseline()*4,
                           color2, NULL, zone_8_label ,
                           letter_spacing);
//-------END------------------------------------------------------------

      clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next_time, NULL);
      offscreen = matrix->SwapOnVSync(offscreen);
      next_time.tv_sec += 1;

//change to +=1 for scrolling right
//      xoff +=1;
//      if(xoff>32) xoff=-30;
  }

  // Finished. Shut down the RGB matrix.
  matrix->Clear();
  delete matrix;

  write(STDOUT_FILENO, "\n", 1);  // Create a fresh new line after ^C on screen
  return 0;
}

