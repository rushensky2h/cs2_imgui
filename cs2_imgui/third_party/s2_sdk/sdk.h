#pragma once

#include <string>
#include "framework.h"

template <typename T = PVOID>
inline constexpr T get_v_func(PVOID func_in, int index)
{
	return (*static_cast<T **>(func_in))[index];
}

using instantiate_interface_fn = void *(*)();

class interface_reg 
{
public:
    instantiate_interface_fn    m_create_fn_;
    const char                  *m_name_;
    interface_reg               *m_next_;
};

inline const interface_reg *get_interfaces(const char *library)
{
    const auto library_handle = GetModuleHandleA(library);

    const auto createinterface_symbol = reinterpret_cast<std::uintptr_t>(GetProcAddress(library_handle, "CreateInterface"));

    const uintptr_t interface_list = createinterface_symbol + *reinterpret_cast<int32_t *>(createinterface_symbol + 3) + 7;

    return *reinterpret_cast<interface_reg **>(interface_list);
}

template <typename T = void *>
T *get_interface(const char *library, const char *partial_version) 
{
    for (const auto *current = get_interfaces(library); current; current = current->m_next_)
    {
        if (std::string_view(current->m_name_).find(partial_version) != std::string_view::npos)
            return reinterpret_cast<T*>(current->m_create_fn_());
    }

    return nullptr;
}

class CInButtonStatePB
{
public:
	char pad_0000[24]; //0x0000
	uint32_t buttons; //0x0018
	char pad_001C[12]; //0x001C
}; //Size: 0x0030

class CMsgQAngle
{
public:
	char pad_0000[24]; //0x0000
	float pitch; //0x0018
	float yaw; //0x001C
	float roll;
	char pad_0020[40]; //0x0020
}; //Size: 0x0050

class CBaseUserCmdPB
{
public:
	char pad_0000[56]; //0x0000
	CInButtonStatePB *button_state; //0x0038
	CMsgQAngle *qangle; //0x0040
	float unk1; //0x0048
	float unk2; //0x004C
	float forwardmove; //0x0050
	float sidemove; //0x0054
	float unk3; //0x0058
	float unk4; //0x005C
	float unk5; //0x0060
	float unk6; //0x0064
	float unk7; //0x0068
	char pad_006C[12]; //0x006C
}; //Size: 120

class CUserCmd
{
public:
	char pad_0000[40]; //0x0000
	void *N00000135; //0x0028
	CBaseUserCmdPB *base_user_cmd_ptr; //0x0030
	char pad_0038[16]; //0x0038
	void *button_state_ptr; //0x0048
	uint32_t buttons; //0x0050
	char pad_0054[28]; //0x0054

	CBaseUserCmdPB *GetBaseUserCmd()
	{
		return *(CBaseUserCmdPB **)((uintptr_t)this + 0x30);
	}
};

typedef float Vector;

class CCSGOInput
{
public:
	char pad_0000[64]; //0x0000
	void *ptr; //0x0040
	char pad_0048[16752]; //0x0048
	bool N00000CCE; //0x41B8
	char pad_41B9[4]; //0x41B9
	bool in_thirdperson; //0x41BD
	char pad_41BE[2]; //0x41BE
	float N00000CCF; //0x41C0
	Vector thirdperson_angles; //0x41C4
	float N00000CD0; //0x41D0
	float N00000D23; //0x41D4
	char pad_41D8[624]; //0x41D8

	CUserCmd *GetUserCmd()
	{
		auto sequence_number = *(int *)((uintptr_t)this + 0x41E0);
		auto cmd_array = (CUserCmd *)((uintptr_t)this + 0x10);
		return &cmd_array[sequence_number % 150];
	}

};

class CSwapChainDx11
{
public:
    char                __pad[0x178];
    IDXGISwapChain      *pSwapChain;
};

class CGlobalVars
{
public:
    float m_realtime; //0x0000 
    __int32 m_framecount; //0x0004 
    float N0000007D; //0x0008 
    float N00000087; //0x000C 
    __int32 m_maxclients; //0x0010 
    float m_intervalpertick; //0x0014 
    __int32 N0000007F; //0x0018 
    __int32 N0000008B; //0x001C 
    void *m_unkfunc; //0x0020 
    float N00000081; //0x0028 
    float m_curtime; //0x002C 
    float m_curtime2; //0x0030 
    char pad_0x0034[0xC]; //0x0034
    __int32 m_tickcount; //0x0040 
    float m_intervalpertick2; //0x0044 
    void *m_current_netchan; //0x0048 
    char pad_0x0050[0x130]; //0x0050
    char *m_current_map; //0x0180 
    char *m_current_mapname; //0x0188 
};

class IHandleEntity {
    char pad_0[0x8];
}; // Size: 0x8

class CEntityInstance : public IHandleEntity {
    char pad_8[0x28];
}; // Size: 0x30

class C_BaseEntity : public CEntityInstance {
public:
    char pad_30[0x2e8];
    int32_t m_iMaxHealth; // 0x318
    int32_t m_iHealth; // 0x31c
    uint8_t m_lifeState; // 0x320
    char pad_321[0x87];
    float m_flSpeed; // 0x3a8
    char pad_3ac[0x3];
    uint8_t m_iTeamNum; // 0x3af
    char pad_3b0[0x8];
    uint32_t m_fFlags; // 0x3b8
    char pad_3bc[0x17c];
}; // Size: 0x538

class CBasePlayerController : public C_BaseEntity {
public:
    char pad_538[0xd0];
    char m_iszPlayerName[128]; // 0x608
    char pad_688[0x8];
    uint64_t m_steamID; // 0x690
    bool m_bIsLocalPlayerController; // 0x698
    char pad_699[0x3];
    uint32_t m_iDesiredFOV; // 0x69c
}; // Size: 0x6a0

class CCSPlayerController : public CBasePlayerController {
public:
    char pad_6a0[0x30];
    uint32_t m_iPing; // 0x6d0
    char pad_6d4[0x2c];
    // Metadata: utf8*
    char *m_sSanitizedPlayerName; // 0x700
    char pad_708[0xd8];
    uint32_t m_iPawnHealth; // 0x7e0
    int32_t m_iPawnArmor; // 0x7e4
    bool m_bPawnHasDefuser; // 0x7e8
    bool m_bPawnHasHelmet; // 0x7e9
    char pad_7ea[0x3e];
}; // Size: 0x828

#include "misc/memalloc.h"

#include "common/CUtlMap.h"
#include "common/CUtlMemory.h"
#include "common/CUtlString.h"
#include "common/CUtlTSHash.h"
#include "common/CUtlVector.h"

#include "interfaces/schemasystem.h"

class IVEngineClient
{
public:
};