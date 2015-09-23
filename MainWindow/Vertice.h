#pragma once

/// <summary>
/// Wierzcho³ek grafu.
/// </summary>
class Vertice
{
	short _id;
	
public:
	Vertice(short id);
	~Vertice();

#pragma region Getters & Setters
	inline short Id() const { return _id; }
	void Id(short val) { _id = val; }
#pragma endregion


#pragma region Operators
	inline bool operator==(Vertice const & other) const
	{
		Id() == other.Id() ? true : false;
	}
#pragma endregion

};

