#include "helpers.h"
#include <stdio.h>
#include <windows.h>

struct Point {
	float x;
	float y;
};
struct Rectangle {
	float x;
	float y;
	float width;
	float height;
};
struct RawFont {
	uint32_t sprId;
	uint8_t width1;
	uint8_t height1;
	uint8_t width2;
	uint8_t height2;
	uint8_t layoutParam2Num;
	uint8_t layoutParam2Div;
	uint8_t padding0a[0x02];
	int32_t fontmapId;
	float layoutParam2NumOverDiv;
	uint8_t padding14[0x04];
	uint64_t texWidthChars;
	int64_t dataBegin;
	int64_t dataEnd;
	int64_t dataCapacityEnd;
	uint8_t layoutParam1;
	uint8_t padding39[0x7];
};

struct FontInfo {
	uint32_t fontId;
	uint8_t padding04[0x4];
	struct RawFont *rawfont;
	uint16_t flag10;
	uint8_t padding12[0x02];
	float width1;
	float height1;
	float width2;
	float height2;
	float userSizeWidth;
	float userSizeHeight;
	float userSizeWidthMultiplier;
	float userSizeHeightMultiplier;
	float spacingWidth;
	float spacingHeight;
};

struct DrawParams {
	uint32_t colour;
	uint32_t fillColour;
	uint8_t clip;
	uint8_t unk09[0x3];
	float clipRectX;
	float clipRectY;
	float clipRectWidth;
	float clipRectHeight;
	uint32_t layer;
	uint32_t unk20;
	uint32_t unk24;
	uint32_t unk28;
	float textCurrentLocX;
	float textCurrentLocY;
	float lineOriginLocX;
	float lineOriginLocY;
	uint8_t padding3c[0x4];
	uint64_t lineLength;
	struct FontInfo *font;
	uint16_t unk50;
};

int32_t coolAet;
int32_t fineAet;
int32_t safeAet;
int32_t sadAet;
int32_t worstAet;

FUNCTION_PTR (void, __stdcall, DivaDrawText, 0x140198500,
			  struct DrawParams *drawParam, uint32_t flags, const char *text,
			  int64_t len);
FUNCTION_PTR (struct FontInfo *, __thiscall, GetFontInfoFromID, 0x140196510,
			  struct FontInfo *fontInfo, uint32_t id);
FUNCTION_PTR (void, __stdcall, FillRectangle, 0x140198D80,
			  struct DrawParams *drawParam, const struct Rectangle *rect);
FUNCTION_PTR (int32_t, __stdcall, DrawAet, 0x14013BE60, int32_t fileId,
			  uint32_t layer, uint32_t flags, const char *name,
			  const struct Point *loc, int32_t unk, const char *animation,
			  const char *animation2, float animationInTime,
			  float animationOutTime, const struct Point *scale,
			  const void *callback);
FUNCTION_PTR (void, __stdcall, DestroyAet, 0x14019D570, int32_t *aet);
HOOK (int64_t, __stdcall, Update, 0x140502CA0) {
	if (*(uint32_t *)0x140EDA82C != 13) {
		if (coolAet != 0 || fineAet != 0 || safeAet != 0 || sadAet != 0
			|| worstAet != 0) {
			DestroyAet (&coolAet);
			coolAet = 0;
			DestroyAet (&fineAet);
			fineAet = 0;
			DestroyAet (&safeAet);
			safeAet = 0;
			DestroyAet (&sadAet);
			sadAet = 0;
			DestroyAet (&worstAet);
			worstAet = 0;
		}
		return originalUpdate ();
	}

	void *ptr = (void *)0x140D0A8C0;
	// int32_t maxCombo = *(int32_t *)(ptr + 0x8);
	int32_t coolCount = *(int32_t *)(ptr + 0x20);
	int32_t fineCount = *(int32_t *)(ptr + 0x24);
	int32_t safeCount = *(int32_t *)(ptr + 0x28);
	int32_t sadCount = *(int32_t *)(ptr + 0x2C);
	int32_t worstCount = *(int32_t *)(ptr + 0x30);

	struct FontInfo fontInfo;
	fontInfo = *GetFontInfoFromID (&fontInfo, 0x11);
	struct Point location;
	location.x = 80;
	location.y = 160;
	struct DrawParams drawParam = { 0 };
	drawParam.colour = 0xC0000000;
	drawParam.fillColour = 0xC0000000;
	drawParam.layer = 0x18;
	drawParam.unk24 = 0xD;
	drawParam.textCurrentLocX = 150;
	drawParam.textCurrentLocY = 70;
	drawParam.font = &fontInfo;
	drawParam.unk50 = 0x25A1;

	struct Rectangle rect = { 0 };
	rect.x = 0;
	rect.y = 60;
	rect.width = 220;
	rect.height = 200;
	FillRectangle (&drawParam, &rect);

	drawParam.layer = 0x19;
	drawParam.colour = 0xFFFFFFFF;

	if (coolAet == 0 || fineAet == 0 || safeAet == 0 || sadAet == 0
		|| worstAet == 0) {
		coolAet = DrawAet (3, drawParam.layer, 0x20000, "value_text_cool01",
						   &location, 0, 0, 0, 0, 0, 0, 0);
		location.y += 40;

		fineAet = DrawAet (3, drawParam.layer, 0x20000, "value_text_fine01",
						   &location, 0, 0, 0, 0, 0, 0, 0);
		location.y += 40;

		safeAet = DrawAet (3, drawParam.layer, 0x20000, "value_text_safe",
						   &location, 0, 0, 0, 0, 0, 0, 0);
		location.y += 40;

		sadAet = DrawAet (3, drawParam.layer, 0x20000, "value_text_sad",
						  &location, 0, 0, 0, 0, 0, 0, 0);
		location.y += 40;

		worstAet = DrawAet (3, drawParam.layer, 0x20000, "value_text_worst",
							&location, 0, 0, 0, 0, 0, 0, 0);
	}
	char buf[32];

	sprintf (buf, "%d", coolCount);
	DivaDrawText (&drawParam, 0x1000, buf, 32);
	drawParam.textCurrentLocY += 40;

	sprintf (buf, "%d", fineCount);
	DivaDrawText (&drawParam, 0x1000, buf, 32);
	drawParam.textCurrentLocY += 40;

	sprintf (buf, "%d", safeCount);
	DivaDrawText (&drawParam, 0x1000, buf, 32);
	drawParam.textCurrentLocY += 40;

	sprintf (buf, "%d", sadCount);
	DivaDrawText (&drawParam, 0x1000, buf, 32);
	drawParam.textCurrentLocY += 40;

	sprintf (buf, "%d", worstCount);
	DivaDrawText (&drawParam, 0x1000, buf, 32);

	return originalUpdate ();
}

BOOL WINAPI
DllMain (HMODULE mod, DWORD cause, void *ctx) {
	if (cause != DLL_PROCESS_ATTACH)
		return 1;

	INSTALL_HOOK (Update);

	return 1;
}
