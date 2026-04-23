/**
 * @file lv_conf.h
 * LVGL configuration for Sharp Memory LCD 128x128 monochrome display
 * on nRF52840 (Neram project).
 */

#if 1 /* Set to "1" to enable */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
/* 1-bit monochrome — matches the Sharp Memory LCD */
#define LV_COLOR_DEPTH 1

/* Swap the 2 bytes of RGB565 color (not used at 1-bit, but required) */
#define LV_COLOR_16_SWAP 0

/* Enable features to draw on transparent background */
#define LV_COLOR_SCREEN_TRANSP 0

/* Dots Per Inch — approx 130 DPI for a 1.3" 128x128 display */
#define LV_DPI_DEF 130

/*=========================
   MEMORY SETTINGS
 *=========================*/
/* Use LVGL's built-in heap */
#define LV_MEM_CUSTOM 0
/* 20 KB — default theme alone can consume several KB on init */
#define LV_MEM_SIZE (20 * 1024U)
/* Set an address for the memory pool. 0 = statically allocated. */
#define LV_MEM_ADR 0
/* Automatically defragment the heap when a `lv_mem_alloc` call fails */
#define LV_MEM_AUTO_DEFRAG 1
/* 0 = no metadata, useful in release builds */
#define LV_MEMCPY_MEMSET_STD 0

/*====================
   HAL SETTINGS
 *====================*/
/* Default display refresh period in ms (~30 Hz) */
#define LV_DISP_DEF_REFR_PERIOD 33

/* Input device read period in ms */
#define LV_INDEV_DEF_READ_PERIOD 30

/* Use a custom tick source. 0 = use lv_tick_inc() manually */
#define LV_TICK_CUSTOM 0

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/
/* Keep LV_DRAW_COMPLEX enabled — required to avoid internal assertion failures */
#define LV_DRAW_COMPLEX 1
#define LV_SHADOW_CACHE_SIZE 0
#define LV_CIRCLE_CACHE_SIZE 4
#define LV_IMG_CACHE_DEF_SIZE 1
#define LV_GRADIENT_MAX_STOPS 2
#define LV_GRAD_CACHE_DEF_SIZE 0
#define LV_DISP_ROT_MAX_BUF (10 * 1024)

/*==================
 * GPU
 *==================*/
#define LV_USE_GPU_NXP_PXP 0
#define LV_USE_GPU_NXP_VG_LITE 0
#define LV_USE_GPU_STM32_DMA2D 0
#define LV_USE_GPU_SWM341_DMA2D 0
#define LV_USE_GPU_ARM2D 0

/*==================
 * LOGGING
 *==================*/
#define LV_USE_LOG 0
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 0
    #define LV_LOG_TRACE_MEM 0
    #define LV_LOG_TRACE_TIMER 0
    #define LV_LOG_TRACE_INDEV 0
    #define LV_LOG_TRACE_DISP_REFR 0
    #define LV_LOG_TRACE_EVENT 0
    #define LV_LOG_TRACE_OBJ_CREATE 0
    #define LV_LOG_TRACE_LAYOUT 0
    #define LV_LOG_TRACE_ANIM 0
#endif

/*=================
 * ASSERT
 *================*/
#define LV_USE_ASSERT_NULL 0
#define LV_USE_ASSERT_MALLOC 0
#define LV_USE_ASSERT_STYLE 0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ 0
#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER while (1);

/*==================
 * DEBUG
 *==================*/
#define LV_USE_PERF_MONITOR 0
#define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
#define LV_USE_MEM_MONITOR 0
#define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
#define LV_USE_REFR_DEBUG 0

/*==================
 * COMPILER SETTINGS
 *==================*/
#define LV_BIG_ENDIAN_SYSTEM 0
#define LV_ATTRIBUTE_TICK_INC
#define LV_ATTRIBUTE_TIMER_HANDLER
#define LV_ATTRIBUTE_FLUSH_READY
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 4
#define LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_LARGE_CONST
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY
#define LV_ATTRIBUTE_FAST_MEM
#define LV_ATTRIBUTE_DMA
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning
#define LV_USE_LARGE_COORD 0

