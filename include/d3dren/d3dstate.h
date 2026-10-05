// d3d.ren Direct3D 7 texture binding and render-state wrappers (owner: unit unk/100098d0, work package W4).
// Used by every drawing unit.  The device pointer itself comes from d3ddevice.h (W3).
//
// NAME: StageStateSet: Jupiter render_a/src/sys/d3d/d3d_draw.h (class StageStateSet: m_Stage, m_State, m_OldVal; the
// constructor Gets the old value, Sets the new one, the destructor restores it).
// NAME: d3d_DisableTexture (0x1000a27b, medium): Jupiter d3d_DisableTexture(stage): SetTexture(stage, NULL) if bound; defined in d3d_texture.h.
// Everything else keeps its Ghidra name (the roles are in the guess comments).
#ifndef __D3DREN_D3DSTATE_H__
#define __D3DREN_D3DSTATE_H__

#include "d3dren/d3ddevice.h"	// the DirectDraw/Direct3D 7 globals (g_pD3DDevice = IDirect3DDevice7 *, ...) and the DX headers

// GLOBAL: D3DREN 0x100528d8
extern int DAT_100528d8;	// guess: set by FUN_100099a9 (flag read by the world polygon draw code)

// GLOBAL: D3DREN 0x100617d8
extern RTextureBase *g_pBoundTextures[8];	// guess: the texture data currently bound on each device stage
// GLOBAL: D3DREN 0x100577a0
// NAME: g_CurFrameCode: Jupiter common_draw.cpp / names_proposal high (defined in sys/d3d/common_draw)
extern uint16 g_CurFrameCode;	// the current texture frame code (RenderStruct::IncCurTextureFrameCode)

// Binds the texture data of pTex on device stage nStage unless it is already there (FUN_10009ea5); returns 0 when pTex has none.
int d3d_SetLightmapTexture(WorldPoly *pTex, int nStage);

// d3d_DisableTexture (d3d_texture.h): unbinds the texture of device stage nStage.  FUN_1000a27b is the exe's out-of-line copy of it, which
// d3d_FullDrawScene calls (unit unk/100098d0 defines it as a wrapper of the inline).
void FUN_1000a27b(int nStage);

// Second (detail) texture stage helpers.
void FUN_1000a1c2(int nMode);	// guess: set up stage 1 for the detail pass (1 = modulate/add-signed, 2 = modulate alpha + add colour)
void FUN_1000a211(void);		// guess: disable stage 1 colour and alpha
void FUN_1000a23a(void);		// guess: stage 1 colour op = add-signed / modulate (DetailTextureAdd)
void FUN_1000a25e(void);		// guess: disable stage 1 colour op and unbind its texture

void FUN_100099a9(int nValue);	// guess: setter of DAT_100528d8
int FUN_100099b3(void);			// guess: getter of DAT_100528d8

// Sets a render state and restores the old value in the destructor (inline in the exe: no symbols).
// NAME: StateSet: Jupiter render_a/src/sys/d3d/d3d_draw.h class StateSet (m_State, m_OldVal; the constructor Gets the old value
// and Sets the new one, the destructor Sets the old one back); the d3d.ren code (d3d_TestAndDrawPS 0x1002970e) is that exactly.
// Added by package W6 (drawparticles_A/drawsprite units use it).
class StateSet
{
public:
	StateSet(D3DRENDERSTATETYPE state, uint32 val)
	{
		m_State = state;
		g_pD3DDevice->GetRenderState(state, (unsigned long *)&m_OldVal);
		g_pD3DDevice->SetRenderState(state, val);
	}

	~StateSet()
	{
		g_pD3DDevice->SetRenderState(m_State, m_OldVal);
	}

	D3DRENDERSTATETYPE	m_State;
	uint32				m_OldVal;
};

// Sets a texture stage state and restores the old value in the destructor.
class StageStateSet
{
public:
	StageStateSet(uint32 stage, D3DTEXTURESTAGESTATETYPE state, uint32 val);
	~StageStateSet();

	uint32						m_Stage;
	D3DTEXTURESTAGESTATETYPE	m_State;
	uint32						m_OldVal;
};

#endif
