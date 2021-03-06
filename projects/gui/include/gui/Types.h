#ifndef REMOXLY_GUI_TYPES_H
#define REMOXLY_GUI_TYPES_H

#define GUI_IS_INSIDE_WIDGET(wid, mx, my) ((mx >= wid->x) && (mx <= (wid->x + wid->w)) && (my >= wid->y)  && (my <= (wid->y + wid->h)))
#define GUI_IS_INSIDE(mx, my, x, y, w, h) ((mx >= x) && (mx <= (x+w)) && (my >= y) && (my <= (y+h)))

#define GUI_TYPE_NONE                        0
#define GUI_TYPE_GROUP                       1          /* The Group instance. */
#define GUI_TYPE_SLIDER_INT                  2          /* Slider for integer values. */
#define GUI_TYPE_SLIDER_FLOAT                3          /* Slider for float values. */
#define GUI_TYPE_BUTTON                      4          /* A basic "clickable" button. */
#define GUI_TYPE_TOGGLE                      5          /* On/Off toggle for boolean values */
#define GUI_TYPE_COLOR_RGB                   6          /* A simple RGB color picker */
#define GUI_TYPE_ICON_BUTTON                 7          /* Square button with an icon. */
#define GUI_TYPE_SCROLL                      8          /* Takes care of drawing a scrollbar; not to be "added" to a gui. */
#define GUI_TYPE_PANEL                       9          /* A Panel element can hold many Group element and stacks them on top of each other. It creates a scroll bar */
#define GUI_TYPE_TEXT                        10         /* Text input */
#define GUI_TYPE_TEXTURE                     11         /* Texture input */
#define GUI_TYPE_SELECT                      12         /* Select (aka listbox) */ 
#define GUI_TYPE_MENU                        13         /* Menu */ 
#define GUI_TYPE_CONTAINER                   14         /* Like `Group` but doesn't draw anything itself. */ 

#define GUI_DIRECTION_UP                     1          /* Can be used by e.g. menus, selects, etc.. can be used to tell that a popup needs to popup to the up, down, right, left. */ 
#define GUI_DIRECTION_DOWN                   2          /* "" */     
#define GUI_DIRECTION_LEFT                   3          /* "" */
#define GUI_DIRECTION_RIGHT                  4          /* "" */

/* STYLES */
#define GUI_STYLE_NONE                       (0)
#define GUI_CORNER_TOP_LEFT                  (1 << 0) 
#define GUI_CORNER_TOP_RIGHT                 (1 << 1)
#define GUI_CORNER_BOTTOM_RIGHT              (1 << 2)
#define GUI_CORNER_BOTTOM_LEFT               (1 << 3) 
#define GUI_OUTLINE                          (1 << 4)  /* adds an outline around e.g. buttons. */
#define GUI_EMBOSS                           (1 << 5)  /* adds a lighter line underneath e.g. buttons to give it some depth. */ 
#define GUI_CORNER_LEFT                      (GUI_CORNER_TOP_LEFT | GUI_CORNER_BOTTOM_LEFT)
#define GUI_CORNER_RIGHT                     (GUI_CORNER_TOP_RIGHT | GUI_CORNER_BOTTOM_RIGHT)
#define GUI_CORNER_TOP                       (GUI_CORNER_TOP_LEFT | GUI_CORNER_TOP_RIGHT)
#define GUI_CORNER_BOTTOM                    (GUI_CORNER_BOTTOM_LEFT | GUI_CORNER_BOTTOM_RIGHT)
#define GUI_CORNER_ALL                       (GUI_CORNER_TOP | GUI_CORNER_BOTTOM) 

#define GUI_CURSOR_WIDTH                     9          /* Default cursor width for text/number inputs.*/                                             

