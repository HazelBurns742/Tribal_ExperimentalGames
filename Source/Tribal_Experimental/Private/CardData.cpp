// Fill out your copyright notice in the Description page of Project Settings.


#include "CardData.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


UCardData::UCardData()
{
	//Default values
	ActionPoints = 0;
	Health = 0;
	Combat = 0;

}
