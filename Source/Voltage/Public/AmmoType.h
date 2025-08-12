// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EAmmoType: uint8
{
	EAT_556 UMETA(DisplayName = "556"),
	EAT_762 UMETA(DisplayName = "762"),
	EAT_919 UMETA(DisplayName = "919"),

	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};