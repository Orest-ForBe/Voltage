// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Freehand UMETA(DisplayName = "Freehand"),
	EWT_Knife UMETA(DisplayName = "Knife"),
	EWT_Primary UMETA(DisplayName = "Primary"),
	EWT_Secondary UMETA(DisplayName = "Secondary"),
	EWT_Parasite UMETA(DisplayName = "Parasite"),
	EWT_HEGrenade UMETA(DisplayName = "HEGrenade"),
	EWT_FlashGrenade UMETA(DisplayName = "FlashGrenade"),
	EWT_EMPGrenade UMETA(DisplayName = "EMPGrenade"),
	EWT_ShockGrenade UMETA(DisplayName = "ShockGrenade"),
	EWT_Pliers UMETA(DisplayName = "Pliers"),
	EWT_C2 UMETA(DisplayName = "C2"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};