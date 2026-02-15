

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ILogger.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UILogger : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERSHOOTER_API IILogger
{
	GENERATED_BODY()


protected:
	virtual void LogInfo(TArray<FString>& LogArray){}

	void LogByLogger(const FName& LogName, const FString& OwnerName);
private:
	void LogStatement(FString& Statement, bool bBegin);
};
