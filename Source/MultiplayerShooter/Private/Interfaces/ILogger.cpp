


#include "MultiplayerShooter/Public/Interfaces/ILogger.h"

void IILogger::LogByLogger(const FName& LogName, const FString& OwnerName)
{
	TArray<FString> LogArray;

	FString Statement;
	LogStatement(Statement, true);
	LogArray.Add(OwnerName + " " + Statement);

	LogInfo(LogArray);

	LogStatement(Statement, false);
	LogArray.Add(OwnerName + " " + Statement);

	FLogCategory<ELogVerbosity::All, ELogVerbosity::All> LogCategory(LogName);

	UE_LOG(LogCategory, Warning, TEXT("%s"), *LogArray[0]);
	for (int32 It = 1; It < LogArray.Num() - 1; It++) {
		UE_LOG(LogCategory, Display, TEXT("%s"), *LogArray[It]);
	}
	UE_LOG(LogCategory, Warning, TEXT("%s"), *LogArray[LogArray.Num() - 1]);
}

void IILogger::LogStatement(FString& Statement, bool bBegin)
{
	Statement = FString::Printf(TEXT("LOG %hs"), bBegin ? "BEGIN" : "END");
}
