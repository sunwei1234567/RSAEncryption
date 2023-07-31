// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "tchar.h"
#include <cstring>
#include <codecvt>
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include "windows.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "RSAEncryption.generated.h"


using namespace std;
/**
 * 
 */
UCLASS()
class RSAEN_API URSAEncryption : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RSAEncryptString_ue4"), Category = "RSA")
		static FString RSAEncryptString_ue4(FString cmd);

public:
	static wstring StrToWstr(const string& str);

	static TCHAR* CharToTCHAR(const char* ch);
};
