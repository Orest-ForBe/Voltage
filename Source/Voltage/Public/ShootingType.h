// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EShootingType : uint8
{
	EST_Auto UMETA(DisplayName = "Auto"),
	EST_Burst UMETA(DisplayName = "Burst"),
	EST_Semi UMETA(DisplayName = "Semi"),
	
	EST_MAX UMETA(DisplayName = "DefaultMAX")
};