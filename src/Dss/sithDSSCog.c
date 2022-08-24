#include "sithDSSCog.h"

#include "Cog/sithCog.h"
#include "Cog/sithCogVm.h"
#include "World/sithThing.h"

int sithDSSCog_SendSendTrigger(sithCog *a1, int a2, int a3, int a4, int a5, int a6, int a7, float param0, float param1, float param2, float param3, int a11)
{
    int v12; // edi
    sithThing *v13; // eax
    sithThing *v14; // eax
    
    NETMSG_START;

    NETMSG_PUSHS32(a7);
    v12 = 1;
    NETMSG_PUSHS16(a1 ? a1->selfCog : -1);
    NETMSG_PUSHU8(a3);
    NETMSG_PUSHU8(a5);

    int a,b;

    if ( a3 == 3 && (v13 = sithThing_GetThingByIdx(a4)) != 0 ) {
        a = v13->thingIdx;
        NETMSG_PUSHS32(v13->thing_id);
    }
    else {
        a = a4;
        NETMSG_PUSHS32(a4);
    }

    if ( a5 == 3 && (v14 = sithThing_GetThingByIdx(a6)) != 0 ) {
        b = v14->thing_id;
        NETMSG_PUSHS32(v14->thing_id);
    }
    else {
        b = a6;
        NETMSG_PUSHS32(a6);
    }

    NETMSG_PUSHS16(a2 & 0xFF);
    
    NETMSG_PUSHF32(param0);
    NETMSG_PUSHF32(param1);
    NETMSG_PUSHF32(param2);
    NETMSG_PUSHF32(param3);

    //printf("%x %x %x %x %x %x %x, %f %f %f %f\n", a7, a1->selfCog, a3, a5, a, b, a2, param0, param1, param2, param3);
    
    NETMSG_END(COGMSG_SENDTRIGGER);

    if ( a2 == SITH_MESSAGE_TOUCHED )
        v12 = 0;
    return sithCogVm_SendMsgToPlayer(&sithCogVm_netMsgTmp, a11, 1, v12);
}

int sithDSSCog_HandleSendTrigger(sithCogMsg *in_netMsg)
{
    int senderType; // edi
    int senderIdx; // ebx
    int sourceIndex; // ebp
    sithThing *v6; // eax
    sithThing *v7; // eax
    int msgid; // eax
    sithThing *v12; // eax
    float param3; // [esp+10h] [ebp-1Ch]
    float param2; // [esp+14h] [ebp-18h]
    float param1; // [esp+18h] [ebp-14h]
    float param0; // [esp+1Ch] [ebp-10h]
    sithCog *cog; // [esp+20h] [ebp-Ch]
    int linkId; // [esp+28h] [ebp-4h]
    int sourceType; // [esp+30h] [ebp+4h]
    
    NETMSG_IN_START(in_netMsg);

    linkId = NETMSG_POPS32();
    cog = sithCog_GetByIdx(NETMSG_POPS16());
    senderType = NETMSG_POPU8();
    sourceType = NETMSG_POPU8();
    senderIdx = NETMSG_POPS32();
    sourceIndex = NETMSG_POPS32();
    if ( senderType == SENDERTYPE_THING )
    {
        v6 = sithThing_GetById(senderIdx);
        if ( v6 )
            senderIdx = v6->thingIdx;
    }
    if ( sourceType == SENDERTYPE_THING )
    {
        v7 = sithThing_GetById(sourceIndex);
        if ( v7 )
            sourceIndex = v7->thingIdx;
    }
    msgid = NETMSG_POPS16() & 0xFF;
    param0 = NETMSG_POPF32();
    param1 = NETMSG_POPF32();
    param2 = NETMSG_POPF32();
    param3 = NETMSG_POPF32();
    //printf("%x %x %x %x %x %x %x, %f %f %f %f\n", linkId, cog->selfCog, senderType, sourceType, senderIdx, sourceIndex, msgid, param0, param1, param2, param3);
    if ( !cog )
    {
        sithCog_SendMessageToAll(msgid, senderType, senderIdx, sourceType, sourceIndex, param0, param1, param2, param3);
        return 1;
    }
    if ( senderType == SENDERTYPE_THING )
    {
        v12 = sithThing_GetThingByIdx(senderIdx);
        if ( !v12 || (v12->thingflags & SITH_TF_CAPTURED) == 0 )
            return 1;
    }
    sithCog_SendMessageEx(cog, msgid, senderType, senderIdx, sourceType, sourceIndex, linkId, param0, param1, param2, param3);
    return 1;
}

