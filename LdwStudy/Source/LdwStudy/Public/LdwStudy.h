// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD,
};

DECLARE_LOG_CATEGORY_EXTERN(LdwStudy, Log, All);
#define LSLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LSLOG_S(Verbosity) UE_LOG(LdwStudy, Verbosity, TEXT("%s"), *LSLOG_CALLINFO)
#define LSLOG(Verbosity, Format, ...) UE_LOG(LdwStudy, Verbosity, TEXT("%s %s"), *LSLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LSCHECK(Expr, ...) {if(!(Expr)){LSLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}