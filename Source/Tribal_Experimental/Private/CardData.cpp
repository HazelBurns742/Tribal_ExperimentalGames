// Fill out your copyright notice in the Description page of Project Settings.


#include "CardData.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


UCardData::UCardData()
{
	//Default values
	Points = 1; 
	Health = 1;
	Combat = 1;

}
