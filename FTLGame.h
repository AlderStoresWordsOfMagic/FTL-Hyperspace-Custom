#pragma once

#pragma warning( disable : 4722 )

#define LIBZHL_API 
#define LIBZHL_INTERFACE __declspec(novtable)

__declspec(noreturn) inline void __cdecl __NOP() {}
#define LIBZHL_PLACEHOLDER {__NOP();}


#include "zhl.h"
#include "rapidxml.hpp"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <deque>
#include <stdarg.h>
#include <unordered_map>
#include <math.h>
#include <queue>
#undef LoadImage

#define CALL_VIRTUAL_1(_id, _classname, _funcname, _returntype, _object) \
    _returntype(_classname::*mfp##_id)() = &_classname::_funcname; \
    (_object->*mfp##_id)

#define CALL_VIRTUAL_0(_id, _classname, _funcname, _returntype, _object) \
	CALL_VIRTUAL_1(_id, _classname, _funcname, _returntype, _object)

#define CALL_VIRTUAL(_classname, _funcname, _returntype, _object) CALL_VIRTUAL_0(__LINE__, _classname, _funcname, _returntype, _object)

#define CALL_VIRTUAL_RETURN_1(_id, _classname, _funcname, _returntype, _object, _return) \
	_returntype(_classname::*mfp##_id)() = &_classname::_funcname; \
	_returntype _return = (_object->*mfp##_id)

#define CALL_VIRTUAL_RETURN_0(_id, _classname, _funcname, _returntype, _object, _return) \
	CALL_VIRTUAL_RETURN_1(_id, _classname, _funcname, _returntype, _object, _return)

#define CALL_VIRTUAL_RETURN(_classname, _funcname, _returntype, _object, _return) CALL_VIRTUAL_RETURN_0(__LINE__, _classname, _funcname, _returntype, _object, _return)

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif


struct RandomAmount
{
	int min;
	int max;
	float chanceNone;
};

struct Point
{
	Point(int xx, int yy) : x(xx), y(yy)  { }
	Point() { }
	
	friend bool operator==(const Point &a, const Point &b) {return a.x==b.x && a.y==b.y;}
	friend bool operator!=(const Point &a, const Point &b) {return a.x!=b.x || a.y!=b.y;}

	int x;
	int y;
};

struct CAchievement
{
};

typedef std::pair<int, int> std_pair_int_int;
typedef std::pair<float, float> std_pair_float_float;
typedef std::pair<std::string, int> std_pair_std_string_int;
typedef std::pair<std::string, RandomAmount> std_pair_std_string_RandomAmount;
typedef std::pair<std::string, std::string> std_pair_std_string_std_string;
typedef std::pair<CAchievement*, Point> std_pair_CAchievement_ptr_Point;
typedef std::map<std::string, std::string> std_map_std_string_std_string;

enum TouchAction
{
  TOUCH_DOWN = 0x1,
  TOUCH_MOVE = 0x2,
  TOUCH_UP = 0x3,
  TOUCH_CANCEL = 0x4,
};

enum InputEventType
{
  INPUT_EVENT_JOYSTICK = 0x1,
  INPUT_EVENT_KEYBOARD = 0x2,
  INPUT_EVENT_MEMORY = 0x3,
  INPUT_EVENT_MOUSE = 0x4,
  INPUT_EVENT_TEXT = 0x5,
  INPUT_EVENT_TOUCH = 0x6,
};

enum InputEventDetail
{
  INPUT_JOYSTICK_CONNECTED = 0x1,
  INPUT_JOYSTICK_DISCONNECTED = 0x2,
  INPUT_JOYSTICK_BUTTON_DOWN = 0x3,
  INPUT_JOYSTICK_BUTTON_UP = 0x4,
  INPUT_JOYSTICK_DPAD_CHANGE = 0x5,
  INPUT_JOYSTICK_STICK_CHANGE = 0x6,
  INPUT_KEYBOARD_KEY_DOWN = 0x7,
  INPUT_KEYBOARD_KEY_UP = 0x8,
  INPUT_KEYBOARD_SYSTEM_KEY_DOWN = 0x9,
  INPUT_KEYBOARD_SYSTEM_KEY_UP = 0xA,
  INPUT_MEMORY_LOW = 0xB,
  INPUT_MOUSE_MOVE = 0xC,
  INPUT_MOUSE_LMB_DOWN = 0xD,
  INPUT_MOUSE_LMB_UP = 0xE,
  INPUT_MOUSE_MMB_DOWN = 0xF,
  INPUT_MOUSE_MMB_UP = 0x10,
  INPUT_MOUSE_RMB_DOWN = 0x11,
  INPUT_MOUSE_RMB_UP = 0x12,
  INPUT_MOUSE_SCROLL_H = 0x13,
  INPUT_MOUSE_SCROLL_V = 0x14,
  INPUT_TEXT_INPUT = 0x15,
  INPUT_TEXT_DONE = 0x16,
  INPUT_TEXT_CANCELLED = 0x17,
  INPUT_TEXT_CLEAR = 0x18,
  INPUT_TEXT_BACKSPACE = 0x19,
  INPUT_TEXT_DELETE = 0x1A,
  INPUT_TEXT_CURSOR_LEFT = 0x1B,
  INPUT_TEXT_CURSOR_RIGHT = 0x1C,
  INPUT_TEXT_CURSOR_HOME = 0x1D,
  INPUT_TEXT_CURSOR_END = 0x1E,
  INPUT_TOUCH_DOWN = 0x1F,
  INPUT_TOUCH_MOVE = 0x20,
  INPUT_TOUCH_UP = 0x21,
  INPUT_TOUCH_CANCEL = 0x22,
};

enum DeleteType
{
  BUFFER = 0x1,
  FRAMEBUFFER = 0x2,
  PROGRAM = 0x3,
  PROGRAM_PIPELINE = 0x4,
  RENDERBUFFER = 0x5,
  SHADER = 0x6,
  TEXTURE = 0x7,
  VERTEX_ARRAY = 0x8,
};

enum FontAlignment
{
  FONT_ALIGN_LEFT = 0x0,
  FONT_ALIGN_CENTER = 0x1,
  FONT_ALIGN_RIGHT = 0x2,
};

enum FramebufferColorType
{
  FBCOLOR_RGB8 = 0x1,
  FBCOLOR_RGBA8 = 0x2,
};

enum GL_StencilMode
{
  STENCIL_IGNORE = 0x0,
  STENCIL_SET = 0x1,
  STENCIL_USE = 0x2,
};

enum ResourceType
{
  RES_UNUSED = 0x0,
  RES_UNKNOWN = 0x1,
  RES_DATA = 0x2,
  RES_TEXTURE = 0x3,
  RES_FONT = 0x4,
  RES_SOUND = 0x5,
  RES_FILE = 0x6,
};


enum SDLKey
{
  SDLK_UNKNOWN = 0x0,
  SDLK_0 = 0x30,
  SDLK_1 = 0x31,
  SDLK_2 = 0x32,
  SDLK_3 = 0x33,
  SDLK_4 = 0x34,
  SDLK_5 = 0x35,
  SDLK_6 = 0x36,
  SDLK_7 = 0x37,
  SDLK_8 = 0x38,
  SDLK_9 = 0x39,
  SDLK_AT = 0x40,
  SDLK_AMPERSAND = 0x26,
  SDLK_ASTERISK = 0x2A,
  SDLK_BACKQUOTE = 0x60,
  SDLK_BACKSLASH = 0x5C,
  SDLK_BACKSPACE = 0x8,
  SDLK_BREAK = 0x13E,
  SDLK_CAPSLOCK = 0x12D,
  SDLK_CARET = 0x5E,
  SDLK_CLEAR = 0xC,
  SDLK_COLON = 0x3A,
  SDLK_COMMA = 0x2C,
  SDLK_COMPOSE = 0x13A,
  SDLK_DELETE = 0x7F,
  SDLK_DOLLAR = 0x24,
  SDLK_DOWN = 0x112,
  SDLK_END = 0x117,
  SDLK_EQUALS = 0x3D,
  SDLK_ESCAPE = 0x1B,
  SDLK_EURO = 0x141,
  SDLK_EXCLAIM = 0x21,
  SDLK_F1 = 0x11A,
  SDLK_F10 = 0x123,
  SDLK_F11 = 0x124,
  SDLK_F12 = 0x125,
  SDLK_F13 = 0x126,
  SDLK_F14 = 0x127,
  SDLK_F15 = 0x128,
  SDLK_F2 = 0x11B,
  SDLK_F3 = 0x11C,
  SDLK_F4 = 0x11D,
  SDLK_F5 = 0x11E,
  SDLK_F6 = 0x11F,
  SDLK_F7 = 0x120,
  SDLK_F8 = 0x121,
  SDLK_F9 = 0x122,
  SDLK_GREATER = 0x3E,
  SDLK_HASH = 0x24,
  SDLK_HELP = 0x13B,
  SDLK_HOME = 0x116,
  SDLK_INSERT = 0x115,
  SDLK_KP0 = 0x100,
  SDLK_KP1 = 0x101,
  SDLK_KP2 = 0x102,
  SDLK_KP3 = 0x103,
  SDLK_KP4 = 0x104,
  SDLK_KP5 = 0x105,
  SDLK_KP6 = 0x106,
  SDLK_KP7 = 0x107,
  SDLK_KP8 = 0x108,
  SDLK_KP9 = 0x109,
  SDLK_KP_PERIOD = 0x10A,
  SDLK_KP_DIVIDE = 0x10B,
  SDLK_KP_MULTIPLY = 0x10C,
  SDLK_KP_MINUS = 0x10D,
  SDLK_KP_PLUS = 0x10E,
  SDLK_KP_ENTER = 0x10F,
  SDLK_KP_EQUALS = 0x110,
  SDLK_LALT = 0x134,
  SDLK_LCTRL = 0x132,
  SDLK_LEFT = 0x114,
  SDLK_LEFTBRACKET = 0x5B,
  SDLK_LEFTPAREN = 0x28,
  SDLK_LESS = 0x3C,
  SDLK_LMETA = 0x136,
  SDLK_LSHIFT = 0x130,
  SDLK_LSUPER = 0x137,
  SDLK_MENU = 0x13F,
  SDLK_MINUS = 0x2D,
  SDLK_MODE = 0x139,
  SDLK_NUMLOCK = 0x12C,
  SDLK_PAGEDOWN = 0x119,
  SDLK_PAGEUP = 0x118,
  SDLK_PAUSE = 0x13,
  SDLK_PERIOD = 0x2E,
  SDLK_PLUS = 0x2B,
  SDLK_POWER = 0x140,
  SDLK_PRINTSCREEN = 0x13C,
  SDLK_QUESTION = 0x3F,
  SDLK_QUOTEDBL = 0x22,
  SDLK_QUOTE = 0x27,
  SDLK_RALT = 0x133,
  SDLK_RCTRL = 0x131,
  SDLK_RETURN = 0xD,
  SDLK_RIGHT = 0x113,
  SDLK_RIGHTBRACKET = 0x5D,
  SDLK_RIGHTPAREN = 0x29,
  SDLK_RMETA = 0x135,
  SDLK_RSHIFT = 0x12F,
  SDLK_RSUPER = 0x138,
  SDLK_SCROLLOCK = 0x12E,
  SDLK_SEMICOLON = 0x3B,
  SDLK_SLASH = 0x2F,
  SDLK_SPACE = 0x20,
  SDLK_SYSREQ = 0x13D,
  SDLK_TAB = 0x9,
  SDLK_UNDERSCORE = 0x5F,
  SDLK_UNDO = 0x142,
  SDLK_UP = 0x111,
  SDLK_a = 0x61,
  SDLK_b = 0x62,
  SDLK_c = 0x63,
  SDLK_d = 0x64,
  SDLK_e = 0x65,
  SDLK_f = 0x66,
  SDLK_g = 0x67,
  SDLK_h = 0x68,
  SDLK_i = 0x69,
  SDLK_j = 0x6A,
  SDLK_k = 0x6B,
  SDLK_l = 0x6C,
  SDLK_m = 0x6D,
  SDLK_n = 0x6E,
  SDLK_o = 0x6F,
  SDLK_p = 0x70,
  SDLK_q = 0x71,
  SDLK_r = 0x72,
  SDLK_s = 0x73,
  SDLK_t = 0x74,
  SDLK_u = 0x75,
  SDLK_v = 0x76,
  SDLK_w = 0x77,
  SDLK_x = 0x78,
  SDLK_y = 0x79,
  SDLK_z = 0x7A,
  SDLK_LAST = 0x143,
};






struct BoardingGoal
{
	float fHealthLimit;
	int causedDamage;
	int targetsDestroyed;
	int target;
	int damageType;
};

struct BeamWeapon
{
};

struct GL_Primitive
{
};

struct Damage
{
	int iDamage;
	int iShieldPiercing;
	int fireChance;
	int breachChance;
	int stunChance;
	int iIonDamage;
	int iSystemDamage;
	int iPersDamage;
	bool bHullBuster;
	int ownerId;
	int selfId;
	bool bLockdown;
	bool crystalShard;
	bool bFriendlyFire;
	int iStun;
};

struct SystemCustomBox
{
};

struct CombatControl;

struct CrewBox;
struct ShipManager;

struct GL_Texture;
struct GL_Primitive;

struct GenericButton;

struct AnimationTracker;

struct LIBZHL_INTERFACE AnimationTracker
{
	virtual void destroy() LIBZHL_PLACEHOLDER
	LIBZHL_API void Update();
	
	float time;
	bool loop;
	float current_time;
	bool running;
	bool reverse;
	bool done;
	float loopDelay;
	float currentDelay;
};

struct Globals
{
	struct Rect
	{
		int x;
		int y;
		int w;
		int h;
	};
	
	struct Ellipse
	{
		Point center;
		float a;
		float b;
	};
	
};

struct GenericButton
{
	LIBZHL_API void *SetLocation(Point pos);
	LIBZHL_API void SetActive(bool active);
	LIBZHL_API bool MouseMove(int x, int y, bool silent);
	
	void *vptr;
	Point position;
	Globals::Rect hitbox;
	bool allowAnyTouch;
	bool touchSelectable;
	bool bRenderOff;
	bool bRenderSelected;
	bool bFlashing;
	AnimationTracker flashing;
	bool bActive;
	bool bHover;
	bool bActivated;
	bool bSelected;
	int activeTouch;
};

struct Button;

struct Button : GenericButton
{
	Button()
	{
		this->constructor();
	}

	LIBZHL_API int SetLocation(Point pos);
	LIBZHL_API void OnInit(std::string &img, Point pos);
	LIBZHL_API void constructor();
	LIBZHL_API void OnRender();
	
	GL_Texture *images[3];
	GL_Primitive *primitives[3];
	Point imageSize;
	bool bMirror;
};

struct Repairable;
struct CrewMember;

struct Door;

struct CrewControl
{
	ShipManager *shipManager;
	std::vector<CrewMember*> selectedCrew;
	std::vector<CrewMember*> potentialSelectedCrew;
	Door *selectedDoor;
	Repairable *selectedRepair;
	Point selectedGrid;
	int selectedRoom;
	bool selectedPlayerShip;
	Point availablePosition;
	std::vector<CrewBox*> crewBoxes;
	Point firstMouse;
	Point currentMouse;
	Point worldFirstMouse;
	Point worldCurrentMouse;
	bool mouseDown;
	bool bUpdated;
	int activeTouch;
	char selectingCrew;
	char selectingCrewOnPlayerShip;
	double selectingCrewStartTime;
	bool doorControlMode;
	bool doorControlOpen;
	bool doorControlOpenSet;
	CombatControl *combatControl;
	unsigned int selectedCrewBox;
	AnimationTracker crewMessage;
	std::string message;
	Button saveStations;
	Button returnStations;
	GL_Primitive *saveStationsBase;
	GL_Primitive *returnStationsBase;
	int stationsLastY;
};

struct SpaceDrone;
struct ProjectileFactory;

struct CombatAI
{
	ShipManager *target;
	std::vector<ProjectileFactory*> weapons;
	std::vector<SpaceDrone*> drones;
	int stance;
	std::vector<int> system_targets;
	bool bFiringWhileCloaked;
	ShipManager *self;
};

struct ShipObject;

struct ShipObject
{
	LIBZHL_API char HasAugmentation(std::string &augment);
	LIBZHL_API float GetAugmentationValue(std::string &augment);
	
	void *vptr;
	int iShipId;
};

struct CrewTarget : ShipObject
{
};

struct Pointf;

struct Pointf
{
	Pointf() : x(0.f), y(0.f) {}
	Pointf(float _x, float _y) : x(_x), y(_y) {}
	Pointf(const Pointf &v) : x(v.x), y(v.y) {}
	
	static Pointf FromAngle(float a) {return Pointf((float)cos((double)a*0.01745329238474369), (float)sin((double)a*0.01745329238474369));}
	
	static Pointf FromDirection(int dir)
	{
		static const Pointf dirs[] = {
			Pointf(-1.f, 0.f),
			Pointf(0.f, -1.f),
			Pointf(1.f, 0.f),
			Pointf(0.f, 1.f)
		};
		
		if(dir < 0 || dir >= 4)
			return Pointf();
		else
			return dirs[dir];
	}
	
	Pointf &operator=(const Pointf &v) {x=v.x; y=v.y; return *this;}
	Pointf &operator+=(const Pointf &v) {x+=v.x; y+=v.y; return *this;}
	Pointf &operator-=(const Pointf &v) {x-=v.x; y-=v.y; return *this;}
	Pointf &operator*=(const Pointf &v) {x*=v.x; y*=v.y; return *this;}
	Pointf &operator/=(const Pointf &v) {x/=v.x; y/=v.y; return *this;}
	Pointf &operator*=(float f) {x*=f; y*=f; return *this;}
	Pointf &operator/=(float f) {x/=f; y/=f; return *this;}
	
	friend bool operator==(const Pointf &a, const Pointf &b) {return a.x==b.x && a.y==b.y;}
	friend bool operator!=(const Pointf &a, const Pointf &b) {return a.x!=b.x || a.y!=b.y;}
	
	Pointf operator-() const {return Pointf(-x, -y);}
	friend Pointf operator+(const Pointf &a, const Pointf &b) {return Pointf(a.x+b.x, a.y+b.y);}
	friend Pointf operator-(const Pointf &a, const Pointf &b) {return Pointf(a.x-b.x, a.y-b.y);}
	friend Pointf operator*(const Pointf &a, const Pointf &b) {return Pointf(a.x*b.x, a.y*b.y);}
	friend Pointf operator/(const Pointf &a, const Pointf &b) {return Pointf(a.x/b.x, a.y/b.y);}
	friend Pointf operator*(const Pointf &a, float f) {return Pointf(a.x*f, a.y*f);}
	friend Pointf operator*(float f, const Pointf &a) {return Pointf(a.x*f, a.y*f);}
	friend Pointf operator/(const Pointf &a, float f) {return Pointf(a.x/f, a.y/f);}
	
	float Dot(const Pointf &v) const {return x*v.x + y*v.y;}
	float Angle() const {return (float)(atan2(y, x)/0.01745329238474369);}
	
	float LengthSqr() const {return x*x+y*y;}
	float Length() const {return sqrt(LengthSqr());}
	float DistanceSqr(Pointf v) const {return (v-=*this).LengthSqr();}
	float Distance(Pointf v) const {return (v-=*this).Length();}
	
	Pointf &Normalize(bool *ok = 0)
	{
		float d = Length();
		if(d > 0.f)
		{
			x/=d;
			y/=d;
			if(ok) *ok = true;
		}
		else
		{
			if(ok) *ok = false;
		}
		
		return *this;
	}
	
	Pointf GetNormalized(bool *ok = 0) const
	{
		return Pointf(*this).Normalize(ok);
	}
	
	Pointf &SetLength(float len, bool *ok = 0)
	{
		float d = Length();
		if(d > 0.f)
		{
			x*=len/d;
			y*=len/d;
			if(ok) *ok = true;
		}
		else
		{
			if(ok) *ok = false;
		}
		
		return *this;
	}
	
	Pointf &Rotate(float a)
	{
		float c = (float)cos((double)a*0.01745329238474369);
		float s = (float)sin((double)a*0.01745329238474369);
		float x0 = x;
		x = x*c - y*s;
		y = x0*s + y*c;
		return *this;
	}
	Pointf GetRotated(float a) const {return Pointf(*this).Rotate(a);}
	
	Pointf &AxisAlign()
	{
		if(x != 0.f || y != 0.f)
		{
			if(abs(y) >= abs(x))
			{
				x = 0.f;
				y = y > 0.f ? 1.f : -1.f;
			}
			else
			{
				y = 0.f;
				x = x > 0.f ? 1.f : -1.f;
			}
		}
		return *this;
	}
	Pointf GetAxisAligned() const {return Pointf(*this).AxisAlign();}
	
	static Pointf Lerp(float c, Pointf a, Pointf b)
	{
		return Pointf(
			(1.f-c)*a.x + c*b.x,
			(1.f-c)*a.y + c*b.y
		);
	}

	LIBZHL_API void constructor(float x, float y);
	
	float x;
	float y;
};

struct AnimationDescriptor
{
	int numFrames;
	int imageWidth;
	int imageHeight;
	int stripStartY;
	int stripStartX;
	int frameWidth;
	int frameHeight;
};

struct Animation
{
	GL_Texture *animationStrip;
	AnimationDescriptor info;
	AnimationTracker tracker;
	Pointf position;
	std::string soundForward;
	std::string soundReverse;
	bool randomizeFrames;
	float fScale;
	float fYStretch;
	int currentFrame;
	bool bAlwaysMirror;
	std::vector<void*> soundQueue;
	float fadeOut;
	float startFadeOut;
	std::string animName;
	int mask_x_pos;
	int mask_x_size;
	int mask_y_pos;
	int mask_y_size;
	GL_Primitive *primitive;
	GL_Primitive *mirroredPrimitive;
};

struct ArmamentBox;
struct CommandGui;

struct ArmamentControl
{
	void *vptr;
	int systemId;
	CommandGui *gui;
	ShipManager *shipManager;
	std::vector<ArmamentBox*> boxes;
	Point location;
	Globals::Rect touchHitBox;
	GL_Texture *holderImage;
	GL_Primitive *holder;
	GL_Primitive *holderTab;
	std::vector<GL_Primitive*> smallBoxHolder;
	std::vector<Animation> smallBoxHackAnim;
	int smallBoxHolderTop;
	bool bOpen;
	Point lastMouse;
	Point currentMouse;
	int draggingBox;
	int draggingTouch;
	bool bDragging;
	int iLastSwapSlot;
	bool bTutorialFlash;
	int iFlashSlot;
	int activeTouch;
};

struct CrewBlueprint;
struct InfoBox;

struct SystemBlueprint;
struct WindowFrame;

struct TextString
{
	std::string data;
	bool isLiteral;
	char _PAD_1;
	char _PAD_2;
	char _PAD_3;
};

struct Description
{
	TextString title;
	TextString shortTitle;
	TextString description;
	int cost;
	int rarity;
	int baseRarity;
	int bp;
	bool locked;
	TextString tooltip;
	std::string tip;
};

struct InfoBox
{

	enum ExpandDir
	{
	  EXPAND_UP = -1,
	  EXPAND_NONE = 0,
	  EXPAND_DOWN = 1,
	};


	LIBZHL_API void OnRender();
	LIBZHL_API int Clear();
	LIBZHL_API int SetDescription(Description *desc, int width, int height, InfoBox::ExpandDir dir);
	
	Point location;
	const SystemBlueprint *blueprint;
	Description desc;
	int tempUpgrade;
	int powerLevel;
	int maxPower;
	int systemId;
	int systemWidth;
	int yShift;
	Point descBoxSize;
	const CrewBlueprint *pCrewBlueprint;
	std::string warning;
	bool bDetailed;
	std::string additionalTip;
	std::string additionalWarning;
	WindowFrame *primaryBox;
	int primaryBoxOffset;
	WindowFrame *secondaryBox;
	std::string droneBlueprint;
};

struct ShipSelect;

struct ShipButton;

struct GL_Color
{
	GL_Color(float rr, float gg, float bb, float aa) : r(rr), g(gg), b(bb), a(aa)
	{
		
	}
	
	GL_Color() {}

	float r;
	float g;
	float b;
	float a;
};

struct TextButton : GenericButton
{
	GL_Primitive *primitives[3];
	GL_Texture *baseImage;
	Point baseImageOffset;
	GL_Primitive *basePrimitive;
	bool colorsSet;
	GL_Color colors[3];
	GL_Color textColor;
	Point buttonSize;
	int cornerInset;
	bool autoWidth;
	int autoWidthMargin;
	int autoWidthMin;
	bool autoRightAlign;
	TextString label;
	int font;
	int lineHeight;
	int textYOffset;
	bool autoShrink;
};

struct FocusWindow
{
	void *vptr;
	bool bOpen;
	bool bFullFocus;
	Point close;
	bool bCloseButtonSelected;
	Point position;
};

struct AugmentBlueprint;

struct DroneBlueprint;

struct Blueprint
{
	void *vptr;
	std::string name;
	Description desc;
	int type;
};

struct CrewBlueprint : Blueprint
{
	TextString crewName;
	TextString crewNameLong;
	std::vector<TextString> powers;
	bool male;
	std::vector<std_pair_int_int> skillLevel;
	std::vector<std::vector<GL_Color>> colorLayers;
	std::vector<int> colorChoices;
};

struct WeaponBlueprint;

struct ResourceEvent
{
	int missiles;
	int fuel;
	int drones;
	int scrap;
	int crew;
	bool traitor;
	bool cloneable;
	TextString cloneText;
	std::string crewType;
	const WeaponBlueprint *weapon;
	const DroneBlueprint *drone;
	const AugmentBlueprint *augment;
	CrewBlueprint crewBlue;
	int systemId;
	int weaponCount;
	int droneCount;
	bool steal;
	bool intruders;
	int fleetDelay;
	int hullDamage;
	int upgradeAmount;
	int upgradeId;
	int upgradeSuccessFlag;
	std::string removeItem;
};

struct ChoiceText
{
	int type;
	std::string text;
	ResourceEvent rewards;
};

struct ChoiceBox : FocusWindow
{
	GL_Texture *textBox;
	WindowFrame *box;
	std::string mainText;
	std::vector<ChoiceText> choices;
	unsigned int columnSize;
	std::vector<Globals::Rect> choiceBoxes;
	int potentialChoice;
	int selectedChoice;
	int fontSize;
	bool centered;
	int gap_size;
	float openTime;
	ResourceEvent rewards;
	GL_Color currentTextColor;
	Pointf lastChoice;
};

struct UnlockArrow;

struct UnlockArrow
{
	LIBZHL_API void constructor(Point pos, int unk1, int unk2);
	LIBZHL_API bool MouseMove(int x, int y);
	LIBZHL_API void OnRender();
	
	int direction;
	int status;
	Globals::Rect shape;
};

struct ShipSelect
{
	LIBZHL_API void SelectShip(int shipType);
	LIBZHL_API void constructor();
	LIBZHL_API int ClearShipButtons();
	LIBZHL_API void MouseMove(int x, int y);
	LIBZHL_API void OnLoop();
	LIBZHL_API int KeyDown(SDLKey key);
	LIBZHL_API void MouseClick();
	LIBZHL_API void PreSelectShip(int shipType);
	LIBZHL_API void Close();
	LIBZHL_API int GetSelectedShip();
	LIBZHL_API void Open();
	LIBZHL_API void OnRender();
	
	Point position;
	Point titlePos;
	std::vector<GL_Primitive*> shipListBase;
	std::vector<ShipButton*> shipButtons;
	std::vector<UnlockArrow> arrows;
	bool bOpen;
	int selectedShip;
	InfoBox infoBox;
	int currentType;
	TextButton typeA;
	TextButton typeB;
	TextButton typeC;
	TextButton confirm;
	bool bConfirmed;
	int activeTouch;
	ChoiceBox tutorial;
	int tutorialPage;
};

struct ScoreKeeper;

struct TopScore
{
	std::string name;
	std::string blueprint;
	GL_Texture *image;
	int sector;
	int score;
	bool victory;
	int difficulty;
	bool advancedContent;
};

struct CrewStatTracker
{
	int max;
	std::string name;
	std::string species;
	bool male;
	std::string title_id;
	bool newStat;
};

struct StatTracker
{
	int max;
	int total;
	int current;
	std::string desc_id;
	int sector;
};

struct ScoreKeeper
{
	LIBZHL_API char GetShipUnlocked(int shipId, int shipVariant);
	LIBZHL_API static std::string *__stdcall GetShipBlueprint(std::string *str, ScoreKeeper *scoreKeeper, int index);
	
	StatTracker stats[4];
	CrewStatTracker crewStats[5];
	int gamesPlayed;
	int victories;
	TopScore currentScore;
	std::vector<std::vector<bool>> unlocked;
	bool firstRun;
	std::vector<TopScore> topScores;
	std::vector<std::vector<std::vector<TopScore>>> shipScores;
	bool bSavedScore;
	int newestHighScore;
	int newestShipBest;
	int newestShipType;
	int newestShipLayout;
	std::vector<std_pair_CAchievement_ptr_Point> achievements;
	std::vector<std_pair_CAchievement_ptr_Point> shipAchievements;
	GL_Texture *rightBox[2];
	GL_Texture *leftBox[2];
	Button rightButtons[2];
	Button leftButtons[2];
	bool activatedRight;
	bool activatedLeft;
	GL_Texture *lockIcon;
	InfoBox infoBox;
	TextButton closeButton;
	TextButton typeA;
	TextButton typeB;
	TextButton typeC;
	Button leftButton;
	Button rightButton;
	TextButton listButton;
	std::vector<GenericButton*> buttons;
	int selectedShip;
	int selectedLayout;
	int selectedAch;
	int selectedShipAch;
	ShipSelect shipSelect;
	int activeTouch;
	bool forceUnlockAll;
	bool gotCloseEvent;
	int loadingGameVersion;
	Point shipListCenter;
	Point typeALoc;
	Point typeBLoc;
	Point typeCLoc;
};

struct CooldownSystemBox
{
};

struct PowerProfile
{
};

struct CrewTask
{
	int taskId;
	int room;
	int _sil__DO_NOT_USE_system;
};

struct CrewAI
{
	ShipManager *ship;
	bool bAIon;
	bool bAirlockRequested;
	bool bMedbayRequested;
	bool bHurtCrew;
	bool bCalmShip;
	std::vector<CrewMember*> crewList;
	std::vector<CrewMember*> intruderList;
	std::vector<Repairable*> hullBreaches;
	std::vector<CrewTask> desiredTaskList;
	std::vector<CrewTask> bonusTaskList;
	std::vector<bool> breachedRooms;
	char unk[8];
	int iTeleportRequest;
	bool bUrgentTeleport;
	int startingCrewCount;
	bool bMultiracialCrew;
	bool bOverrideRace;
};

struct ShipAI
{
	ShipManager *ship;
	ShipManager *target;
	CrewAI crewAI;
	CombatAI combatAI;
	bool playerShip;
	bool surrendered;
	bool escaping;
	bool destroyed;
	int surrenderThreshold;
	int escapeThreshold;
	float escapeTimer;
	int lastMaxPower;
	std::map<std::string, PowerProfile> powerProfiles;
	int boardingProfile;
	int iTeleportRequest;
	int iTeleportTarget;
	int brokenSystems;
	int boardingAi;
	int iCrewNeeded;
	bool bStalemateTrigger;
	float fStalemateTimer;
	int lastHealth;
	bool bBoss;
	int iTimesTeleported;
};

struct HackBox
{
};

struct SectorDescription
{
	std::vector<std_pair_std_string_RandomAmount> eventCounts;
	std::vector<std_pair_std_string_int> rarities;
	bool unique;
	std::vector<TextString> names;
	std::vector<TextString> shortNames;
	std::vector<std::string> musicTracks;
	std::string type;
	TextString name;
	TextString shortName;
	int minSector;
	bool used;
	std::string firstEvent;
};

struct CrewMemberFactory
{
};

struct GL_FrameBuffer
{
};

struct WeaponSystem
{
};

struct RewardDesc
{
	std::string reward;
	int level;
};

struct CloneSystem
{
};

struct BoardingEvent
{
	std::string type;
	int min;
	int max;
	int amount;
	bool breach;
};

struct ShipBlueprint;
struct BlueprintManager;

struct BlueprintManager
{
	LIBZHL_API static std::vector<std::string> &__stdcall GetBlueprintList(std::vector<std::string> &vec, BlueprintManager *bpM, std::string &str);
	LIBZHL_API ShipBlueprint *GetShipBlueprint(std::string &name, int unk);
	
};

struct TimerHelper
{
	int maxTime;
	int minTime;
	float currTime;
	float currGoal;
	bool loop;
	bool running;
};

struct Animation;
struct MainMenu;

struct CreditScreen
{
	float scroll;
	std::string shipName;
	std::string crewString;
	float pausing;
	GL_Texture *bg;
	std::vector<std::string> creditNames;
	int lastValidCredit;
	int touchesDown;
	double touchDownTime;
	float skipMessageTimer;
	char gap5C[3];
	char _unk;
};

struct ConfirmWindow : FocusWindow
{
	TextString text;
	int textHeight;
	int minWidth;
	int windowWidth;
	TextString yesText;
	TextString noText;
	bool autoCenter;
	GL_Texture *windowImage;
	GL_Primitive *window;
	TextButton yesButton;
	TextButton noButton;
	bool result;
};

struct SlideBar
{
	Globals::Rect box;
	bool hovering;
	bool holding;
	Globals::Rect marker;
	Point mouseStart;
	Point rectStart;
	std::pair<int, int> minMax;
};

struct ControlButton
{
};

struct ControlsScreen
{
	std::vector<ControlButton> buttons[4];
	int selectedButton;
	TextButton defaultButton;
	ConfirmWindow resetDialog;
	Button pageButtons[4];
	int currentPage;
	WindowFrame *customBox;
};

struct TextButton;

struct LanguageChooser : FocusWindow
{
	std::vector<TextButton*> buttons;
	int iChoice;
};

struct OptionsScreen : ChoiceBox
{
	Point position;
	Point wipeProfilePosition;
	SlideBar soundVolume;
	SlideBar musicVolume;
	bool bCustomizeControls;
	ControlsScreen controls;
	TextButton closeButton;
	TextButton wipeProfileButton;
	int choiceFullscreen;
	int choiceVSync;
	int choiceFrameLimit;
	int choiceLowend;
	int choiceColorblind;
	int choiceLanguage;
	int choiceDialogKeys;
	int choiceShowPaths;
	int choiceAchievementPopups;
	int choiceAutoPause;
	int choiceTouchAutoPause;
	int choiceControls;
	int lastFullScreen;
	bool isSoundTouch;
	bool isMusicTouch;
	LanguageChooser langChooser;
	bool showWipeButton;
	ConfirmWindow wipeProfileDialog;
	ChoiceBox restartRequiredDialog;
};

struct EquipmentBox;
struct CrewCustomizeBox;

struct SystemCustomBox;

struct ShipBuilder;

struct TextInput;

struct TextInput
{
	enum AllowedCharType
	{
	  ALLOW_ASCII = 0x0,
	  ALLOW_LANGUAGE = 0x1,
	  ALLOW_ANY = 0x2,
	};



	LIBZHL_API int SetText(std::string &text);
	
	std::string prompt;
	std::vector<int> text;
	std::vector<int> oldText;
	int pos;
	int lastPos;
	bool bActive;
	int allowedChars;
	int maxChars;
	TimerHelper blinker;
};

struct ShipAchievementInfo
{
};

struct ShipBuilder
{
	LIBZHL_API void *CheckTypes();
	LIBZHL_API void MouseClick(int x, int y);
	LIBZHL_API void SwitchShip(int shipType, int shipVariant);
	LIBZHL_API void CreateEquipmentBoxes();
	LIBZHL_API void CreateSystemBoxes();
	LIBZHL_API void SetupShipAchievements();
	LIBZHL_API void ClearShipAchievements();
	LIBZHL_API int SwapType(int variant);
	LIBZHL_API void CycleShipNext();
	LIBZHL_API void CycleShipPrevious();
	
	ShipManager *currentShip;
	GL_Primitive *nameBoxPrimitive;
	GL_Primitive *enableAdvancedPrimitive;
	Button resetButton;
	Button clearButton;
	TextButton startButton;
	TextButton backButton;
	TextButton renameButton;
	Button leftButton;
	Button rightButton;
	TextButton listButton;
	TextButton showButton;
	TextButton easyButton;
	TextButton normalButton;
	TextButton hardButton;
	TextButton typeA;
	TextButton typeB;
	TextButton typeC;
	Point typeALoc;
	Point typeBLoc;
	Point typeCLoc;
	TextButton randomButton;
	TextButton advancedOffButton;
	TextButton advancedOnButton;
	std::vector<GenericButton*> buttons;
	std::vector<Animation> animations;
	std::vector<CrewCustomizeBox*> vCrewBoxes;
	bool bOpen;
	GL_Primitive *baseImage;
	GL_Primitive *shipSelectBox;
	GL_Primitive *shipAchBox;
	GL_Primitive *shipEquipBox;
	GL_Primitive *startButtonBox;
	GL_Primitive *advancedButtonBox;
	int typeAOffset;
	int typeBOffset;
	int typeCOffset;
	int shipAchPadding;
	int advancedTitleOffset;
	std::vector<EquipmentBox*> vEquipmentBoxes;
	InfoBox infoBox;
	std::vector<SystemCustomBox*> sysBoxes;
	int shoppingId;
	int currentSlot;
	int currentBox;
	bool bDone;
	const ShipBlueprint *ships[30];
	int currentShipId;
	int storeIds[4];
	bool bRenaming;
	std::string currentName;
	bool bShowRooms;
	bool bCustomizingCrew;
	Animation walkingMan;
	Pointf walkingManPos;
	ShipSelect shipSelect;
	ChoiceBox introScreen;
	bool bShowedIntro;
	int currentType;
	TextInput nameInput;
	int activeTouch;
	bool activeTouchIsShip;
	bool shipDragActive;
	bool shipDragVertical;
	Point shipDragOffset;
	std::vector<ShipAchievementInfo> shipAchievements;
	int selectedAch;
	GL_Texture *arrow;
	WindowFrame *descBox;
	AnimationTracker tracker;
	bool encourageShipList;
};

struct MainMenu
{
	LIBZHL_API void OnRender();
	
	bool bOpen;
	int activeTouch;
	GL_Texture *background;
	GL_Texture *glowy;
	AnimationTracker glowTracker;
	Button continueButton;
	Button startButton;
	Button helpButton;
	Button statButton;
	Button optionsButton;
	Button creditsButton;
	Button quitButton;
	bool itbButtonActive;
	Button itbButton;
	Animation *itbAnim;
	std::vector<Button*> buttons;
	int finalChoice;
	ShipBuilder shipBuilder;
	bool bScoreScreen;
	OptionsScreen optionScreen;
	bool bSelectSave;
	ConfirmWindow confirmNewGame;
	ChoiceBox changelog;
	bool bCreditScreen;
	char _unk[4];
	CreditScreen credits;
	bool bChangedLogin;
	std::vector<CrewMember*> testCrew;
	bool bChangedScreen;
	std::string error;
	char gap34A4[3];
	char _unk2;
};

struct ResourceManager
{
};

struct RepairDrone
{
};

struct CrewLaser
{
};

struct Targetable
{
	void *vptr;
	int type;
	bool hostile;
	bool targeted;
};

struct Store
{
};

struct Selectable
{
	void *vptr;
	int selectedState;
};

struct Room : Selectable
{
	Point GetIntoRoom(Point pos)
	{
		int posX = pos.x;
		int posY = pos.y;
		
		int rectY = this->rect.y;
		int rectH = this->rect.h;
		
		int rectX = this->rect.x;
		int rectW = this->rect.w;
		
		//printf("%d %d %d %d %d %d\n", posX, posY, rectX, rectW, rectY, rectH);
		
		if (rectY + rectH - posY > 15 && posY - rectY > 15)
		{
			if (rectW + rectX - posX > 15 && posX - rectX > 15)
			{
				return Point(-1, -1);
			}
		}
		
		
		
		
		Point center = Point(rectX + rectW / 2, rectY + rectH / 2);
		
		Point toGrid = Point((posX + 2 * (center.x > posX) - 1) / 35, (posY + 2 * (center.y > posY) - 1) / 35);
		Point fromGrid = Point(toGrid.x * 35, toGrid.y * 35);
		
		return Point(fromGrid.x + 17, fromGrid.y + 17);
	}

	Globals::Rect GetRect() 
	{
		return this->rect;
	}

	ShipObject _shipObject;
	Globals::Rect rect;
	int iRoomId;
	bool bBlackedOut;
	std::vector<int> filledSlots;
	std::vector<std::vector<bool>> slots;
	bool bWarningLight;
	AnimationTracker lightTracker;
	int iFireCount;
	std::vector<Animation> fires;
	int primarySlot;
	int primaryDirection;
	float lastO2;
	GL_Primitive *floorPrimitive;
	GL_Primitive *blackoutPrimitive;
	GL_Primitive *highlightPrimitive;
	GL_Primitive *highlightPrimitive2;
	GL_Primitive *o2LowPrimitive;
	GL_Primitive *computerPrimitive;
	GL_Primitive *computerGlowPrimitive;
	GL_Primitive *computerGlowYellowPrimitive;
	GL_Primitive *lightPrimitive;
	GL_Primitive *lightGlowPrimitive;
	Animation stunSparks;
	Animation consoleSparks;
	bool bStunning;
	float fHacked;
	int currentSparkRotation;
	std::vector<Animation> sparks;
	float sparkTimer;
	int sparkCount;
	int iHackLevel;
	Animation roomTapped;
};

struct ShipGraph;

struct ShipGraph
{

	static Point TranslateFromGrid(int xx, int yy)
	{
		return Point(xx * 35, yy * 35);
	}
	
	static Point TranslateToGrid(int xx, int yy)
	{
		return Point(xx / 35, yy / 35);
	}
	
	Point GetIntoRoom(int roomId, Point pos)
	{
		Point ret;
		if (roomId >= 0 && roomId < this->rooms.size())
		{
			auto room = this->rooms[roomId];
			ret = room->GetIntoRoom(pos);
		}
		else
		{
			ret = Point(-1, -1);
		}
		
		return ret;
	}
	
	Pointf GetRoomCenter(int roomId) 
	{
		auto rooms = this->rooms;
		float xx = -1;
		float yy = -1;
		
		for (auto const& x: rooms)
		{
			if (roomId == x->iRoomId)
			{
				auto rect = x->GetRect();
				
				xx = rect.x + rect.w / 2;
				yy = rect.y + rect.h / 2;
			}
		}
		
		return Pointf(xx, yy);
	}

	LIBZHL_API static ShipGraph *__stdcall GetShipInfo(int shipId);
	LIBZHL_API Point GetSlotWorldPosition(int x, int y);
	LIBZHL_API Point GetSlotRenderPosition(int x, int y, char unk);
	LIBZHL_API int DoorCount(int unk);
	LIBZHL_API std::vector<Door*> &GetDoors(int unk);
	LIBZHL_API int IsRoomConnected(int room1, int room2);
	LIBZHL_API int ConnectingDoor(int x1, int y1, int x2, int y2);
	LIBZHL_API int ConnectedGridSquares(int x1, int y1, int x2, int y2);
	LIBZHL_API int ConnectedGridSquaresPoint(Point p1, Point p2);
	LIBZHL_API int ConnectingDoor(Point p1, Point p2);
	LIBZHL_API int GetNumSlots(int room);
	LIBZHL_API static int __stdcall PopClosestDoor(std::vector<int> &unk, std::vector<float> &unk2);
	LIBZHL_API int Dijkstra(Point p1, Point p2, int unk);
	LIBZHL_API int FindPath(Point p1, Point p2, int unk);
	LIBZHL_API float ConvertToWorldAngle(float ang);
	LIBZHL_API float ConvertToLocalAngle(float ang);
	LIBZHL_API int64_t ConvertToWorldPosition(Pointf p);
	LIBZHL_API Pointf ConvertToLocalPosition(Pointf p, char unk);
	LIBZHL_API int ComputeCenter();
	LIBZHL_API bool ContainsPoint(int x, int y);
	LIBZHL_API int GetSelectedRoom(int x, int y, bool unk);
	LIBZHL_API int RoomCount();
	LIBZHL_API Globals::Rect *GetRoomShape(int room);
	LIBZHL_API float GetRoomOxygen(int room);
	LIBZHL_API bool GetRoomBlackedOut(int room);
	
	std::vector<Room*> rooms;
	std::vector<Door*> doors;
	std::vector<int> doorCounts;
	Point center;
	Pointf worldPosition;
	float worldHeading;
	Pointf lastWorldPosition;
	float lastWorldHeading;
	Globals::Rect shipBox;
	std::string shipName;
};

struct ShipBlueprint : Blueprint
{
	struct SystemTemplate
	{
		int systemId;
		int powerLevel;
		std::vector<int> location;
		int bp;
		int maxPower;
		std::string image;
		int slot;
		int direction;
		std::vector<std::string> weapon;
	};
	
	Description desc;
	std::string blueprintName;
	TextString name;
	TextString shipClass;
	std::string layoutFile;
	std::string imgFile;
	std::string cloakFile;
	std::string shieldFile;
	std::string floorFile;
	std::map<int, SystemTemplate> systemInfo;
	std::vector<int> systems;
	int droneCount;
	int originalDroneCount;
	int droneSlots;
	std::string loadDrones;
	std::vector<std::string> drones;
	std::vector<std::string> augments;
	int weaponCount;
	int originalWeaponCount;
	int weaponSlots;
	std::string loadWeapons;
	std::vector<std::string> weapons;
	int missiles;
	int drone_count;
	int health;
	int originalCrewCount;
	std::vector<std::string> defaultCrew;
	std::vector<CrewBlueprint> customCrew;
	int maxPower;
	int boardingAI;
	int bp_count;
	int maxCrew;
	int maxSector;
	int minSector;
	TextString unlock;
};

struct CrewDesc
{
};

struct ShipEvent
{
	bool present;
	std::string name;
	std::string blueprint;
	std::string auto_blueprint;
	std::vector<std::string> blueprintList;
	ShipBlueprint actualBlueprint;
	bool hostile;
	std::string surrender;
	std::string escape;
	std::string destroyed;
	std::string deadCrew;
	std::string gotaway;
	int escapeTimer;
	RandomAmount surrenderThreshold;
	RandomAmount escapeThreshold;
	std::vector<CrewDesc> crewOverride;
	std::vector<std::string> weaponOverride;
	int weaponOverCount;
	std::vector<std::string> droneOverride;
	int droneOverCount;
	int shipSeed;
};

struct GL_Texture
{
};

struct WeaponMount
{
	Point position;
	bool mirror;
	bool rotate;
	int slide;
	int gib;
};

struct CachedPrimitive
{
	void *vptr;
	GL_Primitive *primitive;
};

struct CachedImage : CachedPrimitive
{
	enum Centered
	{
	  CENTERED = 0x0,
	};

	std::string imageName;
	GL_Texture *texture;
	int x;
	int y;
	float wScale;
	float hScale;
	float x_start;
	float y_start;
	float x_size;
	float y_size;
	float rotation;
	bool mirrored;
};

struct WeaponAnimation
{
	Animation anim;
	bool bFireShot;
	bool bFiring;
	float fChargeLevel;
	int iChargedFrame;
	int iFireFrame;
	bool bMirrored;
	bool bRotation;
	Point fireLocation;
	bool bPowered;
	Point mountPoint;
	Point renderPoint;
	Point fireMountVector;
	AnimationTracker slideTracker;
	int slideDirection;
	CachedImage iChargeImage;
	Animation explosionAnim;
	WeaponMount mount;
	float fDelayChargeTime;
	Animation boostAnim;
	int boostLevel;
	bool bShowCharge;
	float fActualChargeLevel;
	int iChargeOffset;
	int iChargeLevels;
	int currentOffset;
	CachedImage chargeBox;
	CachedImage chargeBar;
	int iHackLevel;
	Animation hackSparks;
	bool playerShip;
};

struct ExplosionAnimation : AnimationTracker
{
	ShipObject shipObj;
	std::vector<Animation> explosions;
	std::vector<GL_Texture> pieces;
	std::vector<std::string> pieceNames;
	std::vector<float> rotationSpeed;
	std::vector<float> rotation;
	std::vector<std_pair_float_float> rotationSpeedMinMax;
	std::vector<Pointf> movementVector;
	std::vector<Pointf> position;
	std::vector<Pointf> startingPosition;
	float explosionTimer;
	float soundTimer;
	bool bFinalBoom;
	bool bJumpOut;
	std::vector<WeaponAnimation> weaponAnims;
	Point pos;
};

struct Fire
{
};

struct Spreader_Fire : ShipObject
{
	int count;
	std::vector<int> roomCount;
	std::vector<std::vector<Fire>> grid;
};

struct Path
{
	Point start;
	std::vector<Door*> doors;
	Point finish;
	float distance;
};

struct CachedRect : CachedPrimitive
{
	int x;
	int y;
	int w;
	int h;
};

struct Slot
{
	int roomId;
	int slotId;
	Point worldLocation;
};

struct ShipSystem;

struct CrewAnimation;

struct SCrewStats
{
	std::vector<int> stat;
	std::string species;
	std::string name;
	bool male;
};

struct CrewMember : CrewTarget
{
	LIBZHL_API void UpdateHealth();
	LIBZHL_API void OnLoop();
	LIBZHL_API void DirectModifyHealth(float health);
	LIBZHL_API char GetNewGoal();
	LIBZHL_API void constructor(CrewBlueprint &blueprint, int unk, bool unk2, CrewAnimation *animation);
	LIBZHL_API void OnRender(bool unk);
	LIBZHL_API void SetCurrentTarget(CrewTarget *target, bool unk);
	
	float x;
	float y;
	float size;
	float scale;
	float goal_x;
	float goal_y;
	int width;
	int height;
	std::pair<float, float> health;
	float speed_x;
	float speed_y;
	Path path;
	bool new_path;
	float x_destination;
	float y_destination;
	Door *last_door;
	Repairable *currentRepair;
	bool bSuffocating;
	int moveGoal;
	int selectionState;
	int iRoomId;
	int iManningId;
	int iRepairId;
	int iStackId;
	Slot currentSlot;
	bool intruder;
	bool bFighting;
	bool bSharedSpot;
	CrewAnimation *crewAnim;
	GL_Texture *selectionImage;
	CachedImage healthBox;
	CachedImage healthBoxRed;
	CachedRect healthBar;
	float fMedbay;
	float lastDamageTimer;
	float lastHealthChange;
	int currentShipId;
	AnimationTracker flashHealthTracker;
	Pointf currentTarget;
	CrewTarget *crewTarget;
	BoardingGoal boardingGoal;
	bool bFrozen;
	bool bFrozenLocation;
	CrewTask task;
	std::string type;
	ShipManager *ship;
	Slot finalGoal;
	Door *blockingDoor;
	bool bOutOfGame;
	std::string species;
	bool bDead;
	unsigned __int8 gap_ex_1[2];
	int iOnFire;
	bool bActiveManning;
	ShipSystem *currentSystem;
	int usingSkill;
	CrewBlueprint blueprint;
	Animation healing;
	Animation stunned;
	AnimationTracker levelUp;
	int lastLevelUp;
	SCrewStats stats;
	std::vector<std::vector<bool>> skillsEarned;
	bool clone_ready;
	bool bMindControlled;
	int iDeathNumber;
	Animation mindControlled;
	Animation stunIcon;
	std::vector<std::vector<AnimationTracker>> skillUp;
	int healthBoost;
	float fMindDamageBoost;
	float fCloneDying;
	bool bResisted;
	unsigned __int8 gap_ex_2[2];
	Slot savedPosition;
	float fStunTime;
	CachedImage movementTarget;
	bool bCloned;
};

struct Drone : ShipObject
{
	int selfId;
	bool powered;
	int powerRequired;
	bool deployed;
	int type;
	const DroneBlueprint *blueprint;
	bool bDead;
	int iBonusPower;
	bool poweredAtLocation;
	float destroyedTimer;
	int iHackLevel;
	float hackTime;
};

struct CrewDrone : CrewMember
{
	Drone _drone;
	int droneRoom;
	Animation powerUp;
	Animation powerDown;
	GL_Texture *lightLayer;
	GL_Texture *baseLayer;
};

struct WeaponSystem;
struct DroneSystem;

struct Blueprint;

struct Drone;

struct EquipmentBoxItem
{
	ProjectileFactory *pWeapon;
	Drone *pDrone;
	CrewMember *pCrew;
	const AugmentBlueprint *augment;
};

struct LIBZHL_INTERFACE EquipmentBox
{
	virtual void destroy() LIBZHL_PLACEHOLDER
	virtual void destructor() LIBZHL_PLACEHOLDER
	virtual void SetPosition(Point pos) LIBZHL_PLACEHOLDER
	virtual void OnRender(bool unk) LIBZHL_PLACEHOLDER
	virtual void RenderLabels(bool unk) LIBZHL_PLACEHOLDER
	virtual void RenderIcon() LIBZHL_PLACEHOLDER
	virtual void SetShipManager(ShipManager *ship) LIBZHL_PLACEHOLDER
	virtual void MouseMove(int x, int y) LIBZHL_PLACEHOLDER
	virtual void OnTouch() LIBZHL_PLACEHOLDER
	virtual void UpdateBoxImage(bool unk) LIBZHL_PLACEHOLDER
	virtual void Restart() LIBZHL_PLACEHOLDER
	virtual void AddItem(EquipmentBoxItem item) LIBZHL_PLACEHOLDER
	virtual void Restart1() LIBZHL_PLACEHOLDER
	virtual char CanHoldWeapon() LIBZHL_PLACEHOLDER
	virtual char CanHoldDrone() LIBZHL_PLACEHOLDER
	virtual char CanHoldAugment() LIBZHL_PLACEHOLDER
	virtual void CheckContents() LIBZHL_PLACEHOLDER
	virtual int GetType(bool unk) LIBZHL_PLACEHOLDER
	virtual char IsCargoBox() LIBZHL_PLACEHOLDER
	virtual char CanHoldCrew() LIBZHL_PLACEHOLDER
	virtual char CanDoJob() LIBZHL_PLACEHOLDER
	LIBZHL_API void constructor(Point pos, int slot);
	LIBZHL_API char IsEmpty();
	LIBZHL_API Blueprint *GetBlueprint();
	
	GL_Primitive *blocked_overlay;
	GL_Color overlayColor;
	std::string imageName;
	GL_Primitive *empty;
	GL_Primitive *full;
	GL_Primitive *selected_empty;
	GL_Primitive *selected_full;
	WeaponSystem *weaponSys;
	DroneSystem *droneSys;
	Point location;
	Globals::Rect hitBox;
	EquipmentBoxItem item;
	bool bMouseHovering;
	bool bGlow;
	bool bBlocked;
	int slot;
	bool bLocked;
	int value;
	bool bPermanentLock;
	bool blockDetailed;
};

struct AugmentEquipBox : EquipmentBox
{
	ShipManager *ship;
};

struct OuterHull;

struct LockdownShard
{
};

struct Room;

struct ImageDesc
{
	GL_Texture *tex;
	int resId;
	int w;
	int h;
	int x;
	int y;
	int rot;
};

struct Ship : ShipObject
{
	Pointf GetRoomCenter(int room)
	{
		auto graph = ShipGraph::GetShipInfo(this->iShipId);
		return graph->GetRoomCenter(room);
	}
	
	enum DoorStateType
	{
	  DOOR_CLOSED = 0x0,
	  DOOR_OPEN = 0x1,
	  DOOR_OPEN_FORCED = 0x2,
	  DOOR_HIT = 0x3,
	  DOOR_ANIMATING = 0x4,
	};


	struct DoorState
	{
		DoorStateType state;
		bool hacked;
		int level;
	};
	
	std::vector<Room*> vRoomList;
	std::vector<Door*> vDoorList;
	std::vector<OuterHull*> vOuterWalls;
	std::vector<OuterHull*> vOuterAirlocks;
	std::pair<int, int> hullIntegrity;
	std::vector<WeaponMount> weaponMounts;
	std::string floorImageName;
	ImageDesc shipFloor;
	GL_Primitive *floorPrimitive;
	std::string shipImageName;
	ImageDesc shipImage;
	Point glowOffset;
	GL_Primitive *shipImagePrimitive;
	std::string cloakImageName;
	ImageDesc shipImageCloak;
	GL_Primitive *cloakPrimitive;
	GL_Primitive *gridPrimitive;
	GL_Primitive *wallsPrimitive;
	GL_Primitive *doorsPrimitive;
	std::vector<DoorState> doorState;
	bool lastDoorControlMode;
	GL_Texture *thrustersImage;
	GL_Texture *jumpGlare;
	int vertical_shift;
	int horizontal_shift;
	std::string shipName;
	ExplosionAnimation explosion;
	bool bDestroyed;
	Globals::Ellipse baseEllipse;
	Animation engineAnim[2];
	AnimationTracker cloakingTracker;
	bool bCloaked;
	bool bExperiment;
	bool bShowEngines;
	std::vector<LockdownShard> lockdowns;
};

struct CachedRectOutline : CachedPrimitive
{
	int x;
	int y;
	int w;
	int h;
	int thickness;
};

struct Repairable : Selectable
{
	ShipObject shipObj;
	float fDamage;
	Point pLoc;
	float fMaxDamage;
	std::string name;
	int roomId;
	int iRepairCount;
};

struct ShipSystem : Repairable
{
	LIBZHL_API char DamageOverTime(float unk);
	LIBZHL_API int CheckForRepower();
	LIBZHL_API static int __stdcall NameToSystemId(std::string &name);
	LIBZHL_API int IsMannedBoost();
	LIBZHL_API static std::string &__stdcall SystemIdToName(std::string &strRef, int systemId);
	LIBZHL_API void OnLoop();
	
	int iSystemType;
	bool bNeedsManned;
	bool bManned;
	int iActiveManned;
	bool bBoostable;
	std::pair<int, int> powerState;
	int iRequiredPower;
	GL_Texture *imageIcon;
	GL_Primitive *iconPrimitive;
	GL_Primitive *iconBorderPrimitive;
	GL_Primitive *iconPrimitives[20];
	CachedRect partialDamageRect;
	CachedRectOutline lockOutline;
	Globals::Rect roomShape;
	bool bOnFire;
	bool bBreached;
	std::pair<int, int> healthState;
	float fDamageOverTime;
	float fRepairOverTime;
	bool damagedLastFrame;
	bool repairedLastFrame;
	int originalPower;
	bool bNeedsPower;
	int iTempPowerCap;
	int iTempPowerLoss;
	int iTempDividePower;
	int iLockCount;
	TimerHelper lockTimer;
	bool bExploded;
	bool bOccupied;
	bool bFriendlies;
	std::string interiorImageName;
	GL_Primitive *interiorImage;
	GL_Primitive *interiorImageOn;
	GL_Primitive *interiorImageManned;
	GL_Primitive *interiorImageMannedFancy;
	int lastUserPower;
	int iBonusPower;
	int iLastBonusPower;
	Pointf location;
	int bpCost;
	AnimationTracker flashTracker;
	int maxLevel;
	int iBatteryPower;
	int iHackEffect;
	bool bUnderAttack;
	bool bLevelBoostable;
	bool bTriggerIon;
	std::vector<Animation> damagingEffects;
	int computerLevel;
};

struct InputBox : FocusWindow
{
	WindowFrame *textBox;
	std::string mainText;
	bool bDone;
	bool bInvertCaps;
	std::string inputText;
	std::vector<std::string> lastInputs;
	int lastInputIndex;
};

struct CrewAnimation
{
};

struct Projectile;

struct AsteroidGenerator;

struct AsteroidGenerator
{
	LIBZHL_API void constructor();
	LIBZHL_API float Initialize(int numberOfShips, int shieldCount, bool unk);
	LIBZHL_API void SetNumShips(int ships);
	LIBZHL_API Projectile *GetNextAsteroid();
	LIBZHL_API void OnLoop();
	LIBZHL_API float GetNextState();
	LIBZHL_API void UpdateState();
	LIBZHL_API bool SaveAsteroids(void *file);
	LIBZHL_API void LoadAsteroids(void *file);
	
	std::queue<Projectile*> asteroidQueue;
	RandomAmount spawnRate[3];
	RandomAmount stateLength[3];
	int numberOfShips;
	int iState;
	int currentSpace;
	int iNextDirection;
	float fStateTimer;
	float timer;
	bool bRunning;
	int initShields;
};

struct ShipRepairDrone
{
};

struct BatteryBox
{
};

struct SpaceManager;

struct CompleteShip;

struct CompleteShip
{
	LIBZHL_API int PauseLoop();
	LIBZHL_API int Restart();
	LIBZHL_API void constructor(SpaceManager *space, bool unk, int unk2);
	LIBZHL_API void SetShip(ShipManager *ship);
	LIBZHL_API void OnInit(const ShipBlueprint *blueprint, int unk);
	LIBZHL_API void AddBoarders(int amount, std::string &race, bool unk2);
	LIBZHL_API CrewMember *AddCrewMember(const CrewBlueprint *blueprint, bool hostile);
	LIBZHL_API CrewMember *AddCrewMember1(std::string &race, bool hostile);
	LIBZHL_API Drone *AddDrone(const DroneBlueprint *blueprint, int unk);
	LIBZHL_API CrewMember *AddCrewMember2(CrewMember *member, int unk);
	LIBZHL_API char SaveState(int unk);
	LIBZHL_API void LoadState(int unk);
	LIBZHL_API int InitiateTeleport(int room, int shipId);
	LIBZHL_API void OnLoop();
	LIBZHL_API void GetTeleportingParty();
	LIBZHL_API CrewMember *KillRandomCrew();
	LIBZHL_API int CountCrew(bool boarders);
	LIBZHL_API CrewMember **Jump();
	LIBZHL_API void SetEnemyShip(CompleteShip *other);
	LIBZHL_API bool DeadCrew();
	LIBZHL_API void TeleportCrew(ShipManager *other, int room, bool comingBack);
	LIBZHL_API void OnRender();
	LIBZHL_API void OnRenderSpace();
	LIBZHL_API void OnRenderShip(bool unk1, bool unk2);
	
	void *vptr;
	int iShipId;
	ShipManager *shipManager;
	SpaceManager *spaceManager;
	CompleteShip *enemyShip;
	bool bPlayerShip;
	ShipAI shipAI;
	std::vector<CrewMember*> arrivingParty;
	std::vector<CrewMember*> leavingParty;
	int teleTargetRoom;
};

struct BossShip;

struct BossShip : CompleteShip
{
	LIBZHL_API void Restart();
	LIBZHL_API char IncomingFire();
	LIBZHL_API void constructor(SpaceManager *space);
	LIBZHL_API void SaveBoss(void *file);
	LIBZHL_API int LoadBoss(void *file);
	LIBZHL_API int ClearLocation();
	LIBZHL_API char Defeated();
	LIBZHL_API int GetSubEvent();
	LIBZHL_API void StartStage();
	LIBZHL_API int GetEvent();
	LIBZHL_API void OnLoop();
	
	int currentStage;
	TimerHelper powerTimer;
	int powerCount;
	std::vector<int> crewCounts;
	bool bDeathBegan;
	int nextStage;
};

struct AnaerobicAnimation
{
};

struct DebugHelper
{
};

struct TutorialManager
{
};

struct WarningMessage
{
	void *vptr;
	AnimationTracker tracker;
	Point position;
	bool isImage;
	TextString text;
	bool centerText;
	GL_Color textColor;
	bool useWarningLine;
	CachedImage image;
	std::string imageName;
	bool flash;
	std::string sound;
	AnimationTracker flashTracker;
};

struct DroneControl : ArmamentControl
{
	WarningMessage droneMessage;
	WarningMessage noTargetMessage;
	WarningMessage systemMessage;
};

struct NebulaCloud
{
};

struct AchievementTracker;

struct AchievementTracker
{
	LIBZHL_API void LoadAchievementDescriptions();
	LIBZHL_API static void *__stdcall GetShipAchievements(void *unk, AchievementTracker *tracker, std::string &name);
	
};

struct EngineSystem;
struct HackingSystem;
struct BatterySystem;
struct ParticleEmitter;

struct Collideable
{
	void *vptr;
};

struct MedbaySystem;
struct OxygenSystem;

struct DamageMessage;

struct TeleportSystem;

struct MindSystem;
struct CloakingSystem;

struct ArtillerySystem
{
};

struct CloneSystem;
struct Shields;

struct ShipManager : ShipObject
{
	ShipManager(int shipId) 
	{
		this->constructor(shipId);
	}
	
	Pointf GetRandomRoomCenter()
	{
		auto rng = rand();
		auto graph = ShipGraph::GetShipInfo(this->iShipId);
		auto rooms = graph->rooms.size();
		return this->ship.GetRoomCenter(rng % rooms);
	}


	LIBZHL_API int constructor(int shipId);
	LIBZHL_API void AddInitialCrew(std::vector<CrewBlueprint> &blueprints);
	LIBZHL_API int GetDodgeFactor();
	LIBZHL_API void OnRender(bool unk1, bool unk2);
	LIBZHL_API int CountCrew(bool boarders);
	LIBZHL_API int TeleportCrew(ShipManager *other, int room, bool comingBack);
	LIBZHL_API int OnInit(ShipBlueprint *bp, int shipLevel);
	
	Targetable _targetable;
	Collideable _collideable;
	std::vector<ShipSystem> vSystemList;
	OxygenSystem *oxygenSystem;
	TeleportSystem *teleportSystem;
	CloakingSystem *cloakSystem;
	BatterySystem *batterySystem;
	MindSystem *mindSystem;
	CloneSystem *cloneSystem;
	HackingSystem *hackingSystem;
	bool showNetwork;
	bool addedSystem;
	unsigned __int8 gap_ex_1[2];
	Shields *shieldSystem;
	WeaponSystem *weaponSystem;
	DroneSystem *droneSystem;
	EngineSystem *engineSystem;
	MedbaySystem *medbaySystem;
	std::vector<ArtillerySystem> artillerySystems;
	std::vector<CrewMember*> vCrewList;
	Spreader_Fire fireSpreader;
	Ship ship;
	char statusMessages[40];
	bool bGameOver;
	ShipManager *current_target;
	std::pair<float, float> jump_timer;
	int fuel_count;
	bool hostile_ship;
	bool bDestroyed;
	int iLastDamage;
	AnimationTracker jumpAnimation;
	std::vector<DamageMessage*> damMessages;
	std::vector<int> systemKey;
	int currentScrap;
	bool bJumping;
	bool bAutomated;
	unsigned __int8 gap_ex_2[2];
	int shipLevel;
	ShipBlueprint myBlueprint;
	bool lastEngineStatus;
	bool lastJumpReady;
	bool bContainsPlayerCrew;
	int iIntruderCount;
	std::vector<std::vector<int>> crewCounts;
	int tempDroneCount;
	int tempMissileCount;
	std::vector<Animation> explosions;
	std::vector<bool> tempVision;
	unsigned __int8 unk1[8];
	bool bHighlightCrew;
	std::vector<Drone*> droneTrash;
	std::vector<SpaceDrone*> spaceDrones;
	std::vector<SpaceDrone*> newDroneArrivals;
	int bpCount;
	int iCustomizeMode;
	bool bShowRoom;
	std::vector<Projectile*> superBarrage;
	bool bInvincible;
	std::vector<SpaceDrone*> superDrones;
	GL_Primitive *highlight;
	int failedDodgeCounter;
	std::vector<float> hitByBeam;
	bool enemyDamagedUncloaked;
	int damageCloaked;
	std::map<int, int> killedByBeam;
	int minBeaconHealth;
	std::vector<ParticleEmitter*> fireExtinguishers;
	bool bWasSafe;
};

struct ShipRepair
{
};

struct BoarderPodDrone
{
};

struct TeleportBox
{
};

struct WarningMessage;

struct TextButton0 : GenericButton
{
	GL_Primitive *primitives[3];
	GL_Texture *baseImage;
	Point baseImageOffset;
	GL_Primitive *basePrimitive;
	bool colorsSet;
	char gap65[3];
	GL_Color colors[3];
	GL_Color textColor;
	Point buttonSize;
	int cornerInset;
	bool autoWidth;
	char gapB5[3];
	int autoWidthMargin;
	int autoWidthMin;
	bool autoRightAlign;
	char gapC1[3];
	TextString label;
	int font;
	int lineHeight;
	int textYOffset;
	char autoShrink;
	char ready;
};

struct FTLButton : TextButton0
{
	float ftl_blink;
	float ftl_blink_dx;
	float pullout;
	ShipManager *ship;
	GL_Primitive *baseImage;
	GL_Primitive *baseImageRed;
	GL_Primitive *pulloutBase;
	GL_Primitive *pulloutBaseRed;
	GL_Primitive *pilotOn;
	GL_Primitive *pilotOff1;
	GL_Primitive *pilotOff2;
	GL_Primitive *enginesOn;
	GL_Primitive *enginesOff1;
	GL_Primitive *enginesOff2;
	GL_Texture *FTL_loadingbars;
	GL_Texture *FTL_loadingbars_off;
	GL_Primitive *loadingBars;
	GL_Primitive *loadingBarsOff;
	int lastBarsWidth;
	WarningMessage *enginesDown;
	bool bOutOfFuel;
	bool bBossFight;
	bool bHoverRaw;
	bool bHoverPilot;
	bool bHoverEngine;
};

struct DropBox
{
	Point position;
	bool isSellBox;
	GL_Texture *boxImage[2];
	int selectedImage;
	TextString titleText;
	TextString bodyText;
	int bodySpace;
	TextString lowerText;
	TextString sellText;
	std::string sellCostText;
	int textWidth;
	int insertHeight;
	int titleInsert;
};

struct LocationEvent;
struct EventGenerator;

struct EventGenerator
{
	LIBZHL_API LocationEvent *GetBaseEvent(std::string &name, int unk1, char load, int unk3);
	
};

struct BombProjectile
{
};

struct EffectsBlueprint
{
	std::vector<std::string> launchSounds;
	std::vector<std::string> hitShipSounds;
	std::vector<std::string> hitShieldSounds;
	std::vector<std::string> missSounds;
	std::string image;
};

struct OuterHull
{
};

struct ItemStoreBox
{
};

struct CrewCustomizeBox
{
};

struct SettingValues
{
};

struct EventDamage
{
	int _sil__DO_NOT_USE_system;
	int amount;
	int effect;
};

struct AugmentStoreBox
{
};

struct RockAlien
{
};

struct StatusEffect
{
};

struct ChoiceReq
{
	std::string object;
	int min_level;
	int max_level;
	int max_group;
	bool blue;
};

struct ShipTemplate
{
	std::string shipEventName;
	bool hostile;
};

struct ResourcesTemplate
{
	RandomAmount fuel;
	RandomAmount drones;
	RandomAmount missiles;
	RandomAmount scrap;
	RandomAmount items;
	int crew;
	std::string crewType;
	TextString crewName;
	bool cloneable;
	TextString cloneText;
	std::vector<std_pair_int_int> crewSkills;
	std::string weapon;
	std::string drone;
	std::string augment;
	std::string _sil__DO_NOT_USE_system;
	bool steal;
	int upgradeAmount;
	int upgradeId;
	std::string removeItem;
};

struct EventTemplate
{
	struct ChoiceTemplate
	{
		TextString text;
		bool loadText;
		std::string eventName;
		bool hiddenReward;
		ChoiceReq requirement;
	};
	
	ResourcesTemplate resources;
	RewardDesc reward;
	TextString text;
	bool loadText;
	bool unique;
	ShipTemplate shipTemplate;
	std::string eventName;
	std::vector<EventDamage> damage;
	int environmentFlag;
	int environmentTarget;
	bool store;
	bool repair;
	bool reveal_map;
	std::vector<StatusEffect> statusEffects;
	bool distressBeacon;
	BoardingEvent boarders;
	int modifyPursuit;
	std::vector<ChoiceTemplate> choices;
	std::vector<std_pair_std_string_std_string> nameDefinitions;
	std::string imageBackground;
	std::string imagePlanet;
	std::string quest;
	TextString unlockShipText;
	int unlockShip;
	int fleet;
	bool secretSector;
};

struct WeaponEquipBox
{
};

struct Projectile : Collideable
{
	Targetable _targetable;
	Pointf position;
	Pointf last_position;
	float speed_magnitude;
	Pointf target;
	float heading;
	int ownerId;
	unsigned int selfId;
	Damage damage;
	float lifespan;
	int destinationSpace;
	int currentSpace;
	int targetId;
	bool dead;
	Animation death_animation;
	Animation flight_animation;
	Pointf speed;
	bool missed;
	bool hitTarget;
	std::string hitSolidSound;
	std::string hitShieldSound;
	std::string missSound;
	float entryAngle;
	bool startedDeath;
	bool passedTarget;
	bool bBroadcastTarget;
	AnimationTracker flashTracker;
	GL_Color color;
};

struct MenuScreen : FocusWindow
{
	GL_Texture *mainImage;
	GL_Primitive *menuPrimitive;
	int menuWidth;
	std::vector<TextButton*> buttons;
	int command;
	std::vector<int> commands;
	Point position;
	ConfirmWindow confirmDialog;
	int tempCommand;
	GenericButton *saveQuit;
	bool bShowControls;
	Point statusPosition;
	GL_Texture *difficultyBox;
	int difficultyWidth;
	std::string difficultyLabel;
	std::string difficultyText;
	GL_Texture *dlcBox;
	int dlcWidth;
	std::string dlcLabel;
	std::string dlcText;
	GL_Texture *achBox;
	GL_Primitive *achBoxPrimitive;
	int achWidth;
	std::string achLabel;
	std::vector<ShipAchievementInfo> shipAchievements;
	int selectedAch;
	InfoBox info;
};

struct CrewEquipBox;

struct CrewManifest : FocusWindow
{
	GL_Primitive *box;
	DropBox overBox;
	ShipManager *shipManager;
	std::vector<CrewEquipBox*> crewBoxes;
	InfoBox infoBox;
	int confirmingDelete;
	ConfirmWindow deleteDialog;
};

struct EnergyAnimation
{
};

struct SystemBlueprint
{
};

struct Spreadable
{
};

struct SystemBox
{
};

struct MantisAlien
{
};

struct DefenseDrone
{
};

struct LocationEvent
{
	struct Choice
	{
		LocationEvent *event;
		TextString text;
		ChoiceReq requirement;
		bool hiddenReward;
	};
	
};

struct AugmentBlueprint
{
};

struct FileHelper
{
};

struct RockAnimation
{
};

struct TeleportSystem : ShipSystem
{
	float chargeLevel;
	bool bCanSend;
	bool bCanReceive;
	int iArmed;
	std::vector<bool> crewSlots;
	char unk[8];
	int iPreparedCrew;
	int iNumSlots;
	bool bSuperShields;
};

struct CAchievement;

struct ShipButton : Button
{
	ShipButton(int x, int y)
	{
		this->constructor(x, y);
	}

	LIBZHL_API void constructor(int shipType, int shipVariant);
	LIBZHL_API void OnInit(std::string &unk, Point pos);
	LIBZHL_API bool MouseMove(int x, int y);
	LIBZHL_API void OnRender();
	
	GL_Texture *iShipImage;
	bool bShipLocked;
	bool bLayoutLocked;
	bool bNoExist;
	std::vector<CAchievement*> achievements;
	int iSelectedAch;
	bool bSelected;
};

struct DroneBox
{
};

struct HandAnimation
{
	GL_Texture *hand;
	Point start;
	Point finish;
	Pointf location;
	bool bRunning;
	float pause;
};

struct WeaponControl;

struct Targetable;

struct WeaponControl : ArmamentControl
{
	LIBZHL_API void Fire(std::vector<Pointf> &points, int target, bool autoFire);
	
	Targetable *currentTarget;
	ProjectileFactory *armedWeapon;
	bool autoFiring;
	TextButton autoFireButton;
	GL_Primitive *autoFireBase;
	GL_Primitive *targetIcon[4];
	GL_Primitive *targetIconYellow[4];
	Pointf autoFireFocus;
	WarningMessage missileMessage;
	WarningMessage systemMessage;
	int armedSlot;
};

struct SystemBox;

struct CombatControl
{
	LIBZHL_API void RenderTarget();
	LIBZHL_API ShipManager *GetCurrentTarget();
	LIBZHL_API char CurrentTargetIsBoss();
	LIBZHL_API char CanTargetSelf();
	
	CommandGui *gui;
	ShipManager *shipManager;
	Point playerShipPosition;
	SpaceManager *space;
	WeaponControl weapControl;
	DroneControl droneControl;
	std::vector<SystemBox*> sysBoxes;
	std::vector<CompleteShip*> enemyShips;
	CompleteShip *currentTarget;
	SpaceDrone *currentDrone;
	Point position;
	int selectedRoom;
	int selectedSelfRoom;
	Point targetPosition;
	Point boxPosition;
	WindowFrame *hostileBoxFrame;
	CachedImage healthMask;
	CachedImage shieldCircleCharged[5];
	CachedImage shieldCircleUncharged[5];
	CachedImage shieldCircleHacked[5];
	CachedImage shieldCircleHackedCharged[5];
	CachedImage shieldChargeBox;
	CachedImage superShieldBox5;
	CachedImage superShieldBox12;
	bool open;
	float shipIconSize;
	Pointf potentialAiming;
	std::vector<Pointf> aimingPoints;
	Pointf lastMouse;
	bool mouseDown;
	bool isAimingTouch;
	bool movingBeam;
	Point beamMoveLast;
	bool invalidBeamTouch;
	Point screenReposition;
	std::pair<int, int> teleportCommand;
	int iTeleportArmed;
	CachedImage teleportTarget_send;
	CachedImage teleportTarget_return;
	CachedImage hackTarget;
	CachedImage mindTarget;
	AnimationTracker ftl_timer;
	WarningMessage ftlWarning;
	AnimationTracker hacking_timer;
	std::vector<std::string> hackingMessages;
	bool boss_visual;
	bool bTeachingBeam;
	WindowFrame *tipBox;
	HandAnimation hand;
};

struct UpgradeBox
{
};

struct SystemControl
{
	ShipManager *shipManager;
	CombatControl *combatControl;
	std::vector<SystemBox*> sysBoxes;
	Globals::Rect SystemPower;
	bool bSystemPowerHover;
	Point position;
	Point systemPowerPosition;
	Point subSystemPosition;
	GL_Primitive *wiresImage;
	GL_Primitive *wiresMask;
	GL_Primitive *noButton;
	GL_Primitive *button;
	GL_Primitive *noButton_cap;
	GL_Primitive *button_cap;
	GL_Primitive *drone;
	GL_Primitive *drone3;
	GL_Primitive *drone2;
	GL_Primitive *sub_box;
	int sub_spacing;
	WarningMessage *notEnoughPower;
	AnimationTracker flashBatteryPower;
	AnimationTracker flashTracker;
};

struct DroneBlueprint
{
};

struct SoundControl;

struct SoundControl
{
	LIBZHL_API int PlaySoundMix(std::string &soundName, float volume, char loop);
	
};

struct StarMap;

struct ShipStatus;

struct WarningWithLines;

struct ShipStatus
{
	LIBZHL_API void RenderHealth(bool unk);
	
	Point location;
	float size;
	ShipManager *ship;
	CombatControl *combat;
	GL_Primitive *hullBox;
	GL_Primitive *hullBox_Red;
	GL_Primitive *shieldBox_On;
	GL_Primitive *shieldBox_Off;
	GL_Primitive *shieldBox_Red;
	GL_Primitive *shieldCircleCharged[4];
	GL_Primitive *shieldCircleUncharged[4];
	GL_Primitive *shieldCircleHacked[4];
	GL_Primitive *shieldCircleHackedCharged[4];
	GL_Primitive *energyShieldBox;
	GL_Primitive *energyShieldBar[5];
	GL_Texture *hullLabel;
	GL_Texture *hullLabel_Red;
	GL_Primitive *shieldBoxPurple;
	GL_Primitive *oxygenPurple;
	GL_Primitive *evadePurple;
	GL_Primitive *evadeOxygenBox;
	GL_Primitive *evadeOxygenBox_topRed;
	GL_Primitive *evadeOxygenBox_bottomRed;
	GL_Primitive *evadeOxygenBox_bothRed;
	GL_Primitive *fuelIcon;
	GL_Primitive *missilesIcon;
	GL_Primitive *dronesIcon;
	GL_Primitive *scrapIcon;
	GL_Primitive *fuelIcon_red;
	GL_Primitive *missilesIcon_red;
	GL_Primitive *dronesIcon_red;
	GL_Primitive *scrapIcon_red;
	GL_Primitive *healthMask;
	GL_Texture *healthMaskTexture;
	int lastHealth;
	Globals::Ellipse baseShield;
	int currentHover;
	Point evadeOxygenBoxLocation;
	int lastFuel;
	int lastDrones;
	int lastScrap;
	int lastMissiles;
	int lastHull;
	WarningWithLines *hullMessage;
	WarningMessage *shieldMessage;
	WarningMessage *oxygenMessage;
	WarningMessage *boardingMessage;
	std::vector<DamageMessage*> resourceMessages;
	AnimationTracker noMoneyTracker;
	AnimationTracker flashTracker;
	bool bBossFight;
	bool bEnemyShip;
	Point noShipShift;
	Point intruderShift;
	Point energyShieldPos;
	Point intruderPos;
};

struct SpaceStatus
{
	GL_Primitive *warningImages[10];
	WarningMessage *warningMessage;
	WarningMessage *incomingFire;
	Globals::Rect hitbox;
	Globals::Rect hitbox2;
	int currentEffect;
	int currentEffect2;
	SpaceManager *space;
	Point position;
	int touchedTooltip;
};

struct FocusWindow;

struct GameOver : FocusWindow
{
	std::vector<TextButton*> buttons;
	GL_Primitive *box;
	int boxWidth;
	int command;
	std::vector<int> commands;
	bool bShowStats;
	Point position;
	std::string gameoverText;
	bool bVictory;
	float openedTimer;
	CreditScreen credits;
	bool bShowingCredits;
};

struct Equipment;
struct AugmentEquipBox;

struct Equipment : FocusWindow
{
	LIBZHL_API void OnInit(ShipManager *ship);
	
	GL_Texture *box;
	GL_Texture *storeBox;
	DropBox overBox;
	DropBox overAugImage;
	DropBox sellBox;
	bool bSellingItem;
	ShipManager *shipManager;
	std::vector<EquipmentBox*> vEquipmentBoxes;
	std::vector<ProjectileFactory*> weaponsTrashList;
	EquipmentBox *overcapacityBox;
	AugmentEquipBox *overAugBox;
	int selectedEquipBox;
	int draggingEquipBox;
	int potentialDraggingBox;
	bool bDragging;
	Point firstMouse;
	Point currentMouse;
	Point dragBoxCenter;
	Point dragBoxOffset;
	InfoBox infoBox;
	std::string sellCostText;
	bool bOverCapacity;
	bool bOverAugCapacity;
	bool bStoreMode;
	int cargoId;
	Point infoBoxLoc;
};

struct TabbedWindow : FocusWindow
{
	std::vector<Button*> buttons;
	std::vector<FocusWindow*> windows;
	std::vector<std::string> names;
	unsigned int currentTab;
	int buttonType;
	TextButton doneButton;
	Point move;
	bool bBlockClose;
	bool bTutorialMode;
	bool bWindowLock;
};

struct UpgradeBox;

struct ReactorButton : Button
{
	int tempUpgrade;
	ShipManager *ship;
	bool selected;
};

struct Upgrades : FocusWindow
{
	GL_Texture *box;
	std::vector<UpgradeBox*> vUpgradeBoxes;
	ShipManager *shipManager;
	TextButton undoButton;
	ReactorButton reactorButton;
	InfoBox infoBox;
	Point infoBoxLoc;
	int systemCount;
	int forceSystemInfoWidth;
};

struct Location;
struct Store;

struct CommandGui
{
	LIBZHL_API void RunCommand(std::string &command);
	LIBZHL_API Store *CreateNewStore(int sectorNumber);
	LIBZHL_API void *GetWorldCoordinates(Point point, bool fromTarget);
	
	ShipStatus shipStatus;
	CrewControl crewControl;
	SystemControl sysControl;
	CombatControl combatControl;
	FTLButton ftlButton;
	SpaceStatus spaceStatus;
	StarMap *starMap;
	CompleteShip *shipComplete;
	std::vector<FocusWindow*> focusWindows;
	Point pauseTextLoc;
	GL_Primitive *pauseImage;
	GL_Primitive *pauseImage2;
	GL_Primitive *pauseImageAuto;
	GL_Primitive *pauseCrewImage;
	GL_Primitive *pauseDoorsImage;
	GL_Primitive *pauseHackingImage;
	GL_Primitive *pauseMindImage;
	GL_Primitive *pauseRoomImage;
	GL_Primitive *pauseTargetImage;
	GL_Primitive *pauseTargetBeamImage;
	GL_Primitive *pauseTeleportLeaveImage;
	GL_Primitive *pauseTeleportArriveImage;
	GL_Primitive *flareImage;
	Point shipPosition;
	std::string locationText;
	std::string loadEvent;
	int loadSector;
	ChoiceBox choiceBox;
	bool gameover;
	bool alreadyWon;
	bool outOfFuel;
	MenuScreen menuBox;
	GameOver gameOverScreen;
	OptionsScreen optionsBox;
	bool bPaused;
	bool bAutoPaused;
	bool menu_pause;
	bool event_pause;
	bool touch_pause;
	int touchPauseReason;
	InputBox inputBox;
	float fShakeTimer;
	TabbedWindow shipScreens;
	TabbedWindow storeScreens;
	Upgrades upgradeScreen;
	CrewManifest crewScreen;
	Equipment equipScreen;
	Location *newLocation;
	SpaceManager *space;
	Button upgradeButton;
	WarningMessage upgradeWarning;
	TextButton storeButton;
	Button optionsButton;
	const float pause_anim_time;
	float pause_animation;
	std::vector<Store*> storeTrash;
	TimerHelper flickerTimer;
	TimerHelper showTimer;
	bool bHideUI;
	CompleteShip *enemyShip;
	bool waitLocation;
	bool lastLocationWait;
	bool dangerLocation;
	std::vector<int> commandKey;
	bool jumpComplete;
	int mapId;
	ConfirmWindow leaveCrewDialog;
	bool secretSector;
	int activeTouch;
	bool activeTouchIsButton;
	bool activeTouchIsCrewBox;
	bool activeTouchIsShip;
	bool activeTouchIsNull;
	std::vector<int> extraTouches;
	bool bTutorialWasRunning;
	bool focusAteMouse;
	bool choiceBoxOpen;
	int systemDetailsWidth;
	ChoiceBox writeErrorDialog;
	bool suppressWriteError;
};

struct TextLibrary;

struct TextLibrary
{
	LIBZHL_API static std::string &__stdcall GetText(std::string &strRef, TextLibrary *textLibrary, std::string &name, std::string &lang);
	
	std::map<std::string, std::string> dictionary;
	std::map<std::string, std_map_std_string_std_string> languageDictionaries;
	std::string currentLanguage;
};

struct DroneEquipBox
{
};

struct PDSFire
{
};

struct IonDrone
{
};

struct CrewEquipBox
{
	LIBZHL_API int RemoveItem();
	
};

struct MedbaySystem
{
};

struct ProjectileFactory : ShipObject
{
	LIBZHL_API static int __stdcall StringToWeapon(std::string &str);
	LIBZHL_API void Fire(std::vector<Pointf> &points, int target);
	LIBZHL_API char FireNextShot();
	LIBZHL_API void constructor(const WeaponBlueprint *bp, int shipId);
	LIBZHL_API int SelectChargeGoal();
	LIBZHL_API bool IsChargedGoal();
	LIBZHL_API void SetHacked(int hackLevel);
	LIBZHL_API void SetCurrentShip(Targetable *target);
	LIBZHL_API void SetCooldownModifier(float cooldownModifier);
	
	std::pair<float, float> cooldown;
	std::pair<float, float> subCooldown;
	float baseCooldown;
	const WeaponBlueprint *blueprint;
	Point localPosition;
	Animation flight_animation;
	bool autoFiring;
	bool fireWhenReady;
	bool powered;
	int requiredPower;
	std::vector<Pointf> targets;
	std::vector<Pointf> lastTargets;
	int targetId;
	int iAmmo;
	std::string name;
	int numShots;
	float currentFiringAngle;
	float currentEntryAngle;
	Targetable *currentShipTarget;
	CloakingSystem *cloakingSystem;
	WeaponAnimation weaponVisual;
	WeaponMount mount;
	std::vector<Projectile*> queuedProjectiles;
	int iBonusPower;
	bool bFiredOnce;
	int iSpendMissile;
	float cooldownModifier;
	int shotsFiredAtTarget;
	int radius;
	int boostLevel;
	int lastProjectileId;
	int chargeLevel;
	int iHackLevel;
	int goalChargeLevel;
	bool isArtillery;
};

struct Sector;

struct Sector
{
	int type;
	bool visited;
	bool reachable;
	std::vector<Sector*> neighbors;
	Point location;
	int level;
	SectorDescription description;
};

struct DroneSystem
{
};

struct WorldManager;

struct WindowFrame
{
	Globals::Rect rect;
	GL_Primitive *outline;
	GL_Primitive *mask;
	GL_Primitive *pattern;
};

struct DistressButton : TextButton
{
	TextString labels[2];
	bool state;
};

struct Location
{
	Pointf loc;
	std::vector<Location*> connectedLocations;
	bool beacon;
	bool known;
	int visited;
	bool dangerZone;
	bool newSector;
	bool nebula;
	bool boss;
	LocationEvent *event;
	ImageDesc planet;
	ImageDesc space;
	ImageDesc beaconImage;
	GL_Texture *imageId;
	bool questLoc;
	AnimationTracker flashTracker;
	bool fleetChanging;
	std::string planetImage;
	std::string spaceImage;
};

struct StarMap : FocusWindow
{
	struct NebulaInfo
	{
		GL_Primitive *primitive;
		int x;
		int y;
		int w;
		int h;
	};
	
	LIBZHL_API void OnRender();
	LIBZHL_API void SetBossStage(int stage);
	
	float visual_size;
	std::vector<Location*> locations;
	std::map<Point, Location*> locations_grid;
	std::vector<Location*> temp_path;
	Location *currentLoc;
	Location *potentialLoc;
	Location *hoverLoc;
	Point position;
	int iPopulatedTiles;
	int iLocationCount;
	int iEmptyTiles;
	bool bInitializedDisplay;
	Pointf translation;
	bool readyToTravel;
	Point dangerZone;
	float dangerZoneRadius;
	float shipRotation[2];
	TextButton endButton;
	TextButton waitButton;
	DistressButton distressButton;
	TextButton jumpButton;
	int worldLevel;
	bool bMapRevealed;
	int pursuitDelay;
	int sectorNameFont;
	WindowFrame mapBorder;
	GL_Primitive *mapBorderTitle;
	GL_Primitive *mapBorderTitleMask;
	GL_Texture *mapBorderSector;
	GL_Texture *mapInsetTextLeft;
	GL_Texture *mapInsetTextMiddle;
	GL_Texture *mapInsetTextRight;
	GL_Texture *mapInsetTextJump;
	GL_Texture *mapInsetWaitDistress;
	GL_Primitive *redLight;
	GL_Primitive *fuelMessage;
	GL_Primitive *waitingMessage;
	GL_Primitive *unexplored;
	GL_Primitive *explored;
	GL_Primitive *danger;
	GL_Primitive *warning;
	GL_Primitive *yellow_warning;
	GL_Primitive *warning_circle;
	GL_Primitive *nebula_circle;
	GL_Texture *boxGreen[3];
	GL_Texture *boxPurple[3];
	GL_Texture *boxWhite[3];
	GL_Primitive *ship;
	GL_Primitive *shipNoFuel;
	GL_Primitive *bossShip;
	GL_Primitive *dangerZoneEdge;
	GL_Texture *dangerZoneTile;
	GL_Primitive *dangerZoneAdvance;
	GL_Primitive *targetBox;
	GL_Primitive *sectorTargetBox_green;
	GL_Primitive *sectorTargetBox_yellow;
	AnimationTracker targetBoxTimer;
	TextButton closeButton;
	WindowFrame *descBox;
	GL_Primitive *shadow;
	GL_Primitive *warning_shadow;
	GL_Primitive *fuelOverlay;
	GL_Primitive *dangerFlash;
	GL_Primitive *mapsBottom[3];
	GL_Texture *dottedLine;
	GL_Texture *cross;
	GL_Texture *boss_jumps_box;
	std::vector<ImageDesc> smallNebula;
	std::vector<ImageDesc> largeNebula;
	std::vector<NebulaInfo> currentNebulas;
	ShipManager *shipManager;
	bool outOfFuel;
	AnimationTracker waiting;
	int dangerWaitStart;
	AnimationTracker distressAnim;
	bool bTutorialGenerated;
	std::vector<std::string> delayedQuests;
	std::vector<Sector*> sectors;
	Sector *currentSector;
	Sector *secretSector;
	bool bChoosingNewSector;
	bool bSecretSector;
	Location dummyNewSector;
	int mapsAnalyzed;
	int locationsCreated;
	int shipsCreated;
	std::map<std::string, int> scrapCollected;
	std::map<std::string, int> dronesCollected;
	std::map<std::string, int> fuelCollected;
	std::map<std::string, int> weaponFound;
	std::map<std::string, int> droneFound;
	int bossLoc;
	int arrivedAtBase;
	bool reversedPath;
	bool bossJumping;
	std::vector<Location*> boss_path;
	bool bossLevel;
	bool boss_wait;
	Pointf bossPosition;
	std::string forceSectorChoice;
	bool bEnemyShip;
	bool bNebulaMap;
	bool bInfiniteMode;
	std::vector<Sector*> lastSectors;
	TextButton closeSectorButton;
	int sectorMapSeed;
	int currentSectorSeed;
	int fuelEventSeed;
	std::string lastEscapeEvent;
	bool waitedLast;
	std::vector<Store*> storeTrash;
	std::vector<std_pair_std_string_int> addedQuests;
	int bossStage;
	TextString bossMessage;
	WarningMessage *bossJumpingWarning;
	bool crystalAlienFound;
	std::map<Location*, bool> foundMap;
	Point sectorMapOffset;
	int potentialSectorChoice;
	int finalSectorChoice;
	std::vector<Globals::Rect> sectorHitBoxes;
};

struct Scroller
{
	GL_Texture *imageId;
	int size_x;
	int size_y;
	int image_x;
	int image_y;
	float fSpeed;
	float current_x;
	bool bInitialized;
};

struct SpaceManager
{
	struct FleetShip
	{
		GL_Texture *image;
		Point location;
	};
	
	LIBZHL_API int SetPlanetaryDefense(char unk, int target);
	
	std::vector<Projectile*> projectiles;
	AsteroidGenerator asteroidGenerator;
	std::vector<ShipManager*> ships;
	std::vector<SpaceDrone*> drones;
	bool dangerZone;
	GL_Texture *currentBack;
	ImageDesc currentPlanet;
	CachedImage planetImage;
	ImageDesc fleetShip;
	GL_Texture *shipIds[8];
	FleetShip fleetShips[9];
	Scroller asteroidScroller[3];
	GL_Texture *sunImage;
	GL_Texture *sunGlow;
	AnimationTracker sunGlow1;
	AnimationTracker sunGlow2;
	AnimationTracker sunGlow3;
	bool sunLevel;
	bool pulsarLevel;
	GL_Texture *pulsarFront;
	GL_Texture *pulsarBack;
	GL_Texture *lowendPulsar;
	bool bPDS;
	int envTarget;
	Point shipPosition;
	float randomPDSTimer;
	std::vector<Projectile*> pdsQueue;
	TimerHelper flashTimer;
	AnimationTracker flashTracker;
	ImageDesc currentBeacon;
	ImageDesc currentBeaconFlash;
	AnimationTracker beaconTracker;
	bool flashSound;
	bool bNebula;
	bool bStorm;
	std::vector<NebulaCloud> nebulaClouds;
	GL_Texture *lowendNebula;
	GL_Texture *lowendStorm;
	GL_Texture *lowendSun;
	GL_Texture *lowendAsteroids;
	float shipHealth;
	bool gamePaused;
	TimerHelper pdsFireTimer;
	int pdsCountdown;
	std::vector<Animation> pdsSmokeAnims;
	bool queueScreenShake;
	bool playerShipInFront;
};

struct WorldManager
{
	LIBZHL_API void constructor();
	LIBZHL_API int OnInit();
	LIBZHL_API CompleteShip *__stdcall CreateShip(ShipEvent *shipEvent, bool boss);
	
	CompleteShip *playerShip;
	BossShip *bossShip;
	SpaceManager space;
	int currentDifficulty;
	std::vector<CompleteShip*> ships;
	StarMap starMap;
	CommandGui *commandGui;
	LocationEvent *baseLocationEvent;
	LocationEvent *lastLocationEvent;
	ShipEvent currentShipEvent;
	std::vector<StatusEffect> currentEffects;
	std::string startingText;
	Location *newLocation;
	bool bStartedGame;
	bool bLoadingGame;
	bool vAutoSaved;
	bool bExtraChoice;
	std::vector<int> choiceHistory;
	std::string generatedEvent;
	TextString lastMainText;
	int playerCrewCount;
	int killedCrew;
	int playerHull;
	std::vector<int> blueRaceChoices;
	int lastSelectedCrewSeed;
	bool testingBlueprints;
	std::vector<LocationEvent::Choice> originalChoiceList;
};

struct PowerManager
{
};

struct DoorBox
{
};

struct WarningWithLines : WarningMessage
{
	GL_Primitive *linePrimitive;
	Point textOrigin;
	TextString topText;
	TextString bottomText;
	int topTextLimit;
	int bottomTextLimit;
};

struct EngineSystem
{
};

struct CloneBox
{
};

struct ShipGenerator
{
};

struct CrystalAnimation
{
};

struct SlugAnimation
{
};

struct EngiAnimation
{
};

struct ToggleButton
{
};

struct TapBoxFrame
{
};

struct WeaponStoreBox
{
};

struct DroneStoreBox
{
};

struct CEvent
{
	enum TextEvent
	{
	  TEXT_CONFIRM = 0x0,
	  TEXT_CANCEL = 0x1,
	  TEXT_CLEAR = 0x2,
	  TEXT_BACKSPACE = 0x3,
	  TEXT_DELETE = 0x4,
	  TEXT_LEFT = 0x5,
	  TEXT_RIGHT = 0x6,
	  TEXT_HOME = 0x7,
	  TEXT_END = 0x8,
	};

	void *vptr;
};

struct Asteroid
{
};

struct SlugAlien
{
};

struct EngiAlien
{
};

struct CombatDrone
{
};

struct ParticleEmitter
{
};

struct SystemStoreBox
{
};

struct CloakingBox
{
};

struct Missile
{
};

struct IonDroneAnimation
{
};

struct Ghost
{
};

struct LaserBlast;

struct LaserBlast : Projectile
{
	LIBZHL_API void OnUpdate();
	
	Targetable *movingTarget;
	float spinAngle;
	float spinSpeed;
};

struct RepairAnimation
{
};

struct BoarderDrone
{
};

struct Algae
{
};

struct EventSystem
{
};

struct OxygenSystem : ShipSystem
{
	float max_oxygen;
	std::vector<float> oxygenLevels;
	float fTotalOxygen;
	bool bLeakingO2;
};

struct GL_Color;

struct CSurface
{
	LIBZHL_API static int __stdcall GL_SetColor(float r, float g, float b, float a);
	LIBZHL_API static GL_Color &__stdcall GL_GetColor();
	LIBZHL_API static void __stdcall GL_SetColorTint(float r, float g, float b, float a);
	LIBZHL_API static void __stdcall GL_RemoveColorTint();
	LIBZHL_API static GL_Color &__stdcall GetColorTint(GL_Color &ref);
	
};

struct PackageModuleInfo
{
};

struct ItemBlueprint
{
};

struct CrystalAlien
{
};

struct RandomAmount;
struct EventsParser;
struct EventTemplate;

struct EventsParser
{
	LIBZHL_API RandomAmount *PullMinMax(rapidxml::xml_node<char> *node, std::string &name);
	LIBZHL_API void **ProcessEvent(rapidxml::xml_node<char> *node, std::string &event, EventTemplate &eventref);
	LIBZHL_API static ShipEvent *__stdcall ProcessShipEvent(EventsParser *eventsParser, rapidxml::xml_node<char> *node);
	LIBZHL_API static bool ParseBoolean(std::string &str);
	
	std::unordered_map<std::string, EventTemplate*> eventTemplates;
	std::vector<EventTemplate*> trashList;
	std::unordered_map<std::string, ShipEvent> shipTemplates;
};

struct DamageMessage
{

	enum MessageType
	{
	  MISS = 0x0,
	  RESIST = 0x1,
	};


};

struct BattleDrone
{
};

struct TouchTooltip;

struct ArmamentBox
{
	void *vptr;
	std::vector<GL_Primitive*> background;
	GL_Primitive *emptyBackground;
	GL_Primitive *hoverHighlight;
	GL_Primitive *outline;
	GL_Primitive *emptyOutline;
	GL_Primitive *powerBarGlow[4];
	GL_Primitive *iconBackground;
	GL_Primitive *iconInsetBackground;
	GL_Primitive *icon;
	GL_Primitive *iconDoubleSize;
	std::string iconName;
	std::string iconBackgroundName;
	Point lastIconPos;
	Point location;
	int xOffset;
	Point largeIconOffset;
	Point nameOffset;
	int nameWidth;
	bool mouseHover;
	bool touchHover;
	bool touchHighlight;
	bool selected;
	int hotKey;
	int activeTouch;
	TouchTooltip *touchTooltip;
	Animation hackAnimation;
	GL_Primitive *touchButtonBorder;
	Globals::Rect touchButtonBorderRect;
	float touchButtonSlidePos;
	std::vector<GenericButton*> touchButtons;
	Globals::Rect touchButtonHitbox;
	GL_Color iconColor;
	bool droneVariation;
	bool bIoned;
};

struct TouchTooltip
{
};

struct MindBox
{
};

struct Settings
{
	LIBZHL_API static char __stdcall GetCommandConsole();
	
};

struct freetype
{
	struct font_data
	{
	};
	
	LIBZHL_API static int __stdcall easy_print(int fontData, float x, float y, std::string &text);
	LIBZHL_API static int __stdcall easy_printRightAlign(int fontData, float x, float y, std::string &text);
	LIBZHL_API static int __stdcall easy_printNewlinesCentered(int fontData, float x, float y, int width, std::string &text);
	LIBZHL_API static int __stdcall easy_printAutoNewlines(int fontData, float x, float y, int width, std::string &text);
	LIBZHL_API static int __stdcall easy_printCenter(int fontData, float x, float y, std::string &text);
	LIBZHL_API static int __stdcall easy_printAutoShrink(int fontData, float x, float y, int width, char unk, std::string &text);
	
};

struct HackingSystem
{
};

struct StoreBox
{
};

struct MantisAnimation
{
};

struct RepairStoreBox
{
};

struct WeaponBox : ArmamentBox
{
	ProjectileFactory *pWeapon;
	bool armed;
	bool armedForAutofire;
	float cooldownMax;
	float cooldownModifier;
	Point cooldownPoint;
	int cooldownWidth;
	int cooldownHeight;
	std::vector<GL_Primitive*> cooldownBox;
	GL_Primitive *cooldownBar;
	std::vector<CachedImage> chargeIcons;
	bool defaultAutofire;
	bool wasCharged;
};

struct MouseControl
{
	Point position;
	Point lastPosition;
	int aiming_required;
	int iTeleporting;
	int iMindControlling;
	bool bSellingStuff;
	bool valid;
	bool newHover;
	bool lastValid;
	int animateDoor;
	GL_Texture *validPointer;
	GL_Texture *invalidPointer;
	GL_Texture *selling;
	Animation openDoor;
	int tooltipFont;
	std::string tooltip;
	float tooltipTimer;
	bool bMoving;
	bool bHideMouse;
	GL_Texture *lastIcon;
	GL_Texture *lastAddition;
	bool bForceTooltip;
	std::string tooltipTitle;
	std::string lastTooltipText;
	int iHacking;
	int overrideTooltipWidth;
	Point staticTooltip;
};

struct WeaponSystemBox
{
};

struct ArtilleryBox
{
};

struct SpaceDrone
{
};

struct SuperShieldDrone
{
};

struct CloakingSystem
{
};

struct GL_FrameBuffer;

struct CApp;

struct CApp : CEvent
{
	LIBZHL_API int OnInputFocus();
	LIBZHL_API int OnInputBlur();
	LIBZHL_API int OnExit();
	LIBZHL_API void OnMButtonDown(int x, int y);
	LIBZHL_API void OnTextEvent(CEvent::TextEvent textEvent);
	LIBZHL_API void OnTextInput(int unk);
	LIBZHL_API int OnRequestExit();
	LIBZHL_API int OnMouseMove(int x, int y, int xdiff, int ydiff, bool holdingLMB, bool holdingRMB, bool holdingMMB);
	LIBZHL_API void OnRButtonUp(int x, int y);
	LIBZHL_API void OnLButtonUp(int x, int y);
	LIBZHL_API void OnRButtonDown(int x, int y);
	LIBZHL_API void OnLButtonDown(int x, int y);
	LIBZHL_API void OnLanguageChange();
	LIBZHL_API void OnKeyUp(SDLKey key);
	LIBZHL_API void OnKeyDown(SDLKey key);
	LIBZHL_API void constructor();
	LIBZHL_API void ParseArgs(int unk, const char **args);
	LIBZHL_API void OnLoop();
	LIBZHL_API int SetupWindow();
	LIBZHL_API void OnCleanup();
	LIBZHL_API void GenInputEvents();
	LIBZHL_API void OnSuspend();
	LIBZHL_API void OnResume();
	LIBZHL_API Pointf TranslateMouse(int x, int y);
	LIBZHL_API void UpdateFullScreen();
	LIBZHL_API int UpdateWindowSettings();
	LIBZHL_API void OnRender();
	LIBZHL_API int OnInit();
	LIBZHL_API int OnExecute();
	LIBZHL_API int OnEvent(void *inputEvent);
	
	bool Running;
	bool shift_held;
	CommandGui *gui;
	WorldManager *world;
	MainMenu menu;
	LanguageChooser langChooser;
	int screen_x;
	int screen_y;
	int modifier_x;
	int modifier_y;
	bool fullScreenLastState;
	bool minimized;
	bool minLastState;
	bool focus;
	bool focusLastState;
	bool steamOverlay;
	bool steamOverlayLastState;
	bool rendering;
	bool gameLogic;
	float mouseModifier_x;
	float mouseModifier_y;
	GL_FrameBuffer *framebuffer;
	bool fboSupport;
	int x_bar;
	int y_bar;
	bool lCtrl;
	bool useFrameBuffer;
	bool manualResolutionError;
	int manualResErrorX;
	int manualResErrorY;
	bool nativeFullScreenError;
	bool fbStretchError;
	char __NULL[2];
	char lastLanguage[24];
	char inputFocus;
	char useDirect3D;
};

struct HackingDrone
{
};

struct EnergyAlien
{
};

struct AnaerobicAlien
{
};

struct ResourceControl;
struct PackageModuleInfo;
struct ResourceManager;

struct ResourceControl
{
	enum ImageSwappingMode
	{
	  SWAP_NONE = 0x0,
	  SWAP_SHIP_SETS = 0x1,
	  SWAP_DYNAMIC_SHIPS = 0x2,
	};

	struct DynamicImageInfo
	{
		std::string name;
		int resid;
	};
	
	struct LogicalTexInfo
	{
		std::string physName;
		int x;
		int y;
		int w;
		int h;
	};
	
	LIBZHL_API void constructor();
	LIBZHL_API void CreateImagePrimitive(GL_Texture *tex, int unk1, int unk2, int unk3, GL_Color color, float unk4, char unk5);
	LIBZHL_API void OnInit(int imageSwappingMode);
	LIBZHL_API char *LoadFile(std::string &fileName);
	LIBZHL_API void *GetImageId(std::string &name);
	
	std::unordered_map<std::string, GL_Texture*> images;
	std::unordered_map<int, freetype::font_data> fonts;
	std::unordered_map<GL_Texture*, ImageDesc> imageData;
	std::pair<int, int> preLoadProgress;
	std::string nextImageLoaded;
	int imageSwappingMode;
	std::vector<std::string> playerShipImages;
	std::vector<std::string> hangarShipImages;
	std::vector<std::string> enemyShipImages;
	std::vector<DynamicImageInfo> dynamicImages;
	int dynamicLoadMark;
	std::unordered_map<std::string, LogicalTexInfo> atlasData;
	ResourceManager *resmgr;
	PackageModuleInfo *package;
	GL_Texture *loading_box;
	GL_Texture *loading_bar;
	int screenWidth;
	int screenHeight;
	int renderPointX;
	int renderPointY;
	GL_FrameBuffer *frameBuffer;
	int fbViewportX;
	int fbViewportY;
	int fbViewportW;
	int fbViewportH;
};

struct CFPS;

struct CFPS
{
	LIBZHL_API float GetSpeedFactor();
	
	float RunningTime;
	char _unk[4];
	double OldTime;
	double LastTime;
	float SpeedFactor;
	int NumFrames;
	int Frames;
	int speedLevel;
};

struct Moddable
{
};

struct ShipInfo;

struct ShipInfo
{
	LIBZHL_API char AddAugmentation(std::string &augment);
	
	std::map<std::string, int> augList;
	std::map<std::string, int> equipList;
	int augCount;
};

struct Door : CrewTarget
{
public:
	Point GetCenterPoint()
	{
		Point ret = Point(this->x, this->y);
		return ret;
	}
	Point GetEntryWay(int room)
	{
		Point pos1 = Point(this->x, this->y);
		Point pos2;

		if (this->bVertical)
		{
			pos2 = Point(pos1.x + 17, pos1.y);
			pos1 = Point(this->x - 17, pos1.y);
		}
		else
		{
			pos2 = Point(this->x, pos1.y + 17);
			pos1 = Point(this->x, this->y - 18);
		}

		auto shipInfo = ShipGraph::GetShipInfo(this->iShipId);

		if (room == shipInfo->GetSelectedRoom(pos2.x, pos2.y, false))
		{
			return pos2;
		}
		else
		{
			return pos1;
		}
	}

	LIBZHL_API void FakeClose();
	LIBZHL_API bool IsSealed(int shipId);
	LIBZHL_API void FakeOpen();
	
	Selectable _selectable;
	int iRoom1;
	int iRoom2;
	bool bOpen;
	int iBlast;
	bool bFakeOpen;
	int width;
	int height;
	GL_Primitive *outlinePrimitive;
	GL_Primitive *highlightPrimitive;
	Animation doorAnim;
	Animation doorAnimLarge;
	int iDoorId;
	int baseHealth;
	int health;
	AnimationTracker forcedOpen;
	AnimationTracker gotHit;
	int doorLevel;
	bool bIoned;
	float fakeOpenTimer;
	AnimationTracker lockedDown;
	float lastbase;
	int iHacked;
	int x;
	int y;
	bool bVertical;
};

struct CrewStoreBox
{
};

struct Shields
{
	LIBZHL_API void *CollisionReal(float x, float y, Damage damage, bool unk);
	
};

struct MindSystem
{
};

struct BatterySystem
{
};

struct WeaponBlueprint : Blueprint
{
	struct BoostPower
	{
		int type;
		float amount;
		int count;
	};
	
	struct MiniProjectile
	{
		std::string image;
		bool fake;
	};
	
	std::string typeName;
	Damage damage;
	int shots;
	int missiles;
	float cooldown;
	int power;
	int length;
	float speed;
	int miniCount;
	EffectsBlueprint effects;
	std::string weaponArt;
	std::string combatIcon;
	std::string explosion;
	int radius;
	std::vector<MiniProjectile> miniProjectiles;
	BoostPower boostPower;
	int drone_targetable;
	int spin;
	int chargeLevels;
	TextString flavorType;
	GL_Color color;
};

struct CrewBox
{
};

