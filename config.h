#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
/* static const char *fonts[]          = { "SauceCodePro Nerd Font Mono:size=13" }; */
static const char *fonts[] = {
    "BlexMono Nerd Font:size=13:antialias=true:autohint=true",
    "Sarasa UI SC:size=12:antialias=true:autohint=true",
};
static const char dmenufont[]       = "Sauce Code Pro Nerd Font Mono:size=13";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]        = "#42A5F5";
static char normbgcolor[] = "#2E3440";
static char normbordercolor[] = "#3B4252";
static char normfgcolor[] = "#ECEFF4";
static char selfgcolor[] = "#D8DEE9";
static char selbordercolor[] = "#5E81AC";
static char selbgcolor[] = "#5E81AC";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	/* [SchemeNorm] = { col_gray3, col_gray1, col_gray2 }, */
	/* [SchemeSel]  = { col_gray4, col_cyan,  col_border  }, */
	/* [SchemeHid]  = { col_cyan,  col_gray1, col_border  }, */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
    [SchemeHid] = {selbgcolor, normbgcolor, selbordercolor},
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
/* static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; */
static const char *tags[] ={ "", "", "" ,"", "", "", "", "", "" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static int attachbelow = 1;    /* 1 means attach after the currently active window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "><",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* static const char* ncmodes[] = { "🡶", "🡴" }; */
/* static unsigned int ncmode = 1; */

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *roficmd[] = { "rofi -show drun -theme config.rasi", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *trayercmd[]  = { "/home/piko/scripts/tr-toggle.sh", NULL };
static const char *printcurrentscreen[]  = { "scrot", "-u", "/home/piko/Pictures/screenshots/%Y-%m-%d-%s_$wx$h.jpg", NULL };
static const char *printrectanglescreen[]  = { "scrot", "-s", "/home/piko/Pictures/screenshots/%Y-%m-%d-%s_$wx$h.jpg", NULL };
static const char *flameshot[]  = { "flameshot","gui", NULL };
static const char *upvol[]   = { "/home/piko/scripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/piko/scripts/vol-down.sh",  NULL };
static const char *mutevol[] = { "/home/piko/scripts/vol-toggle.sh",  NULL };

static const char *wpcmd[]  = { "/home/piko/scripts/wp-change.sh", NULL };
static const char *sktogglecmd[]  = { "/home/piko/scripts/sck-tog.sh", NULL };//set screenkey
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x24", NULL };

static const char *setcolemakcmd[]  = { "/home/piko/scripts/setxmodmap-colemak.sh", NULL };
static const char *setqwertycmd[]  = { "/home/piko/scripts/setxmodmap-qwerty.sh", NULL };

static const char *suspendcmd[]  = { "/home/piko/scripts/suspend.sh", NULL };

static const char *neteasemusiccmd[] = {"netease-cloud-music",NULL };

static const char *chKeyboardLayout[] = {"/home/piko/scripts/chKeyboardLayout.sh",NULL };

static const char *lightdown[] = {"/home/piko/scripts/lightdown.sh",NULL };
static const char *lightup[] = {"/home/piko/scripts/lightup.sh",NULL };

static Key keys[] = {
	/* modifier            key                      function        argument */
	{ MODKEY,              XK_d,                    spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_s,                    spawn,          {.v = roficmd } },
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY,              XK_c,                    spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,    XK_d,                    spawn,          {.v = trayercmd } },
//	{ MODKEY|ShiftMask,    XK_w,                    spawn,          {.v = setqwertycmd } },
//	{ MODKEY|ShiftMask,    XK_m,                    spawn,          {.v = setcolemakcmd } },
    /* { MODKEY,              XK_a,                    spawn,          {.v = neteasemusiccmd}}, */
    { MODKEY,              XK_a,                    spawn,          {.v = toggleAttachBelow}}, //not work on toggle
	{ MODKEY|ShiftMask,    XK_p,                    spawn,          {.v = suspendcmd } },
	/* { MODKEY|ControlMask,  XK_s,                    spawn,          {.v = sktogglecmd } }, */ //set screenkey
    //volume control
	{ 0,                   XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
	{ 0,                   XF86XK_AudioMute,        spawn,          {.v = mutevol } },
	{ 0,                   XF86XK_AudioRaiseVolume, spawn,          {.v = upvol   } },
	{ MODKEY,              XK_bracketleft,          spawn,          {.v = downvol } },
	{ MODKEY,              XK_backslash,            spawn,          {.v = mutevol } },
	{ MODKEY,              XK_bracketright,         spawn,          {.v = upvol   } },
    //light control
	{ MODKEY|ShiftMask,    XK_bracketright,         spawn,          {.v = lightup   } },
	{ MODKEY|ShiftMask,    XK_bracketleft,          spawn,          {.v = lightdown } },
	{ 0,              XF86XK_MonBrightnessUp,         spawn,          {.v = lightup   } },
	{ 0,              XF86XK_MonBrightnessDown,          spawn,          {.v = lightdown } },

	{ MODKEY,              XK_b,                    spawn,          {.v = wpcmd } },
	{ MODKEY,              XK_m,                    spawn,          {.v = chKeyboardLayout } },
	{ 0,              XK_Print,                    spawn,          {.v = printcurrentscreen} },
	{ MODKEY,              XK_Print,                    spawn,          {.v = printrectanglescreen} },
	{ MODKEY|ShiftMask,              XK_Print,                    spawn,          {.v = flameshot} },
	{ MODKEY|ShiftMask,    XK_j,                    rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_k,                    rotatestack,    {.i = -1 } },
	{ MODKEY,              XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                    focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_n,                    incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_v,                    incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,                    setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_e,                    hidewin,        {0} },
	{ MODKEY|ShiftMask,    XK_e,                    restorewin,     {0} },
	{ MODKEY|ShiftMask,    XK_Return,               zoom,           {0} },
	{ MODKEY,              XK_Tab,                  view,           {0} },
	{ MODKEY,              XK_q,                    killclient,     {0} },
	//{ MODKEY,              XK_t,                    setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,              XK_m,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_f,                    fullscreen,     {0} },
	{ MODKEY,              XK_space,                setlayout,      {0} },
	/* { MODKEY|ShiftMask,    XK_space,                togglefloating, {0} }, */
	{ MODKEY,              XK_apostrophe,           togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,              XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                    tag,            {.ui = ~0 } },
	{ MODKEY,              XK_comma,                focusmon,       {.i = -1 } },
	{ MODKEY,              XK_period,               focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,                tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,               tagmon,         {.i = +1 } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ MODKEY|ControlMask,  XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

