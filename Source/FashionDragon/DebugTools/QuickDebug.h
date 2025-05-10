#pragma once

class FDebugString
{
public:
	FString Value;

	// Constructor from const char*
	FDebugString(const char* InValue) : Value(UTF8_TO_TCHAR(InValue)) {}
    
	// Constructor from FString
	FDebugString(const FString& InValue) : Value(InValue) {}
    
	// Operator+ for concatenation with const char*
	FDebugString operator+(const char* Other) const
	{
		return FDebugString(Value + FString(UTF8_TO_TCHAR(Other)));
	}
    
	// Operator+ for concatenation with another FDebugString
	FDebugString operator+(const FDebugString& Other) const
	{
		return FDebugString(Value + Other.Value);
	}
};

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
class FASHIONDRAGON_API Debug
{
public:
	template<typename... Args>
	static void Print(const char* First, Args... Rest)
	{
		FString Message = UTF8_TO_TCHAR(First);
    
		(AppendArg(Message, Rest), ...);
    
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		}
	}

	template<typename... Args>
	static void PrintStable(const int32 Key, const char* First, Args... Rest)
	{
		FString Message = UTF8_TO_TCHAR(First);
    
		(AppendArg(Message, Rest), ...);
    
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, 5.0f, FColor::Yellow, Message);
		}
	}

private:
	// Helper for const char*
	static void AppendArg(FString& Message, const char* Arg)
	{
		Message.Append(UTF8_TO_TCHAR(Arg));
	}
    
	// Helper for FDebugString
	static void AppendArg(FString& Message, const FDebugString& Arg)
	{
		Message.Append(Arg.Value);
	}
    
	// Helper for other types - convert to string safely
	template<typename T>
	static typename std::enable_if_t<!std::is_convertible_v<T, const char*> && !std::is_same_v<T, FDebugString>, void>
	AppendArg(FString& Message, const T& Arg)
	{
		if constexpr (std::is_enum_v<T>)
			Message.Append(FString::Printf(TEXT("%d"), static_cast<int>(Arg)));
		else if constexpr (std::is_arithmetic_v<T>)
			Message.Append(FString::Printf(TEXT("%d"), Arg));
		else if constexpr (std::is_same_v<T, FVector>)
			Message.Append(FString::Printf(TEXT("FVector(%f; %f; %f)"), Arg.X, Arg.Y, Arg.Z));
		else if constexpr (std::is_same_v<T, std::string>)
			Message.Append(UTF8_TO_TCHAR(Arg.c_str()));
		else
			Message.Append(TEXT("[Unknown Type]"));
	}
};