#define GUI_STATE_NONE                       0x0000     /* Default state of a widget. */
#define GUI_STATE_EDITABLE                   0x0001     /* The text of the widget is editable; only one per group. */
#define GUI_STATE_HIDDEN                     0x0002     /* The widget is hidden, will not be used for interaction etc... */
#define GUI_STATE_CLOSED                     0x0004
#define GUI_STATE_UNUSED                     0x0006     /* Combination of CLOSED and HIDDEN. Widget is not draw or used in measurements. This is used in combination with close()/open() and show()/hide(). Using a open-close and show-hide we are able to open and keep hidden widgets hidden */
#define GUI_STATE_DOWN_INSIDE                0x0008     /* Use clicked mouse down inside the clickable area of the widget. */
#define GUI_STATE_DOWN_CUSTOM0               0x0010     /* Use this if you have more than one hit area */
#define GUI_STATE_DOWN_CUSTOM1               0x0020     /* Use this if you have more than two hit areas */
#define GUI_STATE_DOWN_CUSTOM2               0x0040     /* Use this if you have more than three hit areas */
#define GUI_STATE_DOWN_CUSTOM3               0x0080     /* Use this if you have more than four hit areas */
#define GUI_STATE_POSITION_LOCKED            0x0100     /* We can lock and unlock the position of a widget. When locked it means we cannot change the position of if. */
#define GUI_STATE_NOTIFICATIONS_DISABLED     0x0200     /* We can disable notification, which is necessary when we want to set the value of widgets when we receive them over the network */

#define GUI_EVENT_VALUE_CHANGED              0x0001     /* Listeners of widgets will be notified whenever a value changes with this event type */

#define GUI_MOD_NONE                         0x0000
#define GUI_MOD_SHIFT                        0x0001
#define GUI_MOD_CONTROL                      0x0002
#define GUI_MOD_ALT                          0x0004

