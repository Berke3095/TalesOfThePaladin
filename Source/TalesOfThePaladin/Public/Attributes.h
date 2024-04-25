#pragma once

struct Attributes
{
	float Health{};
	float Mana{};
	float Damage{};

	Attributes() {};

	Attributes(float AssignedHealth, float AssignedMana, float AssignedDamage)
	{
		Health = AssignedHealth;
		Mana = AssignedMana;
		Damage = AssignedDamage;
	}
};