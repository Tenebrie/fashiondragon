class APickup_Bling : APickup
{
	void OnPickup(FWalletTransaction& Transaction) override
	{
		Transaction.Gold = 1000;
	}
}