/*==================
 * FONT USAGE
 *==================*/
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0
#define LV_FONT_MONTSERRAT_12_SUBPX 0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0
#define LV_FONT_SIMSUN_16_CJK 0
#define LV_FONT_UNSCII_8 0
#define LV_FONT_UNSCII_16 0

#define LV_FONT_CUSTOM_DECLARE
#define LV_FONT_DEFAULT &lv_font_montserrat_14

#define LV_USE_FONT_COMPRESSED 0
#define LV_USE_FONT_SUBPX 0

/*==================
 * TEXT SETTINGS
 *==================*/
#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS " ,.;:-_"
#define LV_TXT_LINE_BREAK_LONG_LEN 0
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3
#define LV_TXT_COLOR_CMD "#"
#define LV_USE_BIDI 0
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
 * THEMES
 * Must be declared explicitly — undeclared themes default to 1 in
 * lv_conf_internal.h and can exhaust the heap on first lv_disp_drv_register().
 *==================*/
#define LV_USE_THEME_DEFAULT 0  /* Heavy; requires gradients and many style props */
#define LV_USE_THEME_BASIC   0  /* Still allocates more than needed */
#define LV_USE_THEME_MONO    1  /* Dedicated 1-bit monochrome theme — use this */

/*==================
 * WIDGETS
 *==================*/
#define LV_USE_ARC        0
#define LV_USE_BAR        0
#define LV_USE_BTN        0
#define LV_USE_BTNMATRIX  0
#define LV_USE_CANVAS     0
#define LV_USE_CHECKBOX   0
#define LV_USE_DROPDOWN   0
#define LV_USE_IMG        0
#define LV_USE_LINE       0
#define LV_USE_ROLLER     0
#define LV_USE_SLIDER     0
#define LV_USE_SWITCH     0
#define LV_USE_TEXTAREA   0
#define LV_USE_TABLE      0

#define LV_USE_LABEL 1
    #define LV_LABEL_TEXT_SELECTION 0
    #define LV_LABEL_LONG_TXT_HINT  0

/*==================
 * EXTRA COMPONENTS
 *==================*/
/* Extra widgets — all disabled; must be explicit or they default to enabled */
#define LV_USE_ANIMIMG    0
#define LV_USE_CALENDAR   0
#define LV_USE_CHART      0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGBTN     0
#define LV_USE_KEYBOARD   0
#define LV_USE_LED        0
#define LV_USE_LIST       0
#define LV_USE_MENU       0
#define LV_USE_METER      0
#define LV_USE_MSGBOX     0
#define LV_USE_SPAN       0
#define LV_USE_SPINBOX    0
#define LV_USE_SPINNER    0
#define LV_USE_TABVIEW    0
#define LV_USE_TILEVIEW   0
#define LV_USE_WIN        0

/* Layouts */
#define LV_USE_FLEX 0
#define LV_USE_GRID 0

/* File systems — none needed */
#define LV_USE_FS_FATFS  0
#define LV_USE_FS_PC     0
#define LV_USE_FS_POSIX  0
#define LV_USE_FS_WIN32  0
#define LV_USE_FS_STDIO  0

/* Image/animation decoders — not used */
#define LV_USE_PNG    0
#define LV_USE_BMP    0
#define LV_USE_SJPG   0
#define LV_USE_GIF    0
#define LV_USE_QRCODE 0

/* Advanced font rendering — not needed */
#define LV_USE_FREETYPE 0
#define LV_USE_RLOTTIE  0
#define LV_USE_FFT      0

/* Misc extras */
#define LV_USE_SNAPSHOT  0
#define LV_USE_MONKEY    0
#define LV_USE_GRIDNAV   0
#define LV_USE_FRAGMENT  0
#define LV_USE_IMGFONT   0
#define LV_USE_MSG       0
#define LV_USE_IME_PINYIN 0

/* Examples & demos — disabled to save flash */
#define LV_BUILD_EXAMPLES       0
#define LV_USE_DEMO_WIDGETS     0
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0
#define LV_USE_DEMO_BENCHMARK   0
#define LV_USE_DEMO_STRESS      0
#define LV_USE_DEMO_MUSIC       0

#endif /* LV_CONF_H */

#endif /* "Content enable" */
