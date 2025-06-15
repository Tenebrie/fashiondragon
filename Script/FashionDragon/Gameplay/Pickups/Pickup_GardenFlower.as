class APickup_GardenFlower : APickup
{
	void OnPickup(FWalletTransaction& Transaction) override
	{
		Transaction.Flowers = 1;
	}
}