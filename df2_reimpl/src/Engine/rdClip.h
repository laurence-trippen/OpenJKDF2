#ifndef _RDCLIP_H
#define _RDCLIP_H

#include "rdCanvas.h"

#define rdClip_Line2_ADDR (0x0046D6F0)
#define rdClip_CalcOutcode2_ADDR (0x0046DA10)
#define rdClip_Point3_ADDR (0x0046DA50)
#define rdClip_Line3_ADDR (0x0046DB10)
#define rdClip_Line3Ortho_ADDR (0x0046E480)
#define rdClip_Line3Project_ADDR (0x0046E9D0)
#define rdClip_Face3W_ADDR (0x0046EAF0)
#define rdClip_Face3WOrtho_ADDR (0x0046F420)
#define rdClip_Face3S_ADDR (0x0046FAE0)
#define rdClip_Face3SOrtho_ADDR (0x00470410)
#define rdClip_Face3GS_ADDR (0x00470AD0)
#define rdClip_Face3GSOrtho_ADDR (0x004716F0)
#define rdClip_Face3T_ADDR (0x00472090)
#define rdClip_Face3TOrtho_ADDR (0x00472DC0)
#define rdClip_Face3GT_ADDR (0x00473880	0000104C)
#define rdClip_Face3GTOrtho_ADDR (0x004748D0)
#define rdClip_SphereInFrustrum_ADDR (0x004756E0)

enum CLIP_OUTCODE
{
    CLIP_LEFT = 0x1,
    CLIP_RIGHT = 0x10,
    CLIP_BOTTOM = 0x100,
    CLIP_TOP = 0x1000
};

int rdClip_Line2(rdCanvas *canvas, signed int *pX1, signed int *pY1, signed int *pX2, signed int *pY2);
int rdClip_CalcOutcode2(rdCanvas *canvas, int x, int y);

#endif // _RDCLIP_H