int sithDSSCog_SendSyncCog(sithCog *cog, int sendto_id, int mpFlags)
{
    sithCogSymboltable *v13; // ebp
    
    NETMSG_START;

    NETMSG_PUSHS32(cog->selfCog);
    NETMSG_PUSHS32(cog->script_running);
    NETMSG_PUSHS32(cog->flags);
    if (cog->script_running)
    {
        NETMSG_PUSHS32(cog->wakeTimeMs);
        NETMSG_PUSHS32(cog->execPos);
        NETMSG_PUSHS32(cog->senderId);
        NETMSG_PUSHS32(cog->senderRef);
        NETMSG_PUSHS32(cog->senderType);
        NETMSG_PUSHS32(cog->sourceRef);
        NETMSG_PUSHS32(cog->sourceType);
    }
    if ( (cog->flags & COGFLAGS_PULSE) != 0 )
    {
        NETMSG_PUSHS32(cog->pulsePeriodMs);
        NETMSG_PUSHS32(cog->nextPulseMs);
    }
    if ( (cog->flags & COGFLAGS_8) != 0 )
        NETMSG_PUSHS32(cog->field_20);
    v13 = cog->pSymbolTable;
    if ( v13->entry_cnt )
    {
        for (int i = 0; i < v13->entry_cnt; i++)
        {
            NETMSG_PUSHU8(v13->buckets[i].val.type & 0xFF);
        }

        // TODO: figure out how to handle this in 64-bit
        for (int i = 0; i < v13->entry_cnt; i++)
        {
            sithCogSymbol* sym = &v13->buckets[i];
            if (sym->val.type == COG_VARTYPE_FLEX)
            {
                NETMSG_PUSHS32((uint32_t)sym->val.data[0]);
            }
            else if ( sym->val.type == COG_VARTYPE_VECTOR )
            {
                NETMSG_PUSHF32(sym->val.dataAsFloat[0]);
                NETMSG_PUSHF32(sym->val.dataAsFloat[1]);
                NETMSG_PUSHF32(sym->val.dataAsFloat[2]);
            }
            else
            {
                NETMSG_PUSHS32((uint32_t)sym->val.data[0]); // TODO ??? this is a pointer?
            }
        }
    }

    NETMSG_END(COGMSG_SYNCCOG);
    return sithCogVm_SendMsgToPlayer(&sithCogVm_netMsgTmp, sendto_id, mpFlags, 1);
}

int sithDSSCog_HandleSyncCog(sithCogMsg *msg)
{
    sithCog *cog; // eax
    sithCogSymboltable *v13; // ebp
    
    NETMSG_IN_START(msg);

    cog = sithCog_GetByIdx(NETMSG_POPS32());
    if (!cog)
        return 0;

    cog->script_running = NETMSG_POPS32();
    cog->flags = NETMSG_POPS32();
    if (cog->script_running)
    {
        cog->wakeTimeMs = NETMSG_POPS32();
        cog->execPos = NETMSG_POPS32();
        cog->senderId = NETMSG_POPS32();
        cog->senderRef = NETMSG_POPS32();
        cog->senderType = NETMSG_POPS32();
        cog->sourceRef = NETMSG_POPS32();
        cog->sourceType = NETMSG_POPS32();
    }

    if (cog->flags & COGFLAGS_PULSE)
    {
        cog->pulsePeriodMs = NETMSG_POPS32();
        cog->nextPulseMs = NETMSG_POPS32();
    }

    if (cog->flags & COGFLAGS_8)
    {
        cog->field_20 = NETMSG_POPS32();
    }
    
    v13 = cog->pSymbolTable;
    if ( v13->entry_cnt )
    {
        for (int i = 0; i < v13->entry_cnt; i++)
        {
            v13->buckets[i].val.type = NETMSG_POPU8();
        }

        // TODO: verify in 64-bit, particularly with AICLASS
        for (int i = 0; i < v13->entry_cnt; i++)
        {
            sithCogSymbol* sym = &v13->buckets[i];
            if (sym->val.type == COG_VARTYPE_FLEX)
            {
                sym->val.data[0] = NETMSG_POPS32();
            }
            else if ( sym->val.type == COG_VARTYPE_VECTOR )
            {
                sym->val.dataAsFloat[0] = NETMSG_POPF32();
                sym->val.dataAsFloat[1] = NETMSG_POPF32();
                sym->val.dataAsFloat[2] = NETMSG_POPF32();
            }
            else
            {
                sym->val.data[0] = NETMSG_POPS32();
            }
        }
    }

    return 1;
}
