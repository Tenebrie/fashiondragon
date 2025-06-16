struct FWalletTransaction
{
	float Gold = 0;
	float Threads = 0;
	float Crystals = 0;
	float Flowers = 0;

	void Add(FWalletTransaction Other)
	{
		Gold += Other.Gold;
		Threads += Other.Threads;
		Crystals += Other.Crystals;
		Flowers += Other.Flowers;
	}
}

class UPlayerWallet : UPlayerComponent
{
	FWalletTransaction Current = FWalletTransaction();
	UPlayerWalletWidget Widget;

	void Add(FWalletTransaction Transaction)
	{
		Current.Add(Transaction);
		Widget.Update(Current);
	}
	
	void OnPickup(APickup Pickup)
	{
		FWalletTransaction Transaction = Pickup.GetTransaction();
		Current.Add(Transaction);
		Widget.Update(Current);
	}
}