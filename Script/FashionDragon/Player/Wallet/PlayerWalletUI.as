UCLASS(Abstract)
class UPlayerWalletWidget : UUserWidget
{
	UPROPERTY(BindWidget) protected UTextBlock CoinCount;
	UPROPERTY(BindWidget) protected UTextBlock YarnCount;
	UPROPERTY(BindWidget) protected UTextBlock FlowerCount;

	void Update(FWalletTransaction State)
	{
		FNumberFormattingOptions Options;
		CoinCount.SetText(FText::AsNumber(State.Gold, Options));
		YarnCount.SetText(FText::AsNumber(State.Threads, Options));
		FlowerCount.SetText(FText::AsNumber(State.Flowers, Options));
	}
}