#define GUI_ICON_GLASS                       0xf000
#define GUI_ICON_MUSIC                       0xf001
#define GUI_ICON_SEARCH                      0xf002
#define GUI_ICON_ENVELOPE_O                  0xf003
#define GUI_ICON_HEART                       0xf004
#define GUI_ICON_STAR                        0xf005
#define GUI_ICON_STAR_O                      0xf006
#define GUI_ICON_USER                        0xf007
#define GUI_ICON_FILM                        0xf008
#define GUI_ICON_TH_LARGE                    0xf009
#define GUI_ICON_TH                          0xf00a
#define GUI_ICON_TH_LIST                     0xf00b
#define GUI_ICON_CHECK                       0xf00c
#define GUI_ICON_TIMES                       0xf00d
#define GUI_ICON_SEARCH_PLUS                 0xf00e
#define GUI_ICON_SEARCH_MINUS                0xf010
#define GUI_ICON_POWER_OFF                   0xf011
#define GUI_ICON_SIGNAL                      0xf012
#define GUI_ICON_COG                         0xf013
#define GUI_ICON_TRASH_O                     0xf014
#define GUI_ICON_HOME                        0xf015
#define GUI_ICON_FILE_O                      0xf016
#define GUI_ICON_CLOCK_O                     0xf017
#define GUI_ICON_ROAD                        0xf018
#define GUI_ICON_DOWNLOAD                    0xf019
#define GUI_ICON_ARROW_CIRCLE_O_DOWN         0xf01a
#define GUI_ICON_ARROW_CIRCLE_O_UP           0xf01b
#define GUI_ICON_INBOX                       0xf01c
#define GUI_ICON_PLAY_CIRCLE_O               0xf01d
#define GUI_ICON_REPEAT                      0xf01e
#define GUI_ICON_REFRESH                     0xf021
#define GUI_ICON_LIST_ALT                    0xf022
#define GUI_ICON_LOCK                        0xf023
#define GUI_ICON_FLAG                        0xf024
#define GUI_ICON_HEADPHONES                  0xf025
#define GUI_ICON_VOLUME_OFF                  0xf026
#define GUI_ICON_VOLUME_DOWN                 0xf027
#define GUI_ICON_VOLUME_UP                   0xf028
#define GUI_ICON_QRCODE                      0xf029
#define GUI_ICON_BARCODE                     0xf02a
#define GUI_ICON_TAG                         0xf02b
#define GUI_ICON_TAGS                        0xf02c
#define GUI_ICON_BOOK                        0xf02d
#define GUI_ICON_BOOKMARK                    0xf02e
#define GUI_ICON_PRINT                       0xf02f
#define GUI_ICON_CAMERA                      0xf030
#define GUI_ICON_FONT                        0xf031
#define GUI_ICON_BOLD                        0xf032
#define GUI_ICON_ITALIC                      0xf033
#define GUI_ICON_TEXT_HEIGHT                 0xf034
#define GUI_ICON_TEXT_WIDTH                  0xf035
#define GUI_ICON_ALIGN_LEFT                  0xf036
#define GUI_ICON_ALIGN_CENTER                0xf037
#define GUI_ICON_ALIGN_RIGHT                 0xf038
#define GUI_ICON_ALIGN_JUSTIFY               0xf039
#define GUI_ICON_LIST                        0xf03a
#define GUI_ICON_OUTDENT                     0xf03b
#define GUI_ICON_INDENT                      0xf03c
#define GUI_ICON_VIDEO_CAMERA                0xf03d
#define GUI_ICON_PICTURE_O                   0xf03e
#define GUI_ICON_PENCIL                      0xf040
#define GUI_ICON_MAP_MARKER                  0xf041
#define GUI_ICON_ADJUST                      0xf042
#define GUI_ICON_TINT                        0xf043
#define GUI_ICON_PENCIL_SQUARE_O             0xf044
#define GUI_ICON_SHARE_SQUARE_O              0xf045
#define GUI_ICON_CHECK_SQUARE_O              0xf046
#define GUI_ICON_ARROWS                      0xf047
#define GUI_ICON_STEP_BACKWARD               0xf048
#define GUI_ICON_FAST_BACKWARD               0xf049
#define GUI_ICON_BACKWARD                    0xf04a
#define GUI_ICON_PLAY                        0xf04b
#define GUI_ICON_PAUSE                       0xf04c
#define GUI_ICON_STOP                        0xf04d
#define GUI_ICON_FORWARD                     0xf04e
#define GUI_ICON_FAST_FORWARD                0xf050
#define GUI_ICON_STEP_FORWARD                0xf051
#define GUI_ICON_EJECT                       0xf052
#define GUI_ICON_CHEVRON_LEFT                0xf053
#define GUI_ICON_CHEVRON_RIGHT               0xf054
#define GUI_ICON_PLUS_CIRCLE                 0xf055
#define GUI_ICON_MINUS_CIRCLE                0xf056
#define GUI_ICON_TIMES_CIRCLE                0xf057
#define GUI_ICON_CHECK_CIRCLE                0xf058
#define GUI_ICON_QUESTION_CIRCLE             0xf059
#define GUI_ICON_INFO_CIRCLE                 0xf05a
#define GUI_ICON_CROSSHAIRS                  0xf05b
#define GUI_ICON_TIMES_CIRCLE_O              0xf05c
#define GUI_ICON_CHECK_CIRCLE_O              0xf05d
#define GUI_ICON_BAN                         0xf05e
#define GUI_ICON_ARROW_LEFT                  0xf060
#define GUI_ICON_ARROW_RIGHT                 0xf061
#define GUI_ICON_ARROW_UP                    0xf062
#define GUI_ICON_ARROW_DOWN                  0xf063
#define GUI_ICON_SHARE                       0xf064
#define GUI_ICON_EXPAND                      0xf065
#define GUI_ICON_COMPRESS                    0xf066
#define GUI_ICON_PLUS                        0xf067
#define GUI_ICON_MINUS                       0xf068
#define GUI_ICON_ASTERISK                    0xf069
#define GUI_ICON_EXCLAMATION_CIRCLE          0xf06a
#define GUI_ICON_GIFT                        0xf06b
#define GUI_ICON_LEAF                        0xf06c
#define GUI_ICON_FIRE                        0xf06d
#define GUI_ICON_EYE                         0xf06e
#define GUI_ICON_EYE_SLASH                   0xf070
#define GUI_ICON_EXCLAMATION_TRIANGLE        0xf071
#define GUI_ICON_PLANE                       0xf072
#define GUI_ICON_CALENDAR                    0xf073
#define GUI_ICON_RANDOM                      0xf074
#define GUI_ICON_COMMENT                     0xf075
#define GUI_ICON_MAGNET                      0xf076
#define GUI_ICON_CHEVRON_UP                  0xf077
#define GUI_ICON_CHEVRON_DOWN                0xf078
#define GUI_ICON_RETWEET                     0xf079
#define GUI_ICON_SHOPPING_CART               0xf07a
#define GUI_ICON_FOLDER                      0xf07b
#define GUI_ICON_FOLDER_OPEN                 0xf07c
#define GUI_ICON_ARROWS_V                    0xf07d
#define GUI_ICON_ARROWS_H                    0xf07e
#define GUI_ICON_BAR_CHART_O                 0xf080
#define GUI_ICON_TWITTER_SQUARE              0xf081
#define GUI_ICON_FACEBOOK_SQUARE             0xf082
#define GUI_ICON_CAMERA_RETRO                0xf083
#define GUI_ICON_KEY                         0xf084
#define GUI_ICON_COGS                        0xf085
#define GUI_ICON_COMMENTS                    0xf086
#define GUI_ICON_THUMBS_O_UP                 0xf087
#define GUI_ICON_THUMBS_O_DOWN               0xf088
#define GUI_ICON_STAR_HALF                   0xf089
#define GUI_ICON_HEART_O                     0xf08a
#define GUI_ICON_SIGN_OUT                    0xf08b
#define GUI_ICON_LINKEDIN_SQUARE             0xf08c
#define GUI_ICON_THUMB_TACK                  0xf08d
#define GUI_ICON_EXTERNAL_LINK               0xf08e
#define GUI_ICON_SIGN_IN                     0xf090
#define GUI_ICON_TROPHY                      0xf091
#define GUI_ICON_GITHUB_SQUARE               0xf092
#define GUI_ICON_UPLOAD                      0xf093
#define GUI_ICON_LEMON_O                     0xf094
#define GUI_ICON_PHONE                       0xf095
#define GUI_ICON_SQUARE_O                    0xf096
#define GUI_ICON_BOOKMARK_O                  0xf097
#define GUI_ICON_PHONE_SQUARE                0xf098
#define GUI_ICON_TWITTER                     0xf099
#define GUI_ICON_FACEBOOK                    0xf09a
#define GUI_ICON_GITHUB                      0xf09b
#define GUI_ICON_UNLOCK                      0xf09c
#define GUI_ICON_CREDIT_CARD                 0xf09d
#define GUI_ICON_RSS                         0xf09e
#define GUI_ICON_HDD_O                       0xf0a0
#define GUI_ICON_BULLHORN                    0xf0a1
#define GUI_ICON_BELL                        0xf0f3
#define GUI_ICON_CERTIFICATE                 0xf0a3
#define GUI_ICON_HAND_O_RIGHT                0xf0a4
#define GUI_ICON_HAND_O_LEFT                 0xf0a5
#define GUI_ICON_HAND_O_UP                   0xf0a6
#define GUI_ICON_HAND_O_DOWN                 0xf0a7
#define GUI_ICON_ARROW_CIRCLE_LEFT           0xf0a8
#define GUI_ICON_ARROW_CIRCLE_RIGHT          0xf0a9
#define GUI_ICON_ARROW_CIRCLE_UP             0xf0aa
#define GUI_ICON_ARROW_CIRCLE_DOWN           0xf0ab
#define GUI_ICON_GLOBE                       0xf0ac
#define GUI_ICON_WRENCH                      0xf0ad
#define GUI_ICON_TASKS                       0xf0ae
#define GUI_ICON_FILTER                      0xf0b0
#define GUI_ICON_BRIEFCASE                   0xf0b1
#define GUI_ICON_ARROWS_ALT                  0xf0b2
#define GUI_ICON_USERS                       0xf0c0
#define GUI_ICON_LINK                        0xf0c1
#define GUI_ICON_CLOUD                       0xf0c2
#define GUI_ICON_FLASK                       0xf0c3
#define GUI_ICON_SCISSORS                    0xf0c4
#define GUI_ICON_FILES_O                     0xf0c5
#define GUI_ICON_PAPERCLIP                   0xf0c6
#define GUI_ICON_FLOPPY_O                    0xf0c7
#define GUI_ICON_SQUARE                      0xf0c8
#define GUI_ICON_BARS                        0xf0c9
#define GUI_ICON_LIST_UL                     0xf0ca
#define GUI_ICON_LIST_OL                     0xf0cb
#define GUI_ICON_STRIKETHROUGH               0xf0cc
#define GUI_ICON_UNDERLINE                   0xf0cd
#define GUI_ICON_TABLE                       0xf0ce
#define GUI_ICON_MAGIC                       0xf0d0
#define GUI_ICON_TRUCK                       0xf0d1
#define GUI_ICON_PINTEREST                   0xf0d2
#define GUI_ICON_PINTEREST_SQUARE            0xf0d3
#define GUI_ICON_GOOGLE_PLUS_SQUARE          0xf0d4
#define GUI_ICON_GOOGLE_PLUS                 0xf0d5
#define GUI_ICON_MONEY                       0xf0d6
#define GUI_ICON_CARET_DOWN                  0xf0d7
#define GUI_ICON_CARET_UP                    0xf0d8
#define GUI_ICON_CARET_LEFT                  0xf0d9
#define GUI_ICON_CARET_RIGHT                 0xf0da
#define GUI_ICON_COLUMNS                     0xf0db
#define GUI_ICON_SORT                        0xf0dc
#define GUI_ICON_SORT_ASC                    0xf0dd
#define GUI_ICON_SORT_DESC                   0xf0de
#define GUI_ICON_ENVELOPE                    0xf0e0
#define GUI_ICON_LINKEDIN                    0xf0e1
#define GUI_ICON_UNDO                        0xf0e2
#define GUI_ICON_GAVEL                       0xf0e3
#define GUI_ICON_TACHOMETER                  0xf0e4
#define GUI_ICON_COMMENT_O                   0xf0e5
#define GUI_ICON_COMMENTS_O                  0xf0e6
#define GUI_ICON_BOLT                        0xf0e7
#define GUI_ICON_SITEMAP                     0xf0e8
#define GUI_ICON_UMBRELLA                    0xf0e9
#define GUI_ICON_CLIPBOARD                   0xf0ea
#define GUI_ICON_LIGHTBULB_O                 0xf0eb
#define GUI_ICON_EXCHANGE                    0xf0ec
#define GUI_ICON_CLOUD_DOWNLOAD              0xf0ed
#define GUI_ICON_CLOUD_UPLOAD                0xf0ee
#define GUI_ICON_USER_MD                     0xf0f0
#define GUI_ICON_STETHOSCOPE                 0xf0f1
#define GUI_ICON_SUITCASE                    0xf0f2
#define GUI_ICON_BELL_O                      0xf0a2
#define GUI_ICON_COFFEE                      0xf0f4
#define GUI_ICON_CUTLERY                     0xf0f5
#define GUI_ICON_FILE_TEXT_O                 0xf0f6
#define GUI_ICON_BUILDING_O                  0xf0f7
#define GUI_ICON_HOSPITAL_O                  0xf0f8
#define GUI_ICON_AMBULANCE                   0xf0f9
#define GUI_ICON_MEDKIT                      0xf0fa
#define GUI_ICON_FIGHTER_JET                 0xf0fb
#define GUI_ICON_BEER                        0xf0fc
#define GUI_ICON_H_SQUARE                    0xf0fd
#define GUI_ICON_PLUS_SQUARE                 0xf0fe
#define GUI_ICON_ANGLE_DOUBLE_LEFT           0xf100
#define GUI_ICON_ANGLE_DOUBLE_RIGHT          0xf101
#define GUI_ICON_ANGLE_DOUBLE_UP             0xf102
#define GUI_ICON_ANGLE_DOUBLE_DOWN           0xf103
#define GUI_ICON_ANGLE_LEFT                  0xf104
#define GUI_ICON_ANGLE_RIGHT                 0xf105
#define GUI_ICON_ANGLE_UP                    0xf106
#define GUI_ICON_ANGLE_DOWN                  0xf107
#define GUI_ICON_DESKTOP                     0xf108
#define GUI_ICON_LAPTOP                      0xf109
#define GUI_ICON_TABLET                      0xf10a
#define GUI_ICON_MOBILE                      0xf10b
#define GUI_ICON_CIRCLE_O                    0xf10c
#define GUI_ICON_QUOTE_LEFT                  0xf10d
#define GUI_ICON_QUOTE_RIGHT                 0xf10e
#define GUI_ICON_SPINNER                     0xf110
#define GUI_ICON_CIRCLE                      0xf111
#define GUI_ICON_REPLY                       0xf112
#define GUI_ICON_GITHUB_ALT                  0xf113
#define GUI_ICON_FOLDER_O                    0xf114
#define GUI_ICON_FOLDER_OPEN_O               0xf115
#define GUI_ICON_SMILE_O                     0xf118
#define GUI_ICON_FROWN_O                     0xf119
#define GUI_ICON_MEH_O                       0xf11a
#define GUI_ICON_GAMEPAD                     0xf11b
#define GUI_ICON_KEYBOARD_O                  0xf11c
#define GUI_ICON_FLAG_O                      0xf11d
#define GUI_ICON_FLAG_CHECKERED              0xf11e
#define GUI_ICON_TERMINAL                    0xf120
#define GUI_ICON_CODE                        0xf121
#define GUI_ICON_REPLY_ALL                   0xf122
#define GUI_ICON_MAIL_REPLY_ALL              0xf122
#define GUI_ICON_STAR_HALF_O                 0xf123
#define GUI_ICON_LOCATION_ARROW              0xf124
#define GUI_ICON_CROP                        0xf125
#define GUI_ICON_CODE_FORK                   0xf126
#define GUI_ICON_CHAIN_BROKEN                0xf127
#define GUI_ICON_QUESTION                    0xf128
#define GUI_ICON_INFO                        0xf129
#define GUI_ICON_EXCLAMATION                 0xf12a
#define GUI_ICON_SUPERSCRIPT                 0xf12b
#define GUI_ICON_SUBSCRIPT                   0xf12c
#define GUI_ICON_ERASER                      0xf12d
#define GUI_ICON_PUZZLE_PIECE                0xf12e
#define GUI_ICON_MICROPHONE                  0xf130
#define GUI_ICON_MICROPHONE_SLASH            0xf131
#define GUI_ICON_SHIELD                      0xf132
#define GUI_ICON_CALENDAR_O                  0xf133
#define GUI_ICON_FIRE_EXTINGUISHER           0xf134
#define GUI_ICON_ROCKET                      0xf135
#define GUI_ICON_MAXCDN                      0xf136
#define GUI_ICON_CHEVRON_CIRCLE_LEFT         0xf137
#define GUI_ICON_CHEVRON_CIRCLE_RIGHT        0xf138
#define GUI_ICON_CHEVRON_CIRCLE_UP           0xf139
#define GUI_ICON_CHEVRON_CIRCLE_DOWN         0xf13a
#define GUI_ICON_HTML5                       0xf13b
#define GUI_ICON_CSS3                        0xf13c
#define GUI_ICON_ANCHOR                      0xf13d
#define GUI_ICON_UNLOCK_ALT                  0xf13e
#define GUI_ICON_BULLSEYE                    0xf140
#define GUI_ICON_ELLIPSIS_H                  0xf141
#define GUI_ICON_ELLIPSIS_V                  0xf142
#define GUI_ICON_RSS_SQUARE                  0xf143
#define GUI_ICON_PLAY_CIRCLE                 0xf144
#define GUI_ICON_TICKET                      0xf145
#define GUI_ICON_MINUS_SQUARE                0xf146
#define GUI_ICON_MINUS_SQUARE_O              0xf147
#define GUI_ICON_LEVEL_UP                    0xf148
#define GUI_ICON_LEVEL_DOWN                  0xf149
#define GUI_ICON_CHECK_SQUARE                0xf14a
#define GUI_ICON_PENCIL_SQUARE               0xf14b
#define GUI_ICON_EXTERNAL_LINK_SQUARE        0xf14c
#define GUI_ICON_SHARE_SQUARE                0xf14d
#define GUI_ICON_COMPASS                     0xf14e
#define GUI_ICON_CARET_SQUARE_O_DOWN         0xf150
#define GUI_ICON_CARET_SQUARE_O_UP           0xf151
#define GUI_ICON_CARET_SQUARE_O_RIGHT        0xf152
#define GUI_ICON_EUR                         0xf153
#define GUI_ICON_GBP                         0xf154
#define GUI_ICON_USD                         0xf155
#define GUI_ICON_INR                         0xf156
#define GUI_ICON_JPY                         0xf157
#define GUI_ICON_RUB                         0xf158
#define GUI_ICON_KRW                         0xf159
#define GUI_ICON_BTC                         0xf15a
#define GUI_ICON_FILE                        0xf15b
#define GUI_ICON_FILE_TEXT                   0xf15c
#define GUI_ICON_SORT_ALPHA_ASC              0xf15d
#define GUI_ICON_SORT_ALPHA_DESC             0xf15e
#define GUI_ICON_SORT_AMOUNT_ASC             0xf160
#define GUI_ICON_SORT_AMOUNT_DESC            0xf161
#define GUI_ICON_SORT_NUMERIC_ASC            0xf162
#define GUI_ICON_SORT_NUMERIC_DESC           0xf163
#define GUI_ICON_THUMBS_UP                   0xf164
#define GUI_ICON_THUMBS_DOWN                 0xf165
#define GUI_ICON_YOUTUBE_SQUARE              0xf166
#define GUI_ICON_YOUTUBE                     0xf167
#define GUI_ICON_XING                        0xf168
#define GUI_ICON_XING_SQUARE                 0xf169
#define GUI_ICON_YOUTUBE_PLAY                0xf16a
#define GUI_ICON_DROPBOX                     0xf16b
#define GUI_ICON_STACK_OVERFLOW              0xf16c
#define GUI_ICON_INSTAGRAM                   0xf16d
#define GUI_ICON_FLICKR                      0xf16e
#define GUI_ICON_ADN                         0xf170
#define GUI_ICON_BITBUCKET                   0xf171
#define GUI_ICON_BITBUCKET_SQUARE            0xf172
#define GUI_ICON_TUMBLR                      0xf173
#define GUI_ICON_TUMBLR_SQUARE               0xf174
#define GUI_ICON_LONG_ARROW_DOWN             0xf175
#define GUI_ICON_LONG_ARROW_UP               0xf176
#define GUI_ICON_LONG_ARROW_LEFT             0xf177
#define GUI_ICON_LONG_ARROW_RIGHT            0xf178
#define GUI_ICON_APPLE                       0xf179
#define GUI_ICON_WINDOWS                     0xf17a
#define GUI_ICON_ANDROID                     0xf17b
#define GUI_ICON_LINUX                       0xf17c
#define GUI_ICON_DRIBBBLE                    0xf17d
#define GUI_ICON_SKYPE                       0xf17e
#define GUI_ICON_FOURSQUARE                  0xf180
#define GUI_ICON_TRELLO                      0xf181
#define GUI_ICON_FEMALE                      0xf182
#define GUI_ICON_MALE                        0xf183
#define GUI_ICON_GITTIP                      0xf184
#define GUI_ICON_SUN_O                       0xf185
#define GUI_ICON_MOON_O                      0xf186
#define GUI_ICON_ARCHIVE                     0xf187
#define GUI_ICON_BUG                         0xf188
#define GUI_ICON_VK                          0xf189
#define GUI_ICON_WEIBO                       0xf18a
#define GUI_ICON_RENREN                      0xf18b
#define GUI_ICON_PAGELINES                   0xf18c
#define GUI_ICON_STACK_EXCHANGE              0xf18d
#define GUI_ICON_ARROW_CIRCLE_O_RIGHT        0xf18e
#define GUI_ICON_ARROW_CIRCLE_O_LEFT         0xf190
#define GUI_ICON_CARET_SQUARE_O_LEFT         0xf191
#define GUI_ICON_DOT_CIRCLE_O                0xf192
#define GUI_ICON_WHEELCHAIR                  0xf193
#define GUI_ICON_VIMEO_SQUARE                0xf194
#define GUI_ICON_TRY                         0xf195
#define GUI_ICON_PLUS_SQUARE_O               0xf196
#define GUI_ICON_UNSORTED                    0xf0dc
                                    
#define GUI_KEY_SPACE                        32
#define GUI_KEY_ENTER                        257
#define GUI_KEY_BACKSPACE                    259
#define GUI_KEY_DELETE                       261
#define GUI_KEY_RIGHT                        262
#define GUI_KEY_LEFT                         263

typedef void(*gui_button_callback)(int id, void* user); /* Generic click callback. */

#endif
