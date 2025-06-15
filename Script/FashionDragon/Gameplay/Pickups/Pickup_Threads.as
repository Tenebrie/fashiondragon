class APickup_Threads : APickup
{
	default PickupMesh.RelativeLocation = FVector(0, 0, 750);

	void OnPickup(FWalletTransaction& Transaction) override
	{
		Transaction.Threads = 551;
	}
}
